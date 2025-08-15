import AudioPluginUI
import SwiftUI

struct PitchAndGlitchSection: View {
    var parameterTree: APParameterGroup

    var body: some View {
        GeometryReader { proxy in
            let w = proxy.size.width
            let h = proxy.size.height
            let spacing: CGFloat = 12
            let hp: CGFloat = 16
            let vp: CGFloat = 16
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
            
            let toggleHeight: CGFloat = 44

            VStack(spacing: spacing) {
                LazyVGrid(columns: grid, spacing: spacing) {
                    ArcKnob(
                        param: parameterTree.global.pitch_shift,
                        range: -12.0...12.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity, alignment: .center)

                    ArcKnob(
                        param: parameterTree.global.pitch_shift2,
                        range: -12.0...12.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity, alignment: .center)

                    ArcKnob(
                        param: parameterTree.global.glitch_macro,
                        range: 0.0...1.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity, alignment: .center)

                    ArcKnob(
                        param: parameterTree.global.phaser_depth,
                        range: 0.0...1.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity, alignment: .center)
                }
                .frame(maxWidth: 400)
                .frame(maxWidth: .infinity)
                
                OnOffSwitch(param: parameterTree.global.bit_crush)
                    .frame(height: toggleHeight)
                    .padding(.top, spacing)
            }
            .padding(.horizontal, hp)
            .padding(.vertical, vp)
        }
    }
}
