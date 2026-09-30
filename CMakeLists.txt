cmake_minimum_required(VERSION 3.22)

project(OrchNoteMapper VERSION 0.1.0)

# Build-timestamp label (see cmake/GenerateOrchNoteMapperBuildInfo.cmake): a
# plain custom target with no OUTPUT/DEPENDS tracking, so it reruns and
# rewrites the header on every single build, not just every configure or only
# when a source file happens to change.
set(OrchNoteMapperGeneratedDir "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${OrchNoteMapperGeneratedDir}")

add_custom_target(OrchNoteMapperGenerateBuildInfo ALL
    COMMAND ${CMAKE_COMMAND}
        "-DOUTPUT_FILE=${OrchNoteMapperGeneratedDir}/OrchNoteMapperBuildInfo.h"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/GenerateOrchNoteMapperBuildInfo.cmake"
    BYPRODUCTS "${OrchNoteMapperGeneratedDir}/OrchNoteMapperBuildInfo.h"
    COMMENT "Stamping OrchNoteMapper build timestamp"
)

# Path to your JUCE folder.
add_subdirectory("C:/JUCE/JUCE" JUCE)

juce_add_plugin(OrchNoteMapper
    COMPANY_NAME "JohnPascu"
    PLUGIN_MANUFACTURER_CODE Jpas
    PLUGIN_CODE Onmp

    FORMATS VST3 Standalone

    PRODUCT_NAME "OrchNoteMapper"
    PLUGIN_NAME "OrchNoteMapper"
    BUNDLE_ID "com.johnpascu.OrchNoteMapper"

    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT TRUE
    VST3_REPLACES_VST2 FALSE

    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_sources(OrchNoteMapper
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

add_dependencies(OrchNoteMapper OrchNoteMapperGenerateBuildInfo)
target_include_directories(OrchNoteMapper PRIVATE ${OrchNoteMapperGeneratedDir})

target_compile_definitions(OrchNoteMapper
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(OrchNoteMapper
    PRIVATE
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_gui_extra

    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# JUCE 9 VST3 wrapper warning control.
if (TARGET OrchNoteMapper_VST3)
    target_compile_definitions(OrchNoteMapper_VST3
        PRIVATE
            JUCE_VST3_CAN_REPLACE_VST2=0
    )
endif()
