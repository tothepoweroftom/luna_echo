import AVFoundation
import SwiftUI

struct MoonParameterSlider: View {
    @ObservedObject var param: ObservableAUParameter
    @State private var dragOffset: CGFloat = 0.0

    var body: some View {
        VStack {
            GeometryReader { geometry in
                ZStack {
                    Circle()
                        .stroke(lineWidth: 2)
                    Circle()
                        .trim(
                            from: 0,
                            to: CGFloat(self.param.value) / CGFloat(self.param.max)
                                - CGFloat(self.param.min)
                        )
                        .fill()
                        .rotationEffect(Angle(degrees: -90))
                        .frame(width: geometry.size.width - 4, height: geometry.size.height - 4)
                        .gesture(
                            DragGesture(minimumDistance: 0)
                                .onChanged { value in
                                    //                                let dragAmount = value.translation.height / geometry.size.height
                                    //                                let newValue = CGFloat(self.param.value)/CGFloat(self.param.max)-CGFloat(self.param.min) - dragAmount
                                    ////                                self.param.valueNormalized = min(max(newValue, 0), 1)
                                    self.dragOffset = value.translation.height
                                    print(self.dragOffset)
                                }
                                .onEnded { _ in
                                    self.dragOffset = 0
                                })
                }
            }
            .aspectRatio(1, contentMode: .fit)
            Text("\(param.displayName): \(param.value, specifier: specifier)")
                .frame(maxHeight: 20)
        }
        .padding()
    }

    var specifier: String {
        switch param.unit {
        case .midiNoteNumber:
            return "%.0f"
        default:
            return "%.2f"
        }
    }
} 