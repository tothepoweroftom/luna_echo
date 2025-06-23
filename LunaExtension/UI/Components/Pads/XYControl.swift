import AVFoundation
import SwiftUI

struct XYControl: View {
    @State private var location: CGPoint = .zero
    @ObservedObject var paramX: ObservableAUParameter
    @ObservedObject var paramY: ObservableAUParameter
    @ObservedObject var paramXSynced: ObservableAUParameter
    @ObservedObject var synced: ObservableAUParameter
    var showing3D: Bool

    private let fractionPositions: [CGFloat] = [
        0, 1 / 17, 2 / 17, 3 / 17, 4 / 17, 5 / 17, 6 / 17, 7 / 17, 8 / 17, 9 / 17, 10 / 17, 11 / 17,
        12 / 17, 13 / 17, 14 / 17, 15 / 17, 16 / 17, 1,
    ]

    var body: some View {
        GeometryReader { geometry in
            let size = min(geometry.size.width, geometry.size.height)
            VStack {
                HStack {
                    Text(currentValueText)
                        .font(.custom("AzeretMono-Light", size: min(max(size * 0.02, 9), 15)))
                        .foregroundColor(.text)
                    Spacer()
                    CustomCheckbox(
                        isChecked: Binding(
                            get: { self.synced.value == 1 },
                            set: { self.synced.value = $0 ? 1 : 0 }
                        ), onToggle: {}
                    )
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
            xValue =
                paramXSynced.valueStrings?[Int(paramXSynced.value)]
                ?? String(format: "%.2f", paramXSynced.value)
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
            let nearestIndex = fractionPositions.indices.min(by: {
                abs(fractionPositions[$0] * size - newX) < abs(fractionPositions[$1] * size - newX)
            })!
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
            let nearestIndex = fractionPositions.indices.min(by: {
                abs(fractionPositions[$0] * size - location.x)
                    < abs(fractionPositions[$1] * size - location.x)
            })!
            paramXSynced.value = Float(nearestIndex)
        } else {
            paramX.value = Float(location.x / size) * (paramX.max - paramX.min) + paramX.min
        }
    }
} 