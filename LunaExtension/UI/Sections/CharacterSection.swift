import AudioPluginUI
import SwiftUI

struct CharacterSection: View {
    var parameterTree: APParameterGroup

    var body: some View {
        GeometryReader { proxy in
            let w = proxy.size.width
            let h = proxy.size.height
            let spacing: CGFloat = 12
            let hp: CGFloat = 10
            let vp: CGFloat = 10
            let columns: Int = 2
            let grid = Array(repeating: GridItem(.flexible(), spacing: spacing), count: columns)
            let items = 4
            let rows = Int(ceil(Double(items) / Double(columns)))
            
            // Standardized layout calculations across all sections
            let cellWidth = (w - hp * 2 - spacing * CGFloat(max(columns - 1, 0))) / CGFloat(columns)
            let reservedHeight: CGFloat = 60 // Consistent reserved space for all sections
            let gridAvailH = h - vp * 2 - reservedHeight - spacing
            let cellHeight = (gridAvailH - spacing * CGFloat(max(rows - 1, 0))) / CGFloat(max(rows, 1))
            let knobSize = max(60, min(cellWidth, cellHeight)) // Min 60px, responsive to space
            
            let sliderHeight: CGFloat = 48

            VStack(spacing: spacing) {
                LazyVGrid(columns: grid, spacing: spacing) {
                    ArcKnob(
                        param: parameterTree.global.tape_noise_macro,
                        range: 0.0...1.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity)

                    ArcKnob(
                        param: parameterTree.global.wow_flutter_macro,
                        range: 0.0...1.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity)

                    ArcKnob(
                        param: parameterTree.global.diffusion_amount,
                        range: 0.0...0.7
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity)

                    ArcKnob(
                        param: parameterTree.global.spread_amount,
                        range: 0.0...1.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity)
                }
                .frame(maxWidth: 400)
                .frame(maxWidth: .infinity)
                
                TwoHandledParameterSlider(
                    lowParam: parameterTree.global.highpass,
                    highParam: parameterTree.global.lowpass,
                    range: 20.0...20000.0
                )
                .frame(height: sliderHeight)
                .frame(maxWidth: 400)
                .padding(.top, spacing)
            }
            .padding(.horizontal, hp)
            .padding(.vertical, vp)
        }
    }
}
