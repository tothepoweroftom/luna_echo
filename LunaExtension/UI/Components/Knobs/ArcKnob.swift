import AVFoundation
import Controls
import SwiftUI

#if os(iOS) || os(tvOS)
    import UIKit

    public typealias PlatformImage = UIImage
#elseif os(macOS)
    import AppKit

    public typealias PlatformImage = NSImage
#endif

extension Color {
    // background #0B1119 in rgb
    static let navy = Color(hex: "#0B1119")
    static let accent = Color(hex: "#7496B1")
    static let text = Color(hex: "#E7FBFF")
    static let accentRed = Color(hex: "#AD4A44")
}

public struct ArcKnob: View {
    @ObservedObject var param: ObservableAUParameter
    var range: ClosedRange<Float>
    @State private var isDragging = false

    var backgroundColor: Color = .gray.opacity(0.3)
    var foregroundColor: Color = .text
    var glowColor: Color = .text.opacity(0.5)
    var shadowColor: Color = .text.opacity(0.3)
    var backgroundImage: PlatformImage?  // Changed to PlatformImage

    private let startAngle: Double = -225
    private let endAngle: Double = 45

    init(
        param: ObservableAUParameter, range: ClosedRange<Float>,
        backgroundImage: PlatformImage? = nil
    ) {
        self.param = param
        self.range = range
        self.backgroundImage = backgroundImage
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
                    // Background Image
                    if let image = backgroundImage {
                        #if os(iOS) || os(tvOS)

                            Image(uiImage: image)
                                .resizable()
                                .aspectRatio(contentMode: .fit)
                                .padding(10)
                        #elseif os(macOS)
                            Image(nsImage: image)
                                .resizable()
                                .aspectRatio(contentMode: .fit)
                                .padding(10)
                        #endif
                    }

                    // Background Arc
                    Arc(
                        startAngle: .degrees(startAngle), endAngle: .degrees(endAngle),
                        clockwise: false
                    )
                    .stroke(backgroundColor, lineWidth: 1)
                    .padding(10)

                    // Value Arc
                    Arc(
                        startAngle: .degrees(startAngle),
                        endAngle: .degrees(startAngle + (endAngle - startAngle) * normalizedValue),
                        clockwise: false
                    )
                    .stroke(foregroundColor, lineWidth: 3)
                    .padding(10)

                    // Glow effect
                    Arc(
                        startAngle: .degrees(startAngle),
                        endAngle: .degrees(startAngle + (endAngle - startAngle) * normalizedValue),
                        clockwise: false
                    )
                    .stroke(glowColor, lineWidth: 6)
                    .padding(10)
                    .blur(radius: 5)
                }
                .drawingGroup()
                .aspectRatio(1, contentMode: .fit)
            }
            .aspectRatio(1, contentMode: .fit)

            Spacer()
                .frame(height: 1)

            Text(
                isDragging
                    ? String(format: "%.2f", locale: Locale(identifier: "en_US_POSIX"), param.value)
                    : "\(param.displayName)"
            )
            .font(.custom("AzeretMono-Light", size: 11))
            .foregroundColor(.text)
            .minimumScaleFactor(0.5)
            .lineLimit(1)
            .shadow(color: glowColor.opacity(0.2), radius: 1, x: 6, y: -3)
        }
    }
}

public extension ArcKnob {
    func backgroundColor(_ backgroundColor: Color) -> ArcKnob {
        var copy = self
        copy.backgroundColor = backgroundColor
        return copy
    }

    func foregroundColor(_ foregroundColor: Color) -> ArcKnob {
        var copy = self
        copy.foregroundColor = foregroundColor
        return copy
    }

    func glowColor(_ glowColor: Color) -> ArcKnob {
        var copy = self
        copy.glowColor = glowColor
        return copy
    }

    func shadowColor(_ shadowColor: Color) -> ArcKnob {
        var copy = self
        copy.shadowColor = shadowColor
        return copy
    }

    func backgroundImage(_ image: PlatformImage) -> ArcKnob {
        var copy = self
        copy.backgroundImage = image
        return copy
    }
} 