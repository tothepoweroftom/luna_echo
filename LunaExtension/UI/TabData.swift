import SwiftUI

struct TabData: Identifiable {
    let id: Int 
    let title: String
    let color: Color
    let content: AnyView
    var textColor: Color {
        // The Presets tab (id 3) has a light background and needs dark text.
        id == 3 ? Color.black : Color.white
    }
}