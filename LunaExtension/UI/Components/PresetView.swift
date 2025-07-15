import SwiftUI
import AVFoundation

struct PresetView: View {
    @ObservedObject var audioUnitWrapper: AudioUnitWrapper
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
                // Factory Presets Section
                Section(header: Text("Factory Presets")) {
                    ForEach(audioUnitWrapper.factoryPresets, id: \.self) { preset in
                        HStack {
                            Text(preset.name)
                            Spacer()
                            if preset.number == audioUnitWrapper.currentPreset?.number && 
                               audioUnitWrapper.currentPreset?.number ?? -1 >= 0 {
                                Image(systemName: "checkmark")
                                    .foregroundColor(.accentColor)
                            }
                        }
                        .contentShape(Rectangle())
                        .onTapGesture {
                            audioUnitWrapper.selectPreset(preset)
                        }
                    }
                }
                
                // User Presets Section
                Section(header: Text("User Presets")) {
                    ForEach(audioUnitWrapper.userPresets, id: \.self) { preset in
                        HStack {
                            Text(preset.name)
                            Spacer()
                            if preset.name == audioUnitWrapper.currentPreset?.name &&
                               audioUnitWrapper.currentPreset?.number ?? 0 < 0 {
                                Image(systemName: "checkmark")
                                    .foregroundColor(.accentColor)
                            }
                        }
                        .contentShape(Rectangle())
                        .onTapGesture {
                            audioUnitWrapper.selectPreset(preset)
                        }
                        .swipeActions {
                            Button(role: .destructive) {
                                presetToDelete = preset
                            } label: {
                                Label("Delete", systemImage: "trash")
                            }
                        }
                    }
                    
                    if audioUnitWrapper.userPresets.isEmpty {
                        Text("No user presets")
                            .foregroundColor(.secondary)
                            .italic()
                    }
                }
            }
            .listStyle(InsetGroupedListStyle())
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .alert("Save Preset", isPresented: $showingSaveAlert) {
            TextField("Preset Name", text: $newPresetName)
            Button("Cancel", role: .cancel) {}
            Button("Save") {
                if !newPresetName.isEmpty {
                    if audioUnitWrapper.userPresets.contains(where: { $0.name == newPresetName }) {
                        presetToOverwriteName = newPresetName
                        showingOverwriteAlert = true
                    } else {
                        audioUnitWrapper.savePreset(name: newPresetName)
                    }
                }
            }
        }
        .alert(item: $presetToDelete) { preset in
            Alert(
                title: Text("Delete Preset"),
                message: Text("Are you sure you want to delete \"\(preset.name)\"?"),
                primaryButton: .destructive(Text("Delete")) {
                    audioUnitWrapper.deletePreset(preset)
                },
                secondaryButton: .cancel()
            )
        }
        .alert(
            "Overwrite Preset?", isPresented: $showingOverwriteAlert,
            actions: {
                Button("Cancel", role: .cancel) {}
                Button("Overwrite") {
                    audioUnitWrapper.overwritePreset(name: presetToOverwriteName)
                }
            },
            message: {
                Text(
                    "A preset named \"\(presetToOverwriteName)\" already exists. Do you want to overwrite it?"
                )
            })
    }
}

// Wrapper to make AUAudioUnit observable for SwiftUI
class AudioUnitWrapper: ObservableObject {
    @Published var factoryPresets: [AUAudioUnitPreset] = []
    @Published var userPresets: [AUAudioUnitPreset] = []
    @Published var currentPreset: AUAudioUnitPreset?

    private var userPresetsManager: UserPresetsManager
    private var observation: NSKeyValueObservation?

    init(audioUnit: LunaExtensionAudioUnit) {
        self.userPresetsManager = UserPresetsManager(for: audioUnit)
        
        self.factoryPresets = userPresetsManager.factoryPresets
        self.userPresets = userPresetsManager.presetsOrderedByName
        self.currentPreset = userPresetsManager.currentPreset
        
        observation = audioUnit.observe(\.currentPreset, options: [.new]) { [weak self] _, change in
            DispatchQueue.main.async {
                self?.currentPreset = change.newValue as? AUAudioUnitPreset
                self?.refresh()
            }
        }
    }

    private func refresh() {
        self.userPresets = userPresetsManager.presetsOrderedByName
        self.factoryPresets = userPresetsManager.factoryPresets
        self.currentPreset = userPresetsManager.currentPreset
    }

    func selectPreset(_ preset: AUAudioUnitPreset) {
        userPresetsManager.makeCurrentPreset(preset)
        refresh()
    }

    func savePreset(name: String) {
        do {
            try userPresetsManager.create(name: name)
            refresh()
        } catch {
            print("Failed to save preset: \(error)")
        }
    }
    
    func overwritePreset(name: String) {
        guard let preset = userPresetsManager.find(name: name) else { return }
        do {
            try userPresetsManager.update(preset: preset)
            refresh()
        } catch {
            print("Failed to overwrite preset: \(error)")
        }
    }

    func deletePreset(_ preset: AUAudioUnitPreset) {
        userPresetsManager.makeCurrentPreset(preset)
        do {
            try userPresetsManager.deleteCurrent()
            refresh()
        } catch {
            print("Failed to delete preset: \(error)")
        }
    }
}

extension AUAudioUnitPreset: Identifiable {
    public var id: String {
        return "\(number)_\(name)"
    }
}