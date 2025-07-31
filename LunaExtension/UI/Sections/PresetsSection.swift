import SwiftUI
import AudioToolbox
import AudioPluginUI

struct PresetsSection: View {
    @Environment(\.audioPluginTheme) private var theme
    var parameterTree: APParameterGroup
    @StateObject private var presetManager: LunaPresetManager
    
    @State private var showingSaveAlert = false
    @State private var newPresetName = ""
    @State private var presetToDelete: LunaPreset?
    @State private var showingOverwriteAlert = false
    @State private var presetToOverwriteName = ""

    init(
        parameterTree: APParameterGroup,
        audioUnit: LunaExtensionAudioUnit
    ) {
        self.parameterTree = parameterTree
        self._presetManager = StateObject(wrappedValue: LunaPresetManager(audioUnit: audioUnit, parameterTree: parameterTree))
    }

    var body: some View {
        VStack(spacing: theme.layout.spacing.medium) {
            headerView
            
            LunaPresetListView(
                manager: presetManager,
                showingSaveAlert: $showingSaveAlert,
                presetToDelete: $presetToDelete
            )
            .frame(maxHeight: .infinity)
        }
        .padding(theme.layout.spacing.medium)
        .background(theme.colors.background)
        .modifier(SavePresetAlert(
            isPresented: $showingSaveAlert,
            presetName: $newPresetName,
            onSave: { name in
                if presetManager.userPresets.contains(where: { $0.name == name }) {
                    presetToOverwriteName = name
                    showingOverwriteAlert = true
                } else {
                    _ = presetManager.savePreset(name: name)
                }
            }
        ))
        .alert(item: $presetToDelete) { preset in
            Alert(
                title: Text("Delete Preset"),
                message: Text("Are you sure you want to delete \"\(preset.name)\"?"),
                primaryButton: .destructive(Text("Delete")) {
                    _ = presetManager.deletePreset(preset)
                },
                secondaryButton: .cancel()
            )
        }
        .alert(
            "Overwrite Preset?",
            isPresented: $showingOverwriteAlert,
            actions: {
                Button("Cancel", role: .cancel) { }
                Button("Overwrite") {
                    _ = presetManager.savePreset(name: presetToOverwriteName)
                }
            },
            message: {
                Text("A preset named \"\(presetToOverwriteName)\" already exists. Do you want to overwrite it?")
            }
        )
    }
    
    private var headerView: some View {
        HStack {
            Text("Presets")
                .font(theme.typography.title)
                .foregroundColor(theme.colors.primaryText)
            
            Spacer()
            
            Button("New") {
                newPresetName = ""
                showingSaveAlert = true
            }
            .font(theme.typography.body)
            .foregroundColor(theme.colors.accent)
            .padding(.horizontal, theme.layout.spacing.medium)
            .padding(.vertical, theme.layout.spacing.small)
            .background(
                RoundedRectangle(cornerRadius: theme.controls.buttons.cornerRadius)
                    .stroke(theme.colors.accent, lineWidth: theme.controls.buttons.borderWidth)
            )
        }
    }
}

// MARK: - Alert View Modifier

private struct SavePresetAlert: ViewModifier {
    @Binding var isPresented: Bool
    @Binding var presetName: String
    let onSave: (String) -> Void
    
    func body(content: Content) -> some View {
        content
            .alert("Save Preset", isPresented: $isPresented) {
                TextField("Preset Name", text: $presetName)
                Button("Cancel", role: .cancel) { }
                Button("Save") {
                    if !presetName.isEmpty {
                        onSave(presetName)
                    }
                }
            }
    }
} 
