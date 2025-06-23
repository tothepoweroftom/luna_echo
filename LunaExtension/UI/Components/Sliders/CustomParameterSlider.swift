import AVFoundation
import Sliders
import SwiftUI

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
                        ValueSlider(
                            value: $param.value, in: param.min...param.max,
                            onEditingChanged: { editing in
                                self.isDragging = editing
                            }
                        )
                        .valueSliderStyle(
                            VerticalValueSliderStyle(
                                track:
                                    VerticalValueTrack(
                                        // Use the adaptive color for the track foreground
                                        view: Rectangle().foregroundColor(trackForegroundColor)
                                            .cornerRadius(0.0),
                                        mask: Rectangle().frame(width: 3)
                                    )
                                    // Keep accent for the background/fill part
                                    .background(
                                        Rectangle().foregroundColor(Color.accent).cornerRadius(0)
                                            .frame(width: 2)),

                                thumb: ThumbView(),  // ThumbView might need similar adaptation if Color.text is fixed
                                thumbSize: CGSize(width: 10, height: 10),
                                thumbInteractiveSize: CGSize(width: 30, height: 30),
                                options: .interactiveTrack
                            )
                        )
                        Spacer()
                        // Text already uses Color.text, which might be adaptive or need changing
                        Text(
                            isDragging
                                ? String(
                                    format: "%.2f", locale: Locale(identifier: "en_US_POSIX"),
                                    param.value) : "\(param.displayName)"
                        )
                        .font(
                            .custom(
                                "AzeretMono-Light",
                                size: min(max(geometry.size.height * 0.02, 7), 15))
                        )
                        .foregroundColor(Color.text)
                        .lineLimit(1)
                    }
                } else {
                    VStack {
                        ValueSlider(
                            value: $param.value, in: param.min...param.max,
                            onEditingChanged: { editing in
                                self.isDragging = editing
                            }
                        )
                        .valueSliderStyle(
                            HorizontalValueSliderStyle(
                                track:
                                    HorizontalValueTrack(
                                        // Use the adaptive color for the track foreground
                                        view: Rectangle().foregroundColor(trackForegroundColor)
                                            .cornerRadius(0.0),
                                        mask: Rectangle().frame(height: 3)
                                    )
                                    // Keep accent for the background/fill part
                                    .background(
                                        Rectangle().foregroundColor(Color.accent).cornerRadius(0)
                                            .frame(height: 1)),

                                thumb: ThumbView(),  // ThumbView might need similar adaptation
                                thumbSize: CGSize(width: 10, height: 10),
                                thumbInteractiveSize: CGSize(width: 30, height: 30),
                                options: .interactiveTrack
                            )
                        )
                        Spacer()
                        // Text already uses Color.text
                        Text(
                            isDragging
                                ? String(
                                    format: "%.2f", locale: Locale(identifier: "en_US_POSIX"),
                                    param.value) : "\(param.displayName)"
                        )
                        .font(
                            .custom(
                                "AzeretMono-Light",
                                size: min(max(geometry.size.width * 0.02, 11), 15))
                        )
                        .foregroundColor(Color.text)
                        .lineLimit(1)
                    }
                }
            }
        }
    }
} 