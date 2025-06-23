//
//  Parameters.swift
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

import AudioToolbox
import Foundation

let LunaExtensionParameterSpecs = ParameterTreeSpec {
    ParameterGroupSpec(identifier: "global", name: "Global") {
        // Visible Parameters based on updated Macalla.dsp and Addresses.h

        ParameterSpec(
            address: .spread_amount,
            identifier: "spread_amount",
            name: "Spread Amount",
            units: .percent, // Assuming 0-1 maps to 0-100%
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .output_gain,
            identifier: "output_gain",
            name: "Output Gain",
            units: .decibels,
            valueRange: -96.0 ... 12.0,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .delay_time,
            identifier: "delay_time",  // Kept identifier for consistency
            name: "Delay Time",
            units: .milliseconds,
            valueRange: 1.0 ... 1000.0,
            defaultValue: 300.0,
            flags: [.flag_IsWritable, .flag_IsReadable, .flag_DisplayLogarithmic] // Log scale often good for time
        )

        ParameterSpec(
            address: .feedback,
            identifier: "feedback",
            name: "Feedback",
            units: .percent, // Assuming 0-1 maps to 0-100%
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.5
        )

        ParameterSpec(
            address: .pitch_shift,
            identifier: "pitch_shift",
            name: "Pitch Shift",
            units: .generic, // Use generic units
            valueRange: -12.0 ... 12.0,
            defaultValue: 0.0,
            unitName: "st" // Specify unit name as semitones
        )

        ParameterSpec(
            address: .pitch_shift2,
            identifier: "pitch_shift2",
            name: "Pitch Shift 2",
            units: .generic,  // Use generic units
            valueRange: -12.0...12.0,
            defaultValue: 0.0,
            unitName: "st"  // Specify unit name as semitones
        )

        ParameterSpec(
            address: .highpass,
            identifier: "highpass",
            name: "Highpass",
            units: .hertz,
            valueRange: 20.0 ... 20000.0,
            defaultValue: 250.0,
            flags: [.flag_IsWritable, .flag_IsReadable, .flag_DisplayLogarithmic] // Log scale for frequency
        )

        ParameterSpec(
            address: .lowpass,
            identifier: "lowpass",
            name: "Lowpass",
            units: .hertz,
            valueRange: 20.0 ... 20000.0,
            defaultValue: 10000.0,
            flags: [.flag_IsWritable, .flag_IsReadable, .flag_DisplayLogarithmic] // Log scale for frequency
        )

        ParameterSpec(
            address: .diffusion_amount, // New
            identifier: "diffusion_amount",
            name: "Diffusion",
            units: .percent, // Maps 0-0.7 in Faust to 0-100% UI
            valueRange: 0.0 ... 0.7,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .tape_noise_macro,  // New Macro
            identifier: "tape_noise_macro",
            name: "Tape Noise",
            units: .percent,  // Maps 0-1 in Faust to 0-100% UI
            valueRange: 0.0...1.0,
            defaultValue: 0.1  // Default matches Faust
        )

        ParameterSpec(
            address: .wow_flutter_macro,  // New Macro
            identifier: "wow_flutter_macro",
            name: "Wow & Flutter",
            units: .percent, // Maps 0-1 in Faust to 0-100% UI
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.1 // Default matches Faust
        )

        ParameterSpec(
            address: .glitch_macro, // New Macro
            identifier: "glitch_macro",
            name: "Glitch",
            units: .percent, // Maps 0-1 in Faust to 0-100% UI
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .ducking_macro, // New Macro
            identifier: "ducking_macro",
            name: "Ducking Amount",
            units: .percent, // Maps 0-1 in Faust to 0-100% UI
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .duck_attack, // Renamed from 'attack'
            identifier: "duck_attack",
            name: "Ducking Attack",
            units: .milliseconds,
            valueRange: 1.0 ... 100.0,
            defaultValue: 10.0,
            flags: [.flag_IsWritable, .flag_IsReadable, .flag_DisplayLogarithmic] // Log scale often good for time
        )

        ParameterSpec(
            address: .duck_release, // Renamed from 'release'
            identifier: "duck_release",
            name: "Ducking Release",
            units: .milliseconds,
            valueRange: 50.0 ... 1000.0,
            defaultValue: 200.0,
            flags: [.flag_IsWritable, .flag_IsReadable, .flag_DisplayLogarithmic] // Log scale often good for time
        )

        ParameterSpec(
            address: .mix,
            identifier: "mix",
            name: "Mix",
            units: .percent, // Maps 0-100 in Faust code to 0-100% UI
            valueRange: 0.0 ... 100.0,
            defaultValue: 50.0
        )

        // Sync Parameters (New)
        ParameterSpec(
            address: .delay_time_sync,
            identifier: "delay_time_sync",
            name: "Time Sync",
            units: .indexed,
            valueRange: 0 ... 17,
            defaultValue: 4, // Default to 1/4 note?
            valueStrings: ["1/64T", "1/64", "1/32T", "1/32", "1/16T", "1/16", "1/8T", "1/8", "1/4T", "1/4", "1/2T", "1/2", "1/1T", "1/1", "2/1T", "2/1", "4/1T", "4/1"]
        )

        ParameterSpec(
            address: .sync_enabled,
            identifier: "sync_enabled",
            name: "Sync",
            units: .boolean,
            valueRange: 0 ... 1,
            defaultValue: 0
        )

        ParameterSpec(
            address: .bit_crush,
            identifier: "bit_crush",
            name: "Bit Crush",
            units: .boolean,
            valueRange: 0 ... 1,
            defaultValue: 0
        )

        ParameterSpec(
            address: .phaser_depth,
            identifier: "phaser_depth",
            name: "Phaser Depth",
            units: .percent,
            valueRange: 0.0...1.0,
            defaultValue: 0.0
        )

        // Removed ParameterSpecs for hidden parameters:
        // noise_amount, random_mod, wow, wow_intensity,
        // flutter, flutter_intensity, ducking (threshold),
        // ratio, glitch_rate, glitch_amount
    }
}

extension ParameterSpec {
    init(
        address: LunaExtensionParameterAddress,
        identifier: String,
        name: String,
        units: AudioUnitParameterUnit,
        valueRange: ClosedRange<AUValue>,
        defaultValue: AUValue,
        unitName: String? = nil,
        flags: AudioUnitParameterOptions = [AudioUnitParameterOptions.flag_IsWritable, AudioUnitParameterOptions.flag_IsReadable],
        valueStrings: [String]? = nil,
        dependentParameters: [NSNumber]? = nil
    ) {
        self.init(address: address.rawValue,
                  identifier: identifier,
                  name: name,
                  units: units,
                  valueRange: valueRange,
                  defaultValue: defaultValue,
                  unitName: unitName,
                  flags: flags,
                  valueStrings: valueStrings,
                  dependentParameters: dependentParameters)
    }
}
