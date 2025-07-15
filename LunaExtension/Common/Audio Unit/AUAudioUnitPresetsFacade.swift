
import os
import AudioToolbox

/**
 Subset of AUAudioUnit functionality that is used by UserPresetsManager.
 */
public protocol AUAudioUnitPresetsFacade: NSObject {

  /// Obtain an array of factory presets that is never nil.
  var factoryPresets: [AUAudioUnitPreset]? { get }

  /// Obtain an array of user presets.
  var userPresets: [AUAudioUnitPreset] { get }

  /// Currently active preset (user or factory). May be nil.
  dynamic var currentPreset: AUAudioUnitPreset? { get set }

  /// Save the given user preset.
  func saveUserPreset(_ preset: AUAudioUnitPreset) throws

  /// Delete the given user preset.
  func deleteUserPreset(_ preset: AUAudioUnitPreset) throws

  /// Returns true if audio unit supports user presets
  var supportsUserPresets: Bool { get }
}

extension AUAudioUnit: AUAudioUnitPresetsFacade {} 