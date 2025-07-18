import AVFoundation
import SwiftUI
import AudioPluginUI

struct PitchShiftDropdown: View {
    @ObservedObject var pitchParam: ObservableAUParameter
    let pitchOptions = Array(-12...12)

    var body: some View {
        HStack {
            Text("Pitch: \(Int(pitchParam.value))")
                .foregroundColor(.blue)
        }
        .padding()
        .background(Color.gray.opacity(0.1))
        .cornerRadius(8)
    }
} 