import("stdfaust.lib");
declare name "Macalla ";
declare category "Echo / Delay";

import("stdfaust.lib");
import("compressors.lib");
import("analyzers.lib");

// Global Parameters
spread_amount = hslider("spread_amount", 0.0, 0.0, 1.0, 0.01): si.smoo;
output_gain = hslider("output_gain", 0.0, -96.0, 12.0, 0.01): si.smoo;
delMs = hslider("delay(ms)", 300, 1, 1000, 1);
feedback = hslider("feedback", 0.5, 0, 1.0, 0.01):si.smoo;
pitch_shift = hslider("pitch_shift", 0.0, -12, 12, 1.0);
hifr1 = hslider("highpass", 250, 20, 20000, 1) : si.smoo;
lofr1 = hslider("lowpass", 10000, 20, 20000, 1) : si.smoo;
diffusion_amount = hslider("diffusion", 0.0, 0.0, 1.0, 0.01) : * (0.95) : si.smoo;

// Macro Controls
tape_wear_macro = hslider("Tape Noise[unit:percent]", 0.1, 0.0, 1.0, 0.01) : si.smoo;
wow_flutter_macro = hslider("Wow & Flutter[unit:percent]", 0.1, 0.0, 1.0, 0.01) : si.smoo;
glitch_macro = hslider("Glitch[unit:percent]", 0.0, 0.0, 1.0, 0.01) : si.smoo;
ducking_macro = hslider("Ducking Amount[unit:percent]", 0.0, 0.0, 1.0, 0.01) : si.smoo;

// Hidden Original Parameters
noise_amount_orig = hslider("[1]noise_amount[hidden]", 0.0, 0, 1.0, 0.01) : si.smoo;
random_mod_orig = hslider("[2]random_mod[hidden]", 0.002, 0, 0.01, 0.0001);
wow_rate_orig = hslider("[3]wow_rate[hidden]", 0.5, 0.01, 2, 0.01);
wow_intensity_orig = hslider("[4]wow_intensity[hidden]", 0.3, 0, 1, 0.01) : si.smooth(0.9999);
flutter_rate_orig = hslider("[5]flutter_rate[hidden]", 8, 2, 20, 0.01);
flutter_intensity_orig = hslider("[6]flutter_intensity[hidden]", 0.1, 0, 1, 0.01) : si.smooth(0.9999);
glitch_rate_orig = hslider("[7]glitch_rate[hidden]", 0.5, 0.01, 5, 0.01) : si.smooth(0.999);
glitch_amount_orig = hslider("[8]glitch_amount[hidden]", 0.0, 0.0, 1, 0.01) : si.smooth(0.999);
thresh_orig = hslider("[9]duck_thresh[hidden]", -30.0, -60.0, 0.0, 0.1);
ratio_orig = hslider("[10]duck_ratio[hidden]", 4.0, 1.0, 20.0, 0.1);

// Ducking Timing Controls
att = hslider("duck_attack", 10.0, 1.0, 100.0, 1.0);
rel = hslider("duck_release", 200.0, 50.0, 1000.0, 1.0);
knee = 3;
prePost = 0;

// Derived Parameters from Macros
derived_tape_noise_params = environment {
    macro = tape_wear_macro;
    noise_amount = macro * macro;
};

derived_wow_flutter_params = environment {
    macro = wow_flutter_macro;
    random_mod = macro * 0.01;
    wow_rate = 0.01 + macro * (2.0 - 0.01);
    wow_intensity = macro * 0.60;
    flutter_rate = 2.0 + macro * (20.0 - 2.0);
    flutter_intensity = macro * 0.50;
};

derived_glitch_params = environment {
    macro = glitch_macro;
    glitch_amount = macro * 1.0;
    glitch_rate = 0.01 + macro * (5.0 - 0.01);
};

derived_ducking_params = environment {
     macro = ducking_macro;
     thresh = 0.0 - macro * 60.0;
     ratio = 1.0 + macro * 19.0;
     attack = att;
     release = rel;
};

// Tape Modulation Helpers
filterLength = 500;
movingAvg(x) = x : (+ ~ (_ * (1.0 - avgFactor))) * avgFactor
   with { 
    avgFactor = 1.0/filterLength;
    };
pulse(freq) = os.lf_saw(freq) * (ma.PI);

// Tape Modulation Function
tape_modulation(spread) = delayProcessed
with {
    delSec = delMs / 1000.0;
    delayAvg = delMs * (ma.SR/1000.0) : movingAvg;

    noiseAmp = 0.00075 * delSec * derived_tape_noise_params.noise_amount;
    LPfilter = fi.lowpass(1, 70);
    delNoise = (no.noise * noiseAmp * ma.SR) : LPfilter;

    delayTotal = delNoise + delayAvg;
    delayProcessed = delayTotal;
};

// Wow and Flutter Effects
tanh(x) = x * (27 + x * x) / (27 + 9 * x * x);
saturator = ef.dryWetMixerConstantPower(drive, _ <: tanh * drive * 20 : co.limiter_1176_R4_mono : tanh);

apply_wow_flutter(spread, x) = wowflutter(x)
with {
    wow = os.lf_trianglepos(derived_wow_flutter_params.wow_rate + spread * 0.2)
          + no.noise * derived_wow_flutter_params.random_mod : si.smooth(0.9999) * 1000.;
    
    flutter = (os.lf_trianglepos(derived_wow_flutter_params.flutter_rate + spread * 0.2)
              + no.noise * derived_wow_flutter_params.random_mod) * (0.8 + os.oscsin(0.1) * 0.2)
              : si.smooth(0.9999) * 1000.;
    
    wowflutter = de.fdelay1(ma.SR, 100. + wow*derived_wow_flutter_params.wow_intensity + flutter*(derived_wow_flutter_params.flutter_intensity/5))
                <:fi.lowpass(2,8000);
};

// Dynamic tape noise generation
generate_tape_noise(x) = tapenoise * dynamic_noise_level * derived_tape_noise_params.noise_amount
with {
    ampFollower = x : an.amp_follower_ar(0.05, 0.5);
    dynamic_noise_level = ampFollower;
    
    tapenoise = (
        no.pink_noise * 1.2 + 
        no.sparse_noise(20) * 0.5 + 
        os.triangle(60) * 0.2 + 
        os.oscsin(528) * 0.01 + 
        os.oscsin(110) * 0.01
    ) : fi.bandpass(1, 500, 10000);
};

// Ducking
gainReduction = co.peak_compression_gain_mono_db(co.ratio2strength(derived_ducking_params.ratio),
    derived_ducking_params.thresh, derived_ducking_params.attack * 1e-3, derived_ducking_params.release * 1e-3, knee, prePost);

duckedGain(x) = ba.db2linear(gainReduction(x));
ducker(x, effect) = x*dry + wet*((x : effect) * duckedGain(x));

// Main delay processing
dide(spread) = *(ba.db2linear(output_gain)) : ef.transpose(4096, 512, pitch_shift) : delx :  add_tape_noise : co.limiter_1176_R4_mono
with {
    delx = digd;
    add_tape_noise(x) = x + generate_tape_noise(x + 0.0001);

    digd = (+:(delayed))~(fback)
    with {
        modulated_delay = tape_modulation(spread);
        N = int(2^19);

        glitch_amount = derived_glitch_params.glitch_amount;
        glitch_rate = derived_glitch_params.glitch_rate;

        glitch_mod = ma.fabs(os.lf_triangle(glitch_rate + spread*2))
                     * glitch_amount * (ma.SR + 1000*spread);

        interp = 50*ma.SR/1000.0;
        delay_time = min(N, max(0, modulated_delay + glitch_mod));
        delayed = de.sdelay(N, interp, min(2^19, dtime + 40*spread + glitch_mod)) with {
            dtime = hslider("delaytime", 500, 1, 5000, 0.1)*ma.SR/1000.0;
            dbpm = ba.tempo(hslider("bpm",120,24,360,1));
            interp = 50*ma.SR/1000.0;
            N = int(2^19);
        };

        feed(signal) = (processed_signal : diffusion_stage) * feedback
        with {
            tape_processed = apply_wow_flutter(spread, signal);
            bitcrush_enable = checkbox("BitCrush Enable");
            
            processed_signal = tape_processed <: 
                                select2(bitcrush_enable, 
                                    _,
                                    bitcrusher(8) : sampleRedux(11000)
                                ) : co.limiter_1176_R4_mono 
                                : add_tape_noise
                                : fi.highpass(2, hifr1)
                                : fi.lowpass(2, lofr1)
                                : *(0.98);

            sampleRedux(rate) = ba.downSample(rate);

            scaler(nbits) = float(2^nbits-1);
            custom_round(x) = floor(x+0.5);
            bitcrusher(nbits,x) = x :abs: *(scaler(nbits)) : custom_round : /(scaler(nbits)) * (2*(x>0)-1.0);

            // Diffusion allpass network
            diffusion_stage = fi.allpass_fcomb(max_d1, d1, g) : fi.allpass_fcomb(max_d2, d2, g*0.9)
            with {
                max_delay_ms = 50;
                max_d1 = int(max_delay_ms * ma.SR / 1000) + 1;
                max_d2 = int(max_delay_ms * ma.SR / 1000) + 1;

                base_d1_ms = 7 + spread * 0.5;
                base_d2_ms = 11.3 + spread * 0.7;
                mod_rate1 = 0.15 + spread * 0.02;
                mod_rate2 = 0.23 + spread * 0.03;
                mod_depth_ms = 1.5;

                d1_raw = (base_d1_ms + os.lf_triangle(mod_rate1) * mod_depth_ms) * ma.SR / 1000;
                d2_raw = (base_d2_ms + os.lf_triangle(mod_rate2) * mod_depth_ms) * ma.SR / 1000;

                d1 = max(0.0, min(float(max_d1 - 1), d1_raw));
                d2 = max(0.0, min(float(max_d2 - 1), d2_raw));

                g = diffusion_amount;
            };
        };

        fback = feed;
    };
};

// Mix controls
wet = hslider("mix", 50, 0, 100, 1) : /(100);
dry = 1 - wet;

// Main process
process(x, y) = ducker(x, dide(0)), ducker(y, dide(spread_amount));