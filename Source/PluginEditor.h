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
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

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
    juce::Label blockControlCcsTitleLabel;
    juce::Label ksBankCcEnableTitleLabel;

    juce::ComboBox rangeSourceBox;
    juce::ComboBox rangeModeBox;
    juce::ComboBox mappingModeBox;
    juce::ComboBox instrumentPresetBox;
    juce::ComboBox keyswitchModeBox;
    juce::ComboBox ksDestinationPresetBox;
    juce::ComboBox lowKsProtectBox;
    juce::ComboBox highKsProtectBox;
    juce::ComboBox blockControlCcsBox;
    juce::ComboBox ksBankCcEnableBox;

    std::unique_ptr<ComboBoxAttachment> rangeSourceAttachment;
    std::unique_ptr<ComboBoxAttachment> rangeModeAttachment;
    std::unique_ptr<ComboBoxAttachment> mappingModeAttachment;
    std::unique_ptr<ComboBoxAttachment> instrumentPresetAttachment;
    std::unique_ptr<ComboBoxAttachment> keyswitchModeAttachment;
    std::unique_ptr<ComboBoxAttachment> ksDestinationPresetAttachment;
    std::unique_ptr<ComboBoxAttachment> lowKsProtectAttachment;
    std::unique_ptr<ComboBoxAttachment> highKsProtectAttachment;

    juce::Label ksBankCcNumberTitleLabel;
    juce::Label ksBankCountTitleLabel;
    juce::Label ksBankCcOffsetTitleLabel;
    juce::Slider ksBankCcNumberSlider;
    juce::Slider ksBankCountSlider;
    juce::Slider ksBankCcOffsetSlider;
    std::unique_ptr<SliderAttachment> ksBankCcNumberAttachment;
    std::unique_ptr<SliderAttachment> ksBankCountAttachment;
    std::unique_ptr<SliderAttachment> ksBankCcOffsetAttachment;
    juce::Label ksBankStatusLabel;

    juce::Label ksGenEnableTitleLabel;
    juce::ComboBox ksGenEnableBox;
    juce::Label ksGenIntervalTitleLabel;
    juce::Label ksGenProbabilityTitleLabel;
    juce::Label ksGenChannelTitleLabel;
    juce::Slider ksGenIntervalSlider;
    juce::Slider ksGenProbabilitySlider;
    juce::Slider ksGenChannelSlider;
    std::unique_ptr<SliderAttachment> ksGenIntervalAttachment;
    std::unique_ptr<SliderAttachment> ksGenProbabilityAttachment;
    std::unique_ptr<SliderAttachment> ksGenChannelAttachment;
    juce::Label ksGenStatusLabel;

    juce::Label presetRangeLabel;
    juce::Label rangeColumnHeaderLabel;
    juce::Label lowKsColumnHeaderLabel;
    juce::Label highKsColumnHeaderLabel;
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

    // 2026-09-28: "Destination Min" above is a read-only label showing the
    // EFFECTIVE value - correct when a fixed 12-note preset governs (nothing
    // to edit, the preset already IS the band), but Custom mode previously
    // had literally no control surface at all for lowKsDestinationMin/Max -
    // only reachable via the host's own generic parameter list. Live-rig bug
    // this fixes: a low instrument (Bass Clarinet/Bassoon/Contrabassoon) can
    // have real playable notes as low as 34-36, so the fixed "24 - 35" preset
    // hands its own generated/forwarded keyswitches straight into audible
    // pitch territory - Custom needs a real ceiling the user can actually
    // set. These two sliders replace lowKsDestinationMinLabel/Max display
    // (shown instead of it, never alongside) whenever KS Destination is set
    // to Custom - see timerCallback's own visibility toggle.
    juce::Label lowKsDestinationMinTitleLabel;
    juce::Label lowKsDestinationMaxTitleLabel;
    juce::Slider lowKsDestinationMinSlider;
    juce::Slider lowKsDestinationMaxSlider;
    std::unique_ptr<SliderAttachment> lowKsDestinationMinAttachment;
    std::unique_ptr<SliderAttachment> lowKsDestinationMaxAttachment;

    // Same story as the Low KS pair above, High side.
    juce::Label highKsDestinationMinTitleLabel;
    juce::Label highKsDestinationMaxTitleLabel;
    juce::Slider highKsDestinationMinSlider;
    juce::Slider highKsDestinationMaxSlider;
    std::unique_ptr<SliderAttachment> highKsDestinationMinAttachment;
    std::unique_ptr<SliderAttachment> highKsDestinationMaxAttachment;

    juce::Label activeRangeLabel;
    juce::Label lastNoteDebugLabel;
    juce::Label lastKeyswitchDebugLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrchNoteMapperAudioProcessorEditor)
};













