import SwiftUI
import AudioPluginUI

struct CoreToneSection: View {
    var parameterTree: APParameterGroup
    var body: some View {
        GeometryReader { geometry in
            let squareSize = min(geometry.size.width, geometry.size.height) - 32  // Account for padding
            let xyControlSize = squareSize * 0.9  // 90% for XY control
            let mixSliderHeight = CGFloat(30)  // 30px for mix slider

            VStack(spacing: 16) {
                // XYControl for Delay Time and Feedback
                APXYPad(
                    paramX: parameterTree.global.delay_time,
                    paramY: parameterTree.global.feedback,
                    paramXSynced: parameterTree.global.delay_time_sync,
                    synced: parameterTree.global.sync_enabled
                )
                .frame(width: xyControlSize, height: xyControlSize)
                Spacer()
                // CustomParameterSlider for Mix
                APSlider(
                    param: parameterTree.global.mix,
                    horizontal: false
                )
                .frame(width: xyControlSize, height: mixSliderHeight)
                Spacer()
            }
            .frame(width: squareSize, height: squareSize)
            .position(x: geometry.size.width / 2, y: geometry.size.height / 2)
        }
        .aspectRatio(1, contentMode: .fit)  // Force square aspect ratio
    }
} 
