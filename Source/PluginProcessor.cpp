#include "PluginProcessor.h"
#include "PluginEditor.h"

OrchNoteMapperAudioProcessor::OrchNoteMapperAudioProcessor()
    : AudioProcessor (BusesProperties()),
      parameters (*this, nullptr, "PARAMETERS", createParameterLayout())
{
    activeNoteMap.fill (-1);
}

OrchNoteMapperAudioProcessor::~OrchNoteMapperAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout OrchNoteMapperAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    juce::StringArray rangeModes;
    rangeModes.add ("Min/Max");
    rangeModes.add ("Center/Span");

    juce::StringArray rangeSources;
    rangeSources.add ("Manual");
    rangeSources.add ("Preset");

    juce::StringArray mappingModes;
    mappingModes.add ("Clamp");
    mappingModes.add ("Octave Fold");

    juce::StringArray keyswitchModes;
    keyswitchModes.add ("Off");
    keyswitchModes.add ("Protect Only");
    keyswitchModes.add ("Map Low to Destination");
    keyswitchModes.add ("Map High to Destination");
    keyswitchModes.add ("Map Low + High to Destination");

    juce::StringArray ksDestinationPresets;
    ksDestinationPresets.add ("Custom");
    ksDestinationPresets.add ("0 - 11");
    ksDestinationPresets.add ("12 - 23");
    ksDestinationPresets.add ("24 - 35");
    ksDestinationPresets.add ("36 - 47");
    ksDestinationPresets.add ("48 - 59");
    ksDestinationPresets.add ("60 - 71");
    ksDestinationPresets.add ("72 - 83");
    ksDestinationPresets.add ("84 - 95");
    ksDestinationPresets.add ("96 - 107");
    ksDestinationPresets.add ("108 - 119");

    juce::StringArray onOffChoices;
    onOffChoices.add ("Off");
    onOffChoices.add ("On");

    juce::StringArray instrumentPresets;
    instrumentPresets.add ("Custom");
    instrumentPresets.add ("Piccolo");
    instrumentPresets.add ("Flute");
    instrumentPresets.add ("Oboe");
    instrumentPresets.add ("English Horn");
    instrumentPresets.add ("Clarinet");
    instrumentPresets.add ("Bass Clarinet");
    instrumentPresets.add ("Bassoon");
    instrumentPresets.add ("Contrabassoon");
    instrumentPresets.add ("French Horn");
    instrumentPresets.add ("Trumpet");
    instrumentPresets.add ("Trombone");
    instrumentPresets.add ("Bass Trombone");
    instrumentPresets.add ("Tuba");
    instrumentPresets.add ("Timpani");
    instrumentPresets.add ("Violin");
    instrumentPresets.add ("Viola");
    instrumentPresets.add ("Cello");
    instrumentPresets.add ("Double Bass");
    instrumentPresets.add ("Glockenspiel");
    instrumentPresets.add ("Xylophone");
    instrumentPresets.add ("Marimba");
    instrumentPresets.add ("Vibraphone");
    instrumentPresets.add ("Tubular Bells");

    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "rangeSource", 1 },
        "Range Source",
        rangeSources,
        0
    ));

    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "rangeMode", 1 },
        "Range Mode",
        rangeModes,
        0
    ));

    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "mappingMode", 1 },
        "Mapping Mode",
        mappingModes,
        0
    ));

    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "instrumentPreset", 1 },
        "Instrument Preset",
        instrumentPresets,
        0
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "minNote", 1 },
        "Min Note",
        0,
        127,
        36
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "maxNote", 1 },
        "Max Note",
        0,
        127,
        84
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "centerNote", 1 },
        "Center Note",
        0,
        127,
        60
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "windowSpan", 1 },
        "Window Span",
        0,
        127,
        24
    ));

    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "keyswitchMode", 1 },
        "Keyswitch Mode",
        keyswitchModes,
        0
    ));

    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "ksDestinationPreset", 1 },
        "KS Destination",
        ksDestinationPresets,
        0
    ));

    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "lowKsProtect", 1 },
        "Low KS Protect",
        onOffChoices,
        0
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "lowKsMax", 1 },
        "Low KS Max",
        0,
        127,
        35
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "lowKsSourceMin", 1 },
        "Low KS Source Min",
        0,
        127,
        12
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "lowKsSourceMax", 1 },
        "Low KS Source Max",
        0,
        127,
        23
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "lowKsDestinationMin", 1 },
        "Low KS Destination Min",
        0,
        127,
        24
    ));

    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "highKsProtect", 1 },
        "High KS Protect",
        onOffChoices,
        0
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "highKsMin", 1 },
        "High KS Min",
        0,
        127,
        96
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "highKsSourceMin", 1 },
        "High KS Source Min",
        0,
        127,
        96
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "highKsSourceMax", 1 },
        "High KS Source Max",
        0,
        127,
        107
    ));

    params.push_back (std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "highKsDestinationMin", 1 },
        "High KS Destination Min",
        0,
        127,
        24
    ));

    // Scrub the OrchConductor / MPL control-CC zone from the output so it never
    // reaches the instrument (e.g. HALion Sonic reading CC32 as Bank Select LSB).
    // By the time MIDI passes this device - the last in the Orch chain - every
    // upstream Orch plugin has already consumed the CCs it needs. Off by default.
    params.push_back (std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID { "blockControlCcs", 1 },
        "Block Control CCs",
        juce::StringArray { "Off", "CC 20-54 (OrchConductor)", "CC 20-64 (OC + MPL)" },
        0
    ));

    return { params.begin(), params.end() };
}

const juce::String OrchNoteMapperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OrchNoteMapperAudioProcessor::acceptsMidi() const
{
    return true;
}

bool OrchNoteMapperAudioProcessor::producesMidi() const
{
    return true;
}

bool OrchNoteMapperAudioProcessor::isMidiEffect() const
{
    return true;
}

double OrchNoteMapperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OrchNoteMapperAudioProcessor::getNumPrograms()
{
    return 1;
}

int OrchNoteMapperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OrchNoteMapperAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String OrchNoteMapperAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void OrchNoteMapperAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void OrchNoteMapperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    activeNoteMap.fill (-1);
}

void OrchNoteMapperAudioProcessor::releaseResources()
{
    activeNoteMap.fill (-1);
}

bool OrchNoteMapperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    juce::ignoreUnused (layouts);
    return true;
}

int OrchNoteMapperAudioProcessor::getParameterIntValue (const juce::String& parameterID, int fallback) const
{
    auto* parameter = parameters.getParameter (parameterID);

    if (parameter == nullptr)
        return fallback;

    if (auto* choiceParameter = dynamic_cast<juce::AudioParameterChoice*> (parameter))
        return choiceParameter->getIndex();

    if (auto* intParameter = dynamic_cast<juce::AudioParameterInt*> (parameter))
        return intParameter->get();

    return fallback;
}


juce::String OrchNoteMapperAudioProcessor::getCurrentInstrumentPresetName() const
{
    const int preset = getParameterIntValue ("instrumentPreset", 0);

    switch (preset)
    {
        case 1:  return "Piccolo";
        case 2:  return "Flute";
        case 3:  return "Oboe";
        case 4:  return "English Horn";
        case 5:  return "Clarinet";
        case 6:  return "Bass Clarinet";
        case 7:  return "Bassoon";
        case 8:  return "Contrabassoon";
        case 9:  return "French Horn";
        case 10: return "Trumpet";
        case 11: return "Trombone";
        case 12: return "Bass Trombone";
        case 13: return "Tuba";
        case 14: return "Timpani";
        case 15: return "Violin";
        case 16: return "Viola";
        case 17: return "Cello";
        case 18: return "Double Bass";
        case 19: return "Glockenspiel";
        case 20: return "Xylophone";
        case 21: return "Marimba";
        case 22: return "Vibraphone";
        case 23: return "Tubular Bells";
        default: return "Custom";
    }
}

int OrchNoteMapperAudioProcessor::getPresetMinNote() const
{
    const int preset = getParameterIntValue ("instrumentPreset", 0);

    switch (preset)
    {
        case 1:  return 74; // Piccolo practical sounding min
        case 2:  return 60; // Flute practical sounding min
        case 3:  return 58; // Oboe practical sounding min
        case 4:  return 52; // English Horn practical sounding min
        case 5:  return 50; // Clarinet practical sounding min
        case 6:  return 38; // Bass Clarinet practical sounding min
        case 7:  return 34; // Bassoon practical sounding min
        case 8:  return 22; // Contrabassoon practical sounding min
        case 9:  return 35; // French Horn practical sounding min
        case 10: return 54; // Trumpet practical sounding min
        case 11: return 40; // Trombone practical sounding min
        case 12: return 34; // Bass Trombone practical sounding min
        case 13: return 28; // Tuba practical sounding min
        case 14: return 38; // Timpani practical sounding min
        case 15: return 55; // Violin practical sounding min
        case 16: return 48; // Viola practical sounding min
        case 17: return 36; // Cello practical sounding min
        case 18: return 28; // Double Bass practical sounding min
        case 19: return 79; // Glockenspiel practical sounding min
        case 20: return 65; // Xylophone practical sounding min
        case 21: return 36; // Marimba practical sounding min
        case 22: return 53; // Vibraphone practical sounding min
        case 23: return 60; // Tubular Bells practical sounding min
        default: return -1; // Custom/manual fallback
    }
}

int OrchNoteMapperAudioProcessor::getPresetMaxNote() const
{
    const int preset = getParameterIntValue ("instrumentPreset", 0);

    switch (preset)
    {
        case 1:  return 108; // Piccolo practical sounding max
        case 2:  return 98;  // Flute practical sounding max
        case 3:  return 93;  // Oboe practical sounding max
        case 4:  return 81;  // English Horn practical sounding max
        case 5:  return 94;  // Clarinet practical sounding max
        case 6:  return 84;  // Bass Clarinet practical sounding max
        case 7:  return 76;  // Bassoon practical sounding max
        case 8:  return 58;  // Contrabassoon practical sounding max
        case 9:  return 77;  // French Horn practical sounding max
        case 10: return 86;  // Trumpet practical sounding max
        case 11: return 72;  // Trombone practical sounding max
        case 12: return 70;  // Bass Trombone practical sounding max
        case 13: return 65;  // Tuba practical sounding max
        case 14: return 60;  // Timpani practical sounding max
        case 15: return 105; // Violin practical sounding max
        case 16: return 88;  // Viola practical sounding max
        case 17: return 81;  // Cello practical sounding max
        case 18: return 67;  // Double Bass practical sounding max
        case 19: return 108; // Glockenspiel practical sounding max
        case 20: return 108; // Xylophone practical sounding max
        case 21: return 96;  // Marimba practical sounding max
        case 22: return 89;  // Vibraphone practical sounding max
        case 23: return 77;  // Tubular Bells practical sounding max
        default: return -1;  // Custom/manual fallback
    }
}

juce::String OrchNoteMapperAudioProcessor::getPresetRangeDisplayText() const
{
    const int presetMin = getPresetMinNote();
    const int presetMax = getPresetMaxNote();

    if (presetMin < 0 || presetMax < 0)
        return "Preset Range: Custom";

    return "Preset Range: "
           + getCurrentInstrumentPresetName()
           + " "
           + juce::String (presetMin)
           + " - "
           + juce::String (presetMax);
}

int OrchNoteMapperAudioProcessor::getManualActiveMinNote() const
{
    const int rangeMode = getParameterIntValue ("rangeMode", 0);

    if (rangeMode == 0)
    {
        const int minNote = getParameterIntValue ("minNote", 36);
        const int maxNote = getParameterIntValue ("maxNote", 84);
        return juce::jlimit (0, 127, juce::jmin (minNote, maxNote));
    }

    const int centerNote = getParameterIntValue ("centerNote", 60);
    const int windowSpan = getParameterIntValue ("windowSpan", 24);
    const int halfSpan = windowSpan / 2;

    return juce::jlimit (0, 127, centerNote - halfSpan);
}

int OrchNoteMapperAudioProcessor::getManualActiveMaxNote() const
{
    const int rangeMode = getParameterIntValue ("rangeMode", 0);

    if (rangeMode == 0)
    {
        const int minNote = getParameterIntValue ("minNote", 36);
        const int maxNote = getParameterIntValue ("maxNote", 84);
        return juce::jlimit (0, 127, juce::jmax (minNote, maxNote));
    }

    const int centerNote = getParameterIntValue ("centerNote", 60);
    const int windowSpan = getParameterIntValue ("windowSpan", 24);
    const int halfSpan = windowSpan / 2;

    return juce::jlimit (0, 127, centerNote + halfSpan);
}


bool OrchNoteMapperAudioProcessor::isLowKeyswitchSourceNote (int noteNumber) const
{
    auto* keyswitchModeParam = dynamic_cast<juce::AudioParameterChoice*> (
        parameters.getParameter ("keyswitchMode"));

    auto* lowKsProtectParam = dynamic_cast<juce::AudioParameterChoice*> (
        parameters.getParameter ("lowKsProtect"));

    auto* lowKsSourceMinParam = dynamic_cast<juce::AudioParameterInt*> (
        parameters.getParameter ("lowKsSourceMin"));

    auto* lowKsSourceMaxParam = dynamic_cast<juce::AudioParameterInt*> (
        parameters.getParameter ("lowKsSourceMax"));

    if (keyswitchModeParam == nullptr
        || lowKsProtectParam == nullptr
        || lowKsSourceMinParam == nullptr
        || lowKsSourceMaxParam == nullptr)
        return false;

    const int keyswitchMode = keyswitchModeParam->getIndex();
    const bool lowKsEnabled = lowKsProtectParam->getIndex() == 1;

    // 0 = Off
    // 1 = Protect Only
    // 2 = Map Low to Destination
    // 3 = Map High to Destination
    // 4 = Map Low + High to Destination
    const bool lowMappingMode = keyswitchMode == 2 || keyswitchMode == 4;

    if (! lowMappingMode || ! lowKsEnabled)
        return false;

    const int sourceMin = lowKsSourceMinParam->get();
    const int sourceMax = lowKsSourceMaxParam->get();

    if (sourceMin > sourceMax)
        return false;

    return noteNumber >= sourceMin && noteNumber <= sourceMax;
}

int OrchNoteMapperAudioProcessor::getEffectiveKeyswitchDestinationMin (int fallbackDestinationMin) const
{
    const int presetIndex = getParameterIntValue ("ksDestinationPreset", 0);

    if (presetIndex <= 0)
        return juce::jlimit (0, 127, fallbackDestinationMin);

    const int presetStart = (presetIndex - 1) * 12;

    return juce::jlimit (0, 127, presetStart);
}
int OrchNoteMapperAudioProcessor::mapLowKeyswitchNoteToDestination (int noteNumber) const
{
    auto* lowKsSourceMinParam = dynamic_cast<juce::AudioParameterInt*> (
        parameters.getParameter ("lowKsSourceMin"));

    auto* lowKsDestinationMinParam = dynamic_cast<juce::AudioParameterInt*> (
        parameters.getParameter ("lowKsDestinationMin"));

    if (lowKsSourceMinParam == nullptr || lowKsDestinationMinParam == nullptr)
        return noteNumber;

    const int sourceMin = lowKsSourceMinParam->get();
    const int destinationMin = getEffectiveKeyswitchDestinationMin (lowKsDestinationMinParam->get());

    const int mappedNote = destinationMin + (noteNumber - sourceMin);

    return juce::jlimit (0, 127, mappedNote);
}
int OrchNoteMapperAudioProcessor::getActiveMinNote() const
{
    const int rangeSource = getParameterIntValue ("rangeSource", 0);
    const int rangeMode = getParameterIntValue ("rangeMode", 0);

    if (rangeSource == 1)
    {
        const int presetMin = getPresetMinNote();
        const int presetMax = getPresetMaxNote();

        if (presetMin >= 0 && presetMax >= 0)
        {
            const int safePresetMin = juce::jlimit (0, 127, juce::jmin (presetMin, presetMax));
            const int safePresetMax = juce::jlimit (0, 127, juce::jmax (presetMin, presetMax));

            // Preset + Min/Max:
            // Use the full instrument preset range.
            if (rangeMode == 0)
                return safePresetMin;

            // Preset + Center/Span:
            // Use the manual center/span window as a target register,
            // but clip it inside the full playable preset range.
            const int targetMin = getManualActiveMinNote();
            const int targetMax = getManualActiveMaxNote();

            const int clippedTargetMin = juce::jlimit (safePresetMin, safePresetMax, targetMin);
            const int clippedTargetMax = juce::jlimit (safePresetMin, safePresetMax, targetMax);

            return juce::jmin (clippedTargetMin, clippedTargetMax);
        }
    }

    return getManualActiveMinNote();
}

int OrchNoteMapperAudioProcessor::getActiveMaxNote() const
{
    const int rangeSource = getParameterIntValue ("rangeSource", 0);
    const int rangeMode = getParameterIntValue ("rangeMode", 0);

    if (rangeSource == 1)
    {
        const int presetMin = getPresetMinNote();
        const int presetMax = getPresetMaxNote();

        if (presetMin >= 0 && presetMax >= 0)
        {
            const int safePresetMin = juce::jlimit (0, 127, juce::jmin (presetMin, presetMax));
            const int safePresetMax = juce::jlimit (0, 127, juce::jmax (presetMin, presetMax));

            // Preset + Min/Max:
            // Use the full instrument preset range.
            if (rangeMode == 0)
                return safePresetMax;

            // Preset + Center/Span:
            // Use the manual center/span window as a target register,
            // but clip it inside the full playable preset range.
            const int targetMin = getManualActiveMinNote();
            const int targetMax = getManualActiveMaxNote();

            const int clippedTargetMin = juce::jlimit (safePresetMin, safePresetMax, targetMin);
            const int clippedTargetMax = juce::jlimit (safePresetMin, safePresetMax, targetMax);

            return juce::jmax (clippedTargetMin, clippedTargetMax);
        }
    }

    return getManualActiveMaxNote();
}
int OrchNoteMapperAudioProcessor::clampNoteToActiveRange (int noteNumber) const
{
    return juce::jlimit (getActiveMinNote(), getActiveMaxNote(), noteNumber);
}


bool OrchNoteMapperAudioProcessor::isHighKeyswitchSourceNote (int noteNumber) const
{
    auto* keyswitchModeParam = dynamic_cast<juce::AudioParameterChoice*> (
        parameters.getParameter ("keyswitchMode"));

    auto* highKsProtectParam = dynamic_cast<juce::AudioParameterChoice*> (
        parameters.getParameter ("highKsProtect"));

    auto* highKsSourceMinParam = dynamic_cast<juce::AudioParameterInt*> (
        parameters.getParameter ("highKsSourceMin"));

    auto* highKsSourceMaxParam = dynamic_cast<juce::AudioParameterInt*> (
        parameters.getParameter ("highKsSourceMax"));

    if (keyswitchModeParam == nullptr
        || highKsProtectParam == nullptr
        || highKsSourceMinParam == nullptr
        || highKsSourceMaxParam == nullptr)
        return false;

    const int keyswitchMode = keyswitchModeParam->getIndex();
    const bool highKsEnabled = highKsProtectParam->getIndex() == 1;

    // 0 = Off
    // 1 = Protect Only
    // 2 = Map Low to Destination
    // 3 = Map High to Destination
    // 4 = Map Low + High to Destination
    const bool highMappingMode = keyswitchMode == 3 || keyswitchMode == 4;

    if (! highMappingMode || ! highKsEnabled)
        return false;

    const int sourceMin = highKsSourceMinParam->get();
    const int sourceMax = highKsSourceMaxParam->get();

    if (sourceMin > sourceMax)
        return false;

    return noteNumber >= sourceMin && noteNumber <= sourceMax;
}

int OrchNoteMapperAudioProcessor::mapHighKeyswitchNoteToDestination (int noteNumber) const
{
    auto* highKsSourceMinParam = dynamic_cast<juce::AudioParameterInt*> (
        parameters.getParameter ("highKsSourceMin"));

    auto* highKsDestinationMinParam = dynamic_cast<juce::AudioParameterInt*> (
        parameters.getParameter ("highKsDestinationMin"));

    if (highKsSourceMinParam == nullptr || highKsDestinationMinParam == nullptr)
        return noteNumber;

    const int sourceMin = highKsSourceMinParam->get();
    const int destinationMin = getEffectiveKeyswitchDestinationMin (highKsDestinationMinParam->get());

    const int mappedNote = destinationMin + (noteNumber - sourceMin);

    return juce::jlimit (0, 127, mappedNote);
}
bool OrchNoteMapperAudioProcessor::isProtectedKeyswitchNote (int noteNumber) const
{
    const int keyswitchMode = getParameterIntValue ("keyswitchMode", 0);

    if (keyswitchMode == 0) // Off
        return false;

    const int safeNote = juce::jlimit (0, 127, noteNumber);

    const bool lowProtectEnabled = getParameterIntValue ("lowKsProtect", 0) == 1;
    const int lowMax = getParameterIntValue ("lowKsMax", 35);

    if (lowProtectEnabled && safeNote <= lowMax)
        return true;

    const bool highProtectEnabled = getParameterIntValue ("highKsProtect", 0) == 1;
    const int highMin = getParameterIntValue ("highKsMin", 96);

    if (highProtectEnabled && safeNote >= highMin)
        return true;

    return false;
}
int OrchNoteMapperAudioProcessor::getNoteMapIndex (int midiChannel, int inputNoteNumber)
{
    const int channelIndex = juce::jlimit (1, 16, midiChannel) - 1;
    const int noteIndex = juce::jlimit (0, 127, inputNoteNumber);

    return channelIndex * 128 + noteIndex;
}

template <typename FloatType>
void OrchNoteMapperAudioProcessor::processMidiAndClearAudio (juce::AudioBuffer<FloatType>& buffer,
                                                             juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    juce::MidiBuffer processedMidi;

    for (const auto metadata : midiMessages)
    {
        const auto message = metadata.getMessage();
        const int samplePosition = metadata.samplePosition;

        if (message.isNoteOn())
        {
            const int channel = message.getChannel();
            const int inputNote = message.getNoteNumber();
            const int activeMin = getActiveMinNote();
            const int activeMax = getActiveMaxNote();

            int outputNote = inputNote;
            int midiRoleForDebug = 0; // 0 = performance, 1 = protected keyswitch, 2 = mapped keyswitch

            if (isLowKeyswitchSourceNote (inputNote))
            {
                outputNote = mapLowKeyswitchNoteToDestination (inputNote);
                midiRoleForDebug = 2;
            }
            else if (isHighKeyswitchSourceNote (inputNote))
            {
                outputNote = mapHighKeyswitchNoteToDestination (inputNote);
                midiRoleForDebug = 2;
            }
            else if (isProtectedKeyswitchNote (inputNote))
            {
                outputNote = inputNote;
                midiRoleForDebug = 1;
            }
            else
            {
                const int mappingMode = getParameterIntValue ("mappingMode", 0);
                const int centerNote = getParameterIntValue ("centerNote", 60);

                if (mappingMode == 1) // Octave Fold
                {
                    int bestNote = juce::jlimit (activeMin, activeMax, inputNote);
                    int bestDistance = 9999;

                    for (int candidate = inputNote - 120; candidate <= inputNote + 120; candidate += 12)
                    {
                        if (candidate >= activeMin && candidate <= activeMax)
                        {
                            const int distance = std::abs (candidate - centerNote);

                            if (distance < bestDistance)
                            {
                                bestDistance = distance;
                                bestNote = candidate;
                            }
                        }
                    }

                    outputNote = juce::jlimit (activeMin, activeMax, bestNote);
                }
                else // Clamp
                {
                    outputNote = juce::jlimit (activeMin, activeMax, inputNote);
                }

                midiRoleForDebug = 0;
            }
            if (midiRoleForDebug == 0)
            {
                lastPerformanceInputNoteForDebug.store (inputNote);
                lastPerformanceOutputNoteForDebug.store (outputNote);
                lastPerformanceActiveMinForDebug.store (activeMin);
                lastPerformanceActiveMaxForDebug.store (activeMax);
            }
            else
            {
                lastKeyswitchInputNoteForDebug.store (inputNote);
                lastKeyswitchOutputNoteForDebug.store (outputNote);
                lastKeyswitchRoleForDebug.store (midiRoleForDebug);
            }

            activeNoteMap[(size_t) getNoteMapIndex (channel, inputNote)] = outputNote;

            auto mappedMessage = juce::MidiMessage::noteOn (channel,
                                                            outputNote,
                                                            message.getVelocity());
            processedMidi.addEvent (mappedMessage, samplePosition);
        }
        else if (message.isNoteOff())
        {
            const int channel = message.getChannel();
            const int inputNote = message.getNoteNumber();
            const int mapIndex = getNoteMapIndex (channel, inputNote);
            const int rememberedOutputNote = activeNoteMap[(size_t) mapIndex];

            int outputNote = rememberedOutputNote;

            if (outputNote < 0)
            {
                if (isLowKeyswitchSourceNote (inputNote))
                    outputNote = mapLowKeyswitchNoteToDestination (inputNote);
                else if (isHighKeyswitchSourceNote (inputNote))
                    outputNote = mapHighKeyswitchNoteToDestination (inputNote);
                else if (isProtectedKeyswitchNote (inputNote))
                    outputNote = inputNote;
                else
                    outputNote = clampNoteToActiveRange (inputNote);
            }
activeNoteMap[(size_t) mapIndex] = -1;

            auto mappedMessage = juce::MidiMessage::noteOff (channel,
                                                             outputNote,
                                                             message.getVelocity());
            processedMidi.addEvent (mappedMessage, samplePosition);
        }
        else if (message.isAllNotesOff() || message.isAllSoundOff())
        {
            activeNoteMap.fill (-1);
            processedMidi.addEvent (message, samplePosition);
        }
        else if (message.isController())
        {
            const int blockMode = getParameterIntValue ("blockControlCcs", 0);

            if (blockMode > 0)
            {
                const int hi = blockMode == 1 ? 54 : 64;
                const int cc = message.getControllerNumber();

                if (cc >= 20 && cc <= hi)
                    continue; // scrubbed before the instrument
            }

            processedMidi.addEvent (message, samplePosition);
        }
        else
        {
            processedMidi.addEvent (message, samplePosition);
        }
    }

    midiMessages.swapWith (processedMidi);
}

void OrchNoteMapperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    processMidiAndClearAudio (buffer, midiMessages);
}

void OrchNoteMapperAudioProcessor::processBlock (juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages)
{
    processMidiAndClearAudio (buffer, midiMessages);
}

bool OrchNoteMapperAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* OrchNoteMapperAudioProcessor::createEditor()
{
    return new OrchNoteMapperAudioProcessorEditor (*this);
}

void OrchNoteMapperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());

    if (xml != nullptr)
        copyXmlToBinary (*xml, destData);
}

void OrchNoteMapperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        auto newState = juce::ValueTree::fromXml (*xmlState);

        if (newState.isValid())
            parameters.replaceState (newState);
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OrchNoteMapperAudioProcessor();
}
























