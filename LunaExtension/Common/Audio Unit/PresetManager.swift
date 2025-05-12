//
//  PresetManager.swift
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

import AVFoundation
import Foundation

// Extension to simplify creation of AUAudioUnitPreset
extension AUAudioUnitPreset {
    static func createPreset(number: Int, name: String) -> AUAudioUnitPreset {
        // Create an AUAudioUnitPreset using normal ObjC init methods
        let preset = AUAudioUnitPreset()
        preset.number = number
        preset.name = name
        return preset
    }
}

/// A class responsible for managing presets in the Luna audio unit.
/// This includes factory presets, user presets, and applying preset settings.
class PresetManager {
    // MARK: - Properties

    private(set) var currentPreset: AUAudioUnitPreset?
    private weak var parameterTree: AUParameterTree?
    private var userPresets: [AUAudioUnitPreset] = []

    // User defaults key for storing preset data
    private static let userPresetsKey = "com.wildsurmisemusic.LunaExtension.userPresets"

    // MARK: - Initialization

    init(parameterTree: AUParameterTree?) {
        self.parameterTree = parameterTree
        // Names are set during lazy initialization of factoryPresets
        // Load user presets
        loadUserPresets()
        // Set current preset after factory presets are available
        currentPreset = factoryPresets.first
    }

    // MARK: - Factory Presets

    /// The available factory presets
    let factoryPresets: [AUAudioUnitPreset] = {
        // Create factory presets with explicit names
        let preset0 = AUAudioUnitPreset.createPreset(number: 0, name: "Default")
        let preset1 = AUAudioUnitPreset.createPreset(number: 1, name: "Vintage Tape")
        let preset2 = AUAudioUnitPreset.createPreset(number: 2, name: "Glitchy Repeat")
        let preset3 = AUAudioUnitPreset.createPreset(number: 3, name: "Ambience")

        // Ensure names are properly set
        preset0.name = "Default"
        preset1.name = "Vintage Tape"
        preset2.name = "Glitchy Repeat"
        preset3.name = "Ambience"

        // Set initial state to first preset
        return [preset0, preset1, preset2, preset3]
    }()

    /// Set a preset by its preset object
    /// - Parameter preset: The preset to apply
    func setPreset(_ preset: AUAudioUnitPreset?) {
        guard let preset = preset else { return }

        // Factory presets
        if preset.number >= 0 {
            applyFactoryPreset(preset.number)
        } else {
            // User presets
            applyUserPreset(preset.name)
        }

        currentPreset = preset
    }

    /// Get a factory preset by number
    /// - Parameter presetNumber: The preset number to find
    /// - Returns: The preset if found
    func getFactoryPreset(number presetNumber: Int) -> AUAudioUnitPreset? {
        return factoryPresets.first { $0.number == presetNumber }
    }

    // MARK: - User Presets

    /// Get all available presets (factory + user)
    var allPresets: [AUAudioUnitPreset] {
        return factoryPresets + userPresets
    }

    /// Save the current parameter state as a user preset
    /// - Parameter name: The name to give the preset
    /// - Returns: The newly created preset
    func saveUserPreset(name: String) -> AUAudioUnitPreset? {
        guard let parameterTree = parameterTree else { return nil }

        // Create a new preset with a negative number (indicating user preset)
        let newPreset = AUAudioUnitPreset.createPreset(number: -1, name: name)

        // Create a dictionary to store parameter values
        var presetData: [String: Double] = [:]

        // Store all current parameter values
        for param in parameterTree.allParameters {
            presetData["\(param.address)"] = Double(param.value)
        }

        // Store the preset data
        saveUserPresetData(presetName: name, data: presetData)

        // Add to our list of user presets
        if !userPresets.contains(where: { $0.name == name }) {
            userPresets.append(newPreset)
            saveUserPresetList()
        }

        return newPreset
    }

    /// Delete a user preset
    /// - Parameter presetName: The name of the preset to delete
    func deleteUserPreset(named presetName: String) {
        // Remove from our list
        userPresets.removeAll { $0.name == presetName }

        // Remove from storage
        let userDefaults = UserDefaults.standard
        let key = "\(PresetManager.userPresetsKey).\(presetName)"
        userDefaults.removeObject(forKey: key)

        // Save updated list
        saveUserPresetList()
    }

    // MARK: - Preset Application

    /// Apply factory preset settings
    /// - Parameter presetNumber: The preset number to apply
    private func applyFactoryPreset(_ presetNumber: Int) {
        guard let parameterTree = parameterTree else { return }

        // Start by applying default settings to reset everything
        applyDefaultSettings()

        switch presetNumber {
        case 0: // Default
            // Already handled by applyDefaultSettings() above
            break

        case 1: // Vintage Tape
            // Apply vintage tape settings using macros and new params
            setParameterValue(.tape_wear_macro, value: 0.65) // Moderate wear
            setParameterValue(.feedback, value: 0.68)
            setParameterValue(.highpass, value: 320)
            setParameterValue(.lowpass, value: 6500)
            setParameterValue(.delay_ms, value: 350)
            setParameterValue(.mix, value: 45)
            setParameterValue(.diffusion_amount, value: 0.1) // Subtle diffusion

        case 2: // Glitchy Repeat
            // Apply glitchy repeat settings using macros
            setParameterValue(.glitch_macro, value: 0.70) // Significant glitch
            setParameterValue(.tape_wear_macro, value: 0.15) // Low wear for clarity
            setParameterValue(.delay_ms, value: 220)
            setParameterValue(.pitch_shift, value: -3)
            setParameterValue(.feedback, value: 0.72)
            setParameterValue(.mix, value: 55) // Slightly wetter mix

        case 3: // Ambience
            // Apply ambience settings
            setParameterValue(.delay_ms, value: 750)
            setParameterValue(.feedback, value: 0.58)
            setParameterValue(.highpass, value: 180)
            setParameterValue(.lowpass, value: 8000)
            setParameterValue(.mix, value: 38)
            setParameterValue(.spread_amount, value: 0.8)
            setParameterValue(.diffusion_amount, value: 0.45) // More diffusion
            setParameterValue(.tape_wear_macro, value: 0.1) // Low wear

        default:
            // Unknown preset, defaults already applied
            break
        }
    }

    /// Apply user preset settings
    /// - Parameter presetName: The name of the user preset to apply
    private func applyUserPreset(_ presetName: String) {
        guard let parameterTree = parameterTree,
              let presetData = loadUserPresetData(presetName: presetName)
        else {
            // If we can't find the preset data, apply defaults
            applyDefaultSettings()
            return
        }

        // Apply all parameter values from the preset data
        for (addressStr, value) in presetData {
            if let address = AUParameterAddress(addressStr),
               let parameter = parameterTree.parameter(withAddress: address)
            {
                parameter.value = AUValue(value)
            }
        }
    }

    /// Apply default parameter settings based on ParameterSpec definitions
    private func applyDefaultSettings() {
        // Use defaults defined in Parameters.swift
        setParameterValue(.spread_amount, value: 0.0)
        setParameterValue(.output_gain, value: 0.0)
        setParameterValue(.delay_ms, value: 300.0)
        setParameterValue(.feedback, value: 0.5)
        setParameterValue(.pitch_shift, value: 0.0)
        setParameterValue(.highpass, value: 250.0)
        setParameterValue(.lowpass, value: 10000.0)
        setParameterValue(.diffusion_amount, value: 0.0)
        setParameterValue(.tape_wear_macro, value: 0.1) // Default from ParamSpec
        setParameterValue(.glitch_macro, value: 0.0)
        setParameterValue(.ducking_macro, value: 0.0)
        setParameterValue(.duck_attack, value: 10.0)
        setParameterValue(.duck_release, value: 200.0)
        setParameterValue(.mix, value: 50.0)
        // Note: Removed settings for old hidden parameters
    }

    // MARK: - Helper Methods

    /// Set a parameter value using the parameter address
    /// - Parameters:
    ///   - parameterAddress: The parameter address to set
    ///   - value: The value to set
    private func setParameterValue(_ parameterAddress: LunaExtensionParameterAddress, value: AUValue) {
        guard let parameter = parameterTree?.parameter(withAddress: parameterAddress.rawValue) else { return }
        parameter.value = value
    }

    // MARK: - State Serialization

    /// Add preset data to a full state dictionary
    /// - Parameter fullState: The state dictionary to add to
    /// - Returns: The updated dictionary with preset data
    func addStateData(to fullState: [String: Any]) -> [String: Any] {
        var updatedState = fullState

        // Save current preset information
        if let currentPreset = currentPreset {
            if currentPreset.number >= 0 {
                updatedState["factoryPreset"] = currentPreset.number
            } else {
                updatedState["userPreset"] = currentPreset.name
            }
        }

        return updatedState
    }

    /// Restore preset data from a full state dictionary
    /// - Parameter fullState: The state dictionary to restore from
    func restoreStateData(from fullState: [String: Any]) {
        // Restore preset information
        if let factoryPresetNumber = fullState["factoryPreset"] as? NSNumber,
           let preset = factoryPresets.first(where: { $0.number == factoryPresetNumber.intValue })
        {
            setPreset(preset)
        } else if let userPresetName = fullState["userPreset"] as? String {
            let preset = AUAudioUnitPreset.createPreset(number: -1, name: userPresetName)
            setPreset(preset)
        }
    }

    // MARK: - User Preset Storage

    /// Save the list of user presets
    private func saveUserPresetList() {
        let userDefaults = UserDefaults.standard
        // Store just the names as an array
        let presetNames = userPresets.map { $0.name }
        userDefaults.set(presetNames, forKey: PresetManager.userPresetsKey)
    }

    /// Load the list of user presets
    private func loadUserPresets() {
        let userDefaults = UserDefaults.standard
        if let presetNames = userDefaults.array(forKey: PresetManager.userPresetsKey) as? [String] {
            // Convert names to presets
            userPresets = presetNames.map { name in
                AUAudioUnitPreset.createPreset(number: -1, name: name)
            }
        }
    }

    /// Save preset data for a user preset
    /// - Parameters:
    ///   - presetName: The name of the preset
    ///   - data: The parameter data to save
    private func saveUserPresetData(presetName: String, data: [String: Double]) {
        let userDefaults = UserDefaults.standard
        let key = "\(PresetManager.userPresetsKey).\(presetName)"
        userDefaults.set(data, forKey: key)
    }

    /// Load preset data for a user preset
    /// - Parameter presetName: The name of the preset
    /// - Returns: The parameter data if found
    private func loadUserPresetData(presetName: String) -> [String: Double]? {
        let userDefaults = UserDefaults.standard
        let key = "\(PresetManager.userPresetsKey).\(presetName)"
        return userDefaults.dictionary(forKey: key) as? [String: Double]
    }
}
