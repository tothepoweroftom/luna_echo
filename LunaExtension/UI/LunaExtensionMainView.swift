import AudioPluginUI
import AudioToolbox
import SwiftUI

struct LunaExtensionMainView: View {
    var parameterTree: APParameterGroup
    @State private var selectedTabIndex: Int = 0

    private let tabs: [APTabData]

    init(
        parameterTree: APParameterGroup,
        audioUnit: LunaExtensionAudioUnit
    ) {
        self.parameterTree = parameterTree
        tabs = [
            APTabData(
                id: 0, title: "Main", color: Color(hex: "#121420"),
                textColor: Color(hex: "#ffffff"),
                content: { CoreToneSection(parameterTree: parameterTree) }
            ),
            APTabData(
                id: 1, title: "Character", color: Color(hex: "#1B2432"),
                textColor: Color(hex: "#ffffff"),
                content: { CharacterSection(parameterTree: parameterTree) }
            ),
            APTabData(
                id: 2, title: "Pitch & Glitch", color: Color(hex: "#242F42"),
                textColor: Color(hex: "#ffffff"),
                content: { PitchAndGlitchSection(parameterTree: parameterTree) }
            ),
            APTabData(
                id: 3, title: "Dynamics", color: Color(hex: "#2B384F"),
                textColor: Color(hex: "#ffffff"),
                content: { DynamicsOutputSection(parameterTree: parameterTree) }
            ),
            APTabData(
                id: 4, title: "Presets", color: Color(hex: "#C7D4E2"),
                textColor: Color(hex: "#000000"),
                content: {
                    PresetsSection(
                        parameterTree: parameterTree,
                        audioUnit: audioUnit
                    )
                }
            ),
        ]
    }

    var body: some View {
        ThemeContext(theme: .lunaEcho) {
            GeometryReader { geometry in
                let w = geometry.size.width
                let h = geometry.size.height
                let isLandscape = w > h
                let small = min(w, h) < 420
                if isLandscape {
                    APHorizontalAccordion(
                        tabs: tabs,
                        selectedTabIndex: $selectedTabIndex
                    )
                    .collapsedWidth(small ? 40 : 60)
                    .titleFrameSize(CGSize(width: small ? 110 : 150, height: small ? 44 : 60))
                    .contentAnimationDelay(0.05)
                } else {
                    APVerticalAccordion(
                        tabs: tabs,
                        selectedTabIndex: $selectedTabIndex
                    )
                    .headerHeight(small ? 40 : 60)
                    .collapsedHeight(small ? 36 : 44)
                    .contentAnimationDelay(0.05)
                }
            }
            .frame(minWidth: 300, minHeight: 300)
        }
    }
}
