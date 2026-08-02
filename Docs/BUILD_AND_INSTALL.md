# Build and Install OrchNoteMapper

This document describes the safe build/install workflow for OrchNoteMapper.

OrchNoteMapper is a JUCE/CMake VST3 MIDI effect. During development, hosts may keep old plugin binaries loaded in memory, so a successful rebuild does not always mean the host is currently using the newest binary.

---

## 1. Build Release

From the repository root:

```powershell
Set-Location "C:\AudioDev\OrchNoteMapper"

cmake --build build --config Release
```

For a fully clean rebuild:

```powershell
Set-Location "C:\AudioDev\OrchNoteMapper"

cmake --build build --config Release --clean-first
```

The built VST3 is expected at:

```text
C:\AudioDev\OrchNoteMapper\build\OrchNoteMapper_artefacts\Release\VST3\OrchNoteMapper.vst3
```

---

## 2. Copy VST3 to the system VST3 folder

Common destination:

```text
C:\Program Files\Common Files\VST3
```

Copy command:

```powershell
Copy-Item -Path "C:\AudioDev\OrchNoteMapper\build\OrchNoteMapper_artefacts\Release\VST3\OrchNoteMapper.vst3" -Destination "C:\Program Files\Common Files\VST3" -Recurse -Force
```

If permission is denied, run PowerShell as Administrator.

---

## 3. Verify timestamps

Check the built VST3:

```powershell
Get-ChildItem "C:\AudioDev\OrchNoteMapper\build\OrchNoteMapper_artefacts\Release\VST3" -Filter "OrchNoteMapper.vst3" |
    Select-Object FullName, LastWriteTime
```

Check the installed VST3:

```powershell
Get-ChildItem "C:\Program Files\Common Files\VST3" -Filter "OrchNoteMapper.vst3" |
    Select-Object FullName, LastWriteTime
```

The installed VST3 should have a matching or newer timestamp.

---

## 4. Fully reload the host

Many DAWs and plugin hosts keep VST3 binaries loaded in memory.

Safe reload sequence:

1. Close the plugin editor window.
2. Remove the old OrchNoteMapper instance from the track/device chain.
3. Fully close the host/DAW.
4. Reopen the host/DAW.
5. Rescan plugins if needed.
6. Insert a fresh instance of OrchNoteMapper.

Do not rely only on closing the plugin window. The old binary may remain loaded until the host process exits.

---

## 5. Verify correct UI

The current expected UI subtitle is:

```text
Orchestral Range + Keyswitch Mapper
```

If the UI shows an old subtitle such as:

```text
Phase 4: Clamp or octave-fold into active range
```

then the host is loading an old binary or cached plugin instance.

---

## 6. Find duplicate installed copies

If the host still loads an old version, search for duplicate VST3 copies:

```powershell
Get-ChildItem "C:\" -Recurse -Filter "OrchNoteMapper.vst3" -ErrorAction SilentlyContinue |
    Select-Object FullName, LastWriteTime
```

Check whether the host is scanning a different location.

Common VST3 locations include:

```text
C:\Program Files\Common Files\VST3
C:\Program Files\Common Files\Steinberg\VST3
C:\Users\<username>\AppData\Local\Programs\Common\VST3
C:\Users\<username>\Documents\VST3
```

---

## 7. Git status after build

The `build/` folder is intentionally ignored by Git.

After building, this should remain clean:

```powershell
git status --short
```

Expected output:

```text
```

No output means the repository is clean.
