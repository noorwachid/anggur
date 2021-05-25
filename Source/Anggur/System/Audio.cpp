#include <SDL_mixer.h>
#include <Anggur/Helper/Log.hpp>
#include "Audio.hpp"

namespace Anggur {

int Audio::mFrequency = 44100;
int Audio::mBufferSize = 128;
int Audio::mPollSize = 16;

void Audio::SetBuffer(int frequency, int bufferSize)
{
    mFrequency = frequency;
    mBufferSize = bufferSize;
}

void Audio::SetPollSize(int size)
{
    mPollSize = size;
}

void Audio::Initialize()
{
    int mixAudio = Mix_OpenAudio(mFrequency, MIX_DEFAULT_FORMAT, 2, mBufferSize);
    Anggur_Assert(mixAudio == 0, "[Core] Failed to open audio device: %i\n", mixAudio);

    Mix_AllocateChannels(mPollSize);
}

void Audio::Terminate()
{
    Mix_Quit();
}

SoundFx::SoundFx()
{
    mChunkData = nullptr;
}

SoundFx::~SoundFx()
{
    Unload();
}

void SoundFx::Load(const std::string& path, int slot, int volume)
{
    Unload();
    mSlot = slot;
    mVolume = volume;
    mChunkData = Mix_LoadWAV(path.c_str());
    Anggur_Assert(mChunkData, "[Core.Audio] failed to load sound fx %s: %s\n", path.c_str(), Mix_GetError());
}

void SoundFx::Unload()
{
    if (!mChunkData)
        return;

    Mix_FreeChunk(mChunkData);
    mChunkData = nullptr;
}

void SoundFx::SetSlot(int slot)
{
    mSlot = slot;
}

void SoundFx::SetVolume(int volume)
{
    mVolume = volume;
    Mix_Volume(mSlot, mVolume);
}

void SoundFx::Play()
{
    Mix_Volume(mSlot, mVolume);
    Mix_PlayChannel(mSlot, mChunkData, 0);
}

void SoundFx::Pause()
{
    Mix_HaltChannel(mSlot);
}

void SoundFx::FadeIn(float time)
{
    Mix_FadeInChannel(mSlot, mChunkData, 0, time * 1000);
}

void SoundFx::FadeOut(float time)
{
    Mix_FadeOutChannel(mSlot, time * 1000);
}

Music::Music()
{
    mMusicData = nullptr;
}

Music::~Music()
{
    Unload();
}

void Music::Load(const std::string& path)
{
    Unload();
    mMusicData = Mix_LoadMUS(path.c_str());
    Anggur_Assert(mMusicData, "[Core.Audio] failed to load music %s: %s\n", path.c_str(), Mix_GetError());
}

void Music::Unload()
{
    if (!mMusicData)
        return;

    Mix_FreeMusic(mMusicData);
    mMusicData = nullptr;
}

void Music::Play()
{
    Mix_PlayMusic(mMusicData, 0);
}

void Music::Pause()
{
    Mix_PauseMusic();
}


}
