//
//  ParameterSlider.swift
//  Moon DelayExtension
//
//  Created by Thomas Power on 19/03/2024.
//
import AVFoundation
import Controls
import Sliders
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

struct ThumbView: View {
    var body: some View {
        Circle()
            .foregroundColor(Color.text)
            .frame(width: 8, height: 8)
            .padding(8)
            .overlay(
                Circle()
                    .stroke(Color.accent, lineWidth: 1)
            )
    }
}

struct CustomParameterSlider: View {
    @ObservedObject var param: ObservableAUParameter
    public var horizontal: Bool
    @State private var isDragging = false

    // Access the environment's color scheme
    @Environment(\.colorScheme) var colorScheme

    var body: some View {
        // Determine adaptive foreground color
        let trackForegroundColor = colorScheme == .dark ? Color.white : Color.black

        GeometryReader { geometry in
            Group {
                if horizontal {
                    VStack {
                        ValueSlider(value: $param.value, in: param.min ... param.max, onEditingChanged: { editing in
                            self.isDragging = editing
                        })
                        .valueSliderStyle(
                            VerticalValueSliderStyle(
                                track:
                                VerticalValueTrack(
                                    // Use the adaptive color for the track foreground
                                    view: Rectangle().foregroundColor(trackForegroundColor).cornerRadius(0.0),
                                    mask: Rectangle().frame(width: 3)
                                )
                                // Keep accent for the background/fill part
                                .background(Rectangle().foregroundColor(Color.accent).cornerRadius(0).frame(width: 2)),

                                thumb: ThumbView(), // ThumbView might need similar adaptation if Color.text is fixed
                                thumbSize: CGSize(width: 10, height: 10),
                                thumbInteractiveSize: CGSize(width: 30, height: 30),
                                options: .interactiveTrack
                            )
                        )
                        Spacer()
                        // Text already uses Color.text, which might be adaptive or need changing
                        Text(isDragging ? String(format: "%.2f", locale: Locale(identifier: "en_US_POSIX"), param.value) : "\(param.displayName)")
                            .font(.custom("AzeretMono-Light", size: min(max(geometry.size.height * 0.02, 7), 15)))
                            .foregroundColor(Color.text)
                            .lineLimit(1)
                    }
                } else {
                    VStack {
                        ValueSlider(value: $param.value, in: param.min ... param.max, onEditingChanged: { editing in
                            self.isDragging = editing
                        })
                        .valueSliderStyle(
                            HorizontalValueSliderStyle(
                                track:
                                HorizontalValueTrack(
                                    // Use the adaptive color for the track foreground
                                    view: Rectangle().foregroundColor(trackForegroundColor).cornerRadius(0.0),
                                    mask: Rectangle().frame(height: 3)
                                )
                                // Keep accent for the background/fill part
                                .background(Rectangle().foregroundColor(Color.accent).cornerRadius(0).frame(height: 1)),

                                thumb: ThumbView(), // ThumbView might need similar adaptation
                                thumbSize: CGSize(width: 10, height: 10),
                                thumbInteractiveSize: CGSize(width: 30, height: 30),
                                options: .interactiveTrack
                            )
                        )
                        Spacer()
                        // Text already uses Color.text
                        Text(isDragging ? String(format: "%.2f", locale: Locale(identifier: "en_US_POSIX"), param.value) : "\(param.displayName)")
                            .font(.custom("AzeretMono-Light", size: min(max(geometry.size.width * 0.02, 11), 15)))
                            .foregroundColor(Color.text)
                            .lineLimit(1)
                    }
                }
            }
        }
    }
}

/// A SwiftUI Slider container which is bound to an ObservableAUParameter
///
/// This view wraps a SwiftUI Slider, and provides it relevant data from the Parameter, like the minimum and maximum values.
struct ParameterSlider: View {
    @ObservedObject var param: ObservableAUParameter

    var specifier: String {
        switch param.unit {
        case .midiNoteNumber:
            return "%.0f"
        default:
            return "%.2f"
        }
    }

    var body: some View {
        VStack {
            Slider(
                value: $param.value,
                in: param.min ... param.max,
                onEditingChanged: param.onEditingChanged,
                minimumValueLabel: Text("\(param.min, specifier: specifier)"),
                maximumValueLabel: Text("\(param.max, specifier: specifier)")
            ) {
                EmptyView()
            }
            Text("\(param.displayName): \(param.value, specifier: specifier)")
        }
        .padding()
    }
}

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
                        .trim(from: 0, to: CGFloat(self.param.value) / CGFloat(self.param.max) - CGFloat(self.param.min))
                        .fill()
                        .rotationEffect(Angle(degrees: -90))
                        .frame(width: geometry.size.width - 4, height: geometry.size.height - 4)
                        .gesture(DragGesture(minimumDistance: 0)
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

public struct MoonKnob: View {
    @ObservedObject var param: ObservableAUParameter
    var range: ClosedRange<Float> = 0.0 ... 1.0
    @State private var isDragging = false // Add this line to track the dragging state

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

    init(param: ObservableAUParameter, range: ClosedRange<Float> = 0.0 ... 1.0) {
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
            Control(value: $param.value, in: range,
                    geometry: .twoDimensionalDrag(xSensitivity: 0, ySensitivity: 1),
                    onStarted: { self.isDragging = true }, // Add this line
                    onEnded: { self.isDragging = false })
            { geo in // Add this line
                ZStack(alignment: .center) {
                    Circle()
                        .foregroundColor(backgroundColor)
                        .padding(5)
                    Circle()
                        .foregroundColor(foregroundColor)
                        .padding(5)
                        .offset(y: -CGFloat(normalizedValue) * geo.size.height)
                }.drawingGroup() // Drawing groups improve antialiasing of rotated indicator
                    .clipShape(Circle())
                    .aspectRatio(1, contentMode: .fit) // Make the knob square
            }

            .aspectRatio(CGSize(width: 1, height: 1), contentMode: .fit)
            Spacer()
            // Display the parameter name and value
            Text(isDragging ? String(format: "%.2f", locale: Locale(identifier: "en_US_POSIX"), param.value) : "\(param.displayName)")
                .font(.custom("AzeretMono-Light", size: 12)) // Proportional font size with constraints
                .foregroundColor(Color.accent)
                .minimumScaleFactor(0.5)
                .lineLimit(1) // Add this line
        }
    }
}

public extension MoonKnob {
    /// Modifier to change the background color of the knob
    /// - Parameter backgroundColor: background color
    func backgroundColor(_ backgroundColor: Color) -> MoonKnob {
        var copy = self
        copy.backgroundColor = backgroundColor
        return copy
    }

    /// Modifier to change the foreground color of the knob
    /// - Parameter foregroundColor: foreground color
    func foregroundColor(_ foregroundColor: Color) -> MoonKnob {
        var copy = self
        copy.foregroundColor = foregroundColor
        return copy
    }
}

struct CustomCheckbox: View {
    @Binding var isChecked: Bool
    var onToggle: () -> Void

    var body: some View {
        Button(action: {
            self.isChecked.toggle()
            self.onToggle()
        }) {
            Image(systemName: isChecked ? "music.note" : "clock")
                .foregroundColor(isChecked ? .accent : .accent)
        }
    }
}

struct XYControl: View {
    @State private var location: CGPoint = .zero
    @ObservedObject var paramX: ObservableAUParameter
    @ObservedObject var paramY: ObservableAUParameter
    @ObservedObject var paramXSynced: ObservableAUParameter
    @ObservedObject var synced: ObservableAUParameter
    var showing3D: Bool

    private let fractionPositions: [CGFloat] = [0, 1 / 17, 2 / 17, 3 / 17, 4 / 17, 5 / 17, 6 / 17, 7 / 17, 8 / 17, 9 / 17, 10 / 17, 11 / 17, 12 / 17, 13 / 17, 14 / 17, 15 / 17, 16 / 17, 1]

    var body: some View {
        GeometryReader { geometry in
            let size = min(geometry.size.width, geometry.size.height)
            VStack {
                HStack {
                    Text(currentValueText)
                        .font(.custom("AzeretMono-Light", size: min(max(size * 0.02, 9), 15)))
                        .foregroundColor(.text)
                    Spacer()
                    CustomCheckbox(isChecked: Binding(
                        get: { self.synced.value == 1 },
                        set: { self.synced.value = $0 ? 1 : 0 }
                    ), onToggle: {})
                        .frame(width: min(max(size * 0.01, 2.5), 7.5))
                }
                .padding(.bottom, 5)

                ZStack {
                    Rectangle()
                        .foregroundColor(.gray.opacity(0.1))

                    ThumbView()
                        .frame(width: 20, height: 20)
                        .position(x: location.x, y: location.y)
                }
                .frame(width: size, height: size)
                .gesture(
                    DragGesture(minimumDistance: 0)
                        .onChanged { value in
                            updateLocation(value: value, size: size)
                            updateParameters(size: size)
                        }
                )
            }
            .onAppear {
                updateLocationFromParameters(size: size)
            }
            .onChange(of: paramX.value) { _ in updateLocationFromParameters(size: size) }
            .onChange(of: paramY.value) { _ in updateLocationFromParameters(size: size) }
            .onChange(of: paramXSynced.value) { _ in updateLocationFromParameters(size: size) }
            .onChange(of: synced.value) { _ in updateLocationFromParameters(size: size) }
        }
    }

    private func updateLocationFromParameters(size: CGFloat) {
        let yPosition = calculateYPosition(size: size)
        let xPosition = calculateXPosition(size: size)
        location = CGPoint(x: xPosition, y: yPosition)
    }

    private func calculateYPosition(size: CGFloat) -> CGFloat {
        let normalizedY = CGFloat((paramY.value - paramY.min) / (paramY.max - paramY.min))
        return (1 - normalizedY) * size
    }

    private func calculateXPosition(size: CGFloat) -> CGFloat {
        if synced.value == 1 {
            let index = Int(paramXSynced.value)
            return fractionPositions[index] * size
        } else {
            let normalizedX = CGFloat((paramX.value - paramX.min) / (paramX.max - paramX.min))
            return normalizedX * size
        }
    }

    private var currentValueText: String {
        let xValue: String
        if synced.value == 1 {
            xValue = paramXSynced.valueStrings?[Int(paramXSynced.value)] ?? String(format: "%.2f", paramXSynced.value)
        } else {
            xValue = String(format: "%.2f", paramX.value)
        }
        let yValue = String(format: "%.2f", paramY.value)
        return "Time: \(xValue), Fb: \(yValue)"
    }

    private func updateLocation(value: DragGesture.Value, size: CGFloat) {
        var newX = value.location.x
        let newY = min(max(value.location.y, 0), size)

        if synced.value == 1 {
            // Snap to nearest fraction position when synced
            let nearestIndex = fractionPositions.indices.min(by: { abs(fractionPositions[$0] * size - newX) < abs(fractionPositions[$1] * size - newX) })!
            newX = fractionPositions[nearestIndex] * size
        } else {
            newX = min(max(newX, 0), size)
        }

        location = CGPoint(x: newX, y: newY)
    }

    private func updateParameters(size: CGFloat) {
        // Y parameter (always unsync)
        paramY.value = Float(1 - location.y / size) * (paramY.max - paramY.min) + paramY.min

        // X parameter (sync or unsync)
        if synced.value == 1 {
            let nearestIndex = fractionPositions.indices.min(by: { abs(fractionPositions[$0] * size - location.x) < abs(fractionPositions[$1] * size - location.x) })!
            paramXSynced.value = Float(nearestIndex)
        } else {
            paramX.value = Float(location.x / size) * (paramX.max - paramX.min) + paramX.min
        }
    }
}

struct SyncToggle: View {
    @ObservedObject var synced: ObservableAUParameter

    var body: some View {
        Toggle("Sync", isOn: Binding(
            get: { self.synced.value == 1 },
            set: { self.synced.value = $0 ? 1 : 0 }
        ))
    }
}

//
// public struct ArcKnob: View {
//    @ObservedObject var param: ObservableAUParameter
//    var range: ClosedRange<Float>
//    @State private var isDragging = false
//
//    var backgroundColor: Color = .gray.opacity(0.3)
//    var foregroundColor: Color = .text
//
//    private let startAngle: Double = -225
//    private let endAngle: Double = 45
//
//    init(param: ObservableAUParameter, range: ClosedRange<Float>) {
//        self.param = param
//        self.range = range
//    }
//
//    var normalizedValue: Double {
//        Double((param.value - range.lowerBound) / (range.upperBound - range.lowerBound))
//    }
//
//    var specifier: String {
//        switch param.unit {
//        case .midiNoteNumber:
//            return "%.0f"
//        default:
//            return "%.2f"
//        }
//    }
//
//    public var body: some View {
//        VStack {
//            Control(value: $param.value, in: range,
//                    geometry: .twoDimensionalDrag(xSensitivity: 0, ySensitivity: 0.5),
//                    onStarted: { self.isDragging = true },
//                    onEnded: { self.isDragging = false }) { _ in
//                ZStack(alignment: .center) {
//                    // Background Arc
//                    Arc(startAngle: .degrees(startAngle), endAngle: .degrees(endAngle), clockwise: false)
//                        .stroke(backgroundColor, lineWidth: 4)
//                        .padding(5)
//                    // Value Arc
//                    Arc(startAngle: .degrees(startAngle),
//                        endAngle: .degrees(startAngle + (endAngle - startAngle) * normalizedValue),
//                        clockwise: false)
//                        .stroke(foregroundColor, lineWidth: 3)
//                        .padding(5)
//                }
//                .drawingGroup()
//                .aspectRatio(1, contentMode: .fit)
//            }
//            .aspectRatio(CGSize(width: 1, height: 1), contentMode: .fit)
//
//            Spacer()
//
//            // Display the parameter name and value
//            Text(isDragging ? String(format: "%.2f", locale: Locale(identifier: "en_US_POSIX"), param.value) : "\(param.displayName)")
//                .font(.custom("AzeretMono-Light", size: 12))
//                .foregroundColor(Color.accent)
//                .minimumScaleFactor(0.5)
//                .lineLimit(1)
//        }
//    }
// }
//
// public extension ArcKnob {
//    /// Modifier to change the background color of the knob
//    /// - Parameter backgroundColor: background color
//    func backgroundColor(_ backgroundColor: Color) -> ArcKnob {
//        var copy = self
//        copy.backgroundColor = backgroundColor
//        return copy
//    }
//
//    /// Modifier to change the foreground color of the knob
//    /// - Parameter foregroundColor: foreground color
//    func foregroundColor(_ foregroundColor: Color) -> ArcKnob {
//        var copy = self
//        copy.foregroundColor = foregroundColor
//        return copy
//    }
// }
//
struct Arc: Shape {
    var startAngle: Angle
    var endAngle: Angle
    var clockwise: Bool

    func path(in rect: CGRect) -> Path {
        var path = Path()
        path.addArc(center: CGPoint(x: rect.midX, y: rect.midY),
                    radius: rect.width / 2,
                    startAngle: startAngle,
                    endAngle: endAngle,
                    clockwise: clockwise)
        return path
    }
}

public struct ArcKnob: View {
    @ObservedObject var param: ObservableAUParameter
    var range: ClosedRange<Float>
    @State private var isDragging = false

    var backgroundColor: Color = .gray.opacity(0.3)
    var foregroundColor: Color = .text
    var glowColor: Color = .text.opacity(0.5)
    var shadowColor: Color = .text.opacity(0.3)
    var backgroundImage: PlatformImage? // Changed to PlatformImage

    private let startAngle: Double = -225
    private let endAngle: Double = 45

    init(param: ObservableAUParameter, range: ClosedRange<Float>, backgroundImage: PlatformImage? = nil) {
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
            Control(value: $param.value, in: range,
                    geometry: .twoDimensionalDrag(xSensitivity: 0, ySensitivity: 0.5),
                    onStarted: { self.isDragging = true },
                    onEnded: { self.isDragging = false })
            { _ in
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
                    Arc(startAngle: .degrees(startAngle), endAngle: .degrees(endAngle), clockwise: false)
                        .stroke(backgroundColor, lineWidth: 1)
                        .padding(10)

                    // Value Arc
                    Arc(startAngle: .degrees(startAngle),
                        endAngle: .degrees(startAngle + (endAngle - startAngle) * normalizedValue),
                        clockwise: false)
                        .stroke(foregroundColor, lineWidth: 3)
                        .padding(10)

                    // Glow effect
                    Arc(startAngle: .degrees(startAngle),
                        endAngle: .degrees(startAngle + (endAngle - startAngle) * normalizedValue),
                        clockwise: false)
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

            Text(isDragging ? String(format: "%.2f", locale: Locale(identifier: "en_US_POSIX"), param.value) : "\(param.displayName)")
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

//
// struct TwoHandledParameterSlider: View {
//    @ObservedObject var lowParam: ObservableAUParameter
//    @ObservedObject var highParam: ObservableAUParameter
//    public var horizontal: Bool
//    public var range: ClosedRange<Float>
//    @State private var isDragging = false
//
//    @State private var lowHandle: CGFloat = 0
//    @State private var highHandle: CGFloat = 1
//
//    private let middleFrequency: Float = 635
//    private let thumbSize: CGFloat = 24 // 8 + 8 + 8 (inner circle + padding + stroke)
//
//    var body: some View {
//        VStack(alignment: .leading, spacing: 4) {
//            GeometryReader { geometry in
//                ZStack(alignment: .leading) {
//                    // Track
//                    Rectangle()
//                        .fill(Color.accent.opacity(0.2))
//                        .frame(width: horizontal ? nil : 2, height: horizontal ? 2 : nil)
//
//                    // Selected Range
//                    Rectangle()
//                        .fill(Color.accent)
//                        .frame(
//                            width: horizontal ? CGFloat(highHandle - lowHandle) * geometry.size.width : 2,
//                            height: horizontal ? 2 : CGFloat(highHandle - lowHandle) * geometry.size.height
//                        )
//                        .offset(
//                            x: horizontal ? CGFloat(lowHandle) * geometry.size.width : 0,
//                            y: horizontal ? 0 : CGFloat(1 - highHandle) * geometry.size.height
//                        )
//
//                    // Low Handle
//                    ThumbView()
//                        .position(
//                            x: horizontal ? CGFloat(lowHandle) * geometry.size.width : geometry.size.width / 2,
//                            y: horizontal ? geometry.size.height / 2 : CGFloat(1 - lowHandle) * geometry.size.height
//                        )
//                        .gesture(
//                            DragGesture()
//                                .onChanged { gesture in
//                                    isDragging = true
//                                    lowHandle = min(highHandle, max(0, horizontal ? gesture.location.x / geometry.size.width : 1 - gesture.location.y / geometry.size.height))
//                                    updateParams()
//                                }
//                                .onEnded { _ in isDragging = false }
//                        )
//
//                    // High Handle
//                    ThumbView()
//                        .position(
//                            x: horizontal ? CGFloat(highHandle) * geometry.size.width : geometry.size.width / 2,
//                            y: horizontal ? geometry.size.height / 2 : CGFloat(1 - highHandle) * geometry.size.height
//                        )
//                        .gesture(
//                            DragGesture()
//                                .onChanged { gesture in
//                                    isDragging = true
//                                    highHandle = max(lowHandle, min(1, horizontal ? gesture.location.x / geometry.size.width : 1 - gesture.location.y / geometry.size.height))
//                                    updateParams()
//                                }
//                                .onEnded { _ in isDragging = false }
//                        )
//                }
//            }
//            .frame(height: thumbSize)
//
//            Text(isDragging ?
//                String(format: "%.0f - %.0f Hz", lowParam.value, highParam.value) :
//                "\(lowParam.displayName) - \(highParam.displayName)")
//                .font(.custom("AzeretMono-Light", size: 11))
//                .foregroundColor(Color.text)
//                .lineLimit(1)
//        }
//        .frame(width: horizontal ? nil : thumbSize)
//        .onAppear {
//            lowHandle = CGFloat(log2(lowParam.value / middleFrequency) / log2(range.upperBound / range.lowerBound) + 0.5)
//            highHandle = CGFloat(log2(highParam.value / middleFrequency) / log2(range.upperBound / range.lowerBound) + 0.5)
//        }
//    }
//
//    private func updateParams() {
//        let lowOctave = Float(lowHandle - 0.5) * log2(range.upperBound / range.lowerBound)
//        let highOctave = Float(highHandle - 0.5) * log2(range.upperBound / range.lowerBound)
//
//        lowParam.value = middleFrequency * pow(2, lowOctave)
//        highParam.value = middleFrequency * pow(2, highOctave)
//    }
// }

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
                            width: horizontal ? CGFloat(highHandle - lowHandle) * geometry.size.width : 2,
                            height: horizontal ? 2 : CGFloat(highHandle - lowHandle) * geometry.size.height
                        )
                        .offset(
                            x: horizontal ? CGFloat(lowHandle) * geometry.size.width : 0,
                            y: horizontal ? 0 : CGFloat(1 - highHandle) * geometry.size.height
                        )

                    Rectangle()
                        .fill(foregroundColor.opacity(1))
                        .frame(
                            width: horizontal ? CGFloat(highHandle - lowHandle) * geometry.size.width : 4,
                            height: horizontal ? 4 : CGFloat(highHandle - lowHandle) * geometry.size.height
                        )
                        .offset(
                            x: horizontal ? CGFloat(lowHandle) * geometry.size.width : 0,
                            y: horizontal ? 0 : CGFloat(1 - highHandle) * geometry.size.height
                        )
                        .blur(radius: 4)

                    // Low Handle
                    ThumbView()
                        .position(
                            x: horizontal ? CGFloat(lowHandle) * geometry.size.width : geometry.size.width / 2,
                            y: horizontal ? geometry.size.height / 2 : CGFloat(1 - lowHandle) * geometry.size.height
                        )
                        .gesture(
                            DragGesture()
                                .onChanged { gesture in
                                    isDragging = true
                                    lowHandle = min(highHandle, max(0, horizontal ? gesture.location.x / geometry.size.width : 1 - gesture.location.y / geometry.size.height))
                                    updateParams()
                                }
                                .onEnded { _ in isDragging = false }
                        )

                    // High Handle
                    ThumbView()
                        .position(
                            x: horizontal ? CGFloat(highHandle) * geometry.size.width : geometry.size.width / 2,
                            y: horizontal ? geometry.size.height / 2 : CGFloat(1 - highHandle) * geometry.size.height
                        )
                        .gesture(
                            DragGesture()
                                .onChanged { gesture in
                                    isDragging = true
                                    highHandle = max(lowHandle, min(1, horizontal ? gesture.location.x / geometry.size.width : 1 - gesture.location.y / geometry.size.height))
                                    updateParams()
                                }
                                .onEnded { _ in isDragging = false }
                        )
                }
            }
            .frame(height: thumbSize)
            GeometryReader { geometry in
                Text(isDragging ?
                    String(format: "%.0f - %.0f Hz", lowParam.value, highParam.value) :
                    "\(lowParam.displayName) - \(highParam.displayName)")
                    .font(.custom("AzeretMono-Light", size: min(max(geometry.size.width * 0.02, 8), 15)))
                    .foregroundColor(.text)
                    .lineLimit(1)
                    .shadow(color: glowColor, radius: 2, x: 0, y: 0)
            }
        }
        .frame(width: horizontal ? nil : thumbSize)
        .onAppear {
            lowHandle = CGFloat(log2(lowParam.value / middleFrequency) / log2(range.upperBound / range.lowerBound) + 0.5)
            highHandle = CGFloat(log2(highParam.value / middleFrequency) / log2(range.upperBound / range.lowerBound) + 0.5)
        }
    }

    private func updateParams() {
        let lowOctave = Float(lowHandle - 0.5) * log2(range.upperBound / range.lowerBound)
        let highOctave = Float(highHandle - 0.5) * log2(range.upperBound / range.lowerBound)

        lowParam.value = middleFrequency * pow(2, lowOctave)
        highParam.value = middleFrequency * pow(2, highOctave)
    }
}

struct CRTThumbView: View {
    var body: some View {
        ZStack {
            // Main circle
            Rectangle()
                .fill(Color.text)
                .frame(width: 20, height: 20)
        }
    }
}

struct PitchShiftDropdown: View {
    @ObservedObject var pitchParam: ObservableAUParameter
    let pitchOptions = Array(-12 ... 12)

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
                        .font(.custom("AzeretMono-Light", size: min(max(geometry.size.height * 0.02, 7), 15)))
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
