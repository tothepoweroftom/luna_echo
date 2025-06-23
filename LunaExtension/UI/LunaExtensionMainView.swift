import SwiftUI
import AudioToolbox

struct LunaExtensionMainView: View {
    var parameterTree: ObservableAUParameterGroup
    @State private var selectedTabIndex: Int = 0

    private let tabs: [TabData]

    init(
        parameterTree: ObservableAUParameterGroup,
        initialPresets: [AUAudioUnitPreset],
        currentPreset: AUAudioUnitPreset?,
        onLoad: @escaping (AUAudioUnitPreset) -> Void,
        onSave: @escaping (String) -> AUAudioUnitPreset?,
        onDelete: @escaping (AUAudioUnitPreset) -> Void,
        onGetPresets: @escaping () -> [AUAudioUnitPreset]
    ) {
        self.parameterTree = parameterTree
        tabs = [
            TabData(
                id: 0, title: "Main", color: Color(hex: "#121420"),
                content: AnyView(CoreToneSection(parameterTree: parameterTree))),
            TabData(
                id: 1, title: "Character", color: Color(hex: "#1B2432"),
                content: AnyView(CharacterSection(parameterTree: parameterTree))),
            TabData(
                id: 2, title: "Dynamics", color: Color(hex: "#242F42"),
                content: AnyView(DynamicsOutputSection(parameterTree: parameterTree))),
            TabData(
                id: 3, title: "Presets", color: Color(hex: "#F0EFF4"),
                content: AnyView(
                    PresetsSection(
                        parameterTree: parameterTree,
                        initialPresets: initialPresets,
                        currentPreset: currentPreset,
                        onLoad: onLoad,
                        onSave: onSave,
                        onDelete: onDelete,
                        onGetPresets: onGetPresets
                    )))
        ]
    }

    var body: some View {
        GeometryReader { geometry in
            if geometry.size.width > geometry.size.height {
                HorizontalAccordionView(
                    tabs: tabs,
                    selectedTabIndex: $selectedTabIndex
                )
            } else {
                VerticalAccordionView(
                    tabs: tabs,
                    selectedTabIndex: $selectedTabIndex
                )
            }
        }
        .frame(minWidth: 300, minHeight: 300)
        .background(Color(hex: "#121420"))
    }
}
