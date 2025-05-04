
#pragma once

#include <memory>
#include <span>
#include <vector>

// Include Faust-generated header which contains all needed definitions
#include "Macalla.h"

// Define FAUSTFLOAT if not already defined
#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/**
 * FaustWrapper - A generic wrapper for Faust-generated DSP classes
 * Handles both mono and stereo inputs, always producing stereo output
 * Designed for real-time audio thread usage
 *
 * Template parameter DSPType should be a Faust-generated DSP class (e.g.,
 * Macalla)
 */
template<typename DSPType>
class FaustWrapper
{
  public:
    FaustWrapper() = default;
    ~FaustWrapper()
    {
        // Clean up resources
        fDSP.reset();
        fUI.reset();
    }

    /**
     * Initialize the Faust DSP with the given sample rate
     * Must be called before processing audio
     */
    void initialize(double sampleRate)
    {
        fSampleRate = sampleRate;

        // Create the specific Faust DSP instance
        fDSP = std::make_unique<DSPType>();
        fUI  = std::make_unique<MapUI>();

        // Set up UI controls
        fDSP->buildUserInterface(fUI.get());

        // Initialize DSP with sample rate
        fDSP->init(static_cast<int>(fSampleRate));

        // Allocate buffers - make sure this is large enough for max frame count
        monoToStereoBuffer.resize(8192, 0.0f);

        // Preallocate fixed-size vectors for input/output pointers
        fInputs.resize(2, nullptr);
        fOutputs.resize(2, nullptr);

        isInitialized = true;
    }

    /**
     * Process audio through the Faust DSP
     * Handles conversion between std::span and FAUSTFLOAT**
     */
    void process(std::span<float const*> inputBuffers,
                 std::span<float*> outputBuffers,
                 int frameCount)
    {
        if (!isInitialized || !fDSP || !fUI)
        {
            return;
        }

        // Ensure frameCount doesn't exceed our buffer size
        if (frameCount > static_cast<int>(monoToStereoBuffer.size()))
        {
            frameCount = static_cast<int>(monoToStereoBuffer.size());
        }

        // Handle mono to stereo conversion if needed
        if (inputBuffers.size() == 1 && outputBuffers.size() >= 2)
        {
            // Mono input case - duplicate to both channels
            fInputs[0] = const_cast<float*>(inputBuffers[0]);
            fInputs[1] = const_cast<float*>(inputBuffers[0]);
        }
        else if (inputBuffers.size() >= 2)
        {
            // Stereo input case - use both channels
            fInputs[0] = const_cast<float*>(inputBuffers[0]);
            fInputs[1] = const_cast<float*>(inputBuffers[1]);
        }
        else
        {
            // Zero input case (shouldn't happen but handle it safely)
            fInputs[0] = monoToStereoBuffer.data();
            fInputs[1] = monoToStereoBuffer.data();
        }

        // Set output pointers
        if (outputBuffers.size() >= 2)
        {
            fOutputs[0] = outputBuffers[0];
            fOutputs[1] = outputBuffers[1];
        }
        else if (outputBuffers.size() == 1)
        {
            // If only one output channel, use it for the left channel
            // and discard the right channel
            fOutputs[0] = outputBuffers[0];
            fOutputs[1] = monoToStereoBuffer.data();
        }
        else
        {
            // No output case (shouldn't happen)
            return;
        }

        // Call the Faust DSP compute method
        fDSP->compute(frameCount, fInputs.data(), fOutputs.data());
    }

    /**
     * Set a parameter value in the Faust DSP
     */
    void setParameter(const char* path, float value)
    {
        if (fUI)
        {
            fUI->setParamValue(path, value);
        }
    }

    /**
     * Get a parameter value from the Faust DSP
     */
    float getParameter(const char* path)
    {
        if (fUI)
        {
            return fUI->getParamValue(path);
        }
        return 0.0f;
    }

  private:
    std::unique_ptr<DSPType> fDSP;
    std::unique_ptr<MapUI> fUI;

    // Preallocated buffers for mono->stereo conversion
    std::vector<float> monoToStereoBuffer;

    // Preallocated pointers for FAUST compute method
    std::vector<FAUSTFLOAT*> fInputs;
    std::vector<FAUSTFLOAT*> fOutputs;

    double fSampleRate = 44100.0;
    bool isInitialized = false;
};
