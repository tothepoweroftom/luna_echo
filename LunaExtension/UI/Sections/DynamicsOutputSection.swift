import AudioPluginUI
import AudioToolbox
import SwiftUI

struct DynamicsOutputSection: View {
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

            VStack(spacing: spacing) {
                LazyVGrid(columns: grid, spacing: spacing) {
                    ArcKnob(
                        param: parameterTree.global.ducking_macro,
                        range: 0.0...1.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity)
                    ArcKnob(
                        param: parameterTree.global.duck_attack,
                        range: 1.0...100.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity)
                    ArcKnob(
                        param: parameterTree.global.duck_release,
                        range: 50.0...1000.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity)
                      ArcKnob(
                        param: parameterTree.global.output_gain,
                        range: -96.0...12.0
                    )
                    .frame(width: knobSize, height: knobSize)
                    .frame(maxWidth: .infinity)
                }
                .frame(maxWidth: 400)
                .frame(maxWidth: .infinity)
            }
            .padding(.horizontal, hp)
            .padding(.vertical, vp)
        }
    }
}
