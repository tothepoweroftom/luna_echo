import SwiftUI
import AudioPluginUI

struct CharacterSection: View {
    var parameterTree: APParameterGroup

    let gridColumns = [
        GridItem(.flexible(), spacing: 16),
        GridItem(.flexible(), spacing: 16),
        GridItem(.flexible(), spacing: 16),
    ]

    var body: some View {
        GeometryReader { geometry in
            let sliderSize = min((geometry.size.width - 64) / 3, 100)  // Adaptive sizing with padding

            LazyVGrid(columns: gridColumns, spacing: 16) {
                // ArcSlider for pitch shift
                APArcSlider(param: parameterTree.global.pitch_shift)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for pitch shift 2
                APArcSlider(param: parameterTree.global.pitch_shift2)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for tape wear macro
                APArcSlider(param: parameterTree.global.tape_noise_macro)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for wow & flutter macro
                APArcSlider(param: parameterTree.global.wow_flutter_macro)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for glitch macro
                APArcSlider(param: parameterTree.global.glitch_macro)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for diffusion amount
                APArcSlider(param: parameterTree.global.diffusion_amount)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for spread amount
                APArcSlider(param: parameterTree.global.spread_amount)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // Phaser Depth
                APArcSlider(param: parameterTree.global.phaser_depth)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // Checkbox for bit crush
                APArcSlider(param: parameterTree.global.bit_crush)
                    .frame(width: sliderSize, height: sliderSize + 20)

            }
            .padding(.horizontal, 16)

        }
        .colorScheme(.dark)
    }
} 