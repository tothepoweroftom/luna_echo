//
//  LunaExtensionParameterAddresses.h
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

#pragma once

#include <AudioToolbox/AUParameters.h>

typedef NS_ENUM(AUParameterAddress, LunaExtensionParameterAddress) {
    // Original parameter
    gain = 0,

    // Parameters from Macalla.dsp
    spread_amount     = 1,
    output_gain       = 2,
    delay_ms          = 3, // 'delay(ms)' can't use parentheses in enum names
    feedback          = 4,
    noise_amount      = 5,
    pitch_shift       = 6,
    highpass          = 7,
    lowpass           = 8,
    random_mod        = 9,
    wow               = 10,
    wow_intensity     = 11,
    flutter           = 12,
    flutter_intensity = 13,
    ducking           = 14,
    attack            = 15,
    release           = 16,
    ratio             = 17,
    glitch_rate       = 18,
    glitch_amount     = 19,
    mix               = 20
};
