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
feedback = vslider("feedback", 0.5, 0, 1.0, 0.01):si.smoo; // 0-1 range
noise_amount = hslider("noise_amount", 0.0, 0, 1.0, 0.01) : si.smoo;
pitch_shift = hslider("pitch_shift", 0.0, -12, 12, 1.0);
hifr1 = hslider("highpass", 250, 20, 20000, 1) : si.smoo;
lofr1 = hslider("lowpass", 10000, 20, 20000, 1) : si.smoo;

// Wow and Flutter Parameters
random_mod = hslider("random_mod", 0.002, 0, 0.01, 0.0001);
wow_rate = hslider("wow", 0.5, 0, 2, 0.01);
wow_intensity = hslider("wow_intensity", 0.3, 0, 1, 0.01) : si.smooth(0.9999);
flutter_rate = hslider("flutter", 8, 2, 20, 0.01);
flutter_intensity = hslider("flutter_intensity", 0.1, 0, 1, 0.01) : si.smooth(0.9999);

// Dynamic noise parameters
noise_dynamic = hslider("noise_dynamic", 0.8, 0, 1, 0.01) : si.smoo; // Increased default
noise_floor = hslider("noise_floor", 0.3, 0, 0.8, 0.01) : si.smoo;   // Increased default and reduced range
noise_attack = hslider("noise_attack", 5, 1, 50, 1) * 0.001;         // Reduced max range
noise_release = hslider("noise_release", 100, 10, 300, 1) * 0.001;   // Increased default

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

    // Noise
    noiseAmp = 0.00075 * delSec * noise_amount;
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
    // Wow modulation
    wow = os.lf_trianglepos(wow_rate + spread * 0.1) 
          + no.noise * random_mod : si.smooth(0.9999) * 1000.;
    
    // Flutter modulation  
    flutter = (os.lf_trianglepos(flutter_rate + spread * 0.2) 
              + no.noise * random_mod) * (0.8 + os.oscsin(0.1) * 0.2) 
              : si.smooth(0.9999) * 1000.;
    
    // Apply wow and flutter modulation
    wowflutter = de.fdelay1(ma.SR, 100. + wow*wow_intensity + flutter*(flutter_intensity/5)) 
                <:fi.lowpass(2,8000);
};

// Generate tape noise with dynamic response to signal level
generate_tape_noise(x) = tapenoise * dynamic_noise_level * noise_amount
with {
    ampFollower = x : an.amp_follower_ar(0.05, 0.5);

    // Calculate dynamic noise amount - MORE noise during LOUD parts (inverted behavior)
    // Scale it to be more noticeable
    dynamic_noise_level = ampFollower;
    
    // Tape noise with frequency-rich components - amplified for better audibility
    tapenoise = (
        no.pink_noise * 1.5 + 
        no.sparse_noise(20) * 0.5 + 
        os.triangle(60) * 0.2 + 
        os.oscsin(528) * 0.01 + 
        os.oscsin(110) * 0.01
    ) : fi.bandpass(1, 500, 10000) : *(1.5);
};

//----------------------- Ducking Section -----------------------
thresh = hslider("ducking", -30.0, -60.0, 0.0, 0.1);   // Higher default threshold
att = hslider("attack", 10.0, 1.0, 100.0, 1.0);        // Faster attack for more obvious ducking
rel = hslider("release", 200.0, 50.0, 1000.0, 1.0);    // Longer release minimum
ratio = hslider("ratio", 4.0, 1.0, 20.0, 0.1);         // Adjustable ratio
knee = 3;                                              // Small knee for smoother transitions
prePost = 0;                                           // Use pre-compression gain calculation

// Create the gain reduction function
gainReduction = co.peak_compression_gain_mono_db(co.ratio2strength(ratio),
    thresh, att * 1e-3, rel * 1e-3, knee, prePost);

// Compute ducked gain based on input signal x
duckedGain(x) = ba.db2linear(gainReduction(x));

// Apply ducking: dry signal + ducked wet signal
// Note: effect is applied to x *before* wet signal gain is applied
ducker(x, effect) = x*dry + wet*((x : effect) * duckedGain(x));

//----------------------- Delay Section -----------------------
// Processes a single channel with tape delay effect, controlled by spread
dide(spread) = delx : *(ba.db2linear(output_gain)) : add_tape_noise : co.limiter_1176_R4_mono
with {
    // Apply pitch shift then the core delay+feedback loop
    delx = ef.transpose(4096, 512, pitch_shift) : digd;

    // Add tape noise after the main processing - make noise more prominent
    add_tape_noise(x) = x + generate_tape_noise(x + 0.0001);  // Small offset to avoid complete silence

    // Core recursive delay loop structure
    digd = (+:(delayed))~(fback) // Input + feedback -> delay -> process feedback loop
    with {
        // Calculate the modulated delay time for this channel
        modulated_delay = tape_modulation(spread);
        N = int(2^19); // Max delay buffer size

        // Glitchy delay modulation (simplified)
        glitch_rate = vslider("glitch_rate", 0.5, 0.01, 5, 0.01) : si.smooth(0.999);
        glitch_amount = vslider("glitch_amount", 0.0, 0.0, 1, 0.01) : si.smooth(0.999);

        // Simple triangle LFO for glitch modulation
        glitch_mod = ma.fabs(os.lf_triangle(glitch_rate + spread*2)) 
                     * glitch_amount * (ma.SR + 1000*spread);

        // Main delay line with sdelay for smooth parameter changes
        interp = 50*ma.SR/1000.0;
        delay_time = min(N, max(0, modulated_delay + glitch_mod));
        delayed = de.sdelay(N, interp, delay_time);

        // Feedback path processing function
        feed(signal) = (processed_signal) * feedback // Combine processed signal + noise, scale by feedback
        with {
            // Apply wow/flutter effects in the feedback path for authentic tape behavior
            tape_processed = apply_wow_flutter(spread, signal);
             
            // Apply saturation and filtering to the delayed signal
            processed_signal = tape_processed : co.limiter_1176_R4_mono // Saturation
            :add_tape_noise
                                            : fi.highpass(2,hifr1)    // HPF
                                            : fi.lowpass(2,lofr1)     // LPF
                                            : fi.lowpass(2, 7000)     // Addnl LPF
                                            : *(0.96);                // Gain adj
        }; // End of 'with' block for feed

        // For now, always use the feedback path (no freeze control)
        fback = feed;
    }; // End of 'with' block for digd
}; // End of 'with' block for dide

//----------------------- Glitch Section -----------------------
glitch_rate = vslider("glitch_rate", 0.5, 0.01, 5, 0.01) : si.smooth(0.999);
glitch_amount = vslider("glitch_amount", 0.0, 0.0, 1, 0.01) : si.smooth(0.999);

//----------------------- Mix Section -----------------------
wet = vslider("mix", 50, 0, 100, 1) : /(100);
dry = 1 - wet;

//----------------------- Main Process -----------------------
// Apply the ducker (handling dry/wet mix and gain reduction) to each channel,
// passing the appropriate spread value to the dide effect function.
process(x, y) = ducker(x, dide(0)), ducker(y, dide(spread_amount));