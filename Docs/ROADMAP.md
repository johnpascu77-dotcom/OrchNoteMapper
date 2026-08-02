# OrchNoteMapper Roadmap

## Project summary

OrchNoteMapper is a JUCE/CMake VST3 MIDI note effect for orchestral and instrument-range-aware composition workflows, especially in Bitwig.

The core idea is:

- Accept incoming MIDI notes.
- Define an active target note range.
- Remap incoming notes into that range.
- Support practical orchestral instrument ranges.
- Expose important controls as Bitwig-visible parameters.
- Keep user-facing pitch display as raw MIDI note numbers to avoid DAW/octave naming convention mismatches.

Current plugin format:

- VST3
- MIDI effect / Note FX
- Built with JUCE and CMake
- Tested in Bitwig

Current development path:

- Build small validated phases.
- Test each phase in Bitwig.
- Commit each working phase before moving on.

---

## Current actual status

Latest confirmed functional checkpoint:

- Phase 7D-4 — Repository cleanup, build/install workflow documented, roadmap updated, and visible UI build label verified in host

Confirmed working:

- Build succeeds.
- Bitwig loads the plugin as Note FX.
- Plugin UI opens.
- Bitwig sees exposed parameters.
- Parameters can be modulated by Bitwig.
- Range Source system works.
- Instrument Preset system works.
- Center/Span inside preset ranges works.
- Clamp mode works.
- Octave Fold mode works.
- Keyswitch protection works.
- Keyswitch destination mapping works.
- Generic KS destination presets work.
- Randomized playing notes can be mapped into instrument range.
- Randomized keyswitches can be mapped into a stable destination KS range.
- Violin test with randomized note plus randomized KS produced musical articulation transitions.
- Two duplicated violin lanes were tested with independent Bitwig Random modulators assigned to Center Note at different bar rates, creating independent register travel while preserving related musical material.
- Independent articulations and CC1/CC11 per lane produced musically useful divisi-like behavior.

Current preset order:

1. Custom
2. Piccolo
3. Flute
4. Oboe
5. English Horn
6. Clarinet
7. Bass Clarinet
8. Bassoon
9. Contrabassoon
10. French Horn
11. Trumpet
12. Trombone
13. Bass Trombone
14. Tuba
15. Timpani
16. Violin
17. Viola
18. Cello
19. Double Bass
20. Glockenspiel
21. Xylophone
22. Marimba
23. Vibraphone
24. Tubular Bells

Current visible subtitle:

```text
Orchestral Range + Keyswitch Mapper
```

Preset compatibility policy:

- During pre-release development, preset order may be reorganized for musical clarity.
- After the first stable public release, preset indices should be treated as compatibility-sensitive saved-project data.

Current known remaining limitations:

- Keyswitch mapping is numeric/generic, not library-profile-specific.
- Articulation slot mapping is not implemented yet.
- Named articulation maps are not implemented yet.
- Scale/key intelligence is not implemented yet.
- Collision/voice policy is not implemented yet.
- Orchestration participation/gating is not implemented yet.
- OrchGate / OrchConductor are future companion-plugin ideas, documented separately in `Docs/OrchGate_OrchConductor_Roadmap.md`.

---

## Historical note

Some sections below were written before Phase 7A–7C were implemented.

The authoritative current state is the **Current actual status** section near the top of this document.

---

## Current confirmed status

Latest confirmed phase:

- Phase 6B — Normalized presets to practical sounding MIDI-number ranges

Confirmed working:

- Build succeeds.
- Bitwig loads the plugin as Note FX.
- Plugin UI opens.
- Bitwig sees exposed parameters.
- Parameters can be modulated by Bitwig.
- Plugin UI reflects effective parameter values.
- Active Target Range display updates.
- MIDI note remapping works.
- Clamp mode works.
- Octave Fold mode works.
- Range Source system works.
- Instrument Preset system works.
- Center/Span target window can be clipped inside preset instrument ranges.
- Note-off mapping prevents stuck notes when range/mode parameters change while notes are held.
- MIDI debug display shows recent input/output mapping information.

Current parameters:

- Range Source
  - Manual
  - Preset

- Range Mode
  - Min/Max
  - Center/Span

- Mapping Mode
  - Clamp
  - Octave Fold

- Instrument Preset
  - Custom
  - Violin
  - Viola
  - Cello
  - Double Bass
  - Flute
  - Oboe
  - Clarinet
  - Bassoon
  - French Horn
  - Trumpet
  - Trombone
  - Tuba
  - Piccolo
  - English Horn
  - Contrabassoon
  - Bass Trombone
  - Timpani
  - Xylophone
  - Marimba
  - Glockenspiel
  - Vibraphone
  - Tubular Bells

- Min Note
  - 0 to 127
  - default 36

- Max Note
  - 0 to 127
  - default 84

- Center Note
  - 0 to 127
  - default 60

- Window Span
  - 0 to 127
  - default 24

Current UI shows:

- Plugin title
- Current phase/status
- Range Source
- Range Mode
- Mapping Mode
- Instrument Preset
- Instrument Range
- Manual Min Note
- Manual Max Note
- Target Center
- Target Span
- Active Target Range
- Recent MIDI mapping/debug display

Example debug display:

```text
Last MIDI: In 36 -> Out 72 Target 69 - 81 Mode Octave Fold Center 75
```

---

## Core design policies

### User-facing pitch display is numeric-only

The plugin intentionally displays raw MIDI note numbers instead of note names.

Reason:

- Avoids C3/C4 octave convention mismatches between hosts and plugins.
- Bitwig uses C3 = 60 as its only option.
- MIDI numbers are unambiguous.
- The plugin is a MIDI processor, so raw note numbers are the safest reference.

### Preset ranges use practical sounding MIDI numbers

Instrument presets represent practical sounding-pitch MIDI ranges.

This means:

- Transposing instruments are stored by their sounding MIDI result.
- The plugin does not use written staff pitch for preset limits.
- Presets are intended as practical orchestral defaults, not theoretical extremes.
- Future variants may add extended or library-specific ranges.

Examples:

- Piccolo preset uses sounding range.
- Double Bass preset uses sounding range.
- Contrabassoon preset uses sounding range.
- Xylophone and Glockenspiel presets use sounding range.

### Preset list is append-only

Instrument preset indices should be treated as saved-project data.

Therefore:

- Do not reorder existing preset entries.
- Do not remove existing preset entries.
- Add new presets only at the end of the list.
- Keep ComboBox IDs and processor switch indices aligned.

Reason:

- Avoid breaking existing Bitwig projects that have saved parameter values.

---

## Current architecture notes

### Important files

- `CMakeLists.txt`
- `Source/PluginProcessor.h`
- `Source/PluginProcessor.cpp`
- `Source/PluginEditor.h`
- `Source/PluginEditor.cpp`
- `Docs/DEVLOG.md`
- `Docs/ROADMAP.md`

### Core processor responsibilities

`PluginProcessor` currently handles:

- Parameter definition via `createParameterLayout()`
- Range Source selection
- Instrument preset selection
- Preset min/max lookup
- Active target range computation
- MIDI note mapping
- Clamp mapping
- Octave Fold mapping
- Note-on/note-off tracking
- Recent MIDI debug state
- State save/restore

### Core editor responsibilities

`PluginEditor` currently handles:

- Simple display UI
- Parameter display refresh
- Range Source display
- Range Mode display
- Mapping Mode display
- Instrument Preset display
- Instrument Range display
- Manual range display
- Target Center/Span display
- Active Target Range display
- Recent MIDI debug display

---

## Current active range rules

### Range Source: Manual

Manual source uses user-controlled parameters directly.

#### Range Mode: Min/Max

- Active min is the lower of Min Note and Max Note.
- Active max is the higher of Min Note and Max Note.

This means inverted values are safe.

Example:

- Min Note: 80
- Max Note: 60

becomes:

- Active Target Range: 60 - 80

#### Range Mode: Center/Span

Formula:

- `halfSpan = windowSpan / 2`
- `activeMin = centerNote - halfSpan`
- `activeMax = centerNote + halfSpan`

Both are limited to MIDI range 0 to 127.

Example:

- Center Note: 60
- Window Span: 24

becomes:

- Active Target Range: 48 - 72

### Range Source: Preset

Preset source uses the selected instrument preset as the safe instrument range.

#### Range Mode: Min/Max

The active target range is the preset instrument range.

Example:

- Instrument Preset: English Horn
- Preset range: 52 - 81

becomes:

- Active Target Range: 52 - 81

#### Range Mode: Center/Span

Center/Span defines a preferred target window inside the preset instrument range.

The target window is clipped to the preset range.

Example:

- Instrument Preset: English Horn
- Preset range: 52 - 81
- Target Center: 75
- Target Span: 13

raw target window:

- 69 - 81

clipped active target range:

- 69 - 81

Example:

- Instrument Preset: English Horn
- Preset range: 52 - 81
- Target Center: 90
- Target Span: 24

raw target window:

- 78 - 102

clipped active target range:

- 78 - 81

---

## Current mapping modes

### Clamp

Behavior:

- Notes below the active target range map to active minimum.
- Notes above the active target range map to active maximum.
- Notes inside the active target range pass unchanged.
- Other MIDI messages pass through unchanged.

Example with active target range 60 to 71:

- 48 -> 60
- 55 -> 60
- 60 -> 60
- 71 -> 71
- 72 -> 71
- 84 -> 71

### Octave Fold

Behavior:

- Move notes by octaves until they fit inside the active target range.
- If the range is too narrow or folding still fails, fall back to Clamp.

Algorithm:

```cpp
while (note < activeMin)
    note += 12;

while (note > activeMax)
    note -= 12;

if still outside:
    fallback to clamp
```

Example with active target range 60 to 71:

- 48 -> 60
- 49 -> 61
- 50 -> 62
- 55 -> 67
- 59 -> 71
- 60 -> 60
- 71 -> 71
- 72 -> 60
- 73 -> 61
- 84 -> 60

---

## Current preset ranges

All values are practical sounding MIDI note numbers.

Do not change these to note-name-based logic.

| Preset | MIDI Range |
|---|---:|
| Custom | manual |
| Violin | 55 - 105 |
| Viola | 48 - 88 |
| Cello | 36 - 81 |
| Double Bass | 28 - 67 |
| Flute | 60 - 98 |
| Oboe | 58 - 93 |
| Clarinet | 50 - 96 |
| Bassoon | 34 - 76 |
| French Horn | 35 - 77 |
| Trumpet | 54 - 86 |
| Trombone | 40 - 72 |
| Tuba | 28 - 65 |
| Piccolo | 74 - 108 |
| English Horn | 52 - 81 |
| Contrabassoon | 22 - 58 |
| Bass Trombone | 34 - 70 |
| Timpani | 38 - 60 |
| Xylophone | 65 - 108 |
| Marimba | 36 - 96 |
| Glockenspiel | 79 - 108 |
| Vibraphone | 53 - 89 |
| Tubular Bells | 60 - 77 |

---

## Implemented phases

### Phase 1 — MIDI passthrough skeleton

Goal:

- Create the basic JUCE/CMake VST3 plugin.
- Build with Visual Studio.
- Load in Bitwig.
- Pass MIDI through unchanged.

Status:

- Confirmed working.

### Phase 2 — Bitwig-visible range parameters

Goal:

- Add automatable range parameters using `juce::AudioProcessorValueTreeState`.
- Display parameter values in the plugin UI.
- Keep MIDI passthrough unchanged.

Status:

- Confirmed working.

### Phase 3 — Clamp notes into active range

Goal:

- Implement first real MIDI transformation.
- Clamp note-ons into the active range.
- Use remembered output notes for note-offs to prevent stuck notes.

Status:

- Confirmed working.

Important implementation detail:

- Active-note map:
  - key: MIDI channel + input note
  - value: mapped output note

Reason:

- Prevents stuck notes if parameters change while a note is held.

### Phase 4 — Mapping Mode and octave-preserving fold

Goal:

- Add Mapping Mode parameter.
- Preserve Clamp behavior.
- Add Octave Fold behavior.

Status:

- Confirmed working.

### Phase 5A — Instrument preset foundation

Goal:

- Add instrument preset selection.
- Add preset min/max lookup.
- Keep preset list append-only.

Status:

- Confirmed working.

### Phase 5B — Range Source system

Goal:

- Separate manual range behavior from preset range behavior.

Status:

- Confirmed working.

### Phase 5C — Preset range display

Goal:

- Make the selected instrument range visible in the UI.

Status:

- Confirmed working.

### Phase 5D — Center/Span target window inside preset ranges

Goal:

- Allow Center/Span to define a target window clipped inside the preset instrument range.

Status:

- Confirmed working.

Example:

- English Horn preset: 52 - 81
- Center: 75
- Span: 13
- Active Target Range: 69 - 81

### Phase 5E — Melodic percussion presets

Goal:

- Add melodic percussion presets append-only.

Added presets:

- Timpani
- Xylophone
- Marimba
- Glockenspiel
- Vibraphone
- Tubular Bells

Status:

- Confirmed working.

### Phase 5F — Clarified UI range labels

Goal:

- Clarify UI language around preset range vs active target range.

Status:

- Confirmed working.

Important labels:

- Instrument Range
- Manual Min Note
- Manual Max Note
- Target Center
- Target Span
- Active Target Range

### Phase 5G — Rich MIDI debug display

Goal:

- Add clearer recent MIDI mapping display.

Status:

- Confirmed working.

Example:

```text
Last MIDI: In 36 -> Out 72 Target 69 - 81 Mode Octave Fold Center 75
```

### Phase 6A — Practical preset range audit

Goal:

- Define policy for instrument preset ranges.

Policy:

- Presets use practical sounding MIDI note numbers.
- UI remains numeric-only.
- Avoid note-name/octave naming dependence.

Status:

- Confirmed.

### Phase 6B — Normalize presets to practical sounding ranges

Goal:

- Normalize all preset ranges against the practical sounding MIDI-number policy.

Status:

- Confirmed and committed.

---

## Current known limitations

### UI is mostly display-focused

The plugin UI displays the current effective state and debug information.

Most parameter editing is still expected through the DAW/Bitwig device panel.

This is acceptable for now because Bitwig modulation and automation are central to the workflow.

### No note names

The UI intentionally shows raw MIDI note numbers only.

Reason:

- Avoids C3/C4 octave convention mismatches between hosts and plugins.
- Bitwig uses C3 = 60.
- MIDI note numbers are unambiguous.

### No protected keyswitch zones yet

The plugin currently remaps all note-on/note-off messages.

This can be problematic for orchestral libraries that use notes outside the playable range for:

- keyswitches
- articulation selection
- phrase triggers
- special effects
- ornament triggers

This should be the next major functional improvement.

### No scale/key intelligence yet

The plugin does not currently know about:

- Key
- Scale
- Diatonic notes
- Chord tones
- Avoid notes

It only maps chromatic MIDI note numbers.

### No collision policy yet

Different input notes can map to the same output note.

This is expected in Clamp mode and can also occur in narrow ranges.

No special voice allocation or collision handling exists yet.

### No handling for overlapping same input notes

The current active note map stores only one mapped output note for each channel/input-note pair.

This is fine for normal MIDI keyboard behavior.

Potential future issue:

- Repeated note-ons for the same channel/input note before note-off could overwrite the previous mapping.

Possible future solution:

- Use a small stack/vector per channel/input-note, or note-count tracking.

### No pitch-bend range awareness

Pitch bend messages pass through unchanged.

The plugin does not account for instrument range after pitch bend.

### No MPE-specific behavior

Channelized note mapping probably works at a basic level because the active note map includes MIDI channel.

But no explicit MPE features are implemented.

---

## Build and install notes

Project path:

```text
C:\AudioDev\OrchNoteMapper
```

Build commands:

```powershell
Set-Location "C:\AudioDev\OrchNoteMapper"

if (Test-Path ".\build") {
    Remove-Item ".\build" -Recurse -Force
}

cmake -B build -S . -G "Visual Studio 18 2026" -A x64
cmake --build build --config Release
```

VST3 output path:

```text
C:\AudioDev\OrchNoteMapper\build\OrchNoteMapper_artefacts\Release\VST3\OrchNoteMapper.vst3
```

Install/copy command:

```powershell
Copy-Item -Path "C:\AudioDev\OrchNoteMapper\build\OrchNoteMapper_artefacts\Release\VST3\OrchNoteMapper.vst3" -Destination "C:\Program Files\Common Files\VST3" -Recurse -Force
```

If copy fails:

- Close Bitwig.
- Use Administrator PowerShell.

---

## Recommended next phases

### Phase 6C — Update roadmap to current state

Goal:

- Replace outdated Phase 4 roadmap content with current Phase 6B status.

Status:

- In progress.

### Phase 6D — Remove octave-name comments from preset range code

Goal:

- Remove note-name comments from preset min/max switch blocks.
- Keep code comments numeric/policy-based.

Reason:

- Avoid C3/C4 and octave naming ambiguity even in developer-facing range comments.

Behavior change:

- None.

### Phase 7A — Keyswitch classification and protect-only mode

Goal:

- Introduce a clear distinction between performance notes and keyswitch/control notes.
- Prevent articulation trigger notes from being accidentally range-mapped.
- Establish the foundation for later keyswitch remapping.

Design policy:

- Performance notes and keyswitch/control notes are different roles.
- Performance notes may be range-mapped.
- Keyswitch/control notes may be protected or translated.
- Translated/protected keyswitch notes must not be fed into the performance range mapper.

Proposed parameters:

- Keyswitch Mode
  - Off
  - Protect Only

- Low KS Protect
  - Off
  - On

- Low KS Max
  - 0 to 127
  - default 35

- High KS Protect
  - Off
  - On

- High KS Min
  - 0 to 127
  - default 96

Proposed behavior:

```cpp
if (keyswitchMode == ProtectOnly && isInProtectedKeyswitchZone(inputNote))
{
    output note unchanged;
}
else
{
    process as performance note;
}
```

Important note-off behavior:

- Protected note-ons should have protected note-offs passed unchanged.
- Remapped note-ons should still use the active-note map for safe note-offs.
- The existing input-note to output-note tracking can be used for both protected and remapped notes.

### Phase 7B — Unified keyswitch octave remapping

Goal:

- Allow one personal/unified keyswitch octave to control different libraries and sections.
- Replace manual per-track transpose chains.

Proposed Keyswitch Mode values:

- Off
- Protect Only
- Map Octave

Proposed parameters:

- Unified KS Base
  - 0 to 127
  - default 12

- Unified KS Count
  - 1 to 24
  - default 12

- Destination KS Base
  - 0 to 127
  - default 12

Proposed behavior:

```cpp
if (keyswitchMode == MapOctave &&
    inputNote >= unifiedKsBase &&
    inputNote < unifiedKsBase + unifiedKsCount)
{
    outputNote = destinationKsBase + (inputNote - unifiedKsBase);
}
else
{
    process as performance note;
}
```

Important rule:

- Mapped keyswitch notes must be output immediately.
- They must not be sent through the performance range mapper afterward.

Example:

```text
Unified KS Base: 12
Unified KS Count: 12
Destination KS Base: 48

12 -> 48
13 -> 49
14 -> 50
15 -> 51
...
23 -> 59
```

### Phase 7C — Generic keyswitch destination presets

Goal:

- Add quick numeric destination layouts for common chromatic keyswitch octaves.

Possible presets:

- Custom
- Destination 0 - 11
- Destination 12 - 23
- Destination 24 - 35
- Destination 36 - 47
- Destination 48 - 59
- Destination 60 - 71
- Destination 72 - 83
- Destination 84 - 95
- Destination 96 - 107

Reason:

- Stay library-agnostic first.
- Avoid hardcoding potentially inaccurate library assumptions too early.

### Phase 7D — Articulation-slot mapping

Goal:

- Move beyond simple octave-to-octave translation.
- Support libraries where articulation order differs.

Concept:

- Define a personal unified articulation slot layout.
- Map each unified slot to a library-specific destination note.

Example personal slot layout:

```text
Slot 0 = Sustain
Slot 1 = Legato
Slot 2 = Staccato
Slot 3 = Spiccato
Slot 4 = Pizzicato
Slot 5 = Tremolo
Slot 6 = Marcato
Slot 7 = Sordino
```

Example mapping:

```text
Unified Slot 0 -> Library note 52
Unified Slot 1 -> Library note 53
Unified Slot 2 -> Library note 48
Unified Slot 3 -> Library note 50
Unified Slot 4 -> Library note 49
Unified Slot 5 -> Library note 57
```

This is the long-term solution for normalizing different library articulation layouts.

---

## Future phase ideas

### Better plugin UI controls

Add actual plugin UI controls:

- Combo box for Range Source
- Combo box for Range Mode
- Combo box for Mapping Mode
- Combo box for Instrument Preset
- Sliders for Min Note
- Sliders for Max Note
- Sliders for Center Note
- Sliders for Window Span
- Protected zone controls

Recommended only after the MIDI logic is stable.

### Smarter folding variants

Additional mapping modes:

- Clamp
- Octave Fold
- Range Modulo
- Nearest In Range
- Mirror Fold / Bounce
- Random In Range
- Scale-aware Fold

### Scale/key awareness

Add parameters:

- Key
- Scale
- Quantize To Scale
- Prefer Chord Tones

Possible scales:

- Major
- Natural Minor
- Harmonic Minor
- Melodic Minor
- Dorian
- Phrygian
- Lydian
- Mixolydian
- Locrian
- Chromatic

### Multi-range support

Some instruments or libraries may have split ranges:

- playable range
- keyswitch range
- ornament range
- special FX range

Future design could support:

- Protected zones
- Multiple allowed target zones
- Excluded zones

### Collision and voice handling

Address cases where multiple input notes map to one output note.

Possible policies:

- Allow collisions
- Drop duplicate mapped notes
- Spread to nearby notes
- Voice allocation within range
- Preserve intervals where possible

---

## Next-session checklist

At the beginning of the next session:

1. Open this file:
   - `Docs/ROADMAP.md`

2. Open latest devlog:
   - `Docs/DEVLOG.md`

3. Confirm latest commit:
   - `git log --oneline -5`

4. Confirm current build still works:
   - Build Release
   - Copy VST3
   - Load in Bitwig

5. Start from:
   - Phase 6D — Remove octave-name comments from preset range code
   - or Phase 7A — Protected low keyswitch zone

Suggested next prompt:

```text
We are continuing OrchNoteMapper.
Phase 6B is committed and confirmed working.
The roadmap has been updated to current state.
Next goal: Phase 6D comment cleanup or Phase 7A protected low keyswitch zone.
Please help implement the next phase safely.
```

---

## Current safe stopping point

This is a clean checkpoint after Phase 6B.

Before starting major new behavior:

- Confirm Phase 6B commit exists.
- Update `Docs/ROADMAP.md`.
- Update `Docs/DEVLOG.md` if needed.
- Commit documentation updates.


