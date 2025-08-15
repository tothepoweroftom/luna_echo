import AudioPluginUI
import AudioToolbox
import SwiftUI

enum LayoutMode {
    case accordion      // Small screens - traditional accordion
    case twoColumn      // Medium screens - 2 sections side by side
    case multiColumn    // Large screens - 3+ sections visible
}

struct LunaEchoBar: View {
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


struct APMultiOpenHorizontalAccordion: View {
    @Environment(\.audioPluginTheme) private var theme
    public let tabs: [APTabData]
    @Binding public var openTabs: Set<Int>
    let layoutMode: LayoutMode
    
    public var collapsedWidth: CGFloat?
    public var animationDuration: Double?
    public var contentAnimationDelay: Double?
    public var titleRotation: Double?
    public var titleFrameSize: CGSize?
    
    var body: some View {
        HStack(spacing: 0) {
            ForEach(tabs) { tab in
                let isOpen = openTabs.contains(tab.id)
                let maxOpenTabs = 2  // Limit to 2 sections max for cleaner layout
                
                ZStack(alignment: .topLeading) {
                    tab.color.edgesIgnoringSafeArea(.all)

                    if isOpen {
                        // Expanded Content - full content area
                        HStack(spacing: 0) {
                            // Show title strip only for non-Main sections or when in small mode
                            if tab.id != 0 || layoutMode == .accordion {
                                // Small tappable header strip on the left for closing
                                VStack {
                                    Spacer()
                                    Text(tab.title)
                                        .font(theme.typography.headline)
                                        .rotationEffect(.degrees(titleRotation ?? theme.layout.accordion.titleRotation))
                                        .fixedSize()
                                        .frame(
                                            width: (titleFrameSize ?? CGSize(width: 150, height: theme.layout.accordion.headerHeight)).width, 
                                            height: (titleFrameSize ?? CGSize(width: 150, height: theme.layout.accordion.headerHeight)).height
                                        )
                                        .foregroundColor(tab.textColor)
                                    Spacer()
                                }
                                .frame(width: collapsedWidth ?? theme.layout.accordion.collapsedWidth)
                                .contentShape(Rectangle())
                                .onTapGesture {
                                    if tab.id == 0 {
                                        // Core section (Main) - only close if we're in small mode
                                        if layoutMode == .accordion {
                                            openTabs.remove(tab.id)
                                        }
                                    } else {
                                        openTabs.remove(tab.id)
                                    }
                                }
                            }
                            
                            // Content area takes up the rest (not tappable for closing)
                            tab.content
                                .padding()
                                .frame(maxWidth: .infinity, maxHeight: .infinity)
                                .transition(
                                    .opacity.animation(.easeInOut(duration: 0.2).delay(contentAnimationDelay ?? theme.animations.contentDelay)))
                        }
                        .clipped()
                    } else {
                        // Collapsed Content (Vertical Title)
                        VStack {
                            Spacer()
                            Text(tab.title)
                                .font(theme.typography.headline)
                                .rotationEffect(.degrees(titleRotation ?? theme.layout.accordion.titleRotation))
                                .fixedSize()
                                .frame(
                                    width: (titleFrameSize ?? CGSize(width: 150, height: theme.layout.accordion.headerHeight)).width, 
                                    height: (titleFrameSize ?? CGSize(width: 150, height: theme.layout.accordion.headerHeight)).height
                                )
                                .foregroundColor(tab.textColor)
                            Spacer()
                        }
                        .frame(width: collapsedWidth ?? theme.layout.accordion.collapsedWidth)
                        .contentShape(Rectangle())
                        .onTapGesture {
                            if tab.id == 0 {
                                // Core section (Main) - only open if we're in small mode
                                if layoutMode == .accordion {
                                    openTabs.insert(tab.id)
                                }
                                // In larger modes, Core stays open (do nothing)
                            } else {
                                // Other sections
                                if openTabs.count < maxOpenTabs {
                                    openTabs.insert(tab.id)
                                } else {
                                    // Close oldest non-core section, open new
                                    if let firstOpen = openTabs.first(where: { $0 != 0 }) {
                                        openTabs.remove(firstOpen)
                                    }
                                    openTabs.insert(tab.id)
                                }
                            }
                        }
                        .clipped()
                    }
                }
                .frame(
                    maxWidth: isOpen ? (tab.id == 0 ? 400 : .infinity) : (collapsedWidth ?? theme.layout.accordion.collapsedWidth), 
                    maxHeight: .infinity
                )
                .clipped()
                .animation(.easeInOut(duration: animationDuration ?? theme.animations.defaultDuration), value: openTabs)
            }
        }
    }
}

struct APMultiOpenVerticalAccordion: View {
    @Environment(\.audioPluginTheme) private var theme
    public let tabs: [APTabData]
    @Binding public var openTabs: Set<Int>
    let layoutMode: LayoutMode
    
    public var headerHeight: CGFloat?
    public var collapsedHeight: CGFloat?
    public var animationDuration: Double?
    public var contentAnimationDelay: Double?
    public var headerFont: Font?
    
    var body: some View {
        VStack(spacing: 0) {
            ForEach(tabs) { tab in
                let isOpen = openTabs.contains(tab.id)
                let maxOpenTabs = 2  // Limit to 2 sections max for cleaner layout

                ZStack(alignment: .topLeading) {
                    tab.color.edgesIgnoringSafeArea(.all)

                    VStack(spacing: 0) {
                        // Tab Header - hide for Main section in larger modes
                        if tab.id != 0 || layoutMode == .accordion {
                            HStack {
                                Text(tab.title)
                                    .font(headerFont ?? theme.typography.headline)
                                    .padding()
                                    .foregroundColor(tab.textColor)
                                Spacer()
                            }
                            .frame(maxWidth: .infinity)
                            .frame(height: headerHeight ?? theme.layout.accordion.headerHeight)
                            .contentShape(Rectangle())
                            .onTapGesture {
                                if tab.id == 0 {
                                    // Core section (Main) - only close if we're in small mode
                                    if layoutMode == .accordion {
                                        if isOpen {
                                            openTabs.remove(tab.id)
                                        } else {
                                            openTabs.insert(tab.id)
                                        }
                                    }
                                    // In larger modes, Core stays open (do nothing)
                                } else {
                                    // Other sections
                                    if isOpen {
                                        openTabs.remove(tab.id)
                                    } else if openTabs.count < maxOpenTabs {
                                        openTabs.insert(tab.id)
                                    } else {
                                        // Close oldest non-core section, open new
                                        if let firstOpen = openTabs.first(where: { $0 != 0 }) {
                                            openTabs.remove(firstOpen)
                                        }
                                        openTabs.insert(tab.id)
                                    }
                                }
                            }
                        }

                        // Content (conditional visibility and height)
                        if isOpen {
                            tab.content
                                .padding()
                                .frame(maxWidth: .infinity, maxHeight: .infinity)
                                .transition(
                                    .opacity.animation(.easeInOut(duration: 0.2).delay(contentAnimationDelay ?? theme.animations.contentDelay)))
                        }
                    }
                }
                .frame(
                    maxWidth: .infinity, 
                    maxHeight: isOpen ? (tab.id == 0 ? 500 : .infinity) : (collapsedHeight ?? theme.layout.accordion.collapsedHeight)
                )
                .clipped()
                .animation(.easeInOut(duration: animationDuration ?? theme.animations.defaultDuration), value: openTabs)
            }
        }
    }
}

// Extension methods for the multi-open accordions
extension APMultiOpenHorizontalAccordion {
    func collapsedWidth(_ width: CGFloat) -> APMultiOpenHorizontalAccordion {
        var copy = self
        copy.collapsedWidth = width
        return copy
    }
    
    func animationDuration(_ duration: Double) -> APMultiOpenHorizontalAccordion {
        var copy = self
        copy.animationDuration = duration
        return copy
    }
    
    func contentAnimationDelay(_ delay: Double) -> APMultiOpenHorizontalAccordion {
        var copy = self
        copy.contentAnimationDelay = delay
        return copy
    }
    
    func titleRotation(_ rotation: Double) -> APMultiOpenHorizontalAccordion {
        var copy = self
        copy.titleRotation = rotation
        return copy
    }
    
    func titleFrameSize(_ size: CGSize) -> APMultiOpenHorizontalAccordion {
        var copy = self
        copy.titleFrameSize = size
        return copy
    }
}

extension APMultiOpenVerticalAccordion {
    func headerHeight(_ height: CGFloat) -> APMultiOpenVerticalAccordion {
        var copy = self
        copy.headerHeight = height
        return copy
    }
    
    func collapsedHeight(_ height: CGFloat) -> APMultiOpenVerticalAccordion {
        var copy = self
        copy.collapsedHeight = height
        return copy
    }
    
    func animationDuration(_ duration: Double) -> APMultiOpenVerticalAccordion {
        var copy = self
        copy.animationDuration = duration
        return copy
    }
    
    func contentAnimationDelay(_ delay: Double) -> APMultiOpenVerticalAccordion {
        var copy = self
        copy.contentAnimationDelay = delay
        return copy
    }
    
    func headerFont(_ font: Font) -> APMultiOpenVerticalAccordion {
        var copy = self
        copy.headerFont = font
        return copy
    }
}

struct LunaExtensionMainView: View {
    var parameterTree: APParameterGroup
    @State private var selectedTabIndex: Int = 0
    @State private var openTabs: Set<Int> = [0] // Track multiple open tabs, Core (0) always open when space allows
    @State private var currentLayoutMode: LayoutMode = .accordion

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
                
                // Additional check for iPad in portrait mode
                #if os(iOS)
                let isPadPortrait = UIDevice.current.userInterfaceIdiom == .pad && !isLandscape
                #else
                let isPadPortrait = false
                #endif
                
                // Screen size breakpoints - adjusted for GarageBand's wide but shallow windows
                let referenceSize: CGFloat = {
                    if isLandscape {
                        // In landscape, prioritize width and use a lower height threshold
                        return w >= 600 ? min(w * 0.7, h + 200) : min(w, h)
                    } else {
                        // In portrait, use traditional min dimension approach
                        return min(w, h)
                    }
                }()
                
                let small = referenceSize < 420
                let medium = referenceSize >= 420 && referenceSize < 700  // Lowered from 800
                let _ = referenceSize >= 700  // large breakpoint for future use
                
                // Layout mode determination
                let layoutMode: LayoutMode = {
                    if small { return .accordion }
                    if medium { return .twoColumn }
                    return .multiColumn
                }()
                
                // Update current layout mode for onChange detection
                let _ = layoutMode != currentLayoutMode ? (currentLayoutMode = layoutMode) : ()
                

                
                let collapsedWidth: CGFloat = small ? 40 : 60
                let collapsedHeight: CGFloat = small ? 36 : 44
                
                // Layout based on mode and orientation
                switch (layoutMode, isLandscape) {
                case (.accordion, true):
                    // Traditional horizontal accordion for small screens in landscape
                    HStack(spacing: 0) {
                        LunaEchoBar(
                            isHorizontal: true,
                            collapsedWidth: collapsedWidth,
                            collapsedHeight: collapsedHeight,
                            titleFrameSize: CGSize(width: small ? 110 : 150, height: small ? 44 : 60)
                        )
                        
                        APHorizontalAccordion(
                            tabs: tabs,
                            selectedTabIndex: $selectedTabIndex
                        )
                        .collapsedWidth(collapsedWidth)
                        .titleFrameSize(CGSize(width: small ? 110 : 150, height: small ? 44 : 60))
                        .contentAnimationDelay(0.05)
                    }
                    
                case (.accordion, false):
                    // Traditional vertical accordion for small screens in portrait
                    VStack(spacing: 0) {
                        LunaEchoBar(
                            isHorizontal: false,
                            collapsedWidth: collapsedWidth,
                            collapsedHeight: collapsedHeight,
                            titleFrameSize: CGSize(width: small ? 110 : 150, height: small ? 44 : 60)
                        )
                        
                        APVerticalAccordion(
                            tabs: tabs,
                            selectedTabIndex: $selectedTabIndex
                        )
                        .headerHeight(small ? 40 : 60)
                        .collapsedHeight(collapsedHeight)
                        .contentAnimationDelay(0.05)
                    }
                    
                case (.twoColumn, _), (.multiColumn, _):
                    // Multi-open accordion for larger screens
                    // Force vertical layout for iPad portrait, even with large screen size
                    if isLandscape && !isPadPortrait {
                        HStack(spacing: 0) {
                            LunaEchoBar(
                                isHorizontal: true,
                                collapsedWidth: collapsedWidth,
                                collapsedHeight: collapsedHeight,
                                titleFrameSize: CGSize(width: small ? 110 : 150, height: small ? 44 : 60)
                            )
                            
                            APMultiOpenHorizontalAccordion(
                                tabs: tabs,
                                openTabs: $openTabs,
                                layoutMode: layoutMode
                            )
                            .collapsedWidth(collapsedWidth)
                            .titleFrameSize(CGSize(width: small ? 110 : 150, height: small ? 44 : 60))
                            .contentAnimationDelay(0.05)
                        }
                    } else {
                        VStack(spacing: 0) {
                            LunaEchoBar(
                                isHorizontal: false,
                                collapsedWidth: collapsedWidth,
                                collapsedHeight: collapsedHeight,
                                titleFrameSize: CGSize(width: small ? 110 : 150, height: small ? 44 : 60)
                            )
                            
                            APMultiOpenVerticalAccordion(
                                tabs: tabs,
                                openTabs: $openTabs,
                                layoutMode: layoutMode
                            )
                            .headerHeight(small ? 40 : 60)
                            .collapsedHeight(collapsedHeight)
                            .contentAnimationDelay(0.05)
                        }
                    }
                }
            }
            .frame(minWidth: 300, minHeight: 300)
            .onChange(of: currentLayoutMode) { oldValue, newValue in
                // Ensure Core section (0) is always open in larger modes
                if newValue != .accordion && !openTabs.contains(0) {
                    openTabs.insert(0)
                }
            }

        }
    }
}
