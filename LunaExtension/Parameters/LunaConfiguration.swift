//
//  LunaConfiguration.swift
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

import AudioToolbox

/// Configuration struct that holds all parameter values for Luna Echo presets
public struct LunaConfiguration {
    public let spreadAmount: AUValue
    public let outputGain: AUValue
    public let delayTime: AUValue
    public let feedback: AUValue
    public let pitchShift: AUValue
    public let pitchShift2: AUValue
    public let highpass: AUValue
    public let lowpass: AUValue
    public let diffusionAmount: AUValue
    public let tapeNoiseMacro: AUValue
    public let wowFlutterMacro: AUValue
    public let glitchMacro: AUValue
    public let duckingMacro: AUValue
    public let duckAttack: AUValue
    public let duckRelease: AUValue
    public let mix: AUValue
    public let delayTimeSync: AUValue
    public let syncEnabled: AUValue
    public let bitCrush: AUValue
    public let phaserDepth: AUValue
    
    public init(
        spreadAmount: AUValue = 0.0,
        outputGain: AUValue = 0.0,
        delayTime: AUValue = 300.0,
        feedback: AUValue = 0.5,
        pitchShift: AUValue = 0.0,
        pitchShift2: AUValue = 0.0,
        highpass: AUValue = 20.0,
        lowpass: AUValue = 20000.0,
        diffusionAmount: AUValue = 0.0,
        tapeNoiseMacro: AUValue = 0.0,
        wowFlutterMacro: AUValue = 0.0,
        glitchMacro: AUValue = 0.0,
        duckingMacro: AUValue = 0.0,
        duckAttack: AUValue = 10.0,
        duckRelease: AUValue = 100.0,
        mix: AUValue = 0.5,
        delayTimeSync: AUValue = 4.0,
        syncEnabled: AUValue = 0.0,
        bitCrush: AUValue = 0.0,
        phaserDepth: AUValue = 0.0
    ) {
        self.spreadAmount = spreadAmount
        self.outputGain = outputGain
        self.delayTime = delayTime
        self.feedback = feedback
        self.pitchShift = pitchShift
        self.pitchShift2 = pitchShift2
        self.highpass = highpass
        self.lowpass = lowpass
        self.diffusionAmount = diffusionAmount
        self.tapeNoiseMacro = tapeNoiseMacro
        self.wowFlutterMacro = wowFlutterMacro
        self.glitchMacro = glitchMacro
        self.duckingMacro = duckingMacro
        self.duckAttack = duckAttack
        self.duckRelease = duckRelease
        self.mix = mix
        self.delayTimeSync = delayTimeSync
        self.syncEnabled = syncEnabled
        self.bitCrush = bitCrush
        self.phaserDepth = phaserDepth
    }
    
    /// Default configuration - clean delay
    public static let `default` = LunaConfiguration()
    
    /// Factory preset configurations
    public static let factoryPresets: [(name: String, preset: LunaConfiguration)] = [
        ("Default", LunaConfiguration()),
        
        ("Vintage Tape", LunaConfiguration(
            delayTime: 250.0,
            feedback: 0.4,
            highpass: 80.0,
            lowpass: 8000.0,
            tapeNoiseMacro: 0.3,
            wowFlutterMacro: 0.25,
            mix: 0.4
        )),
        
        ("Glitchy Repeat", LunaConfiguration(
            delayTime: 120.0,
            feedback: 0.7,
            pitchShift: 7.0,
            diffusionAmount: 0.3,
            glitchMacro: 0.6,
            mix: 0.6,
            bitCrush: 0.4
        )),
        
        ("Ambience", LunaConfiguration(
            spreadAmount: 0.8,
            delayTime: 450.0,
            feedback: 0.3,
            lowpass: 4000.0,
            diffusionAmount: 0.7,
            mix: 0.3
        ))
    ]
}