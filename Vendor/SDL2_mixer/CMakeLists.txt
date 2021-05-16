cmake_minimum_required(VERSION 3.0)
project(SDL2_MIXER C)

find_path(SDL2_INCLUDE_DIR SDL.h PATH_SUFFIXES SDL2)
find_library(SDL2_LIBRARY NAMES SDL2d SDL2)
set(SDL2_MIXER_INCLUDES ${SDL2_INCLUDE_DIR})
set(SDL2_MIXER_LIBRARIES ${SDL2_LIBRARY})

# builtin formats
set(SDL2_MIXER_DEFINES
    MUSIC_WAV
    MUSIC_MID_NATIVE)

# MP3 support
if(SDL2_MIXER_ENABLE_MP3)
    find_path(MPG123_INCLUDE_DIR mpg123.h)
    find_library(MPG123_LIBRARY NAMES libmpg123 mpg123)
    list(APPEND SDL2_MIXER_INCLUDES ${MPG123_INCLUDE_DIR})
    list(APPEND SDL2_MIXER_LIBRARIES ${MPG123_LIBRARY})
    list(APPEND SDL2_MIXER_DEFINES MUSIC_MP3_MPG123)
endif()

# FLAC support
if(SDL2_MIXER_ENABLE_FLAC)
    find_path(FLAC_INCLUDE_DIR flac/all.h)
    find_library(FLAC_LIBRARY flac)
    list(APPEND SDL2_MIXER_INCLUDES ${FLAC_INCLUDE_DIR})
    list(APPEND SDL2_MIXER_LIBRARIES ${FLAC_LIBRARY})
    list(APPEND SDL2_MIXER_DEFINES MUSIC_FLAC)
endif()

# MOD support
if(SDL2_MIXER_ENABLE_MOD)
    find_path(MODPLUG_INCLUDE_DIR libmodplug/modplug.h)
    find_library(MODPLUG_LIBRARY modplug)
    list(APPEND SDL2_MIXER_INCLUDES ${MODPLUG_INCLUDE_DIR})
    list(APPEND SDL2_MIXER_LIBRARIES ${MODPLUG_LIBRARY})
    list(APPEND SDL2_MIXER_DEFINES MUSIC_MOD_MODPLUG)
endif()

# Ogg-Vorbis support
if(SDL2_MIXER_ENABLE_OGGVORBIS)
    find_path(VORBIS_INCLUDE_DIR vorbis/codec.h)
    find_library(VORBISFILE_LIBRARY vorbisfile)
    list(APPEND SDL2_MIXER_INCLUDES ${VORBIS_INCLUDE_DIR})
    list(APPEND SDL2_MIXER_LIBRARIES ${VORBISFILE_LIBRARY})
    list(APPEND SDL2_MIXER_DEFINES MUSIC_OGG)
endif()


file(GLOB SDL2_MIXER_SOURCE
        "*.c"
        "*.h"
)

file(GLOB SDL2_MIXER_TIMIDITY_SOURCE
        "timidity/*.c"
        "timidity/*.h"
)

file(GLOB SDL2_MIXER_TIMIDITY_SOURCE
        "timidity/*.c"
        "timidity/*.h"
)


add_library(SDL2_mixer
    ${SDL2_MIXER_SOURCE}
    ${SDL2_MIXER_NATIVE_MIDI_SOURCE}
    ${SDL2_MIXER_TIMIDITY_SOURCE}
)

#set_target_properties(SDL2_mixer PROPERTIES DEFINE_SYMBOL SDL2_EXPORTS)
target_compile_definitions(SDL2_mixer PUBLIC ${SDL2_MIXER_DEFINES})
target_include_directories(SDL2_mixer PUBLIC ${SDL2_MIXER_INCLUDES} native_midi)
target_link_libraries(SDL2_mixer ${SDL2_MIXER_LIBRARIES})

install(TARGETS SDL2_mixer
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)

if(NOT SDL2_MIXER_SKIP_HEADERS)
    install(FILES SDL2_MIXER.h DESTINATION include/SDL2)
endif()

message(STATUS "Link-time dependencies:")
foreach(LIBRARY ${SDL2_MIXER_LIBRARIES})
    message(STATUS "  " ${LIBRARY})
endforeach()
