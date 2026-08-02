
## Phase 7D-9B Stabilization Note - PowerShell Patch Workflow

Status: Confirmed working.

The first-open UI preset restore issue in OrchNoteMapper was resolved in Phase 7D-9B by adding a real-time editor synchronization function:

    void syncComboBoxesFromParameters();

The editor now syncs ComboBox selections from the actual juce::AudioParameterChoice indices:

    choiceParameter->getIndex() + 1

The sync is called after the timer starts:

    startTimerHz (20);
    syncComboBoxesFromParameters();
    timerCallback();

The sync is also called at the start of timerCallback().

This corrected the situation where Bitwig restored the processor/APVTS state correctly, but the first editor opening displayed stale/default ComboBox values.

### Important workflow lesson

Do not paste complex multi-line PowerShell scripts containing if / elseif / else blocks directly into the terminal line-by-line.

This caused errors such as:

    elseif : The term 'elseif' is not recognized...
    else : The term 'else' is not recognized...

Those errors were not harmless. They meant the script was being executed in fragments, causing partial or misleading patches where the build label changed but the actual C++ logic did not apply.

### Correct patch workflow

Use a two-step file-based PowerShell workflow:

1. Create a patch script file.

2. Run the script file with:

    powershell -ExecutionPolicy Bypass -File ".\patch_name.ps1"

Then verify the actual source code with Select-String before building.

This file-based workflow is now the preferred patching method for all future OrchNoteMapper / OrchGate source modifications.

### Current confirmed OrchNoteMapper checkpoint

    Build: Phase 7D-9B
    Result: First-open ComboBox restore fixed in Bitwig.

