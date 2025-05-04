//
//  LunaExtensionMainView.swift
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

import SwiftUI

struct LunaExtensionMainView: View {
    var parameterTree: ObservableAUParameterGroup

    var body: some View {
        ScrollView {
            VStack(spacing: 20) {
                // Main Controls Section
                GroupBox("Main Controls") {
                    VStack(spacing: 12) {
                        ParameterSlider(param: parameterTree.global.delayTime)
                        ParameterSlider(param: parameterTree.global.feedback)
                        ParameterSlider(param: parameterTree.global.mix)
                    }
                }

                // Tape Characteristics Section
                GroupBox("Tape Characteristics") {
                    VStack(spacing: 12) {
                        ParameterSlider(param: parameterTree.global.wow)
                        ParameterSlider(param: parameterTree.global.wowIntensity)
                        ParameterSlider(param: parameterTree.global.flutter)
                        ParameterSlider(param: parameterTree.global.flutterIntensity)
                        ParameterSlider(param: parameterTree.global.randomMod)
                        ParameterSlider(param: parameterTree.global.noiseAmount)
                    }
                }

                // Filtering Section
                GroupBox("Filtering") {
                    VStack(spacing: 12) {
                        ParameterSlider(param: parameterTree.global.highpass)
                        ParameterSlider(param: parameterTree.global.lowpass)
                    }
                }

                // Glitch Section
                GroupBox("Glitch Effects") {
                    VStack(spacing: 12) {
                        ParameterSlider(param: parameterTree.global.glitchRate)
                        ParameterSlider(param: parameterTree.global.glitchAmount)
                        ParameterSlider(param: parameterTree.global.pitchShift)
                    }
                }

                // Dynamics Section
                GroupBox("Dynamics") {
                    VStack(spacing: 12) {
                        ParameterSlider(param: parameterTree.global.ducking)
                        ParameterSlider(param: parameterTree.global.attack)
                        ParameterSlider(param: parameterTree.global.release)
                        ParameterSlider(param: parameterTree.global.ratio)
                    }
                }

                // Output Section
                GroupBox("Output") {
                    VStack(spacing: 12) {
                        ParameterSlider(param: parameterTree.global.outputGain)
                        ParameterSlider(param: parameterTree.global.spreadAmount)
                    }
                }
            }
            .padding()
        }
    }
}
