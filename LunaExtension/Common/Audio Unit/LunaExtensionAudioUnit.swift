//
//  LunaExtensionAudioUnit.swift
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

import AVFoundation

public class LunaExtensionAudioUnit: AUAudioUnit, @unchecked Sendable {
    // C++ Objects
    var kernel = LunaExtensionDSPKernel()
    var processHelper: AUProcessHelper?
    var inputBus = BufferedInputBus()

    private var outputBus: AUAudioUnitBus?
    private var _inputBusses: AUAudioUnitBusArray!
    private var _outputBusses: AUAudioUnitBusArray!

    // For parameter persistence
    private var _parameterValues = [AUParameterAddress: AUValue]()

    // Preset management
    private lazy var presetManager = PresetManager(parameterTree: parameterTree)

    @objc override init(componentDescription: AudioComponentDescription, options: AudioComponentInstantiationOptions) throws {
        let format = AVAudioFormat(standardFormatWithSampleRate: 44100, channels: 2)!
        try super.init(componentDescription: componentDescription, options: options)
        outputBus = try AUAudioUnitBus(format: format)
        outputBus?.maximumChannelCount = 2

        // Create the input and output busses.
        inputBus.initialize(format, 8)

        // Create the input and output bus arrays.
        _inputBusses = AUAudioUnitBusArray(audioUnit: self, busType: AUAudioUnitBusType.output, busses: [inputBus.bus!])

        // Create the input and output bus arrays.
        _outputBusses = AUAudioUnitBusArray(audioUnit: self, busType: AUAudioUnitBusType.output, busses: [outputBus!])

        processHelper = AUProcessHelper(&kernel, &inputBus)
    }

    // MARK: - Class Info

    override public var factoryPresets: [AUAudioUnitPreset]? {
        // Ensure preset names are initialized before returning
        return presetManager.factoryPresets.map { preset in
            if preset.name.isEmpty {
                // This logic should ideally be in PresetManager, but ensure it runs
                switch preset.number {
                case 0: preset.name = "Default"
                case 1: preset.name = "Vintage Tape"
                case 2: preset.name = "Glitchy Repeat"
                case 3: preset.name = "Ambience"
                default: preset.name = "Preset \(preset.number)"
                }
            }
            return preset
        }
    }

    override public var supportsUserPresets: Bool {
        return true
    }

    override public var componentName: String {
        return "Luna Delay"
    }

    override public var componentVersion: UInt32 {
        return 0x0001_0600 // 1.6.0
    }

    override public var manufacturerName: String {
        return "Wild Surmise"
    }

    // MARK: - Preset Support

    override public var currentPreset: AUAudioUnitPreset? {
        get {
            return presetManager.currentPreset
        }
        set {
            presetManager.setPreset(newValue)
        }
    }

    /// All available presets (factory and user)
    public var allPresets: [AUAudioUnitPreset] {
        return presetManager.allPresets
    }

    /// Save the current parameter state as a user preset
    /// - Parameter name: The name for the preset
    /// - Returns: The created preset if successful
    public func saveUserPreset(name: String) -> AUAudioUnitPreset? {
        return presetManager.saveUserPreset(name: name)
    }

    /// Delete a user preset
    /// - Parameter presetName: The name of the preset to delete
    public func deleteUserPreset(named presetName: String) {
        presetManager.deleteUserPreset(named: presetName)
    }

    override public var inputBusses: AUAudioUnitBusArray {
        return _inputBusses
    }

    override public var outputBusses: AUAudioUnitBusArray {
        return _outputBusses
    }

    override public var channelCapabilities: [NSNumber] {
        return [NSNumber(value: 2), NSNumber(value: 2)]
    }

    override public var maximumFramesToRender: AUAudioFrameCount {
        get {
            return kernel.maximumFramesToRender()
        }

        set {
            kernel.setMaximumFramesToRender(newValue)
        }
    }

    override public var shouldBypassEffect: Bool {
        get {
            return kernel.isBypassed()
        }

        set {
            kernel.setBypass(newValue)
        }
    }

    // MARK: - Rendering

    override public var internalRenderBlock: AUInternalRenderBlock {
        return processHelper!.internalRenderBlock()
    }

    // Allocate resources required to render.
    // Subclassers should call the superclass implementation.
    override public func allocateRenderResources() throws {
        let inputChannelCount = inputBusses[0].format.channelCount
        let outputChannelCount = outputBusses[0].format.channelCount

        if outputChannelCount != inputChannelCount {
            setRenderResourcesAllocated(false)
            throw NSError(domain: NSOSStatusErrorDomain, code: Int(kAudioUnitErr_FailedInitialization), userInfo: nil)
        }

        inputBus.allocateRenderResources(maximumFramesToRender)

        kernel.setMusicalContextBlock(musicalContextBlock)
        kernel.initialize(Int32(inputChannelCount), Int32(outputChannelCount), outputBus!.format.sampleRate)

        processHelper?.setChannelCount(inputChannelCount, outputChannelCount)

        // Restore saved parameter values
        restoreParameterValues()

        try super.allocateRenderResources()
    }

    // Deallocate resources allocated in allocateRenderResourcesAndReturnError:
    // Subclassers should call the superclass implementation.
    override public func deallocateRenderResources() {
        // Deallocate your resources.
        kernel.deInitialize()

        super.deallocateRenderResources()
    }

    public func setupParameterTree(_ parameterTree: AUParameterTree) {
        self.parameterTree = parameterTree

        // Set the Parameter default values before setting up the parameter callbacks
        for param in parameterTree.allParameters {
            kernel.setParameter(param.address, param.value)
            // Store default values
            _parameterValues[param.address] = param.value
        }

        setupParameterCallbacks()

        // Initialize the preset manager with the parameter tree
        presetManager = PresetManager(parameterTree: parameterTree)

        // Ensure the current preset is set to the first factory preset
        if currentPreset == nil {
            currentPreset = presetManager.factoryPresets.first
        }
    }

    private func setupParameterCallbacks() {
        // implementorValueObserver is called when a parameter changes value.
        parameterTree?.implementorValueObserver = { [weak self] param, value in
            guard let self = self else { return }
            self.kernel.setParameter(param.address, value)
            // Store parameter value changes
            self._parameterValues[param.address] = value
        }

        // implementorValueProvider is called when the value needs to be refreshed.
        parameterTree?.implementorValueProvider = { [weak self] param in
            return self!.kernel.getParameter(param.address)
        }

        // A function to provide string representations of parameter values.
        parameterTree?.implementorStringFromValueCallback = { _, valuePtr in
            guard let value = valuePtr?.pointee else {
                return "-"
            }
            return NSString.localizedStringWithFormat("%.f", value) as String
        }
    }

    // MARK: - Parameter State

    private func restoreParameterValues() {
        // Restore all saved parameter values to the DSP
        for (address, value) in _parameterValues {
            kernel.setParameter(address, value)
        }
    }

    // MARK: - State Saving and Restoration

    override public var fullState: [String: Any]? {
        get {
            // Start with the superclass state, which contains essential info
            var fullStateDict = super.fullState ?? [String: Any]()

            // Add our custom parameter values
            var paramValues = [String: Double]()
            for (address, value) in _parameterValues {
                paramValues["\(address)"] = Double(value)
            }
            fullStateDict["parameters"] = paramValues

            // Add preset information using the PresetManager
            // Make sure keys here don't clash unexpectedly with super.fullState keys
            let presetState = presetManager.addStateData(to: [:]) // Get only preset state
            fullStateDict.merge(presetState) { _, new in new } // Merge preset state in

            // Ensure the 'type' key expected by auval is present, derived from componentDescription
            // This might be redundant if super.fullState provides it, but safe to ensure.
            if fullStateDict["type"] == nil {
                fullStateDict["type"] = componentDescription.componentType
            }
            if fullStateDict["subtype"] == nil {
                fullStateDict["subtype"] = componentDescription.componentSubType
            }
            if fullStateDict["manufacturer"] == nil {
                fullStateDict["manufacturer"] = componentDescription.componentManufacturer
            }

            return fullStateDict
        }
        set {
            // When setting state, pass the relevant parts to the superclass if necessary,
            // although the default AUAudioUnit setter might handle this.
            // For now, we focus on restoring our custom state.
            super.fullState = newValue // Let superclass handle its part

            guard let newFullState = newValue else { return }

            // Restore parameters (as before)
            if let paramValues = newFullState["parameters"] as? [String: Double] {
                _parameterValues.removeAll()
                for (addressStr, value) in paramValues {
                    guard let address = AUParameterAddress(addressStr) else { continue }
                    let auValue = AUValue(value)
                    _parameterValues[address] = auValue
                    kernel.setParameter(address, auValue)
                    parameterTree?.parameter(withAddress: address)?.value = auValue
                }
            }

            // Restore preset information using the PresetManager (as before)
            presetManager.restoreStateData(from: newFullState)

            // Update the currentPreset property based on restored state
            // This ensures the getter for currentPreset reflects the restored state
            if let factoryPresetNumber = newFullState["factoryPreset"] as? NSNumber {
                currentPreset = presetManager.getFactoryPreset(number: factoryPresetNumber.intValue)
            } else if let userPresetName = newFullState["userPreset"] as? String {
                // Need a way to get/create user preset object from name
                // For now, setting via presetManager should update internal state
                // self.currentPreset = ??? // How to get the AUAudioUnitPreset object?
                // Relying on presetManager.restoreStateData to have set the internal currentPreset
                currentPreset = presetManager.currentPreset // Update local property
            } else {
                // If no preset info in state, maybe default?
                currentPreset = presetManager.factoryPresets.first
            }
        }
    }

    override public var fullStateForDocument: [String: Any]? {
        get {
            return fullState
        }
        set {
            fullState = newValue
        }
    }

    // MARK: - Factory Presets

    /// Make sure factory presets have proper names for display
    public func getPresets() -> [AUAudioUnitPreset] {
        // Initialize preset names if they aren't already
        for preset in presetManager.factoryPresets {
            if preset.name.isEmpty {
                switch preset.number {
                case 0: preset.name = "Default"
                case 1: preset.name = "Vintage Tape"
                case 2: preset.name = "Glitchy Repeat"
                case 3: preset.name = "Ambience"
                default: preset.name = "Preset \(preset.number)"
                }
            }
        }
        return presetManager.factoryPresets
    }
}
