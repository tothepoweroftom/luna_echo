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

#include "LunaExtensionParameterAddresses.h"
#include "FaustWrapper.h"
#include "Macalla.h" // Include your Faust-generated DSP header

/*
 LunaExtensionDSPKernel
 As a non-ObjC class, this is safe to use from render thread.
 */
class LunaExtensionDSPKernel
{
  public:
    void initialize(int inputChannelCount,
                    int outputChannelCount,
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
            mFaustWrapper->setParameter("delay(ms)", mDelayMs);
            mFaustWrapper->setParameter("feedback", mFeedback);
            mFaustWrapper->setParameter("noise_amount", mNoiseAmount);
            mFaustWrapper->setParameter("pitch_shift", mPitchShift);
            mFaustWrapper->setParameter("highpass", mHighpass);
            mFaustWrapper->setParameter("lowpass", mLowpass);
            mFaustWrapper->setParameter("random_mod", mRandomMod);
            mFaustWrapper->setParameter("wow", mWow);
            mFaustWrapper->setParameter("wow_intensity", mWowIntensity);
            mFaustWrapper->setParameter("flutter", mFlutter);
            mFaustWrapper->setParameter("flutter_intensity", mFlutterIntensity);
            mFaustWrapper->setParameter("ducking", mDucking);
            mFaustWrapper->setParameter("attack", mAttack);
            mFaustWrapper->setParameter("release", mRelease);
            mFaustWrapper->setParameter("ratio", mRatio);
            mFaustWrapper->setParameter("glitch_rate", mGlitchRate);
            mFaustWrapper->setParameter("glitch_amount", mGlitchAmount);
            mFaustWrapper->setParameter("mix", mMix);
        }
    }

    void deInitialize() {}

    // MARK: - Bypass
    bool isBypassed() { return mBypassed; }

    void setBypass(bool shouldBypass) { mBypassed = shouldBypass; }

    // MARK: - Parameter Getter / Setter
    void setParameter(AUParameterAddress address, AUValue value)
    {
        if (!mFaustWrapper)
            return;

        switch (address)
        {
            // Legacy parameter
            case LunaExtensionParameterAddress::gain:
                mGain = value;
                break;

            // Macalla parameters
            case LunaExtensionParameterAddress::spread_amount:
                mSpreadAmount = value;
                mFaustWrapper->setParameter("spread_amount", value);
                break;

            case LunaExtensionParameterAddress::output_gain:
                mOutputGain = value;
                mFaustWrapper->setParameter("output_gain", value);
                break;

            case LunaExtensionParameterAddress::delay_ms:
                mDelayMs = value;
                mFaustWrapper->setParameter("delay(ms)", value);
                break;

            case LunaExtensionParameterAddress::feedback:
                mFeedback = value;
                mFaustWrapper->setParameter("feedback", value);
                break;

            case LunaExtensionParameterAddress::noise_amount:
                mNoiseAmount = value;
                mFaustWrapper->setParameter("noise_amount", value);
                break;

            case LunaExtensionParameterAddress::pitch_shift:
                mPitchShift = value;
                mFaustWrapper->setParameter("pitch_shift", value);
                break;

            case LunaExtensionParameterAddress::highpass:
                mHighpass = value;
                mFaustWrapper->setParameter("highpass", value);
                break;

            case LunaExtensionParameterAddress::lowpass:
                mLowpass = value;
                mFaustWrapper->setParameter("lowpass", value);
                break;

            case LunaExtensionParameterAddress::random_mod:
                mRandomMod = value;
                mFaustWrapper->setParameter("random_mod", value);
                break;

            case LunaExtensionParameterAddress::wow:
                mWow = value;
                mFaustWrapper->setParameter("wow", value);
                break;

            case LunaExtensionParameterAddress::wow_intensity:
                mWowIntensity = value;
                mFaustWrapper->setParameter("wow_intensity", value);
                break;

            case LunaExtensionParameterAddress::flutter:
                mFlutter = value;
                mFaustWrapper->setParameter("flutter", value);
                break;

            case LunaExtensionParameterAddress::flutter_intensity:
                mFlutterIntensity = value;
                mFaustWrapper->setParameter("flutter_intensity", value);
                break;

            case LunaExtensionParameterAddress::ducking:
                mDucking = value;
                mFaustWrapper->setParameter("ducking", value);
                break;

            case LunaExtensionParameterAddress::attack:
                mAttack = value;
                mFaustWrapper->setParameter("attack", value);
                break;

            case LunaExtensionParameterAddress::release:
                mRelease = value;
                mFaustWrapper->setParameter("release", value);
                break;

            case LunaExtensionParameterAddress::ratio:
                mRatio = value;
                mFaustWrapper->setParameter("ratio", value);
                break;

            case LunaExtensionParameterAddress::glitch_rate:
                mGlitchRate = value;
                mFaustWrapper->setParameter("glitch_rate", value);
                break;

            case LunaExtensionParameterAddress::glitch_amount:
                mGlitchAmount = value;
                mFaustWrapper->setParameter("glitch_amount", value);
                break;

            case LunaExtensionParameterAddress::mix:
                mMix = value;
                mFaustWrapper->setParameter("mix", value);
                break;
        }
    }

    AUValue getParameter(AUParameterAddress address)
    {
        switch (address)
        {
            // Legacy parameter
            case LunaExtensionParameterAddress::gain:
                return (AUValue)mGain;

            // Macalla parameters
            case LunaExtensionParameterAddress::spread_amount:
                return (AUValue)mSpreadAmount;

            case LunaExtensionParameterAddress::output_gain:
                return (AUValue)mOutputGain;

            case LunaExtensionParameterAddress::delay_ms:
                return (AUValue)mDelayMs;

            case LunaExtensionParameterAddress::feedback:
                return (AUValue)mFeedback;

            case LunaExtensionParameterAddress::noise_amount:
                return (AUValue)mNoiseAmount;

            case LunaExtensionParameterAddress::pitch_shift:
                return (AUValue)mPitchShift;

            case LunaExtensionParameterAddress::highpass:
                return (AUValue)mHighpass;

            case LunaExtensionParameterAddress::lowpass:
                return (AUValue)mLowpass;

            case LunaExtensionParameterAddress::random_mod:
                return (AUValue)mRandomMod;

            case LunaExtensionParameterAddress::wow:
                return (AUValue)mWow;

            case LunaExtensionParameterAddress::wow_intensity:
                return (AUValue)mWowIntensity;

            case LunaExtensionParameterAddress::flutter:
                return (AUValue)mFlutter;

            case LunaExtensionParameterAddress::flutter_intensity:
                return (AUValue)mFlutterIntensity;

            case LunaExtensionParameterAddress::ducking:
                return (AUValue)mDucking;

            case LunaExtensionParameterAddress::attack:
                return (AUValue)mAttack;

            case LunaExtensionParameterAddress::release:
                return (AUValue)mRelease;

            case LunaExtensionParameterAddress::ratio:
                return (AUValue)mRatio;

            case LunaExtensionParameterAddress::glitch_rate:
                return (AUValue)mGlitchRate;

            case LunaExtensionParameterAddress::glitch_amount:
                return (AUValue)mGlitchAmount;

            case LunaExtensionParameterAddress::mix:
                return (AUValue)mMix;

            default:
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
        /*
         Note: For an Audio Unit with 'n' input channels to 'n' output channels,
         remove the assert below and modify the check in [LunaExtensionAudioUnit
         allocateRenderResourcesAndReturnError]
         */
        assert(inputBuffers.size() == outputBuffers.size());

        if (mBypassed)
        {
            // Pass the samples through
            for (UInt32 channel = 0; channel < inputBuffers.size(); ++channel)
            {
                std::copy_n(
                  inputBuffers[channel], frameCount, outputBuffers[channel]);
            }
            return;
        }

        // Process with Faust DSP
        if (mFaustWrapper)
        {
            mFaustWrapper->process(inputBuffers, outputBuffers, frameCount);
        }
        else
        {
            // Fallback if Faust DSP is not initialized
            for (UInt32 channel = 0; channel < inputBuffers.size(); ++channel)
            {
                for (UInt32 frameIndex = 0; frameIndex < frameCount;
                     ++frameIndex)
                {
                    // Do your sample by sample dsp here...
                    outputBuffers[channel][frameIndex] =
                      inputBuffers[channel][frameIndex] * mGain;
                }
            }
        }
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

    void handleParameterEvent(AUEventSampleTime now,
                              AUParameterEvent const& parameterEvent)
    {
        setParameter(parameterEvent.parameterAddress, parameterEvent.value);
    }

    // MARK: Member Variables
    AUHostMusicalContextBlock mMusicalContextBlock;

    std::unique_ptr<FaustWrapper<Macalla>> mFaustWrapper;

    double mSampleRate                   = 44100.0;
    double mGain                         = 1.0;
    bool mBypassed                       = false;
    AUAudioFrameCount mMaxFramesToRender = 1024;

    // Macalla parameters
    double mSpreadAmount     = 0.0;
    double mOutputGain       = 0.0;
    double mDelayMs          = 300.0;
    double mFeedback         = 0.5;
    double mNoiseAmount      = 0.0;
    double mPitchShift       = 0.0;
    double mHighpass         = 250.0;
    double mLowpass          = 10000.0;
    double mRandomMod        = 0.002;
    double mWow              = 0.5;
    double mWowIntensity     = 0.3;
    double mFlutter          = 8.0;
    double mFlutterIntensity = 0.1;
    double mDucking          = -30.0;
    double mAttack           = 10.0;
    double mRelease          = 200.0;
    double mRatio            = 4.0;
    double mGlitchRate       = 0.5;
    double mGlitchAmount     = 0.0;
    double mMix              = 50.0;
};
