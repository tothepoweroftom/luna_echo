import AudioPluginUI
import SwiftUI

#if DEBUG
// Debug helper to print available font families
private func debugFonts() {
    #if os(iOS)
    print("=== Available Font Families ===")
    for family in UIFont.familyNames.sorted() {
        if family.lowercased().contains("archivo") || family.lowercased().contains("azeret") {
            print("Family: \(family)")
            for font in UIFont.fontNames(forFamilyName: family) {
                print("  - \(font)")
            }
        }
    }
    #endif
}
#endif

extension AudioPluginTheme {
    static let lunaEcho: AudioPluginTheme = {
        #if DEBUG
        debugFonts()
        #endif
        
        return AudioPluginTheme(
        colors: AudioPluginTheme.ColorScheme(
            primary: Color(hex: "#ffffff"),
            secondary: Color(hex: "#cccccc"),
            accent: Color(hex: "#ffffff"),
            background: Color(hex: "#000000"),
            primaryText: Color.white,
            secondaryText: Color.white.opacity(0.8),
            disabledText: Color.gray,
            controlBackground: Color(hex: "#ffffff").opacity(0.23),
            controlForeground: Color.white,
            controlBorder: Color.white,
            selected: Color.white,
            active: Color.white,
            inactive: Color.gray,
            disabled: Color.gray.opacity(0.5),
            success: Color.green,
            warning: Color.orange,
            error: Color.red,
            glow: Color.white.opacity(0.5),
            shadow: Color.black.opacity(0.3)
        ),
        typography: AudioPluginTheme.Typography(
            title: Font.custom("Archivo Black", size: 20),
            headline: Font.custom("Archivo Black", size: 16),
            body: Font.custom("Archivo Black", size: 13),
            caption: Font.custom("Archivo Black", size: 11),
            callout: Font.custom("Archivo Black", size: 14),
            footnote: Font.custom("Archivo Black", size: 10),
            parameterLabel: Font.custom("Azeret Mono Black", size: 11),
            parameterValue: Font.custom("Azeret Mono Black", size: 11)
        ),
        controls: AudioPluginTheme.Controls(
            knobs: AudioPluginTheme.KnobStyle(
                size: 80,
                lineWidth: 3,
                glowRadius: 0,
                shadowRadius: 0
            ),
            sliders: AudioPluginTheme.SliderStyle(
                trackHeight: 3,
                thumbSize: CGSize(width: 20, height: 20),
                cornerRadius: 0
            ),
            buttons: AudioPluginTheme.ButtonStyle(
                cornerRadius: 4,
                borderWidth: 1
            ),
            toggles: AudioPluginTheme.ToggleStyle(
                size: 20,
                padding: 4
            )
        ),
        layout: AudioPluginTheme.Layout.default,
        animations: AudioPluginTheme.Animations.default
        )
    }()
}
