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
    let colorTab3 = Color(hex: "#242F42") // Dynamics
    let colorTab4 = Color(hex: "#F0EFF4") // Presets
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
            TabData(id: 2, title: "Dynamics", color: Color(hex: "#242F42"), content: AnyView(DynamicsOutputSection(parameterTree: parameterTree))),
            TabData(id: 3, title: "Presets", color: Color(hex: "#F0EFF4"), content: AnyView(PresetsSection(parameterTree: parameterTree))),
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
                            tab.content
                                .padding()
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
                            tab.content
                                .padding()
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

// CoreToneSection with custom sliders - always square
struct CoreToneSection: View {
    var parameterTree: ObservableAUParameterGroup
    var body: some View {
        GeometryReader { geometry in
            let squareSize = min(geometry.size.width, geometry.size.height) - 32 // Account for padding
            let xyControlSize = squareSize * 0.9 // 90% for XY control
            let mixSliderHeight = CGFloat(30) // 30px for mix slider
            
            VStack(spacing: 16) {
                // XYControl for Delay Time and Feedback
                XYControl(
                    paramX: parameterTree.global.delayTime,
                    paramY: parameterTree.global.feedback,
                    paramXSynced: parameterTree.global.delayTimeSync,
                    synced: parameterTree.global.syncEnabled,
                    showing3D: false
                )
                .frame(width: xyControlSize, height: xyControlSize)
                Spacer()
                // CustomParameterSlider for Mix
                CustomParameterSlider(
                    param: parameterTree.global.mix,
                    horizontal: false
                )
                .frame(width: xyControlSize, height: mixSliderHeight)
            }
            .frame(width: squareSize, height: squareSize)
            .position(x: geometry.size.width / 2, y: geometry.size.height / 2)
        }
        .aspectRatio(1, contentMode: .fit) // Force square aspect ratio
        .colorScheme(.dark)
    }
}

// CharacterSection with arc sliders in grid
struct CharacterSection: View {
    var parameterTree: ObservableAUParameterGroup
    
    let gridColumns = [
        GridItem(.flexible(), spacing: 16),
        GridItem(.flexible(), spacing: 16),
        GridItem(.flexible(), spacing: 16)
    ]
    
    var body: some View {
        GeometryReader { geometry in
            let sliderSize = min((geometry.size.width - 64) / 3, 100) // Adaptive sizing with padding
            
            LazyVGrid(columns: gridColumns, spacing: 16) {
                // ArcSlider for pitch shift
                ArcSlider(param: parameterTree.global.pitchShift)
                    .frame(width: sliderSize, height: sliderSize + 20)
                
                // ArcSlider for tape wear macro
                ArcSlider(param: parameterTree.global.tapeWearMacro)
                    .frame(width: sliderSize, height: sliderSize + 20)
                
                // ArcSlider for glitch macro
                ArcSlider(param: parameterTree.global.glitchMacro)
                    .frame(width: sliderSize, height: sliderSize + 20)
                
                // ArcSlider for diffusion amount
                ArcSlider(param: parameterTree.global.diffusionAmount)
                    .frame(width: sliderSize, height: sliderSize + 20)
                
                // ArcSlider for spread amount
                ArcSlider(param: parameterTree.global.spreadAmount)
                    .frame(width: sliderSize, height: sliderSize + 20)
            }
            .padding(.horizontal, 16)
               // TwoHandledParameterSlider for filters
            TwoHandledParameterSlider(
                lowParam: parameterTree.global.highpass,
                highParam: parameterTree.global.lowpass,
                horizontal: true,
                range: 20 ... 20000
            )
            .frame(height: 70)
        }
        .colorScheme(.dark)
    }
}

// DynamicsOutputSection with arc sliders in grid
struct DynamicsOutputSection: View {
    var parameterTree: ObservableAUParameterGroup
    
    let gridColumns = [
        GridItem(.flexible(), spacing: 16),
        GridItem(.flexible(), spacing: 16),
        GridItem(.flexible(), spacing: 16)
    ]
    
    var body: some View {
        GeometryReader { geometry in
            let sliderSize = min((geometry.size.width - 64) / 3, 100) // Adaptive sizing with padding
            
            LazyVGrid(columns: gridColumns, spacing: 16) {
                // ArcSlider for ducking macro
                ArcSlider(param: parameterTree.global.duckingMacro)
                    .frame(width: sliderSize, height: sliderSize + 20)
                
                // ArcSlider for duck attack
                ArcSlider(param: parameterTree.global.duckAttack)
                    .frame(width: sliderSize, height: sliderSize + 20)
                
                // ArcSlider for duck release
                ArcSlider(param: parameterTree.global.duckRelease)
                    .frame(width: sliderSize, height: sliderSize + 20)
                
                // ArcSlider for output gain
                ArcSlider(param: parameterTree.global.outputGain)
                    .frame(width: sliderSize, height: sliderSize + 20)
            }
            .padding(.horizontal, 16)
        }
        .colorScheme(.dark)
    }
}

struct PresetsSection: View {
    var parameterTree: ObservableAUParameterGroup
    var body: some View {
        VStack(spacing: 12) {
            Text("Presets")
        }
        .colorScheme(.light)
        .padding(20)
    }
}