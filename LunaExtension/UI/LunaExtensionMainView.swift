import AudioPluginUI
import AudioToolbox
import SwiftUI

struct PluginBar: View {
    @Environment(\.audioPluginTheme) private var theme
    let isHorizontal: Bool
    let collapsedWidth: CGFloat
    let collapsedHeight: CGFloat
    let titleFrameSize: CGSize
    
    var body: some View {
        ZStack(alignment: .topLeading) {
            Color.black
            
            if isHorizontal {
                // Horizontal accordion style - vertical title in collapsed width
                VStack {
                    Spacer()
                    Text("Luna Echo")
                        .font(theme.typography.headline)
                        .rotationEffect(.degrees(theme.layout.accordion.titleRotation))
                        .fixedSize()
                        .frame(width: titleFrameSize.width, height: titleFrameSize.height)
                        .foregroundColor(.white)
                    Spacer()
                }
                .frame(width: collapsedWidth)
                .clipped()
            } else {
                // Vertical accordion style - horizontal title in collapsed height
                HStack {
                    Text("Luna Echo")
                        .font(theme.typography.headline)
                        .padding()
                        .foregroundColor(.white)
                    Spacer()
                }
                .frame(maxWidth: .infinity)
                .frame(height: collapsedHeight)
            }
        }
        .frame(
            maxWidth: isHorizontal ? collapsedWidth : .infinity,
            maxHeight: isHorizontal ? .infinity : collapsedHeight
        )
        .clipped()
    }
}

struct LunaExtensionMainView: View {
    var parameterTree: APParameterGroup
    @State private var selectedTabIndex: Int = 0

    private let tabs: [APTabData]

    init(
        parameterTree: APParameterGroup,
        audioUnit: LunaExtensionAudioUnit
    ) {
        self.parameterTree = parameterTree
        tabs = [
            APTabData(
                id: 0, title: "Main", color: Color(hex: "#121420"),
                textColor: Color(hex: "#ffffff"),
                content: { CoreToneSection(parameterTree: parameterTree) }
            ),
            APTabData(
                id: 1, title: "Character", color: Color(hex: "#1B2432"),
                textColor: Color(hex: "#ffffff"),
                content: { CharacterSection(parameterTree: parameterTree) }
            ),
            APTabData(
                id: 2, title: "Pitch & Glitch", color: Color(hex: "#242F42"),
                textColor: Color(hex: "#ffffff"),
                content: { PitchAndGlitchSection(parameterTree: parameterTree) }
            ),
            APTabData(
                id: 3, title: "Dynamics", color: Color(hex: "#2B384F"),
                textColor: Color(hex: "#ffffff"),
                content: { DynamicsOutputSection(parameterTree: parameterTree) }
            ),
            APTabData(
                id: 4, title: "Presets", color: Color(hex: "#C7D4E2"),
                textColor: Color(hex: "#000000"),
                content: {
                    PresetsSection(
                        parameterTree: parameterTree,
                        audioUnit: audioUnit
                    )
                }
            ),
        ]
    }

    var body: some View {
        ThemeContext(theme: .lunaEcho) {
            GeometryReader { geometry in
                let w = geometry.size.width
                let h = geometry.size.height
                let isLandscape = w > h
                let small = min(w, h) < 420
                let collapsedWidth: CGFloat = small ? 40 : 60
                let collapsedHeight: CGFloat = small ? 36 : 44
                
                if isLandscape {
                    HStack(spacing: 0) {
                        // Fixed Luna Echo bar on the left
                        PluginBar(
                            isHorizontal: true,
                            collapsedWidth: collapsedWidth,
                            collapsedHeight: collapsedHeight,
                            titleFrameSize: CGSize(width: small ? 110 : 150, height: small ? 44 : 60)
                        )
                        
                        // Accordion content
                        APHorizontalAccordion(
                            tabs: tabs,
                            selectedTabIndex: $selectedTabIndex
                        )
                        .collapsedWidth(collapsedWidth)
                        .titleFrameSize(CGSize(width: small ? 110 : 150, height: small ? 44 : 60))
                        .contentAnimationDelay(0.05)
                    }
                } else {
                    VStack(spacing: 0) {
                        // Fixed Luna Echo bar on the top
                        PluginBar(
                            isHorizontal: false,
                            collapsedWidth: collapsedWidth,
                            collapsedHeight: collapsedHeight,
                            titleFrameSize: CGSize(width: small ? 110 : 150, height: small ? 44 : 60)
                        )
                        
                        // Accordion content
                        APVerticalAccordion(
                            tabs: tabs,
                            selectedTabIndex: $selectedTabIndex
                        )
                        .headerHeight(small ? 40 : 60)
                        .collapsedHeight(collapsedHeight)
                        .contentAnimationDelay(0.05)
                    }
                }
            }
            .frame(minWidth: 300, minHeight: 300)
        }
    }
}
