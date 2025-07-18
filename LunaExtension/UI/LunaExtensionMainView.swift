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
                id: 2, title: "Dynamics", color: Color(hex: "#242F42"),
                textColor: Color(hex: "#ffffff"),
                content: { DynamicsOutputSection(parameterTree: parameterTree) }
            ),
            APTabData(
                id: 3, title: "Presets", color: Color(hex: "#F0EFF4"),
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
                if geometry.size.width > geometry.size.height {
                    APHorizontalAccordion(
                        tabs: tabs,
                        selectedTabIndex: $selectedTabIndex
                    )
                } else {
                    APVerticalAccordion(
                        tabs: tabs,
                        selectedTabIndex: $selectedTabIndex
                    )
                }
            }
            .frame(minWidth: 300, minHeight: 300)
        }
    }
}
