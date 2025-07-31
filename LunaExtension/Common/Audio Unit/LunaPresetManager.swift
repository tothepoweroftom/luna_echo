import AudioPluginUI
import AudioToolbox
import SwiftUI
import os

private let log = Logger(subsystem: "com.wildsurmisemusic.LunaExtension", category: "LunaPresetManager")

/// Luna-specific preset implementation
public struct LunaPreset: Preset, Identifiable {
    public let id: String
    public let name: String
    public let isFactory: Bool
    public let isSelected: Bool
    public let auPreset: AUAudioUnitPreset
    
    public init(auPreset: AUAudioUnitPreset, isSelected: Bool = false) {
        // Use a more stable ID that combines type and number to avoid conflicts
        self.id = auPreset.number >= 0 ? "f_\(auPreset.number)" : "u_\(auPreset.number)"
        self.name = auPreset.name
        self.isFactory = auPreset.number >= 0
        self.isSelected = isSelected
        self.auPreset = auPreset
    }
}

/// Luna preset manager that bridges AUAudioUnit presets with AudioPluginUI protocol
public class LunaPresetManager: PresetManaging {
    @Published public var factoryPresets: [LunaPreset] = []
    @Published public var userPresets: [LunaPreset] = []
    @Published public var currentPreset: LunaPreset?
    
    private let userPresetsManager: UserPresetsManager
    private let audioUnit: LunaExtensionAudioUnit
    private let parameterTree: APParameterGroup
    
    public init(audioUnit: LunaExtensionAudioUnit, parameterTree: APParameterGroup) {
        self.audioUnit = audioUnit
        self.parameterTree = parameterTree
        self.userPresetsManager = UserPresetsManager(for: audioUnit)
        refreshPresets()
    }
    
    private func refreshPresets() {
        log.info("Refreshing presets...")
        
        // Load factory presets
        let factoryAUPresets = userPresetsManager.factoryPresets
        let currentAUPreset = userPresetsManager.currentPreset
        
        log.info("Found \(factoryAUPresets.count) factory presets")
        
        // Update factory presets on main queue
        DispatchQueue.main.async { [weak self] in
            guard let self = self else { return }
            
            self.factoryPresets = factoryAUPresets.map { auPreset in
                LunaPreset(
                    auPreset: auPreset,
                    isSelected: currentAUPreset?.number == auPreset.number
                )
            }
        }
        
        // Load user presets
        let userAUPresets = userPresetsManager.presets
        log.info("Found \(userAUPresets.count) user presets")
        
        // Update user presets on main queue
        DispatchQueue.main.async { [weak self] in
            guard let self = self else { return }
            
            self.userPresets = userAUPresets.map { auPreset in
                LunaPreset(
                    auPreset: auPreset,
                    isSelected: currentAUPreset?.number == auPreset.number
                )
            }.sorted { $0.name.localizedCaseInsensitiveCompare($1.name) == .orderedAscending }
        }
        
        // Set current preset on main queue
        DispatchQueue.main.async { [weak self] in
            guard let self = self else { return }
            
            if let currentAU = currentAUPreset {
                if currentAU.number >= 0 {
                    self.currentPreset = self.factoryPresets.first { $0.auPreset.number == currentAU.number }
                } else {
                    self.currentPreset = self.userPresets.first { $0.auPreset.number == currentAU.number }
                }
                log.info("Current preset: \(currentAU.name) (number: \(currentAU.number))")
            } else {
                self.currentPreset = nil
                log.info("No current preset")
            }
        }
    }
    
    public func selectPreset(_ preset: LunaPreset) {
        log.info("Selecting preset: \(preset.name)")
        
        // Set the current preset in the audio unit
        userPresetsManager.makeCurrentPreset(preset.auPreset)
        
        // If it's a factory preset, apply the parameter values
        if preset.isFactory {
            log.info("Applying factory preset parameter values")
            parameterTree.useFactoryPreset(preset.auPreset)
        }
        
        refreshPresets()
    }
    
    public func savePreset(name: String) -> Bool {
        log.info("Saving preset with name: \(name)")
        do {
            // Get current parameter configuration
            let currentConfig = parameterTree.getCurrentConfiguration()
            log.info("Captured current parameter values for preset")
            
            // Check if preset with same name exists
            if let existingPreset = userPresets.first(where: { $0.name == name }) {
                log.info("Overwriting existing preset: \(existingPreset.name)")
                // First make it current, then update it
                userPresetsManager.makeCurrentPreset(existingPreset.auPreset)
                try userPresetsManager.update(preset: existingPreset.auPreset)
            } else {
                log.info("Creating new preset: \(name)")
                // Create new preset (this captures the current audio unit state)
                try userPresetsManager.create(name: name)
            }
            
            // Note: The AUAudioUnit.saveUserPreset automatically captures the current
            // parameter state, so we don't need to manually store the configuration
            
            // Delay refresh to ensure AUAudioUnit state is updated
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) { [weak self] in
                self?.refreshPresets()
            }
            return true
        } catch {
            log.error("Failed to save preset: \(error.localizedDescription)")
            return false
        }
    }
    
    public func deletePreset(_ preset: LunaPreset) -> Bool {
        guard !preset.isFactory else {
            log.warning("Attempted to delete factory preset: \(preset.name)")
            return false
        }
        
        log.info("Deleting preset: \(preset.name) (number: \(preset.auPreset.number))")
        do {
            let wasCurrentPreset = preset.auPreset.number == userPresetsManager.currentPreset?.number
            
            // If this is not the current preset, make it current first
            if !wasCurrentPreset {
                log.info("Making preset current before deletion")
                userPresetsManager.makeCurrentPreset(preset.auPreset)
            }
            
            // Delete the preset
            try userPresetsManager.deleteCurrent()
            log.info("Successfully deleted preset")
            
            // If we deleted the current preset, select a default one
            if wasCurrentPreset || userPresetsManager.currentPreset == nil {
                log.info("Selecting default factory preset after deletion")
                if let defaultPreset = userPresetsManager.factoryPresets.first {
                    userPresetsManager.makeCurrentPreset(defaultPreset)
                }
            }
            
            // Delay refresh to ensure AUAudioUnit state is updated
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) { [weak self] in
                self?.refreshPresets()
            }
            return true
        } catch {
            log.error("Failed to delete preset: \(error.localizedDescription)")
            return false
        }
    }
    
    public func renamePreset(_ preset: LunaPreset, to name: String) -> Bool {
        guard !preset.isFactory else {
            log.warning("Attempted to rename factory preset: \(preset.name)")
            return false
        }
        
        log.info("Renaming preset from \(preset.name) to \(name)")
        do {
            // If this is not the current preset, make it current first
            if preset.auPreset.number != userPresetsManager.currentPreset?.number {
                log.info("Making preset current before renaming")
                userPresetsManager.makeCurrentPreset(preset.auPreset)
            }
            
            // Rename the current preset
            try userPresetsManager.renameCurrent(to: name)
            log.info("Successfully renamed preset")
            
            // Delay refresh to ensure AUAudioUnit state is updated
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) { [weak self] in
                self?.refreshPresets()
            }
            return true
        } catch {
            log.error("Failed to rename preset: \(error.localizedDescription)")
            return false
        }
    }
}