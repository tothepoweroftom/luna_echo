import("stdfaust.lib");

declare name "Noise Type Selector";
declare category "Generator";

//================================ Noise Definitions for Space Echo ================================

// 1. Classic Tape Hiss
// A mix of pink and brown noise, band-limited to sound like tape.
classic_tape_hiss = (no.pink_noise * 0.7 + no.brown_noise * 0.3)
                    : fi.highpass(1, 100) : fi.lowpass(1, 12000) : *(0.4);

// 2. Motor Rumble
// Low-frequency sine wave with irregular amplitude to simulate a physical motor.
motor_lfo = os.lf_triangle(1.5) * 0.5 + 0.5;
motor_rumble = (os.osc(7) * motor_lfo * 0.3 + no.noise * 0.05)
               : fi.lowpass(1, 80) : *(0.6);

// 3. Saturated Crackle
// Sparse pops from a worn tape, passed through a resonant filter and a saturator.
saturated_crackle = no.sparse_noise(12) * 0.8
                    : fi.resonlp(2500, 3, 1) : ma.tanh : *(0.7);

// 4. Alien Interference
// Resonant noise with a slow frequency sweep for a sci-fi effect.
sweep_lfo = os.lf_triangle(0.1);
filter_freq = 3000 + sweep_lfo * 2500;
alien_interference = no.noise : fi.resonhp(filter_freq, 40, 1) : *(0.3);


//================================= Noise Selection =================================

// A function to select one of the noise generators based on an index.
noise_generator(type) = ba.selectn(4, int(type),
                                   classic_tape_hiss,
                                   motor_rumble,
                                   saturated_crackle,
                                   alien_interference);


//======================================= UI =======================================

// A dropdown menu to select the noise type.
noise_type = nentry("Noise Type[style:menu{'Tape Hiss':0;'Motor Rumble':1;'Saturated Crackle':2;'Alien Interference':3}]", 0, 0, 3, 1);


//===================================== Process ====================================

// The main process function. It generates the selected noise.
process = noise_generator(noise_type); 