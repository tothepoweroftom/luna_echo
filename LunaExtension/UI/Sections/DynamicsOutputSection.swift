import SwiftUI
import AudioToolbox
import AudioPluginUI

struct DynamicsOutputSection: View {
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
                // ArcSlider for ducking macro
                APArcSlider(param: parameterTree.global.ducking_macro)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for duck attack
                APArcSlider(param: parameterTree.global.duck_attack)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for duck release
                APArcSlider(param: parameterTree.global.duck_release)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // ArcSlider for output gain
                APArcSlider(param: parameterTree.global.output_gain)
                    .frame(width: sliderSize, height: sliderSize + 20)

                APArcSlider(param: parameterTree.global.lowpass)
                    .frame(width: sliderSize, height: sliderSize + 20)

                APArcSlider(param: parameterTree.global.highpass)
                    .frame(width: sliderSize, height: sliderSize + 20)

                // TwoHandledParameterSlider for filters
                // TwoHandledParameterSlider(
                //     lowParam: parameterTree.global.highpass,
                //     highParam: parameterTree.global.lowpass,
                //     horizontal: true,
                //     range: 20...20000
                // )
                // .frame(height: 70)
            }
            .padding(.horizontal, 16)
        }
        .colorScheme(.dark)
    }
} 