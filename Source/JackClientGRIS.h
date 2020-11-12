/*
 This file is part of SpatGRIS2.

 Developers: Samuel B�land, Olivier B�langer, Nicolas Masson

 SpatGRIS2 is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 SpatGRIS2 is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with SpatGRIS2.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "macros.h"

DISABLE_WARNINGS
#if defined(WIN32) || defined(_WIN64)
    #include <cstdint>
    #include <mutex>
#else
    #include <unistd.h>
#endif

#ifdef __linux__
    #include <mutex>
#endif

#include <JuceHeader.h>

#if USE_JACK
    #include <jack/jack.h>
    #include <jack/session.h>
    #include <jack/transport.h>
    #include <jack/types.h>
#else
    #include "JackMockup.h"
#endif

#include "spat/lbap.h"
#include "spat/vbap.h"
ENABLE_WARNINGS

#include "AudioRecorder.h"

class Speaker;

// Limits of SpatGRIS2 In/Out.
static unsigned int const MAX_INPUTS = 256;
static unsigned int const MAX_OUTPUTS = 256;

//==============================================================================
struct LbapData {
    lbap_pos pos;
    std::array<float, MAX_OUTPUTS> gains;
    std::array<float, MAX_OUTPUTS> y;
};

//==============================================================================
struct Client {
    juce::String name;
    unsigned int portStart = 0;
    unsigned int portEnd = 0;
    unsigned int portAvailable = 0;
    unsigned int activePorts = 0;
    bool initialized = false;
    bool connected = false;
};

//==============================================================================
struct SourceIn {
    unsigned int id;
    float x{};
    float y{};
    float z{};

    float radAzimuth{};
    float radElevation{};
    float azimuth{};
    float zenith{};
    float radius{ 1.0f };
    float azimuthSpan{};
    float zenithSpan{};

    std::array<float, MAX_OUTPUTS> lbapGains{};
    std::array<float, MAX_OUTPUTS> lbapY{};
    lbap_pos lbapLastPos{ -1, -1, -1, 0.0f, 0.0f, 0.0f };

    bool isMuted = false;
    bool isSolo = false;
    float gain{}; // Not used yet.

    int directOut{};

    VBAP_DATA * paramVBap{};
};

//==============================================================================
struct SpeakerOut {
    unsigned int id;
    float x{};
    float y{};
    float z{};

    float azimuth{};
    float zenith{};
    float radius{};

    float gain{ 1.0f };

    bool hpActive = false;
    double b1{};
    double b2{};
    double b3{};
    double b4{};
    double ha0{};
    double ha1{};
    double ha2{};

    bool isMuted = false;
    bool isSolo = false;

    int outputPatch{};

    bool directOut = false;
};

//==============================================================================
// Spatialization modes.
typedef enum { VBAP = 0, LBAP, VBAP_HRTF, STEREO } ModeSpatEnum;

//==============================================================================
class JackClientGris
{
    // class variables.
    //-----------------
    unsigned int mSampleRate{};
    unsigned int mBufferSize{};
    unsigned int mNumberInputs{};
    unsigned int mNumberOutputs{};
    unsigned int mMaxOutputPatch{};

    std::vector<int> mOutputPatches{};

    // Jack variables.
    jack_client_t * mClient{};

    std::vector<jack_port_t *> mInputsPort{};
    std::vector<jack_port_t *> mOutputsPort{};

    // Interpolation and master gain values.
    float mInterMaster{ 0.8f };
    float mMasterGainOut{ 1.0f };

    // Global solo states.
    bool mSoloIn{ false };
    bool mSoloOut{ false };

    // Pink noise test sound.
    float mPinkNoiseC0{};
    float mPinkNoiseC1{};
    float mPinkNoiseC2{};
    float mPinkNoiseC3{};
    float mPinkNoiseC4{};
    float mPinkNoiseC5{};
    float mPinkNoiseC6{};
    float mPinkNoiseGain{ 0.1f };
    bool mPinkNoiseActive{ false };

    // Crossover highpass filter.
    std::array<double, MAX_OUTPUTS> mCrossoverHighpassX1{};
    std::array<double, MAX_OUTPUTS> mCrossoverHighpassX2{};
    std::array<double, MAX_OUTPUTS> mCrossoverHighpassX3{};
    std::array<double, MAX_OUTPUTS> mCrossoverHighpassX4{};
    std::array<double, MAX_OUTPUTS> mCrossoverHighpassY1{};
    std::array<double, MAX_OUTPUTS> mCrossoverHighpassY2{};
    std::array<double, MAX_OUTPUTS> mCrossoverHighpassY3{};
    std::array<double, MAX_OUTPUTS> mCrossoverHighpassY4{};

    // Mute / Solo / VuMeter.
    std::array<float, MAX_INPUTS> mLevelsIn{};
    std::array<float, MAX_OUTPUTS> mLevelsOut{};

    // Client list.
    std::vector<Client> mClients{};
    std::mutex mClientsLock{};

    // Source and output lists.
    std::array<SourceIn, MAX_INPUTS> mSourcesIn{};
    std::array<SpeakerOut, MAX_OUTPUTS> mSpeakersOut{};

    // Enable/disable jack process callback.
    bool mProcessBlockOn{ true };

    // True when jack reports an xrun.
    bool mIsOverloaded{ false };

    // Which spatialization mode is selected.
    ModeSpatEnum mModeSelected{ VBAP };

    bool mAutoConnection{ false }; // not sure this one is necessary ?

    // VBAP data.
    unsigned mVbapDimensions{};
    std::array<int, MAX_INPUTS> mVbapSourcesToUpdate{};

    std::vector<std::vector<int>> mVbapTriplets{};

    // BINAURAL data.
    std::array<unsigned, 16> mHrtfCount{};
    std::array<std::array<float, 128>, 16> mHrtfInputTmp{};
    std::array<std::array<float, 128>, 16> mVbapHrtfLeftImpulses{};
    std::array<std::array<float, 128>, 16> mVbapHrtfRightImpulses{};

    // STEREO data.
    std::array<float, MAX_INPUTS> mLastAzimuth{};

    // LBAP data.
    lbap_field * mLbapSpeakerField{};

    // Recording parameters.
    unsigned int mIndexRecord{};
    bool mIsRecording{ false };

    std::array<AudioRecorder, MAX_OUTPUTS> mRecorders{};
    juce::Array<juce::File> mOutputFileNames{};

    // LBAP distance attenuation values.
    float mAttenuationLinearGain{ 0.01584893f };       // -36 dB;
    float mAttenuationLowpassCoefficient{ 0.867208f }; // 1000 Hz
    std::array<float, MAX_INPUTS> mLastAttenuationGain{};
    std::array<float, MAX_INPUTS> mLastAttenuationCoefficient{};
    std::array<float, MAX_INPUTS> mAttenuationLowpassY{};
    std::array<float, MAX_INPUTS> mAttenuationLowpassZ{};
    //==============================================================================
    // Tells if an error occured while setting up the client.
    bool mClientReady{ false };

    // Private recording parameters.
    int mRecordFormat{};     // 0 = WAV, 1 = AIFF
    int mRecordFileConfig{}; // 0 = Multiple Mono Files, 1 = Single Interleaved

    juce::String mRecordPath{};

    // This structure is used to compute the VBAP algorithm only once. Each source only gets a copy.
    VBAP_DATA * mParamVBap{};

public:
    //==============================================================================
    // Class methods.
    JackClientGris();
    ~JackClientGris();

    JackClientGris(JackClientGris const &) = delete;
    JackClientGris(JackClientGris &&) = delete;

    JackClientGris & operator=(JackClientGris const &) = delete;
    JackClientGris & operator=(JackClientGris &&) = delete;
    //==============================================================================
    // Audio Status.
    bool isReady() const { return mClientReady; }
    float getCpuUsed() const { return jack_cpu_load(mClient); }
    float getLevelsIn(int const index) const { return mLevelsIn[index]; }
    float getLevelsOut(int const index) const { return mLevelsOut[index]; }

    // Manage Inputs / Outputs.
    void addRemoveInput(unsigned int number);
    void clearOutput();
    bool addOutput(unsigned int outputPatch);
    void removeOutput(int number);

    std::vector<int> getDirectOutOutputPatches() const;

    // Manage clients.
    void connectionClient(juce::String name, bool connect = true);
    void updateClientPortAvailable(bool fromJack);

    std::string getClientName(char const * port) const;

    // Recording.
    void prepareToRecord();
    void startRecord();
    void stopRecord() { this->mIsRecording = false; }
    void setRecordFormat(const int format) { this->mRecordFormat = format; }
    int getRecordFormat() const { return this->mRecordFormat; }
    void setRecordFileConfig(const int config) { this->mRecordFileConfig = config; }
    int getRecordFileConfig() const { return this->mRecordFileConfig; }
    void setRecordingPath(juce::String const & filePath) { this->mRecordPath = filePath; }
    bool isSavingRun() const { return this->mIsRecording; }

    juce::String const & getRecordingPath() const { return this->mRecordPath; }

    // Initialize VBAP algorithm.
    bool initSpeakersTriplet(std::vector<Speaker *> const & listSpk, int dimensions, bool needToComputeVbap);

    // Initialize LBAP algorithm.
    bool lbapSetupSpeakerField(std::vector<Speaker *> const & listSpk);

    // LBAP distance attenuation functions.
    void setAttenuationDb(float value);
    void setAttenuationHz(float value);

    // Need to update a source VBAP data.
    void updateSourceVbap(int idS);

    // Reinit HRTF delay lines.
    void resetHrtf();

    jack_client_t * getClient() { return mClient; }
    void clientRegistrationCallback(char const * name, int regist);
    void portConnectCallback(jack_port_id_t a, jack_port_id_t b, int connect) const;

    unsigned getSampleRate() const { return mSampleRate; }
    unsigned getBufferSize() const { return mBufferSize; }
    unsigned getNumberOutputs() const { return mNumberOutputs; }
    unsigned getNumberInputs() const { return mNumberInputs; }
    ModeSpatEnum getMode() const { return mModeSelected; }
    unsigned getVbapDimensions() const { return mVbapDimensions; }
    auto const & getClients() const { return mClients; }
    auto & getClients() { return mClients; }
    auto & getSourcesIn() { return mSourcesIn; }
    auto const & getSourcesIn() const { return mSourcesIn; }
    auto & getVbapSourcesToUpdate() { return mVbapSourcesToUpdate; }
    auto const & getVbapTriplets() const { return mVbapTriplets; }
    bool getSoloIn() const { return mSoloIn; }
    bool getSoloOut() const { return mSoloOut; }
    auto const & getSpeakersOut() const { return mSpeakersOut; }
    auto & getSpeakersOut() { return mSpeakersOut; }
    unsigned getMaxOutputPatch() const { return mMaxOutputPatch; }
    unsigned getIndexRecord() const { return mIndexRecord; }
    auto const & getRecorders() const { return mRecorders; }
    auto const & getOutputFileNames() const { return mOutputFileNames; }
    auto const & getInputPorts() const { return mInputsPort; }
    auto & getClientsLock() { return mClientsLock; }

    bool isRecording() const { return mIsRecording; }
    bool isOverloaded() const { return mIsOverloaded; }

    void setProcessBlockOn(bool const state) { mProcessBlockOn = state; }
    void setMaxOutputPatch(unsigned const maxOutputPatch) { mMaxOutputPatch = maxOutputPatch; }
    void setVbapDimensions(unsigned const dimensions) { mVbapDimensions = dimensions; }
    void setSoloIn(bool const state) { mSoloIn = state; }
    void setSoloOut(bool const state) { mSoloOut = state; }
    void setMode(ModeSpatEnum const mode) { mModeSelected = mode; }
    void setMasterGainOut(float const gain) { mMasterGainOut = gain; }
    void setInterMaster(float const interMaster) { mInterMaster = interMaster; }

    //==============================================================================
    // Pink noise
    void setPinkNoiseGain(float const gain) { mPinkNoiseGain = gain; }
    void setPinkNoiseActive(bool const state) { mPinkNoiseActive = state; }

    //==============================================================================
    // Audio processing
    void muteSoloVuMeterIn(jack_default_audio_sample_t ** ins, jack_nframes_t nframes, unsigned sizeInputs);
    void muteSoloVuMeterGainOut(jack_default_audio_sample_t ** outs,
                                jack_nframes_t nframes,
                                unsigned sizeOutputs,
                                float gain = 1.0f);
    void addNoiseSound(jack_default_audio_sample_t ** outs, jack_nframes_t nframes, unsigned sizeOutputs);
    void processVbap(jack_default_audio_sample_t ** ins,
                     jack_default_audio_sample_t ** outs,
                     jack_nframes_t nframes,
                     unsigned sizeInputs,
                     unsigned sizeOutputs);
    void processLbap(jack_default_audio_sample_t ** ins,
                     jack_default_audio_sample_t ** outs,
                     jack_nframes_t nframes,
                     unsigned sizeInputs,
                     unsigned sizeOutputs);
    void processVBapHrtf(jack_default_audio_sample_t ** ins,
                         jack_default_audio_sample_t ** outs,
                         jack_nframes_t nframes,
                         unsigned sizeInputs,
                         unsigned sizeOutputs);
    void processStereo(jack_default_audio_sample_t ** ins,
                       jack_default_audio_sample_t ** outs,
                       jack_nframes_t nframes,
                       unsigned sizeInputs,
                       unsigned sizeOutputs);
    int processAudio(jack_nframes_t nframes);

private:
    //==============================================================================
    // Connect the server's outputs to the system's inputs.
    void connectedGrisToSystem();
    //==============================================================================
    JUCE_LEAK_DETECTOR(JackClientGris)
};
