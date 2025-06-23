import AVFoundation
import SwiftUI

struct PitchShiftDropdown: View {
    @ObservedObject var pitchParam: ObservableAUParameter
    let pitchOptions = Array(-12...12)

    var body: some View {
        GeometryReader { geometry in
            Menu {
                ForEach(pitchOptions, id: \.self) { pitch in
                    Button(action: {
                        pitchParam.value = Float(pitch)
                    }) {
                        Text("\(pitch) semitones")
                            .foregroundColor(pitch == Int(pitchParam.value) ? .accent : .primary)
                    }
                }
            } label: {
                HStack {
                    Text("Pitch:")
                    Text("\(Int(pitchParam.value))")
                        .foregroundColor(.accent)
                    Image(systemName: "chevron.down")
                        .font(
                            .custom(
                                "AzeretMono-Light",
                                size: min(max(geometry.size.height * 0.02, 7), 15))
                        )
                        .foregroundColor(.accent)
                }
                .padding(.horizontal, 12)
                .padding(.vertical, 8)
                .cornerRadius(8)
                .overlay(
                    RoundedRectangle(cornerRadius: 8)
                        .stroke(Color.accent, lineWidth: 0.5)
                )
            }
        }
    }
} 