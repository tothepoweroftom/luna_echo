import SwiftUI

struct VerticalAccordionView: View {
    let tabs: [TabData]
    @Binding var selectedTabIndex: Int

    var body: some View {
        VStack(spacing: 0) {
            ForEach(tabs) { tab in
                let isSelected = selectedTabIndex == tab.id

                ZStack(alignment: .topLeading) {
                    tab.color.edgesIgnoringSafeArea(.all)  // Background color

                    VStack(spacing: 0) {
                        // Tab Header (Always visible)
                        HStack {
                            Text(tab.title)
                                .font(.headline)
                                .padding()
                                .foregroundColor(tab.textColor)
                            Spacer()  // Push title left
                        }
                        .frame(maxWidth: .infinity)
                        .frame(height: 60)  // Fixed header height
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
                                .transition(
                                    .opacity.animation(.easeInOut(duration: 0.2).delay(0.1)))
                        }
                        // else: No content shown, frame collapses below
                    }
                    // Content will only take space if isSelected is true due to ScrollView inside
                }
                // Animate the height based on selection
                .frame(maxWidth: .infinity, maxHeight: isSelected ? .infinity : 44)  // Expand height or keep header height
                .clipped()  // Clip content to animated frame
                .animation(.easeInOut(duration: 0.4), value: selectedTabIndex)
            }
        }
    }
} 