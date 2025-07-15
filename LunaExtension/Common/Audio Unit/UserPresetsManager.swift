
import Foundation
import AudioToolbox
import os

private let log = Logger(subsystem: "com.wildsurmisemusic.LunaExtension", category: "UserPresetsManager")

/**
 Manager of user presets for the AUv3 component. Supports creation, renaming, and deletion. Also, manages the
 `currentPreset` attribute of the component.
 */
public class UserPresetsManager {

    /// The slice of the AUv3 component that the manager works with
    public let audioUnit: AUAudioUnitPresetsFacade

    /// The (user) presets straight from the component
    public var presets: [AUAudioUnitPreset] { audioUnit.userPresets }

    /// The (user) presets from the component ordered by preset number in descending order (-1 first)
    public var presetsOrderedByNumber: [AUAudioUnitPreset] { presets.sorted { $0.number > $1.number } }

    /// The (user) presets from the component ordered by preset name in ascending order
    public var presetsOrderedByName: [AUAudioUnitPreset] {
        presets.sorted { $0.name.localizedCaseInsensitiveCompare($1.name) == .orderedAscending }
    }

    public var currentPreset: AUAudioUnitPreset? { audioUnit.currentPreset }
    
    public var factoryPresets: [AUAudioUnitPreset] { audioUnit.factoryPresets ?? [] }

    /**
     Create new instance for the given AUv3 component

     - parameter audioUnit: AUv3 component
     */
    public init(for audioUnit: AUAudioUnitPresetsFacade) {
        self.audioUnit = audioUnit
    }

    /**
     Locate the user preset with the given name

     - parameter name: the name to look for
     - returns: the preset that was found or nil
     */
    public func find(name: String) -> AUAudioUnitPreset? {
        presets.first(where: { $0.name == name })
    }

    /**
     Locate the user preset with the given number

     - parameter number: the number to look for
     - returns: the preset that was found or nil
     */
    public func find(number: Int) -> AUAudioUnitPreset? {
        presets.first(where: { $0.number == number })
    }

    /// Clear the `currentPreset` attribute of the component.
    public func clearCurrentPreset() {
        audioUnit.currentPreset = nil
    }

    /**
     Make the first user preset with the given name the current preset.

     - parameter name: the name to look for
     */
    public func makeCurrentPreset(name: String) {
        if let preset = find(name: name) {
            audioUnit.currentPreset = preset
        }
    }

    /**
     Make the first preset with the given preset number the current preset. NOTE: this
     will access factory presets when the number is non-negative.

     - parameter number: the number to look for
     */
    public func makeCurrentPreset(number: Int) {
        if number >= 0 {
            audioUnit.currentPreset = factoryPresets.first { $0.number == number }
        } else {
            audioUnit.currentPreset = find(number: number)
        }
    }
    
    public func makeCurrentPreset(_ preset: AUAudioUnitPreset) {
        audioUnit.currentPreset = preset
    }

    /**
     Create a new user preset under the given name. The number assigned to the preset is the smallest negative value
     that is not being used by any other user preset. Makes the new preset current.

     - parameter name: the name to use for the preset
     - throws exception from AUAudioUnit
     */
    public func create(name: String) throws {
        let preset = AUAudioUnitPreset(number: nextNumber, name: name)
        try audioUnit.saveUserPreset(preset)
        audioUnit.currentPreset = preset
    }

    /**
     Update a given user preset by saving it again, presumably with new state from the AUv3 component.

     - parameter preset: the existing preset to save
     - throws exception from AUAudioUnit
     */
    public func update(preset: AUAudioUnitPreset) throws {
        guard preset.number < 0 else { return }
        try audioUnit.saveUserPreset(preset)
        audioUnit.currentPreset = preset
    }

    /**
     Change the name of the _current_ preset to a new value.

     - parameter name: the new name to use
     - throws exception from AUAudioUnit
     */
    public func renameCurrent(to name: String) throws {
        guard let old = audioUnit.currentPreset, old.number < 0 else { return }
        let new = AUAudioUnitPreset(number: old.number, name: name)
        try audioUnit.deleteUserPreset(old)
        try audioUnit.saveUserPreset(new)
        audioUnit.currentPreset = new
    }

    /**
     Delete the existing user preset that is currently active.

     - throws exception from AUAudioUnit
     */
    public func deleteCurrent() throws {
        guard let preset = audioUnit.currentPreset, preset.number < 0 else { return }
        audioUnit.currentPreset = nil
        try audioUnit.deleteUserPreset(preset)
    }

    /// Obtain the smallest user preset number that is not being used by any other preset.
    public var nextNumber: Int {
        let ordered = presetsOrderedByNumber
        var number = -1
        for entry in ordered {
            if entry.number != number {
                break
            }
            number -= 1
        }
        return number
    }
} 