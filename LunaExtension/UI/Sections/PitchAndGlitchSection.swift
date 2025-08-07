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
            let cellWidth = (w - hp * 2 - spacing * CGFloat(max(columns - 1, 0))) / CGFloat(columns)
            let toggleHeight: CGFloat = 44
            let gridAvailH = h - vp * 2 - toggleHeight - spacing
            let cellHeight = (gridAvailH - spacing * CGFloat(max(rows - 1, 0))) / CGFloat(max(rows, 1))
            let side = max(0, min(cellWidth, cellHeight))

            VStack(spacing: spacing) {
                LazyVGrid(columns: grid, spacing: spacing) {
                    ArcKnob(
                        param: parameterTree.global.pitch_shift,
                        range: -12.0...12.0
                    )
                    .frame(width: side, height: side)
                    .frame(maxWidth: .infinity, alignment: .center)

                    ArcKnob(
                        param: parameterTree.global.pitch_shift2,
                        range: -12.0...12.0
                    )
                    .frame(width: side, height: side)
                    .frame(maxWidth: .infinity, alignment: .center)

                    ArcKnob(
                        param: parameterTree.global.glitch_macro,
                        range: 0.0...1.0
                    )
                    .frame(width: side, height: side)
                    .frame(maxWidth: .infinity, alignment: .center)

                    ArcKnob(
                        param: parameterTree.global.phaser_depth,
                        range: 0.0...1.0
                    )
                    .frame(width: side, height: side)
                    .frame(maxWidth: .infinity, alignment: .center)
                }
                OnOffSwitch(param: parameterTree.global.bit_crush)
                    .frame(height: toggleHeight)
                    .padding(.top, spacing)
            }
            .padding(.horizontal, hp)
            .padding(.vertical, vp)
        }
    }
}
