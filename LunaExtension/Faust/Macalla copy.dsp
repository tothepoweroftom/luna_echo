// Metadata
declare name "Macalla ";
declare category "Echo / Delay";

import("stdfaust.lib");
import("compressors.lib");

// Global Parameters
spread_amount = hslider("spread_amount", 0.0, 0.0, 1.0, 0.01): si.smoo;
output_gain = hslider("output_gain", 0.0, -96.0, 12.0, 0.01): si.smoo;

//----------------------- Chorus Section -----------------------
chorus_amount = hslider("chorus_amount", 0, 0, 1, 0.01) : si.smoo;
chorus_rate = hslider("chorus_rate", 0.5, 0.1, 5, 0.01) : si.smoo;
chorus_depth = hslider("chorus_depth", 0.3, 0, 1, 0.01) : si.smoo;

// Chorus voices with slightly detuned frequencies
chorus_voice(rate, depth) = de.fdelay(4096, del)
with {
    lfo = (1 + os.osc(rate)) * 0.5;
    del = ma.SR * 0.001 * (1 + depth * lfo);
};

chorus = _ <: _ * (1-chorus_amount), 
    (_ <: chorus_voice(chorus_rate, chorus_depth), 
        chorus_voice(chorus_rate*1.1, chorus_depth*0.7),
        chorus_voice(chorus_rate*0.9, chorus_depth*1.2) :> _) * chorus_amount :> _;

//----------------------- Color Effects -----------------------
color_type = hslider("color_type[style:menu{'None':0;'Tape':1;'Tube':2}]", 0, 0, 2, 1) : int;
drive_amount = hslider("drive_amount", 0, 0, 1, 0.01) : si.smoo;

// Tape-style saturation
tape_sat(x) = x * (1 - abs(x) * 0.3);




color_effect = tape_sat;

//----------------------- Vibrato Section -----------------------
vlfo = os.oscrs;
vfreq = 0.85;
vdepth = 1.0;
vd = 0.001 * ma.SR * 5;
vod = 0.001 * ma.SR * 1;
vdmax = 2048;  // max delay length in samples (2048 / 44100 ~= 46 ms )

vfreq_l = 0.85;  // left channel frequency
vfreq_r = 0.79;  // slightly different for right channel
phase_offset = 0.5; // offset between channels (0-1)

// Separate LFOs for left and right
vlfo_l = os.oscrs;
vlfo_r = os.oscrs;

// Separate random modulators
random_l = no.lfnoise0(12.5);
random_r = no.lfnoise0(5.3); // different rate for more variation

// Separate delay calculations for left and right
vcurdel_l = vod + vd * (1 + vlfo_l(vfreq_l + random_l)) / 2;
vcurdel_r(sprd) = vod + vd * (1 + vlfo_r(vfreq_r + random_r + sprd*0.1)) / 2 + phase_offset;

vibrato_amount = vslider("vibrato_amount", 0.5, 0.0, 1.0, 0.01) : si.smoo;


// Separate vibrato functions for left and right
vibrato_l = de.fdelay(vdmax, vcurdel_l);
vibrato_r(sprd) = de.fdelay(vdmax, vcurdel_r(sprd));



//----------------------- Saturation Section -----------------------
pdrive = 4.0;
psat = hslider("saturation", 0.0, 0.0, 0.7, 0.001) : si.smooth(0.995);
pcurve = 1.3;

// Waveshaping functions
tanh(x) = x * (27 + x * x) / (27 + 9 * x * x);
transfer(x) = tanh(pcurve * x) / tanh(pcurve);

drive(x) = x : *(pdrive) : +(fol(x)) : max(-3) : min(3) with {
    fol = an.amp_follower(0.04);
};

modfilter(x) = x <: _, tap(x) : *(1.0 - psat), *(psat) : + : fi.tf1(b0(x), b1(x), a1(x)) with {
    b0(x) = m(x);
    b1(x) = 1.0;
    a1(x) = m(x);
    m(x) = drive(x) : transfer : *(0.24);
    tap(x) = m(x);
};

//----------------------- Noise Section -----------------------
noise_amount = hslider("noise_amount", 0.0, 0, 1.0, 0.01);
pitch_shift = hslider("pitch_shift", 0.0, -12, 12, 1.0);

ampFollower = _ : an.amp_follower_ar(0.1, 0.4);
noise = no.pink_noise : fi.lowpass(2, 10000);
controlledNoise(input) = noise * (ampFollower(input) * noise_amount);

//----------------------- Ducking Section -----------------------
thresh = hslider("ducking", -40.0, -60.0, 0.0, 0.1);
att = hslider("attack", 50.0, 1.0, 1000.0, 1.0);
rel = hslider("release", 200.0, 1.0, 1000.0, 1.0);
knee = 0;
prePost = 1;

gainReduction = co.peak_compression_gain_mono_db(co.ratio2strength(10), 
    thresh, att * 1e-3, rel * 1e-3, knee, prePost);

duckedSignal(x) = (ba.db2linear(gainReduction(x)));

dwet = 0.9;
ddry = 1-dwet;
ducker(x, effect) = x*dry + (wet*duckedSignal(x) * (x : effect));


//----------------------- Delay Section -----------------------
dide(spread) = _  <:*(dry),(delx(B): *(wet) : *(ba.db2linear(output_gain)): co.limiter_1176_R4_mono):>_ with {
    B = 0;
    delx(n) = _<:(*(1.0-B): ef.transpose(4096, 512, pitch_shift) :digd ), !:>_;

    digd = (+:(delayed))~(fback(B)) with {
        // fback(n) = _<:select2(n, feed, freeze):>_ <:  _ * (1-vibrato_amount), vibrato_amount * vibrato(spread) :> _ ;
        // Update the fback function in digd to use different vibrato for each channel
        fback(n) = _<:select2(n, feed, freeze):>_ <: 
            _ * (1-vibrato_amount), 
            vibrato_amount * (stereo_select * vibrato_l + (1-stereo_select) * vibrato_r(spread)) :> _
        with {
            stereo_select = 1-spread; // 1 for left channel, 0 for right
        };
        // Glitch parameters
        glitch_rate = vslider("glitch_rate", 0.5, 0.01, 5, 0.01) : si.smooth(0.999);
        glitch = vslider("glitch_amount", 0.0, 0.0, 1, 0.01) : si.smooth(0.999);

        feed = *(feedback) + controlledNoise
            : color_effect  // Add color effects
            : fi.highpass(2,hifr1)
            : fi.lowpass(2,lofr1)
            : *(0.9);
        freeze = *(1.0);

        delayed = de.sdelay(N, interp, min(2^19, dtime + 40*spread + 
            (ma.fabs(os.lf_triangle(glitch_rate + spread*2))*glitch*(ma.SR + 1000*spread)))) with {
            dtime = hslider("delaytime", 500, 1, 5000, 0.1)*ma.SR/1000.0;
            dbpm = ba.tempo(hslider("bpm",120,24,360,1));
            interp = 50*ma.SR/1000.0;
            N = int(2^19);
        };

        // Delay parameters
        feedback = vslider("feedback", 50, 0, 99, 1)/100;
        hifr1 = hslider("highpass", 250, 20, 20000, 1) : si.smoo;
        lofr1 = hslider("lowpass", 10000, 20, 20000, 1) : si.smoo;
    };
};

//----------------------- Mix Section -----------------------
wet = vslider("mix", 50, 0, 100, 1) : /(100);
dry = 1 - wet;

//----------------------- Main Process -----------------------
process(x, y) = ducker(x, dide(0)), ducker(y, dide(spread_amount));