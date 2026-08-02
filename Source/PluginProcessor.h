#pragma once

#include <array>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_extra/juce_gui_extra.h>

class OrchNoteMapperAudioProcessor : public juce::AudioProcessor
{
public:
    OrchNoteMapperAudioProcessor();
    ~OrchNoteMapperAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void processBlock (juce::AudioBuffer<double>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::AudioProcessorValueTreeState parameters;

    int getActiveMinNote() const;
    int getActiveMaxNote() const;

    juce::String getCurrentRangeSourceName() const;
    juce::String getCurrentRangeModeName() const;
    juce::String getCurrentMappingModeName() const;
    juce::String getCurrentInstrumentPresetName() const;

    int getPresetMinNote() const;
    int getPresetMaxNote() const;
    juce::String getPresetRangeDisplayText() const;

    int getLastPerformanceInputNoteForDebug() const { return lastPerformanceInputNoteForDebug.load(); }
    int getLastPerformanceOutputNoteForDebug() const { return lastPerformanceOutputNoteForDebug.load(); }
    int getLastPerformanceActiveMinForDebug() const { return lastPerformanceActiveMinForDebug.load(); }
    int getLastPerformanceActiveMaxForDebug() const { return lastPerformanceActiveMaxForDebug.load(); }

    int getLastKeyswitchInputNoteForDebug() const { return lastKeyswitchInputNoteForDebug.load(); }
    int getLastKeyswitchOutputNoteForDebug() const { return lastKeyswitchOutputNoteForDebug.load(); }
    int getLastKeyswitchRoleForDebug() const { return lastKeyswitchRoleForDebug.load(); }

private:
    template <typename FloatType>
    void processMidiAndClearAudio (juce::AudioBuffer<FloatType>& buffer, juce::MidiBuffer& midiMessages);

    int getParameterIntValue (const juce::String& parameterID, int fallback) const;

    int getManualActiveMinNote() const;
    int getManualActiveMaxNote() const;

    int clampNoteToActiveRange (int noteNumber) const;
    int octaveFoldNoteToActiveRange (int noteNumber) const;
    int mapNoteToActiveRange (int noteNumber) const;

    bool isProtectedKeyswitchNote (int noteNumber) const;
    bool isLowKeyswitchSourceNote (int noteNumber) const;
    bool isHighKeyswitchSourceNote (int noteNumber) const;
    int getEffectiveKeyswitchDestinationMin (int fallbackDestinationMin) const;
    int mapLowKeyswitchNoteToDestination (int noteNumber) const;
    int mapHighKeyswitchNoteToDestination (int noteNumber) const;

    static int getNoteMapIndex (int midiChannel, int inputNoteNumber);

    std::atomic<int> lastPerformanceInputNoteForDebug { -1 };
    std::atomic<int> lastPerformanceOutputNoteForDebug { -1 };
    std::atomic<int> lastPerformanceActiveMinForDebug { -1 };
    std::atomic<int> lastPerformanceActiveMaxForDebug { -1 };

    std::atomic<int> lastKeyswitchInputNoteForDebug { -1 };
    std::atomic<int> lastKeyswitchOutputNoteForDebug { -1 };
    std::atomic<int> lastKeyswitchRoleForDebug { 0 }; // 1 = protected keyswitch, 2 = mapped keyswitch

    // Index: channel 1-16 and note 0-127.
    // Value: output note 0-127, or -1 if no active mapped note.
    std::array<int, 16 * 128> activeNoteMap;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrchNoteMapperAudioProcessor)
};









