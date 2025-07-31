import SwiftUI
import AudioPluginUI

struct LunaPresetListView: View {
    @Environment(\.audioPluginTheme) private var theme
    @ObservedObject var manager: LunaPresetManager
    
    @Binding var showingSaveAlert: Bool
    @Binding var presetToDelete: LunaPreset?
    
    var body: some View {
        List {
            // Factory Presets Section
            if !manager.factoryPresets.isEmpty {
                Section {
                    ForEach(manager.factoryPresets, id: \.id) { preset in
                        presetRow(preset)
                            .listRowBackground(Color.clear)
                            .listRowSeparator(.hidden)
                            .background(rowBackground)
                    }
                } header: {
                    sectionHeaderForList("Factory Presets")
                }
            }
            
            // User Presets Section
            Section {
                if manager.userPresets.isEmpty {
                    emptyUserPresetsView
                        .listRowBackground(Color.clear)
                        .listRowSeparator(.hidden)
                        .background(rowBackground)
                } else {
                    ForEach(manager.userPresets, id: \.id) { preset in
                        presetRow(preset)
                            .listRowBackground(Color.clear)
                            .listRowSeparator(.hidden)
                            .background(rowBackground)
                            .swipeActions(edge: .trailing, allowsFullSwipe: true) {
                                Button(role: .destructive) {
                                    presetToDelete = preset
                                } label: {
                                    Image(systemName: "trash")
                                }
                            }
                    }
                }
            } header: {
                sectionHeaderForList("User Presets")
            }
        }
        .listStyle(.plain)
        .scrollContentBackground(.hidden)
        .background(theme.colors.background)
    }
    
    private var rowBackground: some View {
        Rectangle()
            .fill(Color.clear)
            .overlay(
                Rectangle()
                    .frame(height: 1)
                    .foregroundColor(theme.colors.controlBorder.opacity(0.3))
                    .offset(y: 0),
                alignment: .bottom
            )
    }
    
    private func sectionHeader(_ title: String) -> some View {
        HStack {
            Text(title)
                .font(theme.typography.headline)
                .foregroundColor(theme.colors.primaryText)
            Spacer()
        }
        .padding(.horizontal, theme.layout.spacing.small)
        .padding(.vertical, theme.layout.spacing.small)
    }
    
    private func sectionHeaderForList(_ title: String) -> some View {
        HStack {
            Text(title)
                .font(theme.typography.headline)
                .foregroundColor(theme.colors.primaryText)
            Spacer()
        }
        .padding(.vertical, theme.layout.spacing.small)
    }
    
    private func presetRow(_ preset: LunaPreset) -> some View {
        HStack {
            // Preset name
            Text(preset.name)
                .font(theme.typography.body)
                .foregroundColor(theme.colors.primaryText)
            
            Spacer()
            
            // Current preset indicator
            if preset.isSelected {
                Image(systemName: "checkmark.circle.fill")
                    .foregroundColor(theme.colors.accent)
                    .font(.system(size: 16))
            }
            
            // Factory preset indicator
            if preset.isFactory {
                Image(systemName: "lock.fill")
                    .foregroundColor(theme.colors.secondaryText)
                    .font(.system(size: 12))
            }
        }
        .padding(.horizontal, theme.layout.spacing.medium)
        .padding(.vertical, theme.layout.spacing.medium)
        .contentShape(Rectangle())
        .onTapGesture {
            manager.selectPreset(preset)
        }
    }
    
    private var emptyUserPresetsView: some View {
        HStack {
            Text("No user presets")
                .font(theme.typography.body)
                .foregroundColor(theme.colors.secondaryText)
                .italic()
            
            Spacer()
            
            Button("Create First Preset") {
                showingSaveAlert = true
            }
            .font(theme.typography.caption)
            .foregroundColor(theme.colors.accent)
        }
        .padding(.horizontal, theme.layout.spacing.medium)
        .padding(.vertical, theme.layout.spacing.large)
    }
}