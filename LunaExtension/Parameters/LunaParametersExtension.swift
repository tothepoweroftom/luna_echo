//
//  LunaParametersExtension.swift
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

import AudioPluginUI
import AudioToolbox
import Foundation

/// Extension to support factory presets with parameter values
extension APParameterGroup {
    
    /// Array of factory preset configurations
    static let factoryPresetValues: [(name: String, preset: LunaConfiguration)] = LunaConfiguration.factoryPresets
    
    /// Array of `AUAudioUnitPreset` for the factory presets
    static var factoryPresets: [AUAudioUnitPreset] {
        factoryPresetValues.enumerated().map { index, config in
            AUAudioUnitPreset(number: index, name: config.name)
        }
    }
    
    /// Apply a factory preset by setting parameter values
    func useFactoryPreset(_ preset: AUAudioUnitPreset) {
        guard preset.number >= 0 && preset.number < Self.factoryPresetValues.count else {
            return
        }
        
        let config = Self.factoryPresetValues[preset.number].preset
        setValues(config)
    }
    
    /// Set all parameter values from a configuration
    func setValues(_ config: LunaConfiguration) {
        // Access parameters through the parameter tree structure
        // This assumes the parameter tree has a global group with these identifiers
        self.global.spread_amount.value = config.spreadAmount
        self.global.output_gain.value = config.outputGain
        self.global.delay_time.value = config.delayTime
        self.global.feedback.value = config.feedback
        self.global.pitch_shift.value = config.pitchShift
        self.global.pitch_shift2.value = config.pitchShift2
        self.global.highpass.value = config.highpass
        self.global.lowpass.value = config.lowpass
        self.global.diffusion_amount.value = config.diffusionAmount
        self.global.tape_noise_macro.value = config.tapeNoiseMacro
        self.global.wow_flutter_macro.value = config.wowFlutterMacro
        self.global.glitch_macro.value = config.glitchMacro
        self.global.ducking_macro.value = config.duckingMacro
        self.global.duck_attack.value = config.duckAttack
        self.global.duck_release.value = config.duckRelease
        self.global.mix.value = config.mix
        self.global.delay_time_sync.value = config.delayTimeSync
        self.global.sync_enabled.value = config.syncEnabled
        self.global.bit_crush.value = config.bitCrush
        self.global.phaser_depth.value = config.phaserDepth
    }
    
    /// Get current parameter values as a configuration
    func getCurrentConfiguration() -> LunaConfiguration {
        return LunaConfiguration(
            spreadAmount: self.global.spread_amount.value,
            outputGain: self.global.output_gain.value,
            delayTime: self.global.delay_time.value,
            feedback: self.global.feedback.value,
            pitchShift: self.global.pitch_shift.value,
            pitchShift2: self.global.pitch_shift2.value,
            highpass: self.global.highpass.value,
            lowpass: self.global.lowpass.value,
            diffusionAmount: self.global.diffusion_amount.value,
            tapeNoiseMacro: self.global.tape_noise_macro.value,
            wowFlutterMacro: self.global.wow_flutter_macro.value,
            glitchMacro: self.global.glitch_macro.value,
            duckingMacro: self.global.ducking_macro.value,
            duckAttack: self.global.duck_attack.value,
            duckRelease: self.global.duck_release.value,
            mix: self.global.mix.value,
            delayTimeSync: self.global.delay_time_sync.value,
            syncEnabled: self.global.sync_enabled.value,
            bitCrush: self.global.bit_crush.value,
            phaserDepth: self.global.phaser_depth.value
        )
    }
}