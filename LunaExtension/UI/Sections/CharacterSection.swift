import SwiftUI

struct CharacterSection: View {
    var parameterTree: ObservableAUParameterGroup

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
                ArcSlider(param: parameterTree.global.pitch_shift)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for pitch shift 2
                ArcSlider(param: parameterTree.global.pitch_shift2)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for tape wear macro
                ArcSlider(param: parameterTree.global.tape_noise_macro)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for wow & flutter macro
                ArcSlider(param: parameterTree.global.wow_flutter_macro)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for glitch macro
                ArcSlider(param: parameterTree.global.glitch_macro)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for diffusion amount
                ArcSlider(param: parameterTree.global.diffusion_amount)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for spread amount
                ArcSlider(param: parameterTree.global.spread_amount)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // Phaser Depth
                ArcSlider(param: parameterTree.global.phaser_depth)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // Checkbox for bit crush
                ArcSlider(param: parameterTree.global.bit_crush)
                    .frame(width: sliderSize, height: sliderSize + 20)

            }
            .padding(.horizontal, 16)

        }
        .colorScheme(.dark)
    }
} 