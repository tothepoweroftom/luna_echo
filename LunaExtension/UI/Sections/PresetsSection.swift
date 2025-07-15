import SwiftUI
import AudioToolbox

struct PresetsSection: View {
    var parameterTree: ObservableAUParameterGroup
    @StateObject private var audioUnitWrapper: AudioUnitWrapper

    init(
        parameterTree: ObservableAUParameterGroup,
        audioUnit: LunaExtensionAudioUnit
    ) {
        self.parameterTree = parameterTree
        _audioUnitWrapper = StateObject(wrappedValue: AudioUnitWrapper(audioUnit: audioUnit))
    }

    var body: some View {
        PresetView(audioUnitWrapper: audioUnitWrapper)
    }
} 