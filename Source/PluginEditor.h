#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"

class OrchNoteMapperAudioProcessorEditor : public juce::AudioProcessorEditor,
                                           private juce::Timer
{
public:
    explicit OrchNoteMapperAudioProcessorEditor (OrchNoteMapperAudioProcessor&);
    ~OrchNoteMapperAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void syncComboBoxesFromParameters();

    OrchNoteMapperAudioProcessor& audioProcessor;

    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    juce::Label titleLabel;
    juce::Label phaseLabel;
    juce::Label buildLabel;

    juce::Label rangeSourceTitleLabel;
    juce::Label rangeModeTitleLabel;
    juce::Label mappingModeTitleLabel;
    juce::Label instrumentPresetTitleLabel;
    juce::Label keyswitchModeTitleLabel;
    juce::Label ksDestinationPresetTitleLabel;
    juce::Label lowKsProtectTitleLabel;
    juce::Label highKsProtectTitleLabel;

    juce::ComboBox rangeSourceBox;
    juce::ComboBox rangeModeBox;
    juce::ComboBox mappingModeBox;
    juce::ComboBox instrumentPresetBox;
    juce::ComboBox keyswitchModeBox;
    juce::ComboBox ksDestinationPresetBox;
    juce::ComboBox lowKsProtectBox;
    juce::ComboBox highKsProtectBox;

    std::unique_ptr<ComboBoxAttachment> rangeSourceAttachment;
    std::unique_ptr<ComboBoxAttachment> rangeModeAttachment;
    std::unique_ptr<ComboBoxAttachment> mappingModeAttachment;
    std::unique_ptr<ComboBoxAttachment> instrumentPresetAttachment;
    std::unique_ptr<ComboBoxAttachment> keyswitchModeAttachment;
    std::unique_ptr<ComboBoxAttachment> ksDestinationPresetAttachment;
    std::unique_ptr<ComboBoxAttachment> lowKsProtectAttachment;
    std::unique_ptr<ComboBoxAttachment> highKsProtectAttachment;

    juce::Label presetRangeLabel;
    juce::Label minNoteLabel;
    juce::Label maxNoteLabel;
    juce::Label centerNoteLabel;
    juce::Label windowSpanLabel;
    juce::Label lowKsMaxLabel;
    juce::Label lowKsSourceMinLabel;
    juce::Label lowKsSourceMaxLabel;
    juce::Label lowKsDestinationMinLabel;
    juce::Label highKsMinLabel;
    juce::Label highKsSourceMinLabel;
    juce::Label highKsSourceMaxLabel;
    juce::Label highKsDestinationMinLabel;
    juce::Label activeRangeLabel;
    juce::Label lastNoteDebugLabel;
    juce::Label lastKeyswitchDebugLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrchNoteMapperAudioProcessorEditor)
};













