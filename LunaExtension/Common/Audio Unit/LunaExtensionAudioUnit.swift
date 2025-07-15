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
    private var defaultParameterValues = [AUParameterAddress: AUValue]()

    // Private storage for current preset
    private var _currentPreset: AUAudioUnitPreset?

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

    override public var componentName: String {
        return "Luna Delay"
    }

    override public var componentVersion: UInt32 {
        return 0x0001_0600 // 1.6.0
    }

    override public var manufacturerName: String {
        return "Wild Surmise"
    }

    // MARK: - Factory Presets

    override public var factoryPresets: [AUAudioUnitPreset]? {
        return [
            AUAudioUnitPreset(number: 0, name: "Default"),
            AUAudioUnitPreset(number: 1, name: "Vintage Tape"),
            AUAudioUnitPreset(number: 2, name: "Glitchy Repeat"),
            AUAudioUnitPreset(number: 3, name: "Ambience")
        ]
    }

    // Define the parameter values for each factory preset
    private let factoryPresetValues: [[(LunaExtensionParameterAddress, AUValue)]] = [
        // Default (preset 0) - will be handled by default parameter values
        [],
        // Vintage Tape (preset 1)
        [
            (LunaExtensionParameterAddress.tape_noise_macro, 0.65),
            (LunaExtensionParameterAddress.feedback, 0.68),
            (LunaExtensionParameterAddress.highpass, 320),
            (LunaExtensionParameterAddress.lowpass, 6500),
            (LunaExtensionParameterAddress.delay_time, 350),
            (LunaExtensionParameterAddress.mix, 45),
            (LunaExtensionParameterAddress.diffusion_amount, 0.1)
        ],
        // Glitchy Repeat (preset 2)
        [
            (LunaExtensionParameterAddress.glitch_macro, 0.70),
            (LunaExtensionParameterAddress.tape_noise_macro, 0.15),
            (LunaExtensionParameterAddress.delay_time, 220),
            (LunaExtensionParameterAddress.pitch_shift, -3),
            (LunaExtensionParameterAddress.feedback, 0.72),
            (LunaExtensionParameterAddress.mix, 55)
        ],
        // Ambience (preset 3)
        [
            (LunaExtensionParameterAddress.delay_time, 750),
            (LunaExtensionParameterAddress.feedback, 0.58),
            (LunaExtensionParameterAddress.highpass, 180),
            (LunaExtensionParameterAddress.lowpass, 8000),
            (LunaExtensionParameterAddress.mix, 38),
            (LunaExtensionParameterAddress.spread_amount, 0.8),
            (LunaExtensionParameterAddress.diffusion_amount, 0.45),
            (LunaExtensionParameterAddress.tape_noise_macro, 0.1)
        ]
    ]

    // MARK: - User Preset Support

    override public var supportsUserPresets: Bool {
        return true
    }

    // MARK: - Current Preset Handling

    override public var currentPreset: AUAudioUnitPreset? {
        get {
            return _currentPreset
        }
        set {
            // If the newValue is nil, return.
            guard let preset = newValue else {
                _currentPreset = nil
                return
            }
            
            // Factory presets have a number >= 0
            if preset.number >= 0 {
                applyFactoryPreset(preset.number)
                _currentPreset = preset
            }
            // User presets have a number < 0
            else {
                // Attempt to restore the archived state for this user preset
                do {
                    fullStateForDocument = try presetState(for: preset)
                    // Set the currentPreset after successfully restoring the state
                    _currentPreset = preset
                } catch {
                    print("Unable to restore preset \(preset.name): \(error)")
                }
            }
        }
    }

    // MARK: - Factory Preset Application

    private func applyFactoryPreset(_ presetNumber: Int) {
        guard let parameterTree = parameterTree else { return }
        
        // First, reset all parameters to their default values
        for param in parameterTree.allParameters {
            if let defaultValue = defaultParameterValues[param.address] {
                param.value = defaultValue
                kernel.setParameter(param.address, defaultValue)
                _parameterValues[param.address] = defaultValue
            }
        }
        
        // Apply preset-specific values if available
        if presetNumber > 0 && presetNumber < factoryPresetValues.count {
            let presetParams = factoryPresetValues[presetNumber]
            for (paramAddress, value) in presetParams {
                if let param = parameterTree.parameter(withAddress: paramAddress.rawValue) {
                    param.value = value
                    kernel.setParameter(param.address, value)
                    _parameterValues[param.address] = value
                }
            }
        }
    }

    // MARK: - Bus Configuration

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
            defaultParameterValues[param.address] = param.value
            _parameterValues[param.address] = param.value
        }

        setupParameterCallbacks()

        // Set the default preset to the first factory preset
        if currentPreset == nil {
            currentPreset = factoryPresets?.first
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

            // Add current preset information for proper state restoration
            if let currentPreset = _currentPreset {
                if currentPreset.number >= 0 {
                    fullStateDict["factoryPreset"] = currentPreset.number
                } else {
                    fullStateDict["userPreset"] = currentPreset.name
                }
            }

            // Ensure required keys for auval validation
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
            // Let superclass handle its part
            super.fullState = newValue

            guard let newFullState = newValue else { return }

            // Restore parameters
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

            // Restore current preset information
            if let factoryPresetNumber = newFullState["factoryPreset"] as? NSNumber {
                _currentPreset = factoryPresets?.first { $0.number == factoryPresetNumber.intValue }
            } else if let userPresetName = newFullState["userPreset"] as? String {
                // Create a temporary preset object for user presets
                _currentPreset = AUAudioUnitPreset(number: -1, name: userPresetName)
            } else {
                // Default to first factory preset if no preset info in state
                _currentPreset = factoryPresets?.first
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
}
