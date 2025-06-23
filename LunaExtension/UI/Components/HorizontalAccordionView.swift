import SwiftUI

struct HorizontalAccordionView: View {
    let tabs: [TabData]
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
                                .transition(
                                    .opacity.animation(.easeInOut(duration: 0.2).delay(0.15)))
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