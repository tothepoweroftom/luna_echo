//
//  LunaApp.swift
//  Luna
//
//  Created by Thomas Power on 04/05/2025.
//

import SwiftUI

@main
struct LunaApp: App {
    @State private var hostModel = AudioUnitHostModel()

    var body: some Scene {
        WindowGroup {
            ContentView(hostModel: hostModel)
        }
    }
}
