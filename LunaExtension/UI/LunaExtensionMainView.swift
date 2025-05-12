import SwiftUI

// Helper extension to initialize Color from hex string (keep as is)
extension Color {
    init(hex: String) {
        let hex = hex.trimmingCharacters(in: CharacterSet.alphanumerics.inverted)
        var int: UInt64 = 0
        Scanner(string: hex).scanHexInt64(&int)
        let a, r, g, b: UInt64
        switch hex.count {
        case 3: // RGB (12-bit)
            (a, r, g, b) = (255, (int >> 8) * 17, (int >> 4 & 0xF) * 17, (int & 0xF) * 17)
        case 6: // RGB (24-bit)
            (a, r, g, b) = (255, int >> 16, int >> 8 & 0xFF, int & 0xFF)
        case 8: // ARGB (32-bit)
            (a, r, g, b) = (int >> 24, int >> 16 & 0xFF, int >> 8 & 0xFF, int & 0xFF)
        default:
            (a, r, g, b) = (1, 1, 1, 0) // Default to black for invalid hex
        }

        self.init(
            .sRGB,
            red: Double(r) / 255,
            green: Double(g) / 255,
            blue: Double(b) / 255,
            opacity: Double(a) / 255
        )
    }
}

struct LunaExtensionMainView: View {
    var parameterTree: ObservableAUParameterGroup
    @State private var selectedTabIndex: Int = 0 // Start with the first tab expanded

    // Define colors
    let colorTab1 = Color(hex: "#121420") // Core & Tone
    let colorTab2 = Color(hex: "#1B2432") // Character
    let colorTab3 = Color(hex: "#848C8E") // Dynamics
    let textColorLight = Color.white
    let textColorDark = Color.black // For the lighter tab background

    // Define tab data structure
    struct TabData: Identifiable {
        let id: Int // Use index as ID for ForEach
        let title: String
        let color: Color
        let content: AnyView
        var textColor: Color { id == 2 ? Color.black : Color.white } // Determine text color based on index
    }

    // Define the tabs with their content and colors
    let tabs: [TabData]

    init(parameterTree: ObservableAUParameterGroup) {
        self.parameterTree = parameterTree
        tabs = [
            TabData(id: 0, title: "Core & Tone", color: Color(hex: "#121420"), content: AnyView(CoreToneSection(parameterTree: parameterTree))),
            TabData(id: 1, title: "Character", color: Color(hex: "#1B2432"), content: AnyView(CharacterSection(parameterTree: parameterTree))),
            TabData(id: 2, title: "Dynamics", color: Color(hex: "#848C8E"), content: AnyView(DynamicsOutputSection(parameterTree: parameterTree))),
        ]
    }

    var body: some View {
        // Use GeometryReader to detect orientation
        GeometryReader { geometry in
            // Decide layout based on aspect ratio
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
        .frame(minWidth: 300, minHeight: 300) // Set overall minimums
        .background(colorTab1) // Default background if needed, though child views should cover
    }
}

// MARK: - Horizontal Accordion Layout

struct HorizontalAccordionView: View {
    let tabs: [LunaExtensionMainView.TabData]
    @Binding var selectedTabIndex: Int

    var body: some View {
        HStack(spacing: 0) {
            ForEach(tabs) { tab in
                let isSelected = selectedTabIndex == tab.id

                ZStack(alignment: .topLeading) {
                    tab.color.edgesIgnoringSafeArea(.all)

                    if isSelected {
                        // Expanded Content
                        VStack(alignment: .leading, spacing: 0) {
                            ScrollView {
                                tab.content.padding()
                            }
                            .transition(.opacity.animation(.easeInOut(duration: 0.2).delay(0.15)))
                        }
                        .clipped()
                    } else {
                        // Shrunk Content (Vertical Title)
                        VStack {
                            Spacer()
                            Text(tab.title)
                                .font(.headline)
                                .rotationEffect(.degrees(-90))
                                .fixedSize()
                                .frame(width: 150, height: 60)
                                .foregroundColor(tab.textColor)
                            Spacer()
                        }
                        .frame(width: 60)
                        .clipped()
                    }
                }
                .frame(maxWidth: isSelected ? .infinity : 60, maxHeight: .infinity)
                .contentShape(Rectangle())
                .onTapGesture {
                    selectedTabIndex = tab.id
                }
                .clipped()
                .animation(.easeInOut(duration: 0.4), value: selectedTabIndex)
            }
        }
    }
}

// MARK: - Vertical Accordion Layout

struct VerticalAccordionView: View {
    let tabs: [LunaExtensionMainView.TabData]
    @Binding var selectedTabIndex: Int

    var body: some View {
        VStack(spacing: 0) {
            ForEach(tabs) { tab in
                let isSelected = selectedTabIndex == tab.id

                ZStack(alignment: .topLeading) {
                    tab.color.edgesIgnoringSafeArea(.all) // Background color

                    VStack(spacing: 0) {
                        // Tab Header (Always visible)
                        HStack {
                            Text(tab.title)
                                .font(.headline)
                                .padding()
                                .foregroundColor(tab.textColor)
                            Spacer() // Push title left
                        }
                        .frame(maxWidth: .infinity)
                        .frame(height: 60) // Fixed header height
                        .contentShape(Rectangle())
                        .onTapGesture {
                            selectedTabIndex = tab.id
                        }

                        // Content (conditional visibility and height)
                        if isSelected {
                            ScrollView {
                                tab.content.padding()
                            }
                            .frame(maxWidth: .infinity, maxHeight: .infinity)
                            // Animate opacity for smooth appearance
                            .transition(.opacity.animation(.easeInOut(duration: 0.2).delay(0.1)))
                        }
                        // else: No content shown, frame collapses below
                    }
                    // Content will only take space if isSelected is true due to ScrollView inside
                }
                // Animate the height based on selection
                .frame(maxWidth: .infinity, maxHeight: isSelected ? .infinity : 44) // Expand height or keep header height
                .clipped() // Clip content to animated frame
                .animation(.easeInOut(duration: 0.4), value: selectedTabIndex)
            }
        }
    }
}

// MARK: - Section Content Views (Remain the same)

// CoreToneSection with custom sliders
struct CoreToneSection: View {
    var parameterTree: ObservableAUParameterGroup
    var body: some View {
        VStack(spacing: 12) {
            // XYControl for Delay Time and Feedback
            XYControl(
                paramX: parameterTree.global.delayTime,
                paramY: parameterTree.global.feedback,
                paramXSynced: parameterTree.global.delayTimeSync, // Assuming you have this parameter
                synced: parameterTree.global.syncEnabled, // Assuming you have this parameter
                showing3D: false
            )
            .frame(height: 200)

            // CustomParameterSlider for Mix
            CustomParameterSlider(
                param: parameterTree.global.mix,
                horizontal: false
            )
            .frame(height: 100)

            // TwoHandledParameterSlider for filters
            TwoHandledParameterSlider(
                lowParam: parameterTree.global.highpass,
                highParam: parameterTree.global.lowpass,
                horizontal: true,
                range: 20 ... 20000
            )
            .frame(height: 70)

            // CustomParameterSlider for pitch shift
            CustomParameterSlider(
                param: parameterTree.global.pitchShift,
                horizontal: false
            )
            .frame(height: 100)
        }
        .colorScheme(.dark)
        .padding(20)
    }
}

// CharacterSection with custom sliders
struct CharacterSection: View {
    var parameterTree: ObservableAUParameterGroup
    var body: some View {
        VStack(spacing: 12) {
            // CustomParameterSlider for all parameters in this section
            CustomParameterSlider(
                param: parameterTree.global.tapeWearMacro,
                horizontal: false
            )
            .frame(height: 100)

            CustomParameterSlider(
                param: parameterTree.global.glitchMacro,
                horizontal: false
            )
            .frame(height: 100)

            CustomParameterSlider(
                param: parameterTree.global.diffusionAmount,
                horizontal: false
            )
            .frame(height: 100)

            CustomParameterSlider(
                param: parameterTree.global.spreadAmount,
                horizontal: false
            )
            .frame(height: 100)
        }
        .padding(20)
        .colorScheme(.dark)
    }
}

// DynamicsOutputSection with custom sliders
struct DynamicsOutputSection: View {
    var parameterTree: ObservableAUParameterGroup
    var body: some View {
        VStack(spacing: 12) {
            // CustomParameterSlider for all parameters in this section
            CustomParameterSlider(
                param: parameterTree.global.duckingMacro,
                horizontal: false
            )
            .frame(height: 100)

            CustomParameterSlider(
                param: parameterTree.global.duckAttack,
                horizontal: false
            )
            .frame(height: 100)

            CustomParameterSlider(
                param: parameterTree.global.duckRelease,
                horizontal: false
            )
            .frame(height: 100)

            CustomParameterSlider(
                param: parameterTree.global.outputGain,
                horizontal: false
            )
            .frame(height: 100)
        }
        .colorScheme(.light)
        .padding(20)
    }
}
