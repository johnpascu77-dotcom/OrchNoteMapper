#pragma once

#include <array>
#include <vector>
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

    // --- KS destination bank, live-CC-selected (2026-09-24) --------------
    // Lets an arc-driven CC pick which of the 10 fixed ksDestinationPreset
    // bands is currently in effect, instead of the dropdown alone. See
    // resolveEffectiveKsPresetIndex()'s own comment for why this lives here
    // (upstream of Opus) rather than as an Opus-native Controller trigger.
    int getLastKsBankCcValueForUi() const { return lastKsBankCcValue.load(); }
    int getEffectiveKsDestinationPresetIndexForUi() const { return resolveEffectiveKsPresetIndex(); }

    // --- KS generator (2026-09-24) ----------------------------------------
    // Arranger timelines can't rely on a per-track looping clip to keep
    // re-firing a KS note the way a Clip Launcher scene could - see
    // runKeyswitchGenerator()'s own comment. -1 = nothing generated yet.
    int getLastGeneratedKeyswitchNoteForUi() const { return lastGeneratedKeyswitchNoteForUi.load(); }
    int getLastGeneratedKeyswitchChannelForUi() const { return lastGeneratedKeyswitchChannelForUi.load(); }

private:
    template <typename FloatType>
    void processMidiAndClearAudio (juce::AudioBuffer<FloatType>& buffer, juce::MidiBuffer& midiMessages);

    int getParameterIntValue (const juce::String& parameterID, int fallback) const;
    float getParameterFloatValue (const juce::String& parameterID, float fallback) const;

    int getManualActiveMinNote() const;
    int getManualActiveMaxNote() const;

    int clampNoteToActiveRange (int noteNumber) const;
    int octaveFoldNoteToActiveRange (int noteNumber) const;
    int mapNoteToActiveRange (int noteNumber) const;

    bool isProtectedKeyswitchNote (int noteNumber) const;
    bool isLowKeyswitchSourceNote (int noteNumber) const;
    bool isHighKeyswitchSourceNote (int noteNumber) const;
    int resolveEffectiveKsPresetIndex() const;
    int getEffectiveKeyswitchDestinationMin (int fallbackDestinationMin) const;
    int getEffectiveKeyswitchDestinationMax (int fallbackDestinationMax) const;
    int mapLowKeyswitchNoteToDestination (int noteNumber) const;
    int mapHighKeyswitchNoteToDestination (int noteNumber) const;

    static int getNoteMapIndex (int midiChannel, int inputNoteNumber);

    bool isLowKeyswitchGenerationAvailable() const;
    bool isHighKeyswitchGenerationAvailable() const;
    void runKeyswitchGenerator (juce::MidiBuffer& outputBuffer, int numSamples);

    std::atomic<int> lastPerformanceInputNoteForDebug { -1 };
    std::atomic<int> lastPerformanceOutputNoteForDebug { -1 };
    std::atomic<int> lastPerformanceActiveMinForDebug { -1 };
    std::atomic<int> lastPerformanceActiveMaxForDebug { -1 };

    std::atomic<int> lastKeyswitchInputNoteForDebug { -1 };
    std::atomic<int> lastKeyswitchOutputNoteForDebug { -1 };
    std::atomic<int> lastKeyswitchRoleForDebug { 0 }; // 1 = protected keyswitch, 2 = mapped keyswitch

    // Last value seen on the configured ksBankCcNumber, or -1 if that CC
    // hasn't arrived since load. Tracked regardless of ksBankCcEnable so
    // turning the feature on mid-session doesn't need a fresh CC first.
    std::atomic<int> lastKsBankCcValue { -1 };

    // Index: channel 1-16 and note 0-127 (see getNoteMapIndex).
    // Value: a FIFO QUEUE of output notes, one entry per still-open note-on
    // sharing this (channel, input note) - real live-rig bug (2026-09-28):
    // this used to be a single int, overwritten on every note-on. Two
    // overlapping same-(channel,pitch) note-ons - a genuine live legato
    // re-strike, or (the actual case that surfaced this) an upstream
    // OrchDelay instance relaying content that wasn't truly monophonic -
    // clobbered the FIRST note's own remembered output pitch with the
    // SECOND's. If the active range/fold ever differs between the two
    // occurrences (Content-Aware Restlessness or any other CC modulating it
    // mid-note), the first note-off then looked up the SECOND note's mapping
    // (wrong output pitch, releasing nothing real), and the second note-off
    // later found the slot already cleared and fell back to a FRESH
    // recomputation (praying it happened to match) - either way, the
    // instrument's own real sounding voice for one of the two notes never
    // got a genuine matching note-off: permanently stuck, exactly the
    // pattern OrchGate's stuck-note watchdog kept having to recover. Same
    // FIFO-by-occurrence fix as OrchDelay's own captureEvent already uses
    // for the identical bug shape (see that function's own doc comment) -
    // note-on pushes, note-off pops the OLDEST entry for this slot, so N
    // overlapping occurrences always resolve to N correctly-paired note-offs
    // regardless of what the mapping looks like at the moment each fires.
    std::array<std::vector<int>, 16 * 128> activeNoteMap;

    double currentSampleRate = 44100.0;

    // Generates its own brief KS Note-On/Note-Off pairs, landing in whichever
    // source zone (Low/High) is currently mapping-enabled, so the
    // Key-triggered switching (and the CC-bank layer on top of it) keeps
    // getting re-evaluated during real playback - a Clip Launcher scene can
    // loop a KS-only clip forever to do this, but an Arranger timeline has no
    // equivalent without duplicating that clip's content across the whole
    // piece by hand on every track. Only fires while the host transport is
    // actually playing. Always the destination (mapped) note, never the raw
    // source note, so a disable mid-note still releases the note Opus is
    // actually holding.
    //
    // Sent on lastSeenChannel (see below), NOT a hardcoded channel - a real
    // live-rig bug (2026-09-25): hardcoded channel 1 meant an instrument
    // whose real channel wasn't 1 had its generated keyswitches delivered to
    // whatever OTHER instrument's Opus instance WAS listening on channel 1 -
    // Double Bass's own correctly-mapped 72-83 keyswitches were landing on
    // Violin 1 instead, where 72-83 sits inside Violin's normal playable
    // range, so they sounded as real, audible, unexplained short notes
    // rather than silent triggers. Matching whatever channel real notes use
    // keeps generated notes indistinguishable from real ones to everything
    // downstream, regardless of the rig's own channel-routing topology.
    double ksGenSecondsUntilNextTick = 0.0;
    double ksGenSecondsUntilNoteOff = 0.0;
    int ksGenPendingOutputNote = -1;
    int ksGenPendingOutputChannel = 1;
    int lastSeenChannel = 1;
    juce::Random ksGenRandom;
    std::atomic<int> lastGeneratedKeyswitchNoteForUi { -1 };
    std::atomic<int> lastGeneratedKeyswitchChannelForUi { -1 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrchNoteMapperAudioProcessor)
};









