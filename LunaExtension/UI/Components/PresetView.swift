import SwiftUI
import AVFoundation

struct PresetView: View {
    @ObservedObject var presetManagerWrapper: PresetManagerWrapper
    @State private var showingSaveAlert = false
    @State private var newPresetName = ""
    @State private var presetToDelete: AUAudioUnitPreset?
    @State private var showingOverwriteAlert = false
    @State private var presetToOverwriteName = ""

    var body: some View {
        VStack {
            HStack {
                Text("Presets")
                    .font(.title)
                    .foregroundColor(.primary)
                Spacer()
                Button(action: {
                    newPresetName = ""
                    showingSaveAlert = true
                }) {
                    Image(systemName: "plus")
                }
                .padding()
            }
            .padding(.horizontal)

            List {
                ForEach(presetManagerWrapper.allPresets, id: \.self) { preset in
                    HStack {
                        Text(preset.name)
                        Spacer()
                        if preset.name == presetManagerWrapper.currentPreset?.name {
                            Image(systemName: "checkmark")
                                .foregroundColor(.accent)
                        }
                    }
                    .contentShape(Rectangle())
                    .onTapGesture {
                        presetManagerWrapper.loadPreset(preset)
                    }
                    .swipeActions {
                        if preset.number < 0 {  // User preset
                            Button(role: .destructive) {
                                presetToDelete = preset
                            } label: {
                                Label("Delete", systemImage: "trash")
                            }
                        }
                    }
                }
            }
            .listStyle(DefaultListStyle())
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .alert("Save Preset", isPresented: $showingSaveAlert) {
            TextField("Preset Name", text: $newPresetName)
            Button("Cancel", role: .cancel) {}
            Button("Save") {
                if !newPresetName.isEmpty {
                    // Check if a user preset with this name exists. Factory presets cannot be overwritten.
                    let isExistingUserPreset = presetManagerWrapper.allPresets.contains {
                        $0.name == newPresetName && $0.number < 0
                    }

                    if isExistingUserPreset {
                        presetToOverwriteName = newPresetName
                        showingOverwriteAlert = true
                    } else {
                        presetManagerWrapper.savePreset(name: newPresetName)
                    }
                }
            }
        }
        .alert(item: $presetToDelete) { preset in
            Alert(
                title: Text("Delete Preset"),
                message: Text("Are you sure you want to delete \"\(preset.name)\"?"),
                primaryButton: .destructive(Text("Delete")) {
                    presetManagerWrapper.deletePreset(preset)
                },
                secondaryButton: .cancel()
            )
        }
        .alert(
            "Overwrite Preset?", isPresented: $showingOverwriteAlert,
            actions: {
                Button("Cancel", role: .cancel) {}
                Button("Overwrite") {
                    presetManagerWrapper.savePreset(name: presetToOverwriteName)
                }
            },
            message: {
                Text(
                    "A preset named \"\(presetToOverwriteName)\" already exists. Do you want to overwrite it?"
                )
            })
    }
}

// Wrapper to make PresetManager observable
class PresetManagerWrapper: ObservableObject {
    @Published var allPresets: [AUAudioUnitPreset] = []
    @Published var currentPreset: AUAudioUnitPreset?

    private let onLoad: (AUAudioUnitPreset) -> Void
    private let onSave: (String) -> AUAudioUnitPreset?
    private let onDelete: (AUAudioUnitPreset) -> Void
    private let onGetPresets: () -> [AUAudioUnitPreset]

    init(
        initialPresets: [AUAudioUnitPreset],
        currentPreset: AUAudioUnitPreset?,
        onLoad: @escaping (AUAudioUnitPreset) -> Void,
        onSave: @escaping (String) -> AUAudioUnitPreset?,
        onDelete: @escaping (AUAudioUnitPreset) -> Void,
        onGetPresets: @escaping () -> [AUAudioUnitPreset]
    ) {
        self.currentPreset = currentPreset
        self.onLoad = onLoad
        self.onSave = onSave
        self.onDelete = onDelete
        self.onGetPresets = onGetPresets

        // Initial fetch
        refreshPresets()
    }

    func refreshPresets() {
        self.allPresets = onGetPresets()
    }

    func loadPreset(_ preset: AUAudioUnitPreset) {
        onLoad(preset)
        self.currentPreset = preset
    }

    func savePreset(name: String) {
        if onSave(name) != nil {
            refreshPresets()
        }
    }

    func deletePreset(_ preset: AUAudioUnitPreset) {
        onDelete(preset)
        refreshPresets()
    }
}

extension AUAudioUnitPreset: Identifiable {
    public var id: String {
        return name
    }
}