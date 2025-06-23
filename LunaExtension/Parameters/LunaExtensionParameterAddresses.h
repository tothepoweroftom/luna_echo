//
//  LunaExtensionParameterAddresses.h
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

#pragma once

#include <AudioToolbox/AUParameters.h>

typedef NS_ENUM(AUParameterAddress, LunaExtensionParameterAddress) {
    // Visible Parameters from Macalla.dsp
    spread_amount = 0,
    output_gain = 1,
    delay_time = 2,
    feedback = 3,
    pitch_shift = 4,
    pitch_shift2 = 5,
    highpass = 6,
    lowpass = 7,
    diffusion_amount = 8,
    tape_noise_macro = 9,
    wow_flutter_macro = 10,
    glitch_macro = 11,
    ducking_macro = 12,
    duck_attack = 13,
    duck_release = 14,
    mix = 15,
    delay_time_sync = 16,
    sync_enabled = 17,
    bit_crush = 18,
    phaser_depth = 19,

    // Count must be the last element
    ParameterAddressCount
};
