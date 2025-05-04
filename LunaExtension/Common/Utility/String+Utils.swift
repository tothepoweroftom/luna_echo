//
//  String+Utils.swift
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

import Foundation

extension String {
    var range: NSRange {
        NSRange(location: 0, length: count)
    }
    
    func isAlphanumeric() -> Bool {
        if self.isEmpty { return false }
        let regex = try! NSRegularExpression(pattern: "^[a-zA-Z0-9_-]*$", options: .caseInsensitive)
        guard regex.firstMatch(in: self, options: [], range: range) != nil else {
            return false
        }
        return true
    }
}
