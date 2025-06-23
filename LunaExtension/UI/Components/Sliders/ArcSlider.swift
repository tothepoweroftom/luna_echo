import AVFoundation
import Controls
import SwiftUI

public struct ArcSlider: View {
    @ObservedObject var param: ObservableAUParameter
    var range: ClosedRange<Float>
    @State private var isDragging = false

    var backgroundColor: Color = .gray.opacity(0.3)
    var foregroundColor: Color = .text

    private let startAngle: Double = -225
    private let endAngle: Double = 45

    init(param: ObservableAUParameter, range: ClosedRange<Float>? = nil) {
        self.param = param
        self.range = range ?? param.min...param.max
    }

    var normalizedValue: Double {
        Double((param.value - range.lowerBound) / (range.upperBound - range.lowerBound))
    }

    var specifier: String {
        switch param.unit {
        case .midiNoteNumber:
            return "%.0f"
        default:
            return "%.2f"
        }
    }

    public var body: some View {
        VStack {
            Control(
                value: $param.value, in: range,
                geometry: .twoDimensionalDrag(xSensitivity: 0, ySensitivity: 0.5),
                onStarted: { self.isDragging = true },
                onEnded: { self.isDragging = false }
            ) { _ in
                ZStack(alignment: .center) {
                    // Background Arc
                    Arc(
                        startAngle: .degrees(startAngle), endAngle: .degrees(endAngle),
                        clockwise: false
                    )
                    .stroke(backgroundColor, lineWidth: 4)
                    .padding(5)
                    // Value Arc
                    Arc(
                        startAngle: .degrees(startAngle),
                        endAngle: .degrees(startAngle + (endAngle - startAngle) * normalizedValue),
                        clockwise: false
                    )
                    .stroke(foregroundColor, lineWidth: 3)
                    .padding(5)
                }
                .drawingGroup()
                .aspectRatio(1, contentMode: .fit)
            }
            .aspectRatio(CGSize(width: 1, height: 1), contentMode: .fit)

            Spacer()

            // Display the parameter name and value
            Text(
                isDragging
                    ? String(
                        format: specifier, locale: Locale(identifier: "en_US_POSIX"), param.value)
                    : "\(param.displayName)"
            )
            .font(.custom("AzeretMono-Light", size: 12))
            .foregroundColor(Color.accent)
            .minimumScaleFactor(0.5)
            .lineLimit(1)
        }
    }
}

public extension ArcSlider {
    /// Modifier to change the background color of the slider
    /// - Parameter backgroundColor: background color
    func backgroundColor(_ backgroundColor: Color) -> ArcSlider {
        var copy = self
        copy.backgroundColor = backgroundColor
        return copy
    }

    /// Modifier to change the foreground color of the slider
    /// - Parameter foregroundColor: foreground color
    func foregroundColor(_ foregroundColor: Color) -> ArcSlider {
        var copy = self
        copy.foregroundColor = foregroundColor
        return copy
    }
} 