import SwiftUI

struct ThumbView: View {
    var body: some View {
        Circle()
            .foregroundColor(Color.text)
            .frame(width: 8, height: 8)
            .padding(8)
            .overlay(
                Circle()
                    .stroke(Color.accent, lineWidth: 1)
            )
    }
} 