import AVFoundation
import SwiftUI

struct TwoHandledParameterSlider: View {
    @ObservedObject var lowParam: ObservableAUParameter
    @ObservedObject var highParam: ObservableAUParameter
    public var horizontal: Bool
    public var range: ClosedRange<Float>
    @State private var isDragging = false

    @State private var lowHandle: CGFloat = 0
    @State private var highHandle: CGFloat = 1

    private let middleFrequency: Float = 635
    private let thumbSize: CGFloat = 24

    // CRT-style colors
    var backgroundColor: Color = .gray.opacity(0.3)
    var foregroundColor: Color = .text
    var glowColor: Color = .text.opacity(0.5)
    var shadowColor: Color = .text.opacity(0.3)

    var body: some View {
        VStack(alignment: .leading, spacing: 4) {
            GeometryReader { geometry in
                ZStack(alignment: .leading) {
                    // Track
                    Rectangle()
                        .fill(backgroundColor)
                        .frame(width: horizontal ? nil : 1, height: horizontal ? 1 : nil)

                    // Selected Range
                    Rectangle()
                        .fill(foregroundColor)
                        .frame(
                            width: horizontal
                                ? CGFloat(highHandle - lowHandle) * geometry.size.width : 2,
                            height: horizontal
                                ? 2 : CGFloat(highHandle - lowHandle) * geometry.size.height
                        )
                        .offset(
                            x: horizontal ? CGFloat(lowHandle) * geometry.size.width : 0,
                            y: horizontal ? 0 : CGFloat(1 - highHandle) * geometry.size.height
                        )

                    Rectangle()
                        .fill(foregroundColor.opacity(1))
                        .frame(
                            width: horizontal
                                ? CGFloat(highHandle - lowHandle) * geometry.size.width : 4,
                            height: horizontal
                                ? 4 : CGFloat(highHandle - lowHandle) * geometry.size.height
                        )
                        .offset(
                            x: horizontal ? CGFloat(lowHandle) * geometry.size.width : 0,
                            y: horizontal ? 0 : CGFloat(1 - highHandle) * geometry.size.height
                        )
                        .blur(radius: 4)

                    // Low Handle
                    ThumbView()
                        .position(
                            x: horizontal
                                ? CGFloat(lowHandle) * geometry.size.width
                                : geometry.size.width / 2,
                            y: horizontal
                                ? geometry.size.height / 2
                                : CGFloat(1 - lowHandle) * geometry.size.height
                        )
                        .gesture(
                            DragGesture()
                                .onChanged { gesture in
                                    isDragging = true
                                    lowHandle = min(
                                        highHandle,
                                        max(
                                            0,
                                            horizontal
                                                ? gesture.location.x / geometry.size.width
                                                : 1 - gesture.location.y / geometry.size.height))
                                    updateParams()
                                }
                                .onEnded { _ in isDragging = false }
                        )

                    // High Handle
                    ThumbView()
                        .position(
                            x: horizontal
                                ? CGFloat(highHandle) * geometry.size.width
                                : geometry.size.width / 2,
                            y: horizontal
                                ? geometry.size.height / 2
                                : CGFloat(1 - highHandle) * geometry.size.height
                        )
                        .gesture(
                            DragGesture()
                                .onChanged { gesture in
                                    isDragging = true
                                    highHandle = max(
                                        lowHandle,
                                        min(
                                            1,
                                            horizontal
                                                ? gesture.location.x / geometry.size.width
                                                : 1 - gesture.location.y / geometry.size.height))
                                    updateParams()
                                }
                                .onEnded { _ in isDragging = false }
                        )
                }
            }
            .frame(height: thumbSize)
            GeometryReader { geometry in
                Text(
                    isDragging
                        ? String(format: "%.0f - %.0f Hz", lowParam.value, highParam.value)
                        : "\(lowParam.displayName) - \(highParam.displayName)"
                )
                .font(
                    .custom("AzeretMono-Light", size: min(max(geometry.size.width * 0.02, 8), 15))
                )
                .foregroundColor(.text)
                .lineLimit(1)
                .shadow(color: glowColor, radius: 2, x: 0, y: 0)
            }
        }
        .frame(width: horizontal ? nil : thumbSize)
        .onAppear {
            lowHandle = CGFloat(
                log2(lowParam.value / middleFrequency) / log2(range.upperBound / range.lowerBound)
                    + 0.5)
            highHandle = CGFloat(
                log2(highParam.value / middleFrequency) / log2(range.upperBound / range.lowerBound)
                    + 0.5)
        }
    }

    private func updateParams() {
        let lowOctave = Float(lowHandle - 0.5) * log2(range.upperBound / range.lowerBound)
        let highOctave = Float(highHandle - 0.5) * log2(range.upperBound / range.lowerBound)

        lowParam.value = middleFrequency * pow(2, lowOctave)
        highParam.value = middleFrequency * pow(2, highOctave)
    }
} 