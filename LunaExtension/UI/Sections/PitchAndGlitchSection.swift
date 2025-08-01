import AudioPluginUI
import SwiftUI

struct PitchAndGlitchSection: View {
    var parameterTree: APParameterGroup

    var body: some View {
        VStack {
            Grid {
                GridRow {
                    ArcKnob(
                        param: parameterTree.global.pitch_shift,
                        range: -12.0...12.0
                    )
                    .frame(maxWidth: .infinity)
                    
                    ArcKnob(
                        param: parameterTree.global.pitch_shift2,
                        range: -12.0...12.0
                    )
                    .frame(maxWidth: .infinity)
                }
                
                GridRow {
                    ArcKnob(
                        param: parameterTree.global.glitch_macro,
                        range: 0.0...1.0
                    )
                    .frame(maxWidth: .infinity)
                    
                    ArcKnob(
                        param: parameterTree.global.phaser_depth,
                        range: 0.0...1.0
                    )
                    .frame(maxWidth: .infinity)
                }
            }
            
            OnOffSwitch(param: parameterTree.global.bit_crush)
                .padding(.top)
        }
        .padding()
    }
}
