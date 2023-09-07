#include "Anggur/Audio/AudioBuffer.h"
#include "AL/al.h"
#include "AL/alc.h"
#include <fstream>
#include <iostream>

namespace Anggur
{
	struct WavHeader 
	{
		char chunkId[4];
		uint32_t chunkSize;
		char format[4];
		char subchunk1Id[4];
		uint32_t subchunk1Size;
		uint16_t audioFormat;
		uint16_t numChannels;
		uint32_t sampleRate;
		uint32_t byteRate;
		uint16_t blockAlign;
		uint16_t bitsPerSample;
		char subchunk2Id[4];
		uint32_t subchunk2Size;
	};

	AudioBuffer::AudioBuffer(const std::string& path)
	{
		alGenBuffers(1, &_id);

		std::string extension = std::filesystem::path(path).extension().string();

		if (extension == ".wav") 
		{
			const char* wav_filename = "resource/sample.wav";
			std::ifstream wav_file(wav_filename, std::ios::binary);

			if (!wav_file.is_open()) {
				std::cerr << "Failed to open WAV file: " << wav_filename << std::endl;
				return;
			}

			WavHeader header;
			wav_file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

			// Check that the file is a valid WAV file
			if (std::string(header.chunkId, 4) != "RIFF" ||
					std::string(header.format, 4) != "WAVE" ||
					std::string(header.subchunk1Id, 4) != "fmt " ||
					header.audioFormat != 1 || // PCM format
					header.bitsPerSample != 16) {
				std::cerr << "Unsupported or invalid WAV file format." << std::endl;
				return;
			}

			// Read audio data
			std::vector<int16_t> audio_data(header.subchunk2Size / 2); // 16-bit samples
			wav_file.read(reinterpret_cast<char*>(audio_data.data()), header.subchunk2Size);

			// Set the format of the buffer based on the WAV file's channel count
			ALenum format = (header.numChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

			// Set the audio data in the buffer
			alBufferData(_id, format, audio_data.data(), audio_data.size() * sizeof(int16_t), header.sampleRate);
		}
		else
		{
			throw std::runtime_error("Unsupported audio extension");
		}
	}

	AudioBuffer::~AudioBuffer()
	{
		alDeleteBuffers(1, &_id);
	}

	unsigned int AudioBuffer::GetID() const
	{
		return _id;
	}
}


