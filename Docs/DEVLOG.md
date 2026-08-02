# OrchNoteMapper DEVLOG

## Phase 1 — MIDI passthrough skeleton

Goal:

- Create a JUCE/CMake VST3 MIDI effect.
- Build with Visual Studio 2026.
- Load in Bitwig.
- Pass MIDI through unchanged.

Status:

- Working.

Tested:

- Built successfully with CMake and Visual Studio 2026.
- VST3 copied to C:\Program Files\Common Files\VST3.
- Bitwig loads the plugin as Note FX.
- Plugin UI opens.
- MIDI passes through unchanged.

Notes:

- No mapping logic yet.
- No parameters yet.
- This phase only validates project/build/host plumbing.
- JUCE 9 required explicit JUCE_VST3_CAN_REPLACE_VST2=0 for the VST3 wrapper target.

## Phase 2 — Bitwig-visible range parameters

Goal:

- Add automatable range parameters using AudioProcessorValueTreeState.
- Display parameter values in the plugin UI.
- Keep MIDI passthrough unchanged.

Parameters:

- Range Mode: Min/Max or Center/Span
- Min Note: 0 to 127, default 36
- Max Note: 0 to 127, default 84
- Center Note: 0 to 127, default 60
- Window Span: 0 to 127, default 24

Status:

- Working.

Tested:

- Bitwig loads the plugin.
- Plugin UI opens.
- Plugin displays clean MIDI note numbers only.
- Bitwig can see the exposed parameters.
- UI updates when Bitwig/plugin parameters change.
- MIDI passes through unchanged.

Notes:

- MIDI note numbers are used directly.
- No note-name or octave-name display is used, avoiding C3/C4 convention mismatch.
- Mapping logic is not implemented yet.

## Phase 3 — Clamp notes into active range

Goal:

- Implement first real MIDI transformation.
- Clamp incoming note-ons into the active note range.
- Use remembered note mappings for note-offs to prevent stuck notes.
- Display computed Active Range in the UI.

Behavior:

- In Min/Max mode, Active Range uses Min Note and Max Note.
- In Center/Span mode, Active Range is computed from Center Note and Window Span.
- Notes below the active range are mapped to the active minimum.
- Notes above the active range are mapped to the active maximum.
- Notes inside the active range pass unchanged.
- Other MIDI messages pass through unchanged.

Status:

- Working.

Tested:

- Bitwig loads the plugin.
- Plugin UI opens.
- Active Range display updates.
- Bitwig modulation reaches the plugin parameters.
- Min/Max clamp works.
- Center/Span clamp works.
- Note-offs follow remembered note-on mappings.
- No stuck notes observed when changing range while notes are held.

Notes:

- This is a clamp-only mapper.
- No octave folding or scale logic yet.
- MIDI note numbers remain displayed as clean digits only.
- Bitwig may display base parameter values while the plugin UI displays effective modulated values.

## Phase 4 — Mapping Mode and octave-preserving fold

Goal:

- Add Mapping Mode parameter.
- Keep Clamp mode from Phase 3.
- Add Octave Fold mode.
- Preserve pitch class by shifting notes in octaves until they fit inside the active range.

Parameters added:

- Mapping Mode: Clamp or Octave Fold

Behavior:

- Clamp mode maps notes below the range to the active minimum and notes above the range to the active maximum.
- Octave Fold mode moves notes up or down by octaves until they fit inside the active range.
- If octave folding cannot land inside a very narrow active range, the plugin falls back to clamp.
- Note-offs continue to use remembered note-on mappings to prevent stuck notes.
- Other MIDI messages pass through unchanged.

Status:

- Working.

Tested:

- Bitwig loads the plugin.
- Plugin UI opens.
- Bitwig sees Mapping Mode.
- Clamp mode works.
- Octave Fold mode works.
- Active Range display updates.
- No stuck notes observed when changing parameters or mapping mode while notes are held.

Notes:

- Octave Fold is Option A: octave-preserving fold.
- MIDI note numbers remain displayed as clean digits only.
