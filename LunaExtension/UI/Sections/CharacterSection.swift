import AudioPluginUI
import SwiftUI

struct CharacterSection: View {
    var parameterTree: APParameterGroup

    let gridColumns = [
        GridItem(.flexible(), spacing: 16),
        GridItem(.flexible(), spacing: 16),
        GridItem(.flexible(), spacing: 16),
    ]

    var body: some View {
        VStack {
            Grid {
                GridRow {
                    ArcKnob(
                        param: parameterTree.global.tape_noise_macro,
                        range: 0.0...1.0
                    )
                    .frame(maxWidth: .infinity)

                    ArcKnob(
                        param: parameterTree.global.wow_flutter_macro,
                        range: 0.0...1.0
                    )
                    .frame(maxWidth: .infinity)
                }

                GridRow {
                    ArcKnob(
                        param: parameterTree.global.diffusion_amount,
                        range: 0.0...0.7
                    )
                    .frame(maxWidth: .infinity)

                    ArcKnob(
                        param: parameterTree.global.spread_amount,
                        range: 0.0...1.0
                    )
                    .frame(maxWidth: .infinity)
                }
            }
            TwoHandledParameterSlider(
                lowParam: parameterTree.global.highpass,
                highParam: parameterTree.global.lowpass,
                range: 20.0...20000.0
            )
        }
        .padding()
    }
}
