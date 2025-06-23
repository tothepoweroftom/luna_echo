import SwiftUI
import AudioToolbox

struct PresetsSection: View {
    var parameterTree: ObservableAUParameterGroup
    @StateObject private var presetManagerWrapper: PresetManagerWrapper

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
        _presetManagerWrapper = StateObject(
            wrappedValue: PresetManagerWrapper(
                initialPresets: initialPresets,
                currentPreset: currentPreset,
                onLoad: onLoad,
                onSave: onSave,
                onDelete: onDelete,
                onGetPresets: onGetPresets
            ))
    }

    var body: some View {
        PresetView(presetManagerWrapper: presetManagerWrapper)
    }
} 