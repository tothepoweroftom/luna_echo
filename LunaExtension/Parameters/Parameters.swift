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
        ParameterSpec(
            address: .gain,
            identifier: "gain",
            name: "Output Gain",
            units: .linearGain,
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.25
        )

        ParameterSpec(
            address: .spread_amount,
            identifier: "spreadAmount",
            name: "Spread Amount",
            units: .percent,
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .output_gain,
            identifier: "outputGain",
            name: "Output Gain",
            units: .decibels,
            valueRange: -96.0 ... 12.0,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .delay_ms,
            identifier: "delayTime",
            name: "Delay Time",
            units: .milliseconds,
            valueRange: 1.0 ... 1000.0,
            defaultValue: 300.0
        )

        ParameterSpec(
            address: .feedback,
            identifier: "feedback",
            name: "Feedback",
            units: .percent,
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.5
        )

        ParameterSpec(
            address: .noise_amount,
            identifier: "noiseAmount",
            name: "Noise Amount",
            units: .percent,
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .pitch_shift,
            identifier: "pitchShift",
            name: "Pitch Shift",
            units: .midiNoteNumber,
            valueRange: -12.0 ... 12.0,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .highpass,
            identifier: "highpass",
            name: "Highpass",
            units: .hertz,
            valueRange: 20.0 ... 20000.0,
            defaultValue: 250.0
        )

        ParameterSpec(
            address: .lowpass,
            identifier: "lowpass",
            name: "Lowpass",
            units: .hertz,
            valueRange: 20.0 ... 20000.0,
            defaultValue: 10000.0
        )

        ParameterSpec(
            address: .random_mod,
            identifier: "randomMod",
            name: "Random Mod",
            units: .percent,
            valueRange: 0.0 ... 0.01,
            defaultValue: 0.002
        )

        ParameterSpec(
            address: .wow,
            identifier: "wow",
            name: "Wow",
            units: .rate,
            valueRange: 0.0 ... 2.0,
            defaultValue: 0.5
        )

        ParameterSpec(
            address: .wow_intensity,
            identifier: "wowIntensity",
            name: "Wow Intensity",
            units: .percent,
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.3
        )

        ParameterSpec(
            address: .flutter,
            identifier: "flutter",
            name: "Flutter",
            units: .rate,
            valueRange: 2.0 ... 20.0,
            defaultValue: 8.0
        )

        ParameterSpec(
            address: .flutter_intensity,
            identifier: "flutterIntensity",
            name: "Flutter Intensity",
            units: .percent,
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.1
        )

        ParameterSpec(
            address: .ducking,
            identifier: "ducking",
            name: "Ducking",
            units: .decibels,
            valueRange: -60.0 ... 0.0,
            defaultValue: -30.0
        )

        ParameterSpec(
            address: .attack,
            identifier: "attack",
            name: "Attack",
            units: .milliseconds,
            valueRange: 1.0 ... 100.0,
            defaultValue: 10.0
        )

        ParameterSpec(
            address: .release,
            identifier: "release",
            name: "Release",
            units: .milliseconds,
            valueRange: 50.0 ... 1000.0,
            defaultValue: 200.0
        )

        ParameterSpec(
            address: .ratio,
            identifier: "ratio",
            name: "Ratio",
            units: .generic,
            valueRange: 1.0 ... 20.0,
            defaultValue: 4.0
        )

        ParameterSpec(
            address: .glitch_rate,
            identifier: "glitchRate",
            name: "Glitch Rate",
            units: .rate,
            valueRange: 0.01 ... 5.0,
            defaultValue: 0.5
        )

        ParameterSpec(
            address: .glitch_amount,
            identifier: "glitchAmount",
            name: "Glitch Amount",
            units: .percent,
            valueRange: 0.0 ... 1.0,
            defaultValue: 0.0
        )

        ParameterSpec(
            address: .mix,
            identifier: "mix",
            name: "Mix",
            units: .percent,
            valueRange: 0.0 ... 100.0,
            defaultValue: 50.0
        )
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
