import AVFoundation
import SwiftUI

struct SyncToggle: View {
    @ObservedObject var synced: ObservableAUParameter

    var body: some View {
        Toggle(
            "Sync",
            isOn: Binding(
                get: { self.synced.value == 1 },
                set: { self.synced.value = $0 ? 1 : 0 }
            ))
    }
} 