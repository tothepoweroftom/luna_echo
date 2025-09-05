import("stdfaust.lib");
declare name "Scientific Noise Generator";
declare category "Noise / Generator";

// Main Controls
noise_amount = hslider("Noise Amount[unit:percent]", 0.1, 0.0, 1.0, 0.01) : si.smoo;
noise_type = nentry("Noise Type[style:menu{'White':0;'Pink':1;'Brown':2;'Blue':3;'Violet':4;'Frown':5;'Smile':6;'Magnetic Tape':7;'VHS':8;'Vinyl/Record':9;'Cassette':10;'Reel-to-Reel':11;'Electric Hum':12;'Radio Static':13}]", 0, 0, 13, 1);

// Mix Controls
mix_amount = hslider("Mix[unit:percent]", 50.0, 0.0, 100.0, 0.1) : si.smoo : /(100.0);
input_gain = hslider("Input[unit:dB]", 0.0, -24.0, 24.0, 0.1) : ba.db2linear : si.smoo;
output_gain = hslider("Output[unit:dB]", 0.0, -24.0, 24.0, 0.1) : ba.db2linear : si.smoo;

// Post-Noise Filter Controls
hpf_freq = hslider("HPF[unit:Hz]", 20.0, 10.0, 1000.0, 1.0) : si.smoo;
lpf_freq = hslider("LPF[unit:Hz]", 16000.0, 1000.0, 20000.0, 1.0) : si.smoo;

// Envelope Follower Controls
attack_time = hslider("Attack[unit:ms]", 50.0, 1.0, 500.0, 1.0) : /(1000.0) : si.smoo;
release_time = hslider("Release[unit:ms]", 500.0, 10.0, 5000.0, 1.0) : /(1000.0) : si.smoo;

// Stereo Width Control
stereo_width = hslider("Stereo Width[unit:percent]", 100.0, 0.0, 100.0, 1.0) : /(100.0) : si.smoo;

// Common amplitude follower for all dynamic noises
amp_follower(x) = x : an.amp_follower_ar(attack_time, release_time);

// =============================================================================
// WHITE NOISE - Flat frequency response
// =============================================================================
white_noise(x) = no.noise * amp_follower(x) * noise_amount;

// =============================================================================
// PINK NOISE - 3dB fall per octave (equal energy per octave)
// =============================================================================
pink_noise(x) = no.pink_noise * amp_follower(x) * noise_amount;

// =============================================================================
// BROWN NOISE - 6dB fall per octave
// =============================================================================
brown_noise(x) = no.colored_noise(6, -1.0) * amp_follower(x) * noise_amount;

// =============================================================================
// BLUE NOISE - 3dB rise per octave
// =============================================================================
blue_noise(x) = no.colored_noise(6, 0.5) * amp_follower(x) * noise_amount;

// =============================================================================
// VIOLET NOISE - 6dB rise per octave
// =============================================================================
violet_noise(x) = no.colored_noise(6, 1.0) * amp_follower(x) * noise_amount;

// =============================================================================
// FROWN NOISE - Radio static with crackling and tuning artifacts
// =============================================================================
frown_noise(x) = radiostaticnoise * amp_follower(x) * noise_amount
with {
    // Subtle bandpass filtered white noise for gentle radio character
    base_static = no.noise : fi.bandpass(1, 320, 1250) * 0.3;
    
    // Very gentle tuning sweep effect
    tuning_freq = 600 + os.lf_triangle(0.1) * 100;
    tuning_whistle = os.oscsin(tuning_freq) * 0.02;
    
    // Reduced static artifacts
    static_pops = no.sparse_noise(8) * 0.15;
    static_crackle = no.sparse_noise(20) * 0.1;
    
    // Subtle atmospheric effects
    fading = os.lf_triangle(0.05 + no.lfnoise0(0.005) * 0.01) * 0.1 + 0.9;
    
    radiostaticnoise = (
        base_static * fading +
        tuning_whistle +
        static_pops +
        static_crackle
    ) : fi.bandpass(1, 320, 1250);                   // Keep original frown frequency range
};

// =============================================================================
// SMILE NOISE - Bandgap filtered white noise (inverse of frown)
// =============================================================================
smile_noise(x) = (no.noise <: smile_filter :> _) * amp_follower(x) * noise_amount
with {
    // Bandgap = High-pass above 1250Hz + Low-pass below 320Hz
    low_band = fi.lowpass(1, 320);    // Frequencies below 320Hz
    high_band = fi.highpass(1, 1250); // Frequencies above 1250Hz
    smile_filter = low_band, high_band; // Two parallel filters
};

// =============================================================================
// ANALOG TAPE FLAVORS - Dynamic, character-rich noise types
// =============================================================================

// Magnetic Tape - Warm, musical tape saturation
magnetic_tape_noise(x) = tapenoise * dynamic_noise_level * noise_amount
with {
    ampFollower = x : an.amp_follower_ar(0.05, 0.5);
    dynamic_noise_level = ampFollower;
    
    tapenoise = (
        no.pink_noise * 1.2 + 
        no.sparse_noise(20) * 0.5 + 
        os.lf_triangle(60) * 0.2 + 
        os.oscsin(528) * 0.01 + 
        os.oscsin(110) * 0.01
    ) : fi.bandpass(1, 500, 10000);
};

// VHS - Harsh digital artifacts and sync noise with low-frequency clicking
vhs_noise(x) = tapenoise * amp_follower(x) * noise_amount
with {
    // VHS characteristics: harsher, more digital artifacts, sync noise
    sync_artifacts = os.lf_squarewave(59.94) * 0.1; // NTSC sync frequency
    head_switching = os.lf_squarewave(30) * no.noise * 0.03;
    
    // Characteristic VHS low-frequency sawtooth hum around 50Hz
    vhs_hum_freq = 50; // Slight frequency variation
    vhs_hum = os.sawtooth(vhs_hum_freq) * 0.95 * no.lfnoise(1);
    
    tapenoise = (
        no.noise * 0.8 +                    // More white noise content
        no.sparse_noise(50) * 0.7 +         // More frequent dropouts
        sync_artifacts +                     // Video sync artifacts
        head_switching +                     // Head switching noise
        vhs_hum +                            // Low-frequency hum
        os.oscsin(15750) * 0.005            // Horizontal sync frequency
    ) : fi.bandpass(1, 500, 15000)          // Allow lower frequencies for clicking
      : fi.highpass(1, 40);                 // Keep the low-frequency clicking
};

// Vinyl/Record - Surface noise, crackles, pops, rumble
vinyl_noise(x) = tapenoise * dynamic_noise_level * noise_amount
with {
    ampFollower = x : an.amp_follower_ar(0.1, 0.8);
    dynamic_noise_level = ampFollower * 0.7; // Less dynamic response
    
    // Vinyl characteristics: surface noise, crackles, pops, rumble
    surface_crackle = no.sparse_noise(200) * 0.4;
    random_pops = no.sparse_noise(5) * 2.0;
    rumble = os.lf_triangle(33.33/60) * no.pink_noise * 0.1; // 33 RPM turntable
    
    tapenoise = (
        no.pink_noise * 0.6 +                // Surface noise base
        surface_crackle +                     // Crackling sounds
        random_pops +                         // Random pops and clicks
        rumble                                // Low frequency rumble
    ) : fi.lowpass(2, 12000)                 // Natural high frequency roll-off
      : fi.highpass(1, 20);                  // Remove subsonic content
};

// Cassette - Dull, compressed tape sound with wow/flutter
cassette_noise(x) = tapenoise * dynamic_noise_level * noise_amount
with {
    ampFollower = x : an.amp_follower_ar(0.08, 0.6);
    dynamic_noise_level = ampFollower;
    
    // Cassette characteristics: duller than reel-to-reel, more hiss, wow/flutter
    wow_flutter = os.lf_triangle(0.5 + no.noise * 0.2) * 0.02;
    tape_hiss = no.noise * 0.5;
    
    tapenoise = (
        no.pink_noise * 1.0 +                // Base tape noise
        tape_hiss +                          // Tape hiss
        no.sparse_noise(15) * 0.3 +          // Occasional dropouts
        os.oscsin(8000 + wow_flutter * 100) * 0.01  // Wow/flutter artifacts
    ) : fi.lowpass(3, 8000)                  // Significant high frequency roll-off
      : fi.bandpass(1, 100, 12000);
};

// Reel-to-Reel - Clean, professional tape sound with subtle warmth
reel_to_reel_noise(x) = tapenoise * amp_follower(x) * noise_amount * 0.3
with {
    // Reel-to-reel characteristics: cleanest, most subtle, warm saturation
    tape_saturation = no.pink_noise * 0.1 : ma.tanh;
    
    tapenoise = (
        no.pink_noise * 0.4 +                // Very subtle base noise
        tape_saturation +                     // Gentle saturation warmth
        os.oscsin(120) * 0.005 +             // Subtle power line hum
        no.sparse_noise(2) * 0.1             // Very rare artifacts
    ) : fi.bandpass(1, 50, 18000);           // Wide, clean frequency response
};

// Electric Hum - Power line interference and electrical noise
electric_hum_noise(x) = humnoise * (0.5 + amp_follower(x) * 0.5) * noise_amount
with {
    // Main power line frequency (50Hz or 60Hz depending on region)
    power_freq = 60.0; // North American standard (use 50.0 for Europe/Asia)
    
    // Power line fundamental as sawtooth for rich harmonics
    fundamental = os.sawtooth(power_freq) * 0.6;
    
    
    // Electrical interference and buzzing
    buzz_freq = 60 + no.lfnoise0(0.1) * 0.5;
    electrical_buzz = os.lf_squarewave(buzz_freq) * 0.3;
    
    // Transformer hum (subharmonics as sawtooth)
    transformer_hum = os.sawtooth(power_freq / 2) * 0.12;
    
    // Random electrical pops and clicks
    electrical_pops = no.sparse_noise(8) * 0.5;
    
    // Ground loop hum (sawtooth with slight frequency drift)
    ground_loop = os.sawtooth(power_freq + no.lfnoise0(0.05) * 2) * 0.2;
    
    humnoise = (
        fundamental +
        transformer_hum +
        electrical_pops +
        ground_loop
    ) : fi.lowpass(3, 2000)                      // Limit high frequency content
      : fi.highpass(1, 25);                     // Remove DC and very low frequencies
};

// Radio Static - AM/FM radio interference and atmospheric noise
radio_static_noise(x) = staticnoise * (0.3 + amp_follower(x) * 0.7) * noise_amount
with {
    // Base white noise for static
    base_static = no.noise * 0.7;
    
    // AM radio characteristics - amplitude modulation artifacts
    am_carrier_leak = os.oscsin(1000 + no.lfnoise0(0.1) * 200) * 0.1;
    am_heterodyne = os.oscsin(455 + no.lfnoise0(0.05) * 10) * 0.05; // IF frequency artifacts
    
    // FM radio characteristics - frequency sweeps and captures
    fm_sweep_freq = 100000 + no.lfnoise0(0.2) * 10000;
    fm_static = no.noise : fi.bandpass(2, 15000, 20000) * 0.4;
    
    // Atmospheric interference (lightning, solar activity)
    atmospheric_pops = no.sparse_noise(12) * 1.5;
    atmospheric_crackle = no.sparse_noise(50) * 0.8;
    
    // Radio frequency interference patterns - much gentler
    rfi_buzz = os.lf_squarewave(60 + no.lfnoise0(0.05) * 10) * 0.04;
    
    // Subtle digital interference 
    digital_pops = no.sparse_noise(8) * 0.1;
    
    // Very subtle tuning artifacts
    station_bleed = os.oscsin(1500) * 0.02;
    
    // Ionospheric skip and fading
    fading_mod = os.lf_triangle(0.1 + no.lfnoise0(0.01) * 0.05) * 0.3 + 0.7;
    
    staticnoise = (
        base_static * fading_mod +
        am_carrier_leak +
        am_heterodyne +
        fm_static +
        atmospheric_pops +
        atmospheric_crackle +
        rfi_buzz +
        digital_pops +
        station_bleed
    ) : fi.bandpass(1, 400, 8000)                // Narrower, less harsh range
      : fi.highpass(1, 200);                    // Remove very low frequencies
};

// =============================================================================
// NOISE SELECTOR WITH LEVEL NORMALIZATION
// =============================================================================
select_noise = noise_output * level_compensation
with {
    // Level compensation to maintain consistent perceived loudness
    level_compensation = ba.selectn(14, noise_type, 
        1.0,    // White
        1.0,    // Pink  
        0.7,    // Brown (louder due to low freq emphasis)
        1.2,    // Blue
        1.5,    // Violet (quieter due to high freq emphasis)
        1.5,    // Frown (toned down radio static)
        1.8,    // Smile (quieter due to bandgap)
        1.0,    // Magnetic Tape
        1.0,    // VHS
        1.0,    // Vinyl/Record
        1.0,    // Cassette
        1.0,    // Reel-to-Reel
        0.8,    // Electric Hum (tends to be prominent)
        1.0     // Radio Static (toned down)
    );
    
    noise_output = ba.selectn(14, noise_type,
        white_noise(standalone_input),
        pink_noise(standalone_input),
        brown_noise(standalone_input),
        blue_noise(standalone_input),
        violet_noise(standalone_input),
        frown_noise(standalone_input),
        smile_noise(standalone_input),
        magnetic_tape_noise(standalone_input),
        vhs_noise(standalone_input),
        vinyl_noise(standalone_input),
        cassette_noise(standalone_input),
        reel_to_reel_noise(standalone_input),
        electric_hum_noise(standalone_input),
        radio_static_noise(standalone_input)
    );
};

// =============================================================================
// PROCESS
// =============================================================================
// Input for standalone operation of dynamic tape noises
standalone_input = no.pink_noise * 0.5;

// Input mode: checkbox to enable external input
input_mode = checkbox("Use External Input");

// Post-noise filtering function
post_filter(x) = x : fi.highpass(2, hpf_freq) : fi.lowpass(2, lpf_freq);

// Main process function with proper gain staging and mixing
process = select2(input_mode, standalone_mode, effect_mode)
with {
    // Standalone mode: just outputs noise (no dry signal to mix)
    standalone_mode = generate_noise(standalone_input) : post_filter : *(output_gain);
    
    // Effect mode: input signal with noise mixed in
    effect_mode(dry_signal) = final_mix * output_gain
    with {
        // Apply input gain to incoming signal
        gained_input = dry_signal * input_gain;
        
        // Generate noise based on the gained input
        noise_signal = generate_noise(gained_input) : post_filter;
        
        // Mix dry and noise signals
        dry_level = 1.0 - mix_amount;
        wet_level = mix_amount;
        final_mix = gained_input * dry_level + noise_signal * wet_level;
    };
    
    // Unified noise generation function - all noises are now dynamic
    generate_noise(x) = ba.selectn(14, noise_type,
        white_noise(x),                               // White
        pink_noise(x),                                // Pink
        brown_noise(x),                               // Brown
        blue_noise(x),                                // Blue
        violet_noise(x),                              // Violet
        frown_noise(x),                               // Frown
        smile_noise(x),                               // Smile
        magnetic_tape_noise(x),                       // Magnetic Tape
        vhs_noise(x),                                 // VHS
        vinyl_noise(x),                               // Vinyl/Record
        cassette_noise(x),                            // Cassette
        reel_to_reel_noise(x),                        // Reel-to-Reel
        electric_hum_noise(x),                        // Electric Hum
        radio_static_noise(x)                         // Radio Static
    ) * level_compensation
    with {
        level_compensation = ba.selectn(14, noise_type, 
            1.0, 1.0, 0.7, 1.2, 1.5, 1.5, 1.8, 1.0, 1.0, 1.0, 1.0, 1.0, 0.8, 1.0
        );
    };
};
