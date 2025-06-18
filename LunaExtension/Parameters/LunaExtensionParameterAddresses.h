//
//  LunaExtensionParameterAddresses.h
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

#pragma once

#include <AudioToolbox/AUParameters.h>

// Renamed delay(ms) to delay_ms as parentheses are not allowed in enum names.
// Renamed hidden parameters with _orig suffix conceptually, but removed them
// from the public enum. Added new macro and diffusion parameters.

typedef NS_ENUM(AUParameterAddress, LunaExtensionParameterAddress) {
    // Visible Parameters from Macalla.dsp
    spread_amount = 0,
    output_gain = 1,
    delay_ms = 2, // Original: 'delay(ms)'
    feedback = 3,
    pitch_shift = 4,
    highpass = 5,         // Original: 'highpass'
    lowpass = 6,          // Original: 'lowpass'
    diffusion_amount = 7, // New
    tape_wear_macro = 8,  // New Macro
    glitch_macro = 9,     // New Macro
    ducking_macro = 10,   // New Macro
    duck_attack = 11,     // Original: 'attack'
    duck_release = 12,    // Original: 'release'
    mix = 13,

    // Sync Parameters (New)
    delayTimeSync = 14,
    syncEnabled = 15,

    // BitCrush Parameters (New)
    bitCrush = 16,

    // Count must be the last element
    ParameterAddressCount
};
