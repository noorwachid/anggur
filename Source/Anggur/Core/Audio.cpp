#include <SDL_mixer.h>
#include <Anggur/Helper/Log.hpp>
#include "Audio.hpp"

namespace Anggur {

int Audio::frequency = 44100;
int Audio::bufferSize = 128;
int Audio::pollSize = 16;

void Audio::SetBuffer(int frequency, int bufferSize)
{
    Audio::frequency = frequency;
    Audio::bufferSize = bufferSize;
}

void Audio::SetPollSize(int size)
{
    pollSize = size;
}

void Audio::Initialize()
{
    int mixAudio = Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, 2, bufferSize);
    Anggur_Assert(mixAudio == 0, "[Core] Failed to open audio device: %i\n", mixAudio);

    Mix_AllocateChannels(pollSize);
}

void Audio::Terminate()
{
    Mix_Quit();
}

SoundFx::SoundFx()
{
    chuckData = nullptr;
}

SoundFx::~SoundFx()
{
    Unload();
}

void SoundFx::Load(const std::string& path, int slot, int volume)
{
    Unload();
    this->slot = slot;
    this->volume = volume;
    chuckData = Mix_LoadWAV(path.c_str());
    Anggur_Assert(chuckData, "[Core.Audio] failed to load sound fx %s: %s\n", path.c_str(), Mix_GetError());
}

void SoundFx::Unload()
{
    if (!chuckData)
        return;

    Mix_FreeChunk(chuckData);
    chuckData = nullptr;
}

void SoundFx::SetSlot(int slot)
{
    this->slot = slot;
}

void SoundFx::SetVolume(int volume)
{
    this->volume = volume;
    Mix_Volume(slot, volume);
}

void SoundFx::Play()
{
    Mix_Volume(slot, volume);
    Mix_PlayChannel(slot, chuckData, 0);
}

void SoundFx::Pause()
{
    Mix_HaltChannel(slot);
}

void SoundFx::FadeIn(float time)
{
    Mix_FadeInChannel(slot, chuckData, 0, time * 1000);
}

void SoundFx::FadeOut(float time)
{
    Mix_FadeOutChannel(slot, time * 1000);
}

Music::Music()
{
    musicData = nullptr;
}

Music::~Music()
{
    Unload();
}

void Music::Load(const std::string& path)
{
    Unload();
    musicData = Mix_LoadMUS(path.c_str());
    Anggur_Assert(musicData, "[Core.Audio] failed to load music %s: %s\n", path.c_str(), Mix_GetError());
}

void Music::Unload()
{
    if (!musicData)
        return;

    Mix_FreeMusic(musicData);
    musicData = nullptr;
}

void Music::Play()
{
    Mix_PlayMusic(musicData, 0);
}

void Music::Pause()
{
    Mix_PauseMusic();
}


}
