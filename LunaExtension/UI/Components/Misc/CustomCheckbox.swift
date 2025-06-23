import SwiftUI

struct CustomCheckbox: View {
    @Binding var isChecked: Bool
    var onToggle: () -> Void

    var body: some View {
        Button(action: {
            self.isChecked.toggle()
            self.onToggle()
        }) {
            Image(systemName: isChecked ? "music.note" : "clock")
                .foregroundColor(isChecked ? .accent : .accent)
        }
    }
} 