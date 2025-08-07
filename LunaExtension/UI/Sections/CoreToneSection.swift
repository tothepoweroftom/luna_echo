import SwiftUI
import AudioPluginUI

struct CoreToneSection: View {
    var parameterTree: APParameterGroup
    var body: some View {
        GeometryReader { geometry in
            let w = geometry.size.width
            let h = geometry.size.height
            let spacing: CGFloat = 40
            let pad: CGFloat = 12
            let mixSliderHeight = max(CGFloat(72), min(h * 0.18, 120))
            let availableHeightForXY = max(0, h - pad * 2 - mixSliderHeight - spacing)
            let availableWidthForXY = max(0, w - pad * 2)
            let side = max(0, min(availableWidthForXY, availableHeightForXY))

            VStack(spacing: spacing) {
                APXYPad(
                    paramX: parameterTree.global.delay_time,
                    paramY: parameterTree.global.feedback,
                    paramXSynced: parameterTree.global.delay_time_sync,
                    synced: parameterTree.global.sync_enabled
                )
                .frame(width: side, height: side)
                .frame(maxWidth: .infinity, alignment: .center)

                APSlider(
                    param: parameterTree.global.mix,
                    horizontal: false
                )
                .frame(width: side, height: mixSliderHeight)
                .frame(maxWidth: .infinity, alignment: .center)
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity)
            .padding(.horizontal, pad)
            .padding(.vertical, pad)
        }
    }
} 
