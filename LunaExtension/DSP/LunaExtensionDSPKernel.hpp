//
//  LunaExtensionDSPKernel.hpp
//  LunaExtension
//
//  Created by Thomas Power on 04/05/2025.
//

#pragma once

#include <AudioToolbox/AudioToolbox.h>
#include <algorithm>
#include <vector>
#include <span>

#include "../Parameters/LunaExtensionParameterAddresses.h"
#include "FaustWrapper.h"
#include "Macalla.h" // Include your Faust-generated DSP header

/*
 LunaExtensionDSPKernel
 As a non-ObjC class, this is safe to use from render thread.
 */
class LunaExtensionDSPKernel
{
  public:
    void initialize(int /*inputChannelCount*/,
                    int /*outputChannelCount*/,
                    double inSampleRate)
    {
        mSampleRate   = inSampleRate;
        mFaustWrapper = std::make_unique<FaustWrapper<Macalla>>();
        mFaustWrapper->initialize(inSampleRate);

        // Initialize Faust parameters from current stored parameter values
        if (mFaustWrapper)
        {
            mFaustWrapper->setParameter("spread_amount", mSpreadAmount);
            mFaustWrapper->setParameter("output_gain", mOutputGain);
            mFaustWrapper->setParameter("delaytime", mDelayMs);
            mFaustWrapper->setParameter("feedback", mFeedback);
            mFaustWrapper->setParameter("pitch_shift", mPitchShift);
            mFaustWrapper->setParameter("highpass", mHighpass);
            mFaustWrapper->setParameter("lowpass", mLowpass);
            mFaustWrapper->setParameter("diffusion", mDiffusionAmount);
            mFaustWrapper->setParameter("tape_wear", mTapeWearMacro);
            mFaustWrapper->setParameter("glitch", mGlitchMacro);
            mFaustWrapper->setParameter("ducking_amount", mDuckingMacro);
            mFaustWrapper->setParameter("att", mDuckAttack);
            mFaustWrapper->setParameter("rel", mDuckRelease);
            mFaustWrapper->setParameter("mix", mMix);
            mFaustWrapper->setParameter("bitcrush", mBitCrush);
        }
    }

    void deInitialize()
    {
        mFaustWrapper.reset(); // Clean up Faust instance
    }

    // MARK: - Bypass
    bool isBypassed() { return mBypassed; }

    void setBypass(bool shouldBypass) { mBypassed = shouldBypass; }

    // MARK: - Parameter Getter / Setter
    void setParameter(AUParameterAddress address, AUValue value)
    {

        switch (address)
        {
            case LunaExtensionParameterAddress::spread_amount:
                mSpreadAmount = value;
                updateFaustParameter("spread_amount", value);
                break;

            case LunaExtensionParameterAddress::output_gain:
                mOutputGain = value;
                updateFaustParameter("output_gain", value);
                break;

            case LunaExtensionParameterAddress::delay_ms:
                mDelayMs = value;
                updateFaustParameter("delaytime", value);
                break;

            case LunaExtensionParameterAddress::feedback:
                mFeedback = value;
                updateFaustParameter("feedback", value);
                break;

            case LunaExtensionParameterAddress::pitch_shift:
                mPitchShift = value;
                updateFaustParameter("pitch_shift", value);
                break;

            case LunaExtensionParameterAddress::highpass:
                mHighpass = value;
                updateFaustParameter("highpass", value);
                break;

            case LunaExtensionParameterAddress::lowpass:
                mLowpass = value;
                updateFaustParameter("lowpass", value);
                break;

            case LunaExtensionParameterAddress::diffusion_amount:
                mDiffusionAmount = value;
                updateFaustParameter("diffusion", value);
                break;

            case LunaExtensionParameterAddress::tape_wear_macro:
                mTapeWearMacro = value;
                updateFaustParameter("tape_wear", value);
                break;

            case LunaExtensionParameterAddress::glitch_macro:
                mGlitchMacro = value;
                updateFaustParameter("glitch", value);
                break;

            case LunaExtensionParameterAddress::ducking_macro:
                mDuckingMacro = value;
                updateFaustParameter("ducking_amount", value);
                break;

            case LunaExtensionParameterAddress::duck_attack:
                mDuckAttack = value;
                updateFaustParameter("att", value);
                break;

            case LunaExtensionParameterAddress::duck_release:
                mDuckRelease = value;
                updateFaustParameter("rel", value);
                break;

            case LunaExtensionParameterAddress::mix:
                mMix = value;
                updateFaustParameter("mix", value);
                break;

            // --- Sync Parameters ---
            case LunaExtensionParameterAddress::delayTimeSync:
                mDelayTimeSync = static_cast<int>(value);
                updateDelayTime(); // Update Faust delay when sync setting
                                   // changes
                break;
            case LunaExtensionParameterAddress::syncEnabled:
                mSyncEnabled = static_cast<bool>(value);
                updateDelayTime(); // Update Faust delay when sync
                                   // enabled/disabled
                break;

            case LunaExtensionParameterAddress::bitCrush:
                mBitCrush = value;
                updateFaustParameter("bitcrush", value);
                break;

                // Note: Removed cases for hidden parameters
        }
    }

    AUValue getParameter(AUParameterAddress address)
    {
        switch (address)
        {
            case LunaExtensionParameterAddress::spread_amount:
                return (AUValue)mSpreadAmount;
            case LunaExtensionParameterAddress::output_gain:
                return (AUValue)mOutputGain;
            case LunaExtensionParameterAddress::delay_ms:
                return (AUValue)mDelayMs;
            case LunaExtensionParameterAddress::feedback:
                return (AUValue)mFeedback;
            case LunaExtensionParameterAddress::pitch_shift:
                return (AUValue)mPitchShift;
            case LunaExtensionParameterAddress::highpass:
                return (AUValue)mHighpass;
            case LunaExtensionParameterAddress::lowpass:
                return (AUValue)mLowpass;
            case LunaExtensionParameterAddress::diffusion_amount:
                return (AUValue)mDiffusionAmount;
            case LunaExtensionParameterAddress::tape_wear_macro:
                return (AUValue)mTapeWearMacro;
            case LunaExtensionParameterAddress::glitch_macro:
                return (AUValue)mGlitchMacro;
            case LunaExtensionParameterAddress::ducking_macro:
                return (AUValue)mDuckingMacro;
            case LunaExtensionParameterAddress::duck_attack:
                return (AUValue)mDuckAttack;
            case LunaExtensionParameterAddress::duck_release:
                return (AUValue)mDuckRelease;
            case LunaExtensionParameterAddress::mix:
                return (AUValue)mMix;
            case LunaExtensionParameterAddress::bitCrush:
                return (AUValue)mBitCrush;

            // --- Sync Parameters ---
            case LunaExtensionParameterAddress::delayTimeSync:
                return (AUValue)mDelayTimeSync;
            case LunaExtensionParameterAddress::syncEnabled:
                return (AUValue)mSyncEnabled;

            // Note: Removed cases for hidden parameters
            default:
                assert(false);
                return 0.f;
        }
    }

    // MARK: - Max Frames
    AUAudioFrameCount maximumFramesToRender() const
    {
        return mMaxFramesToRender;
    }

    void setMaximumFramesToRender(const AUAudioFrameCount& maxFrames)
    {
        mMaxFramesToRender = maxFrames;
    }

    // MARK: - Musical Context
    void setMusicalContextBlock(AUHostMusicalContextBlock contextBlock)
    {
        mMusicalContextBlock = contextBlock;
    }

    /**
     MARK: - Internal Process

     This function does the core siginal processing.
     Do your custom DSP here.
     */
    void process(std::span<float const*> inputBuffers,
                 std::span<float*> outputBuffers,
                 AUEventSampleTime bufferStartTime,
                 AUAudioFrameCount frameCount)
    {
        // Get Musical Context (Tempo)
        double hostTempo = mCurrentTempo;
        if (mMusicalContextBlock)
        {
            double tempo = 0.0;
            if (mMusicalContextBlock(
                  &tempo, nullptr, nullptr, nullptr, nullptr, nullptr))
            {
                hostTempo = tempo;
            }
        }

        // Check if tempo changed and update delay if sync enabled
        if (hostTempo != mCurrentTempo)
        {
            mCurrentTempo = hostTempo;
            if (mSyncEnabled)
            {
                updateDelayTime();
            }
        }

        assert(inputBuffers.data() != nullptr &&
               outputBuffers.data() != nullptr); // Basic check
        const int channelCount = 2; // Assuming stereo, adjust if dynamic

        if (mBypassed || !mFaustWrapper)
        {
            // Pass the samples through if bypassed or Faust not ready
            for (UInt32 channel = 0; channel < channelCount; ++channel)
            {
                if (inputBuffers[channel] != outputBuffers[channel])
                {
                    std::copy_n(inputBuffers[channel],
                                frameCount,
                                outputBuffers[channel]);
                }
            }
            return;
        }

        // Process with Faust DSP
        mFaustWrapper->process(
          inputBuffers, outputBuffers, static_cast<int>(frameCount));
    }

    void handleOneEvent(AUEventSampleTime now, AURenderEvent const* event)
    {
        switch (event->head.eventType)
        {
            case AURenderEventParameter:
            {
                handleParameterEvent(now, event->parameter);
                break;
            }

            default:
                break;
        }
    }

    void handleParameterEvent(AUEventSampleTime /*now*/,
                              AUParameterEvent const& parameterEvent)
    {
        setParameter(parameterEvent.parameterAddress, parameterEvent.value);
    }

    void updateFaustParameter(std::string_view name, double value)
    {
        if (mFaustWrapper)
        {
            mFaustWrapper->setParameter(name.data(), static_cast<float>(value));
        }
    }

    // --- Helper to Calculate and Set Delay Time ---
    void updateDelayTime()
    {
        if (!mFaustWrapper)
            return;

        double delayMsToSend = mDelayMs; // Default to manual delay time

        if (mSyncEnabled && mCurrentTempo > 0)
        {
            // Sync is enabled, calculate based on tempo and sync division
            double beatsPerSecond        = mCurrentTempo / 60.0;
            double quarterNoteDurationMs = (1.0 / beatsPerSecond) * 1000.0;

            // Map mDelayTimeSync index (0-17) to note duration multipliers
            // Matches valueStrings in Parameters.swift
            double multipliers[] = {
                1.0 / 24.0, // 1/64T (1/16 * 2/3)
                1.0 / 16.0, // 1/64
                1.0 / 16.0, // 1/32T (1/8 * 2/3)
                1.0 / 8.0,  // 1/32
                1.0 / 8.0,  // 1/16T (1/4 * 2/3)
                1.0 / 4.0,  // 1/16
                1.0 / 4.0,  // 1/8T (1/2 * 2/3)
                1.0 / 2.0,  // 1/8
                1.0 / 2.0,  // 1/4T (1 * 2/3)
                1.0,        // 1/4
                1.0 * 1.5,  // 1/2T (2 * 2/3)
                2.0,        // 1/2
                2.0 * 1.5,  // 1/1T (4 * 2/3)
                4.0,        // 1/1
                4.0 * 1.5,  // 2/1T (8 * 2/3)
                8.0,        // 2/1
                8.0 * 1.5,  // 4/1T (16 * 2/3)
                16.0        // 4/1
            };

            if (mDelayTimeSync >= 0 && mDelayTimeSync < std::size(multipliers))
            {
                delayMsToSend =
                  quarterNoteDurationMs * multipliers[mDelayTimeSync];
            }
        }

        // Ensure delay is within Faust's expected range (e.g., 1-1000ms)
        // Clamp the value before sending it.
        // TODO: Read the actual range from Faust declaration if possible or
        // define constants.
        delayMsToSend = std::max(1.0, std::min(delayMsToSend, 2000.0));

        updateFaustParameter("delaytime", delayMsToSend);
    }

    // MARK: Member Variables
    AUHostMusicalContextBlock mMusicalContextBlock = nullptr;

    std::unique_ptr<FaustWrapper<Macalla>> mFaustWrapper;

    double mSampleRate                   = 44100.0;
    bool mBypassed                       = false;
    AUAudioFrameCount mMaxFramesToRender = 1024;

    // --- Visible Parameters ---
    // Keep only the parameters defined in the updated
    // LunaExtensionParameterAddresses.h
    double mSpreadAmount    = 0.0;
    double mOutputGain      = 0.0;
    double mDelayMs         = 300.0;
    double mFeedback        = 0.5;
    double mPitchShift      = 0.0;
    double mHighpass        = 250.0;
    double mLowpass         = 10000.0;
    double mDiffusionAmount = 0.0;
    double mTapeWearMacro   = 0.1;
    double mGlitchMacro     = 0.0;
    double mDuckingMacro    = 0.0;
    double mDuckAttack      = 10.0;
    double mDuckRelease     = 200.0;
    double mMix             = 50.0;
    double mBitCrush = 0.0;

    // --- Sync Parameters ---
    int mDelayTimeSync   = 4;     // Default matches Faust
    bool mSyncEnabled    = false; // Default off
    double mCurrentTempo = 120.0; // Store current tempo

    // Note: Removed member variables for hidden parameters
};
