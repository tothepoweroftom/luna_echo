import AVFoundation
import Controls
import SwiftUI

public struct MoonKnob: View {
    @ObservedObject var param: ObservableAUParameter
    var range: ClosedRange<Float> = 0.0...1.0
    @State private var isDragging = false

    var backgroundColor: Color = .clear
    var foregroundColor: Color = .text

    var specifier: String {
        switch param.unit {
        case .midiNoteNumber:
            return "%.0f"
        default:
            return "%.2f"
        }
    }

    init(param: ObservableAUParameter, range: ClosedRange<Float> = 0.0...1.0) {
        self.param = param
        self.range = range
    }

    var normalizedValue: Double {
        Double((param.value - range.lowerBound) / (range.upperBound - range.lowerBound))
    }

    var offsetX: CGFloat {
        -sin(normalizedValue * 1.6 * .pi + 0.2 * .pi) / 2.0 * 0.75
    }

    var offsetY: CGFloat {
        cos(normalizedValue * 1.6 * .pi + 0.2 * .pi) / 2.0 * 0.75
    }

    public var body: some View {
        VStack {
            Control(
                value: $param.value, in: range,
                geometry: .twoDimensionalDrag(xSensitivity: 0, ySensitivity: 1),
                onStarted: { self.isDragging = true },
                onEnded: { self.isDragging = false }
            ) { geo in
                ZStack(alignment: .center) {
                    Circle()
                        .foregroundColor(backgroundColor)
                        .padding(5)
                    Circle()
                        .foregroundColor(foregroundColor)
                        .padding(5)
                        .offset(y: -CGFloat(normalizedValue) * geo.size.height)
                }.drawingGroup()
                    .clipShape(Circle())
                    .aspectRatio(1, contentMode: .fit)
            }

            .aspectRatio(CGSize(width: 1, height: 1), contentMode: .fit)
            Spacer()
            Text(
                isDragging
                    ? String(format: "%.2f", locale: Locale(identifier: "en_US_POSIX"), param.value)
                    : "\(param.displayName)"
            )
            .font(.custom("AzeretMono-Light", size: 12))
            .foregroundColor(Color.accent)
            .minimumScaleFactor(0.5)
            .lineLimit(1)
        }
    }
}

public extension MoonKnob {
    func backgroundColor(_ backgroundColor: Color) -> MoonKnob {
        var copy = self
        copy.backgroundColor = backgroundColor
        return copy
    }

    func foregroundColor(_ foregroundColor: Color) -> MoonKnob {
        var copy = self
        copy.foregroundColor = foregroundColor
        return copy
    }
}
