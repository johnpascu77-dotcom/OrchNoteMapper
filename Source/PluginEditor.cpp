#include "PluginEditor.h"
#include "OrchNoteMapperBuildInfo.h"

namespace
{
    void setChoiceParameterFromComboBox (juce::AudioProcessorValueTreeState& parameters,
                                         const juce::String& parameterID,
                                         juce::ComboBox& box,
                                         int numberOfChoices)
    {
        auto* parameter = parameters.getParameter (parameterID);

        if (parameter == nullptr)
            return;

        const int selectedIndex = box.getSelectedId() - 1;

        if (selectedIndex < 0)
            return;

        const float normalisedValue = numberOfChoices <= 1
                                    ? 0.0f
                                    : static_cast<float> (selectedIndex) / static_cast<float> (numberOfChoices - 1);

        parameter->beginChangeGesture();
        parameter->setValueNotifyingHost (normalisedValue);
        parameter->endChangeGesture();
    }

    int getChoiceParameterIndex (juce::AudioProcessorValueTreeState& parameters,
                                 const juce::String& parameterID)
    {
        auto* parameter = parameters.getParameter (parameterID);

        if (parameter == nullptr)
            return 0;

        if (auto* choiceParameter = dynamic_cast<juce::AudioParameterChoice*> (parameter))
            return choiceParameter->getIndex();

        return 0;
    }
}

OrchNoteMapperAudioProcessorEditor::OrchNoteMapperAudioProcessorEditor (OrchNoteMapperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setResizable (true, true);
    setResizeLimits (860, 700, 1100, 1380);
    setSize (860, 1015);   // +75 for the Custom-mode Destination Min/Max sliders (2026-09-28)

    auto setupDisplayLabel = [this] (juce::Label& label)
    {
        label.setJustificationType (juce::Justification::centredLeft);
        label.setColour (juce::Label::textColourId, juce::Colours::white);
        label.setFont (juce::FontOptions (16.0f));
        addAndMakeVisible (label);
    };

    auto setupTitleLabel = [this] (juce::Label& label, const juce::String& text)
    {
        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centredLeft);
        label.setColour (juce::Label::textColourId, juce::Colours::white);
        label.setFont (juce::FontOptions (15.0f, juce::Font::bold));
        addAndMakeVisible (label);
    };

    auto setupComboBox = [this] (juce::ComboBox& box)
    {
        box.setColour (juce::ComboBox::backgroundColourId, juce::Colour::fromRGB (38, 46, 56));
        box.setColour (juce::ComboBox::textColourId, juce::Colours::white);
        box.setColour (juce::ComboBox::outlineColourId, juce::Colour::fromRGB (110, 210, 255));
        box.setColour (juce::ComboBox::arrowColourId, juce::Colour::fromRGB (110, 210, 255));
        addAndMakeVisible (box);
    };

    titleLabel.setText ("OrchNoteMapper", juce::dontSendNotification);
    titleLabel.setJustificationType (juce::Justification::centred);
    titleLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    titleLabel.setFont (juce::FontOptions (28.0f, juce::Font::bold));
    addAndMakeVisible (titleLabel);

    phaseLabel.setText ("Orchestral Range + Keyswitch Mapper", juce::dontSendNotification);
    phaseLabel.setJustificationType (juce::Justification::centred);
    phaseLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    phaseLabel.setFont (juce::FontOptions (15.0f));
    addAndMakeVisible (phaseLabel);

    buildLabel.setText (juce::String ("Build: ") + orchNoteMapperBuildTimestamp, juce::dontSendNotification);
    buildLabel.setJustificationType (juce::Justification::centred);
    buildLabel.setColour (juce::Label::textColourId, juce::Colour::fromRGB (140, 160, 180));
    buildLabel.setFont (juce::FontOptions (12.0f));
    addAndMakeVisible (buildLabel);

    setupTitleLabel (rangeSourceTitleLabel, "Range Source");
    setupTitleLabel (rangeModeTitleLabel, "Range Mode");
    setupTitleLabel (mappingModeTitleLabel, "Mapping Mode");
    setupTitleLabel (instrumentPresetTitleLabel, "Instrument Preset");
    setupTitleLabel (keyswitchModeTitleLabel, "Keyswitch Mode");
    setupTitleLabel (ksDestinationPresetTitleLabel, "KS Destination");
    setupTitleLabel (lowKsProtectTitleLabel, "Low KS Protect");
    setupTitleLabel (highKsProtectTitleLabel, "High KS Protect");
    setupTitleLabel (blockControlCcsTitleLabel, "Block Control CCs");
    setupTitleLabel (ksBankCcEnableTitleLabel, "KS Bank CC");

    setupComboBox (rangeSourceBox);
    setupComboBox (rangeModeBox);
    setupComboBox (mappingModeBox);
    setupComboBox (instrumentPresetBox);
    setupComboBox (keyswitchModeBox);
    setupComboBox (ksDestinationPresetBox);
    setupComboBox (lowKsProtectBox);
    setupComboBox (highKsProtectBox);
    setupComboBox (blockControlCcsBox);
    setupComboBox (ksBankCcEnableBox);

    rangeSourceBox.addItem ("Manual", 1);
    rangeSourceBox.addItem ("Preset", 2);

    rangeModeBox.addItem ("Min/Max", 1);
    rangeModeBox.addItem ("Center/Span", 2);

    mappingModeBox.addItem ("Clamp", 1);
    mappingModeBox.addItem ("Octave Fold", 2);

    instrumentPresetBox.addItem ("Custom", 1);
    instrumentPresetBox.addItem ("Piccolo", 2);
    instrumentPresetBox.addItem ("Flute", 3);
    instrumentPresetBox.addItem ("Oboe", 4);
    instrumentPresetBox.addItem ("English Horn", 5);
    instrumentPresetBox.addItem ("Clarinet", 6);
    instrumentPresetBox.addItem ("Bass Clarinet", 7);
    instrumentPresetBox.addItem ("Bassoon", 8);
    instrumentPresetBox.addItem ("Contrabassoon", 9);
    instrumentPresetBox.addItem ("French Horn", 10);
    instrumentPresetBox.addItem ("Trumpet", 11);
    instrumentPresetBox.addItem ("Trombone", 12);
    instrumentPresetBox.addItem ("Bass Trombone", 13);
    instrumentPresetBox.addItem ("Tuba", 14);
    instrumentPresetBox.addItem ("Timpani", 15);
    instrumentPresetBox.addItem ("Violin", 16);
    instrumentPresetBox.addItem ("Viola", 17);
    instrumentPresetBox.addItem ("Cello", 18);
    instrumentPresetBox.addItem ("Double Bass", 19);
    instrumentPresetBox.addItem ("Glockenspiel", 20);
    instrumentPresetBox.addItem ("Xylophone", 21);
    instrumentPresetBox.addItem ("Marimba", 22);
    instrumentPresetBox.addItem ("Vibraphone", 23);
    instrumentPresetBox.addItem ("Tubular Bells", 24);


    keyswitchModeBox.addItem ("Off", 1);
    keyswitchModeBox.addItem ("Protect Only", 2);
    keyswitchModeBox.addItem ("Map Low to Destination", 3);
    keyswitchModeBox.addItem ("Map High to Destination", 4);
    keyswitchModeBox.addItem ("Map Low + High to Destination", 5);

    ksDestinationPresetBox.addItem ("Custom", 1);
    ksDestinationPresetBox.addItem ("0 - 11", 2);
    ksDestinationPresetBox.addItem ("12 - 23", 3);
    ksDestinationPresetBox.addItem ("24 - 35", 4);
    ksDestinationPresetBox.addItem ("36 - 47", 5);
    ksDestinationPresetBox.addItem ("48 - 59", 6);
    ksDestinationPresetBox.addItem ("60 - 71", 7);
    ksDestinationPresetBox.addItem ("72 - 83", 8);
    ksDestinationPresetBox.addItem ("84 - 95", 9);
    ksDestinationPresetBox.addItem ("96 - 107", 10);
    ksDestinationPresetBox.addItem ("108 - 119", 11);

    lowKsProtectBox.addItem ("Off", 1);
    lowKsProtectBox.addItem ("On", 2);

    highKsProtectBox.addItem ("Off", 1);
    highKsProtectBox.addItem ("On", 2);

    blockControlCcsBox.addItem ("Off", 1);
    blockControlCcsBox.addItem ("CC 20-54 (OrchConductor)", 2);
    blockControlCcsBox.addItem ("CC 20-64 (OC + MPL)", 3);

    ksBankCcEnableBox.addItem ("Off", 1);
    ksBankCcEnableBox.addItem ("On", 2);

    rangeSourceBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "rangeSource") + 1,
                                  juce::dontSendNotification);

    rangeModeBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "rangeMode") + 1,
                                juce::dontSendNotification);

    mappingModeBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "mappingMode") + 1,
                                  juce::dontSendNotification);

    instrumentPresetBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "instrumentPreset") + 1,
                                       juce::dontSendNotification);

    keyswitchModeBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "keyswitchMode") + 1,
                                    juce::dontSendNotification);
    ksDestinationPresetBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "ksDestinationPreset") + 1,
                                          juce::dontSendNotification);
    lowKsProtectBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "lowKsProtect") + 1,
                                   juce::dontSendNotification);
    highKsProtectBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "highKsProtect") + 1,
                                    juce::dontSendNotification);
    blockControlCcsBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "blockControlCcs") + 1,
                                      juce::dontSendNotification);
    ksBankCcEnableBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "ksBankCcEnable") + 1,
                                     juce::dontSendNotification);

    rangeSourceBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "rangeSource", rangeSourceBox, 2);
        timerCallback();
    };

    rangeModeBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "rangeMode", rangeModeBox, 2);
        timerCallback();
    };

    mappingModeBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "mappingMode", mappingModeBox, 2);
        timerCallback();
    };

    instrumentPresetBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "instrumentPreset", instrumentPresetBox, 24);
        timerCallback();
    };

    keyswitchModeBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "keyswitchMode", keyswitchModeBox, 5);
        timerCallback();
    };

    ksDestinationPresetBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "ksDestinationPreset", ksDestinationPresetBox, 11);
        timerCallback();
    };

    lowKsProtectBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "lowKsProtect", lowKsProtectBox, 2);
        timerCallback();
    };

    highKsProtectBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "highKsProtect", highKsProtectBox, 2);
        timerCallback();
    };

    blockControlCcsBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "blockControlCcs", blockControlCcsBox, 3);
        timerCallback();
    };

    ksBankCcEnableBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "ksBankCcEnable", ksBankCcEnableBox, 2);
        timerCallback();
    };

    setupTitleLabel (ksBankCcNumberTitleLabel, "Bank CC Number");
    setupTitleLabel (ksBankCountTitleLabel, "Bank Count");
    setupTitleLabel (ksBankCcOffsetTitleLabel, "Bank CC Offset");
    setupTitleLabel (ksGenEnableTitleLabel, "KS Generator");
    setupTitleLabel (ksGenIntervalTitleLabel, "Interval (sec)");
    setupTitleLabel (ksGenProbabilityTitleLabel, "Probability %");
    setupTitleLabel (ksGenChannelTitleLabel, "Channel (0=auto)");

    auto setupSlider = [this] (juce::Slider& slider, double minValue, double maxValue, double interval)
    {
        slider.setSliderStyle (juce::Slider::LinearHorizontal);
        slider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 56, 22);
        slider.setRange (minValue, maxValue, interval);
        slider.setColour (juce::Slider::backgroundColourId, juce::Colour::fromRGB (38, 46, 56));
        slider.setColour (juce::Slider::trackColourId, juce::Colour::fromRGB (110, 210, 255));
        slider.setColour (juce::Slider::thumbColourId, juce::Colour::fromRGB (110, 210, 255));
        addAndMakeVisible (slider);
    };

    setupSlider (ksBankCcNumberSlider, 0.0, 127.0, 1.0);
    setupSlider (ksBankCountSlider, 1.0, 10.0, 1.0);
    setupSlider (ksBankCcOffsetSlider, -63.0, 63.0, 1.0);
    setupSlider (ksGenIntervalSlider, 0.25, 8.0, 0.05);
    setupSlider (ksGenProbabilitySlider, 0.0, 100.0, 1.0);
    setupSlider (ksGenChannelSlider, 0.0, 16.0, 1.0);

    ksBankCcNumberAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "ksBankCcNumber", ksBankCcNumberSlider);
    ksBankCountAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "ksBankCount", ksBankCountSlider);
    ksBankCcOffsetAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "ksBankCcOffset", ksBankCcOffsetSlider);
    ksGenIntervalAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "ksGenIntervalSeconds", ksGenIntervalSlider);
    ksGenProbabilityAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "ksGenProbabilityPercent", ksGenProbabilitySlider);
    ksGenChannelAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "ksGenChannelOverride", ksGenChannelSlider);

    setupComboBox (ksGenEnableBox);
    ksGenEnableBox.addItem ("Off", 1);
    ksGenEnableBox.addItem ("On", 2);
    ksGenEnableBox.setSelectedId (getChoiceParameterIndex (audioProcessor.parameters, "ksGenEnable") + 1,
                                  juce::dontSendNotification);
    ksGenEnableBox.onChange = [this]
    {
        setChoiceParameterFromComboBox (audioProcessor.parameters, "ksGenEnable", ksGenEnableBox, 2);
        timerCallback();
    };

    setupTitleLabel (rangeColumnHeaderLabel, "Range");
    setupTitleLabel (lowKsColumnHeaderLabel, "Low KS");
    setupTitleLabel (highKsColumnHeaderLabel, "High KS");

    setupDisplayLabel (presetRangeLabel);
    setupDisplayLabel (minNoteLabel);
    setupDisplayLabel (maxNoteLabel);
    setupDisplayLabel (centerNoteLabel);
    setupDisplayLabel (windowSpanLabel);
    setupDisplayLabel (lowKsMaxLabel);
    setupDisplayLabel (lowKsSourceMinLabel);
    setupDisplayLabel (lowKsSourceMaxLabel);
    setupDisplayLabel (lowKsDestinationMinLabel);
    setupDisplayLabel (highKsMinLabel);
    setupDisplayLabel (highKsSourceMinLabel);
    setupDisplayLabel (highKsSourceMaxLabel);
    setupDisplayLabel (highKsDestinationMinLabel);
    setupDisplayLabel (activeRangeLabel);

    // Custom-mode Destination Min/Max sliders (2026-09-28) - see these
    // members' own doc comment in the header. Shown INSTEAD of
    // lowKsDestinationMinLabel/highKsDestinationMinLabel (never alongside),
    // toggled per-frame in timerCallback based on whether KS Destination is
    // set to Custom.
    setupTitleLabel (lowKsDestinationMinTitleLabel, "Dest Min");
    setupTitleLabel (lowKsDestinationMaxTitleLabel, "Dest Max");
    setupTitleLabel (highKsDestinationMinTitleLabel, "Dest Min");
    setupTitleLabel (highKsDestinationMaxTitleLabel, "Dest Max");
    setupSlider (lowKsDestinationMinSlider, 0.0, 127.0, 1.0);
    setupSlider (lowKsDestinationMaxSlider, 0.0, 127.0, 1.0);
    setupSlider (highKsDestinationMinSlider, 0.0, 127.0, 1.0);
    setupSlider (highKsDestinationMaxSlider, 0.0, 127.0, 1.0);

    lowKsDestinationMinAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "lowKsDestinationMin", lowKsDestinationMinSlider);
    lowKsDestinationMaxAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "lowKsDestinationMax", lowKsDestinationMaxSlider);
    highKsDestinationMinAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "highKsDestinationMin", highKsDestinationMinSlider);
    highKsDestinationMaxAttachment = std::make_unique<SliderAttachment> (
        audioProcessor.parameters, "highKsDestinationMax", highKsDestinationMaxSlider);

    presetRangeLabel.setColour (juce::Label::textColourId, juce::Colour::fromRGB (180, 220, 255));

    activeRangeLabel.setColour (juce::Label::textColourId, juce::Colour::fromRGB (110, 210, 255));
    activeRangeLabel.setFont (juce::FontOptions (18.0f, juce::Font::bold));

    setupDisplayLabel (lastNoteDebugLabel);
    setupDisplayLabel (lastKeyswitchDebugLabel);
    setupDisplayLabel (ksBankStatusLabel);
    setupDisplayLabel (ksGenStatusLabel);

    lastNoteDebugLabel.setColour (juce::Label::textColourId, juce::Colour::fromRGB (255, 210, 120));
    lastKeyswitchDebugLabel.setColour (juce::Label::textColourId, juce::Colour::fromRGB (255, 210, 120));
    ksBankStatusLabel.setColour (juce::Label::textColourId, juce::Colour::fromRGB (255, 210, 120));
    ksGenStatusLabel.setColour (juce::Label::textColourId, juce::Colour::fromRGB (255, 210, 120));

    lastNoteDebugLabel.setFont (juce::FontOptions (15.0f));
    lastKeyswitchDebugLabel.setFont (juce::FontOptions (15.0f));
    ksBankStatusLabel.setFont (juce::FontOptions (15.0f));
    ksGenStatusLabel.setFont (juce::FontOptions (15.0f));

    startTimerHz (20);
    syncComboBoxesFromParameters();
    timerCallback();
}

OrchNoteMapperAudioProcessorEditor::~OrchNoteMapperAudioProcessorEditor()
{
    stopTimer();
}

void OrchNoteMapperAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB (28, 34, 42));

    auto bounds = getLocalBounds().reduced (6);

    g.setColour (juce::Colour::fromRGB (110, 210, 255));
    g.drawRoundedRectangle (bounds.toFloat(), 10.0f, 2.0f);
}

void OrchNoteMapperAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (28);

    titleLabel.setBounds (area.removeFromTop (42));
    phaseLabel.setBounds (area.removeFromTop (24));
    buildLabel.setBounds (area.removeFromTop (18));

    area.removeFromTop (8);

    auto controlArea = area.removeFromTop (270);

    auto leftColumn = controlArea.removeFromLeft (424);
    controlArea.removeFromLeft (28);
    auto rightColumn = controlArea;

    auto placeComboRow = [] (juce::Rectangle<int>& column,
                             juce::Label& title,
                             juce::ComboBox& box,
                             int titleWidth)
    {
        auto row = column.removeFromTop (40);
        title.setBounds (row.removeFromLeft (titleWidth));
        row.removeFromLeft (10);
        box.setBounds (row);
        column.removeFromTop (12);
    };

    placeComboRow (leftColumn, rangeSourceTitleLabel, rangeSourceBox, 168);
    placeComboRow (leftColumn, rangeModeTitleLabel, rangeModeBox, 168);
    placeComboRow (leftColumn, mappingModeTitleLabel, mappingModeBox, 168);
    placeComboRow (leftColumn, instrumentPresetTitleLabel, instrumentPresetBox, 168);
    placeComboRow (leftColumn, blockControlCcsTitleLabel, blockControlCcsBox, 168);

    placeComboRow (rightColumn, keyswitchModeTitleLabel, keyswitchModeBox, 178);
    placeComboRow (rightColumn, ksDestinationPresetTitleLabel, ksDestinationPresetBox, 178);
    placeComboRow (rightColumn, lowKsProtectTitleLabel, lowKsProtectBox, 178);
    placeComboRow (rightColumn, highKsProtectTitleLabel, highKsProtectBox, 178);
    placeComboRow (rightColumn, ksBankCcEnableTitleLabel, ksBankCcEnableBox, 178);

    area.removeFromTop (12);

    presetRangeLabel.setBounds (area.removeFromTop (30));
    activeRangeLabel.setBounds (area.removeFromTop (34));

    area.removeFromTop (10);

    auto ksBankArea = area.removeFromTop (190);
    auto ksBankColumn = ksBankArea.removeFromLeft (424);
    ksBankArea.removeFromLeft (28);
    auto ksGenColumn = ksBankArea;

    auto placeSliderRow = [] (juce::Rectangle<int>& column,
                              juce::Label& title,
                              juce::Slider& slider,
                              int titleWidth)
    {
        auto row = column.removeFromTop (28);
        title.setBounds (row.removeFromLeft (titleWidth));
        row.removeFromLeft (10);
        slider.setBounds (row);
        column.removeFromTop (8);
    };

    placeSliderRow (ksBankColumn, ksBankCcNumberTitleLabel, ksBankCcNumberSlider, 168);
    placeSliderRow (ksBankColumn, ksBankCountTitleLabel, ksBankCountSlider, 168);
    placeSliderRow (ksBankColumn, ksBankCcOffsetTitleLabel, ksBankCcOffsetSlider, 168);
    ksBankStatusLabel.setBounds (ksBankColumn.removeFromTop (24));

    auto ksGenEnableRow = ksGenColumn.removeFromTop (28);
    ksGenEnableTitleLabel.setBounds (ksGenEnableRow.removeFromLeft (178));
    ksGenEnableRow.removeFromLeft (10);
    ksGenEnableBox.setBounds (ksGenEnableRow);
    ksGenColumn.removeFromTop (8);

    placeSliderRow (ksGenColumn, ksGenIntervalTitleLabel, ksGenIntervalSlider, 178);
    placeSliderRow (ksGenColumn, ksGenProbabilityTitleLabel, ksGenProbabilitySlider, 178);
    placeSliderRow (ksGenColumn, ksGenChannelTitleLabel, ksGenChannelSlider, 178);
    ksGenStatusLabel.setBounds (ksGenColumn.removeFromTop (24));

    area.removeFromTop (10);

    // 3 even columns instead of 2 uneven ones - Low/High KS no longer need 8
    // stacked rows in one column, which was most of why this window got so
    // tall. Range column only ever had 4 rows worth of content anyway.
    // +72 (2026-09-28): room for the Custom-mode Destination Min/Max sliders
    // (see their own doc comment in the header) - shown INSTEAD of
    // lowKsDestinationMinLabel/highKsDestinationMinLabel, in the exact same
    // row, so the row itself needs to be tall enough for a slider (28px) not
    // just a label (24px), plus one whole extra row for Destination Max,
    // which never had a display slot of any kind before this.
    auto displayArea = area.removeFromTop (124 + 72);
    const int displayColumnWidth = (displayArea.getWidth() - 2 * 20) / 3;

    auto rangeDisplayColumn = displayArea.removeFromLeft (displayColumnWidth);
    displayArea.removeFromLeft (20);
    auto lowKsDisplayColumn = displayArea.removeFromLeft (displayColumnWidth);
    displayArea.removeFromLeft (20);
    auto highKsDisplayColumn = displayArea;

    rangeColumnHeaderLabel.setBounds (rangeDisplayColumn.removeFromTop (24));
    lowKsColumnHeaderLabel.setBounds (lowKsDisplayColumn.removeFromTop (24));
    highKsColumnHeaderLabel.setBounds (highKsDisplayColumn.removeFromTop (24));

    auto placeDisplayRow = [] (juce::Rectangle<int>& column, juce::Label& label)
    {
        label.setBounds (column.removeFromTop (24));
    };

    placeDisplayRow (rangeDisplayColumn, minNoteLabel);
    placeDisplayRow (rangeDisplayColumn, maxNoteLabel);
    placeDisplayRow (rangeDisplayColumn, centerNoteLabel);
    placeDisplayRow (rangeDisplayColumn, windowSpanLabel);

    placeDisplayRow (lowKsDisplayColumn, lowKsMaxLabel);
    placeDisplayRow (lowKsDisplayColumn, lowKsSourceMinLabel);
    placeDisplayRow (lowKsDisplayColumn, lowKsSourceMaxLabel);

    // Destination Min: the read-only label and the Custom-mode slider pair
    // share this one row (28px - tall enough for the slider; the label just
    // doesn't fill it). Only one of the two is ever visible at a time - see
    // timerCallback's own visibility toggle.
    auto placeDestinationSliderRow = [] (juce::Rectangle<int>& row, juce::Label& title, juce::Slider& slider)
    {
        auto sliderRow = row;
        title.setBounds (sliderRow.removeFromLeft (66));
        sliderRow.removeFromLeft (8);
        slider.setBounds (sliderRow);
    };

    auto lowKsDestRow = lowKsDisplayColumn.removeFromTop (28);
    lowKsDestinationMinLabel.setBounds (lowKsDestRow);
    placeDestinationSliderRow (lowKsDestRow, lowKsDestinationMinTitleLabel, lowKsDestinationMinSlider);
    lowKsDisplayColumn.removeFromTop (4);
    auto lowKsDestMaxRow = lowKsDisplayColumn.removeFromTop (28);
    placeDestinationSliderRow (lowKsDestMaxRow, lowKsDestinationMaxTitleLabel, lowKsDestinationMaxSlider);

    placeDisplayRow (highKsDisplayColumn, highKsMinLabel);
    placeDisplayRow (highKsDisplayColumn, highKsSourceMinLabel);
    placeDisplayRow (highKsDisplayColumn, highKsSourceMaxLabel);

    auto highKsDestRow = highKsDisplayColumn.removeFromTop (28);
    highKsDestinationMinLabel.setBounds (highKsDestRow);
    placeDestinationSliderRow (highKsDestRow, highKsDestinationMinTitleLabel, highKsDestinationMinSlider);
    highKsDisplayColumn.removeFromTop (4);
    auto highKsDestMaxRow = highKsDisplayColumn.removeFromTop (28);
    placeDestinationSliderRow (highKsDestMaxRow, highKsDestinationMaxTitleLabel, highKsDestinationMaxSlider);

    area.removeFromTop (8);

    lastNoteDebugLabel.setBounds (area.removeFromTop (26));
    lastKeyswitchDebugLabel.setBounds (area.removeFromTop (26));
}

void OrchNoteMapperAudioProcessorEditor::syncComboBoxesFromParameters()
{
    auto syncChoice = [this] (juce::ComboBox& box, const juce::String& parameterID)
    {
        auto* parameter = audioProcessor.parameters.getParameter (parameterID);

        if (parameter == nullptr)
            return;

        auto* choiceParameter = dynamic_cast<juce::AudioParameterChoice*> (parameter);

        if (choiceParameter == nullptr)
            return;

        const int parameterIndex = choiceParameter->getIndex();
        const int desiredSelectedId = parameterIndex + 1;

        if (desiredSelectedId > 0 && box.getSelectedId() != desiredSelectedId)
            box.setSelectedId (desiredSelectedId, juce::dontSendNotification);
    };

    syncChoice (rangeSourceBox, "rangeSource");
    syncChoice (rangeModeBox, "rangeMode");
    syncChoice (mappingModeBox, "mappingMode");
    syncChoice (instrumentPresetBox, "instrumentPreset");
    syncChoice (keyswitchModeBox, "keyswitchMode");
    syncChoice (ksDestinationPresetBox, "ksDestinationPreset");
    syncChoice (lowKsProtectBox, "lowKsProtect");
    syncChoice (highKsProtectBox, "highKsProtect");
    syncChoice (blockControlCcsBox, "blockControlCcs");
    syncChoice (ksBankCcEnableBox, "ksBankCcEnable");
    syncChoice (ksGenEnableBox, "ksGenEnable");
}
void OrchNoteMapperAudioProcessorEditor::timerCallback()
{
    syncComboBoxesFromParameters();

    auto* minNoteParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("minNote"));

    auto* maxNoteParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("maxNote"));

    auto* centerNoteParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("centerNote"));

    auto* windowSpanParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("windowSpan"));

    auto* lowKsMaxParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("lowKsMax"));

    auto* lowKsSourceMinParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("lowKsSourceMin"));

    auto* lowKsSourceMaxParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("lowKsSourceMax"));

    auto* lowKsDestinationMinParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("lowKsDestinationMin"));

    auto* highKsMinParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("highKsMin"));

    auto* highKsSourceMinParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("highKsSourceMin"));

    auto* highKsSourceMaxParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("highKsSourceMax"));

    auto* highKsDestinationMinParam = dynamic_cast<juce::AudioParameterInt*> (
        audioProcessor.parameters.getParameter ("highKsDestinationMin"));

    presetRangeLabel.setText (audioProcessor.getPresetRangeDisplayText().replace ("Preset Range:", "Instrument Range:"),
                              juce::dontSendNotification);

    if (minNoteParam != nullptr)
        minNoteLabel.setText ("Manual Min Note: " + juce::String (minNoteParam->get()),
                              juce::dontSendNotification);

    if (maxNoteParam != nullptr)
        maxNoteLabel.setText ("Manual Max Note: " + juce::String (maxNoteParam->get()),
                              juce::dontSendNotification);

    if (centerNoteParam != nullptr)
        centerNoteLabel.setText ("Target Center: " + juce::String (centerNoteParam->get()),
                                 juce::dontSendNotification);

    if (windowSpanParam != nullptr)
        windowSpanLabel.setText ("Target Span: " + juce::String (windowSpanParam->get()),
                                 juce::dontSendNotification);

    if (lowKsMaxParam != nullptr)
        lowKsMaxLabel.setText ("Max: " + juce::String (lowKsMaxParam->get()),
                               juce::dontSendNotification);

    if (lowKsSourceMinParam != nullptr)
        lowKsSourceMinLabel.setText ("Source Min: " + juce::String (lowKsSourceMinParam->get()),
                                     juce::dontSendNotification);

    if (lowKsSourceMaxParam != nullptr)
        lowKsSourceMaxLabel.setText ("Source Max: " + juce::String (lowKsSourceMaxParam->get()),
                                     juce::dontSendNotification);

    const int ksDestinationPresetIndex = getChoiceParameterIndex (audioProcessor.parameters, "ksDestinationPreset");
    const int effectiveKsBankPresetIndex = audioProcessor.getEffectiveKsDestinationPresetIndexForUi();

    if (lowKsDestinationMinParam != nullptr)
    {
        const int effectiveLowDestinationMin = effectiveKsBankPresetIndex > 0
            ? (effectiveKsBankPresetIndex - 1) * 12
            : lowKsDestinationMinParam->get();

        lowKsDestinationMinLabel.setText ("Destination Min: "
                                          + juce::String (effectiveLowDestinationMin),
                                          juce::dontSendNotification);
    }

    if (highKsMinParam != nullptr)
        highKsMinLabel.setText ("Min: " + juce::String (highKsMinParam->get()),
                                juce::dontSendNotification);

    if (highKsSourceMinParam != nullptr)
        highKsSourceMinLabel.setText ("Source Min: " + juce::String (highKsSourceMinParam->get()),
                                      juce::dontSendNotification);

    if (highKsSourceMaxParam != nullptr)
        highKsSourceMaxLabel.setText ("Source Max: " + juce::String (highKsSourceMaxParam->get()),
                                      juce::dontSendNotification);

    if (highKsDestinationMinParam != nullptr)
    {
        const int effectiveHighDestinationMin = effectiveKsBankPresetIndex > 0
            ? (effectiveKsBankPresetIndex - 1) * 12
            : highKsDestinationMinParam->get();

        highKsDestinationMinLabel.setText ("Destination Min: "
                                           + juce::String (effectiveHighDestinationMin),
                                           juce::dontSendNotification);
    }

    // Custom-mode Destination Min/Max sliders (2026-09-28) - shown INSTEAD of
    // the read-only "Destination Min" labels above (never alongside): a fixed
    // preset already IS the band (nothing to edit), Custom is the one mode
    // where the user needs a real control surface - see these controls' own
    // doc comment in the header for the live-rig bug this fixes.
    {
        const bool isCustom = ksDestinationPresetIndex <= 0;

        lowKsDestinationMinLabel.setVisible (! isCustom);
        lowKsDestinationMinTitleLabel.setVisible (isCustom);
        lowKsDestinationMinSlider.setVisible (isCustom);
        lowKsDestinationMaxTitleLabel.setVisible (isCustom);
        lowKsDestinationMaxSlider.setVisible (isCustom);

        highKsDestinationMinLabel.setVisible (! isCustom);
        highKsDestinationMinTitleLabel.setVisible (isCustom);
        highKsDestinationMinSlider.setVisible (isCustom);
        highKsDestinationMaxTitleLabel.setVisible (isCustom);
        highKsDestinationMaxSlider.setVisible (isCustom);
    }

    {
        const bool bankCcEnabled = getChoiceParameterIndex (audioProcessor.parameters, "ksBankCcEnable") == 1;
        const int bankCcValue = audioProcessor.getLastKsBankCcValueForUi();

        if (! bankCcEnabled)
        {
            ksBankStatusLabel.setText ("Bank CC: off (dropdown governs)", juce::dontSendNotification);
        }
        else if (ksDestinationPresetIndex <= 0)
        {
            ksBankStatusLabel.setText ("Bank CC: on, but KS Destination is Custom - no band to offset from",
                                       juce::dontSendNotification);
        }
        else if (bankCcValue < 0)
        {
            ksBankStatusLabel.setText ("Bank CC: no CC seen yet - using dropdown band",
                                       juce::dontSendNotification);
        }
        else
        {
            const int bandStart = (effectiveKsBankPresetIndex - 1) * 12;
            const int bandEnd = bandStart + 11;

            auto* ccOffsetParam = dynamic_cast<juce::AudioParameterInt*> (
                audioProcessor.parameters.getParameter ("ksBankCcOffset"));
            const int ccOffset = ccOffsetParam != nullptr ? ccOffsetParam->get() : 0;
            const juce::String offsetSuffix = ccOffset != 0
                ? (" (offset " + juce::String (ccOffset > 0 ? "+" : "") + juce::String (ccOffset) + ")")
                : juce::String();

            ksBankStatusLabel.setText ("Bank CC: " + juce::String (bankCcValue) + offsetSuffix
                                       + " -> band " + juce::String (bandStart) + " - " + juce::String (bandEnd),
                                       juce::dontSendNotification);
        }
    }

    {
        const bool genEnabled = getChoiceParameterIndex (audioProcessor.parameters, "ksGenEnable") == 1;
        const int lastGenerated = audioProcessor.getLastGeneratedKeyswitchNoteForUi();
        const int lastGeneratedChannel = audioProcessor.getLastGeneratedKeyswitchChannelForUi();

        if (! genEnabled)
            ksGenStatusLabel.setText ("Generator: off", juce::dontSendNotification);
        else if (lastGenerated < 0)
            ksGenStatusLabel.setText ("Generator: on, waiting for first tick", juce::dontSendNotification);
        else
            ksGenStatusLabel.setText ("Generator: last fired -> " + juce::String (lastGenerated)
                                      + " on ch " + juce::String (lastGeneratedChannel),
                                      juce::dontSendNotification);
    }

    activeRangeLabel.setText ("Active Target Range: "
                              + juce::String (audioProcessor.getActiveMinNote())
                              + " - "
                              + juce::String (audioProcessor.getActiveMaxNote()),
                              juce::dontSendNotification);

    const int lastNoteIn = audioProcessor.getLastPerformanceInputNoteForDebug();
    const int lastNoteOut = audioProcessor.getLastPerformanceOutputNoteForDebug();
    const int lastNoteMin = audioProcessor.getLastPerformanceActiveMinForDebug();
    const int lastNoteMax = audioProcessor.getLastPerformanceActiveMaxForDebug();

    const int lastKsIn = audioProcessor.getLastKeyswitchInputNoteForDebug();
    const int lastKsOut = audioProcessor.getLastKeyswitchOutputNoteForDebug();
    const int lastKsRole = audioProcessor.getLastKeyswitchRoleForDebug();

    const int mappingModeIndex = getChoiceParameterIndex (audioProcessor.parameters, "mappingMode");
    const juce::String mappingModeText = mappingModeIndex == 1 ? "Octave Fold" : "Clamp";

    const int centerNoteForDebug = centerNoteParam != nullptr
        ? centerNoteParam->get()
        : 60;

    if (lastNoteIn >= 0)
    {
        lastNoteDebugLabel.setText ("Last Note: In "
                                    + juce::String (lastNoteIn)
                                    + " -> Out "
                                    + juce::String (lastNoteOut)
                                    + " Target "
                                    + juce::String (lastNoteMin)
                                    + " - "
                                    + juce::String (lastNoteMax)
                                    + " Mode "
                                    + mappingModeText,
                                    juce::dontSendNotification);
    }
    else
    {
        lastNoteDebugLabel.setText ("Last Note: none"
                                    + juce::String (" Mode ")
                                    + mappingModeText
                                    + " Center "
                                    + juce::String (centerNoteForDebug),
                                    juce::dontSendNotification);
    }

    if (lastKsIn >= 0)
    {
        const juce::String ksRoleText = lastKsRole == 2 ? "Mapped" : "Protected";

        lastKeyswitchDebugLabel.setText ("Last KS: "
                                         + ksRoleText
                                         + " In "
                                         + juce::String (lastKsIn)
                                         + " -> Out "
                                         + juce::String (lastKsOut),
                                         juce::dontSendNotification);
    }
    else
    {
        lastKeyswitchDebugLabel.setText ("Last KS: none",
                                         juce::dontSendNotification);
    }
}





























