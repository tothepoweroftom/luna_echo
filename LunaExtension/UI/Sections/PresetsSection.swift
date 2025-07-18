import SwiftUI
import AudioToolbox
import AudioPluginUI

struct PresetsSection: View {
    var parameterTree: APParameterGroup
    @StateObject private var presetManager = APSimplePresetManager()

    init(
        parameterTree: APParameterGroup,
        audioUnit: LunaExtensionAudioUnit
    ) {
        self.parameterTree = parameterTree
    }

    var body: some View {
        VStack {
            // Use the new AudioPluginUI preset manager
            APPresetManagerView(manager: presetManager)
                .frame(maxHeight: .infinity)
        }
    }
} 
