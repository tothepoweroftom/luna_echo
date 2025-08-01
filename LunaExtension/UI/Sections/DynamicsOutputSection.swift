import AudioPluginUI
import AudioToolbox
import SwiftUI

struct DynamicsOutputSection: View {
    var parameterTree: APParameterGroup

    var body: some View {
        VStack {
            HStack(spacing: 16) {
                ArcKnob(
                    param: parameterTree.global.ducking_macro,
                    range: 0.0...1.0
                )
                .frame(maxWidth: .infinity)
                ArcKnob(
                    param: parameterTree.global.duck_attack,
                    range: 1.0...100.0
                )
                .frame(maxWidth: .infinity)
                ArcKnob(
                    param: parameterTree.global.duck_release,
                    range: 50.0...1000.0
                )
                .frame(maxWidth: .infinity)
            }
            .frame(maxHeight: .infinity)
            CustomParameterSlider(
                param: parameterTree.global.output_gain
            )
            .frame(maxHeight: 60)
        }
        .padding()
    }
}
