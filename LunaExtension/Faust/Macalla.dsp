// Metadata
declare name "Macalla ";
declare category "Echo / Delay";

import("stdfaust.lib");
import("compressors.lib");
import("analyzers.lib");

// Global Parameters
spread_amount = hslider("spread_amount", 0.0, 0.0, 1.0, 0.01): si.smoo;
output_gain = hslider("output_gain", 0.0, -96.0, 12.0, 0.01): si.smoo;
delMs = hslider("delay(ms)", 300, 1, 1000, 1);
feedback = hslider("feedback", 0.5, 0, 1.0, 0.01):si.smoo; // 0-1 range
pitch_shift = hslider("pitch_shift", 0.0, -12, 12, 1.0);
hifr1 = hslider("highpass", 250, 20, 20000, 1) : si.smoo;
lofr1 = hslider("lowpass", 10000, 20, 20000, 1) : si.smoo;
diffusion_amount = hslider("diffusion", 0.0, 0.0, 1.0, 0.01) : * (0.95) : si.smoo; // Diffusion control (controls feedback in allpass)

// --- Macro Controls ---
tape_wear_macro = hslider("Tape Wear[unit:percent]", 0.1, 0.0, 1.0, 0.01) : si.smoo; // Default slightly worn
glitch_macro = hslider("Glitch[unit:percent]", 0.0, 0.0, 1.0, 0.01) : si.smoo;
ducking_macro = hslider("Ducking Amount[unit:percent]", 0.0, 0.0, 1.0, 0.01) : si.smoo;

// --- Hidden Original Parameters ---
// Tape Wear Related (Hidden)
noise_amount_orig = hslider("[1]noise_amount[hidden]", 0.0, 0, 1.0, 0.01) : si.smoo;
random_mod_orig = hslider("[2]random_mod[hidden]", 0.002, 0, 0.01, 0.0001);
wow_rate_orig = hslider("[3]wow_rate[hidden]", 0.5, 0.01, 2, 0.01); // Min rate slightly above 0
wow_intensity_orig = hslider("[4]wow_intensity[hidden]", 0.3, 0, 1, 0.01) : si.smooth(0.9999);
flutter_rate_orig = hslider("[5]flutter_rate[hidden]", 8, 2, 20, 0.01);
flutter_intensity_orig = hslider("[6]flutter_intensity[hidden]", 0.1, 0, 1, 0.01) : si.smooth(0.9999);

// Glitch Related (Hidden)
glitch_rate_orig = hslider("[7]glitch_rate[hidden]", 0.5, 0.01, 5, 0.01) : si.smooth(0.999);
glitch_amount_orig = hslider("[8]glitch_amount[hidden]", 0.0, 0.0, 1, 0.01) : si.smooth(0.999);

// Ducking Related (Hidden - Threshold & Ratio)
thresh_orig = hslider("[9]duck_thresh[hidden]", -30.0, -60.0, 0.0, 0.1);
ratio_orig = hslider("[10]duck_ratio[hidden]", 4.0, 1.0, 20.0, 0.1);

// Ducking Timing (Kept Visible)
att = hslider("duck_attack", 10.0, 1.0, 100.0, 1.0);        // Faster attack for more obvious ducking
rel = hslider("duck_release", 200.0, 50.0, 1000.0, 1.0);    // Longer release minimum
knee = 3;                                              // Small knee for smoother transitions (Could be made param later)
prePost = 0;                                           // Use pre-compression gain calculation

// --- Derived Parameters from Macros ---
derived_tape_wear_params = environment {
    macro = tape_wear_macro;
    // Map macro 0->1 to parameter ranges
    noise_amount = macro * macro; // Use power scaling (macro^2)
    random_mod = macro * 0.01;
    wow_rate = 0.01 + macro * (2.0 - 0.01); // Scale from 0.01 to 2.0
    wow_intensity = macro * 0.60;
    flutter_rate = 2.0 + macro * (20.0 - 2.0); // Scale from 2.0 to 20.0
    flutter_intensity = macro * 0.50;
};

derived_glitch_params = environment {
    macro = glitch_macro;
    glitch_amount = macro * 1.0; // Map 0-1 macro to 0-1 amount
    glitch_rate = 0.01 + macro * (5.0 - 0.01); // Map 0-1 macro to 0.01-5 rate
};

derived_ducking_params = environment {
     macro = ducking_macro;
     // Map macro 0->1 to thresh 0 -> -60
     thresh = 0.0 - macro * 60.0;
     // Map macro 0->1 to ratio 1 -> 20
     ratio = 1.0 + macro * 19.0;
     // Keep attack/release from sliders
     attack = att;
     release = rel;
};

//----------------------- Tape Modulation Helpers -----------------------
// Average filter
filterLength = 500;
movingAvg(x) = x : (+ ~ (_ * (1.0 - avgFactor))) * avgFactor
   with { 
    avgFactor = 1.0/filterLength;
    };
// creating an angular frequency
pulse(freq) = os.lf_saw(freq) * (ma.PI);

//----------------------- Simplified Tape Modulation Function -----------------------
// Takes spread (0 for L, spread_amount for R) and returns modulated delay in samples (float)
tape_modulation(spread) = delayProcessed
with {
    delSec = delMs / 1000.0;
    delayAvg = delMs * (ma.SR/1000.0) : movingAvg;

    // Noise - USE DERIVED PARAM
    noiseAmp = 0.00075 * delSec * derived_tape_wear_params.noise_amount;
    LPfilter = fi.lowpass(1, 70);
    // Use separate noise instances for L/R by calling no.noise within the calculation path
    delNoise = (no.noise * noiseAmp * ma.SR) : LPfilter;

    // Total Delay Calculation
    delayTotal = delNoise + delayAvg;

    // Final Processed Delay (float for de.fdelay)
    delayProcessed = delayTotal;
};

//----------------------- Wow and Flutter Effects -----------------------
// Wow and flutter tape effects
tanh(x) = x * (27 + x * x) / (27 + 9 * x * x);
saturator = ef.dryWetMixerConstantPower(drive, _ <: tanh * drive * 20 : co.limiter_1176_R4_mono : tanh);

// Apply just wow and flutter modulation (no noise) for feedback loop
apply_wow_flutter(spread, x) = wowflutter(x)
with {
    // Wow modulation - USE DERIVED PARAMS
    wow = os.lf_trianglepos(derived_tape_wear_params.wow_rate + spread * 0.2)
          + no.noise * derived_tape_wear_params.random_mod : si.smooth(0.9999) * 1000.;
    
    // Flutter modulation - USE DERIVED PARAMS
    flutter = (os.lf_trianglepos(derived_tape_wear_params.flutter_rate + spread * 0.2)
              + no.noise * derived_tape_wear_params.random_mod) * (0.8 + os.oscsin(0.1) * 0.2)
              : si.smooth(0.9999) * 1000.;
    
    // Apply wow and flutter modulation - USE DERIVED PARAMS
    wowflutter = de.fdelay1(ma.SR, 100. + wow*derived_tape_wear_params.wow_intensity + flutter*(derived_tape_wear_params.flutter_intensity/5))
                <:fi.lowpass(2,8000);
};

// Generate tape noise with dynamic response to signal level
generate_tape_noise(x) = tapenoise * dynamic_noise_level * derived_tape_wear_params.noise_amount // USE DERIVED PARAM
with {
    ampFollower = x : an.amp_follower_ar(0.05, 0.5); // Keep original dynamic noise params for now

    // Calculate dynamic noise amount - MORE noise during LOUD parts (inverted behavior)
    // Scale it to be more noticeable
    dynamic_noise_level = ampFollower;
    
    // Tape noise with frequency-rich components - amplified for better audibility
    tapenoise = (
        no.pink_noise * 1.2 + 
        no.sparse_noise(20) * 0.5 + 
        os.triangle(60) * 0.2 + 
        os.oscsin(528) * 0.01 + 
        os.oscsin(110) * 0.01
    ) : fi.bandpass(1, 500, 10000);
};

//----------------------- Ducking Section -----------------------
// Parameters defined above with macros

// Create the gain reduction function - USE DERIVED PARAMS
gainReduction = co.peak_compression_gain_mono_db(co.ratio2strength(derived_ducking_params.ratio),
    derived_ducking_params.thresh, derived_ducking_params.attack * 1e-3, derived_ducking_params.release * 1e-3, knee, prePost);

// Compute ducked gain based on input signal x
duckedGain(x) = ba.db2linear(gainReduction(x));

// Apply ducking: dry signal + ducked wet signal
// Note: effect is applied to x *before* wet signal gain is applied
ducker(x, effect) = x*dry + wet*((x : effect) * duckedGain(x));

//----------------------- Delay Section -----------------------
// Processes a single channel with tape delay effect, controlled by spread
dide(spread) = *(ba.db2linear(output_gain)) : ef.transpose(4096, 512, pitch_shift) : delx :  add_tape_noise : co.limiter_1176_R4_mono
with {
    // Apply pitch shift then the core delay+feedback loop
    delx = digd;

    // Add tape noise after the main processing - make noise more prominent
    add_tape_noise(x) = x + generate_tape_noise(x + 0.0001);  // Small offset to avoid complete silence

    // Core recursive delay loop structure
    digd = (+:(delayed))~(fback) // Input + feedback -> delay -> process feedback loop
    with {
        // Calculate the modulated delay time for this channel
        modulated_delay = tape_modulation(spread);
        N = int(2^19); // Max delay buffer size

        // Glitch parameters derived from macro
        glitch_amount = derived_glitch_params.glitch_amount;
        glitch_rate = derived_glitch_params.glitch_rate;

        // Simple triangle LFO for glitch modulation
        glitch_mod = ma.fabs(os.lf_triangle(glitch_rate + spread*2))
                     * glitch_amount * (ma.SR + 1000*spread);

        // Main delay line with sdelay for smooth parameter changes
        interp = 50*ma.SR/1000.0;
        delay_time = min(N, max(0, modulated_delay + glitch_mod));
        delayed = de.sdelay(N, interp, min(2^19, dtime + 40*spread + glitch_mod)) with {
            dtime = hslider("delaytime", 500, 1, 5000, 0.1)*ma.SR/1000.0;
            dbpm = ba.tempo(hslider("bpm",120,24,360,1));
            interp = 50*ma.SR/1000.0;
            N = int(2^19);
        };

        // Feedback path processing function
        feed(signal) = (processed_signal : diffusion_stage) * feedback // Apply diffusion before feedback scaling
        with {
            // Apply wow/flutter effects in the feedback path for authentic tape behavior
            tape_processed = apply_wow_flutter(spread, signal);
          
            // Add checkbox for bitcrushing
            bitcrush_enable = checkbox("BitCrush Enable");
            
            // Apply saturation and filtering to the delayed signal
            processed_signal = tape_processed <: 
                                // Use select2 to choose between direct or bitcrushed signal
                                select2(bitcrush_enable, 
                                    _,                                  // Bypass when disabled
                                    bitcrusher(7) : sampleRedux(6000)  // Fixed 7 bits and 12000Hz when enabled
                                ) : co.limiter_1176_R4_mono 
                                : add_tape_noise
                                : fi.highpass(2, hifr1)    // HPF
                                : fi.lowpass(2, lofr1)     // LPF
                                : fi.lowpass(2, 8000)      // Addnl LPF
                                : *(0.96);                 // Gain adj

            // Sample rate reduction function with fixed value
            sampleRedux(rate) = ba.downSample(rate);

            // Bitcrusher function with fixed bit depth
            scaler(nbits) = float(2^nbits-1);
            custom_round(x) = floor(x+0.5);
            bitcrusher(nbits,x) = x :abs: *(scaler(nbits)) : custom_round : /(scaler(nbits)) * (2*(x>0)-1.0);

            // Diffusion stage using two nested allpass filters
            diffusion_stage = fi.allpass_fcomb(max_d1, d1, g) : fi.allpass_fcomb(max_d2, d2, g*0.9) // Series allpass
            with {
                max_delay_ms = 50; // Maximum delay for diffusion allpass in ms
                max_d1 = int(max_delay_ms * ma.SR / 1000) + 1; // Ensure space for max float delay
                max_d2 = int(max_delay_ms * ma.SR / 1000) + 1; // Ensure space for max float delay

                // Short, modulated delay times for the allpass filters (uncorrelated)
                // Using prime number relation for base ms can help avoid resonances
                base_d1_ms = 7 + spread * 0.5; // Base delay in ms
                base_d2_ms = 11.3 + spread * 0.7; // Base delay in ms
                mod_rate1 = 0.15 + spread * 0.02;
                mod_rate2 = 0.23 + spread * 0.03;
                mod_depth_ms = 1.5; // Modulation depth in ms

                d1_raw = (base_d1_ms + os.lf_triangle(mod_rate1) * mod_depth_ms) * ma.SR / 1000;
                d2_raw = (base_d2_ms + os.lf_triangle(mod_rate2) * mod_depth_ms) * ma.SR / 1000;

                // Ensure delay times are non-negative and within bounds for fdelay (N-1)
                d1 = max(0.0, min(float(max_d1 - 1), d1_raw));
                d2 = max(0.0, min(float(max_d2 - 1), d2_raw));

                // Feedback gain for the allpass filters controlled by the diffusion slider
                g = diffusion_amount;
            };
        }; // End of 'with' block for feed

        // For now, always use the feedback path (no freeze control)
        fback = feed;
    }; // End of 'with' block for digd
}; // End of 'with' block for dide

//----------------------- Mix Section -----------------------
wet = hslider("mix", 50, 0, 100, 1) : /(100);
dry = 1 - wet;

//----------------------- Main Process -----------------------
// Apply the ducker (handling dry/wet mix and gain reduction) to each channel,
// passing the appropriate spread value to the dide effect function.
process(x, y) = ducker(x, dide(0)), ducker(y, dide(spread_amount));