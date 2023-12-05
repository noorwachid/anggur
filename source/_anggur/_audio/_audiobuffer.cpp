#include "anggur/audio/audiobuffer.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "stb_vorbis.c"
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
			std::ifstream wav_file(path, std::ios::binary);

			if (!wav_file.is_open())
			{
				throw std::runtime_error("Failed to open WAV file: " + path);
			}

			WavHeader header;
			wav_file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

			// Check that the file is a valid WAV file
			if (std::string(header.chunkId, 4) != "RIFF" || std::string(header.format, 4) != "WAVE" ||
				std::string(header.subchunk1Id, 4) != "fmt " || header.audioFormat != 1 || // PCM format
				header.bitsPerSample != 16)
			{
				throw std::runtime_error("Unsupported or invalid WAV file format");
			}

			// Read audio data
			std::vector<int16_t> audio_data(header.subchunk2Size / 2); // 16-bit samples
			wav_file.read(reinterpret_cast<char*>(audio_data.data()), header.subchunk2Size);

			// Set the format of the buffer based on the WAV file's channel count
			ALenum format = (header.numChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

			// Set the audio data in the buffer
			alBufferData(_id, format, audio_data.data(), audio_data.size() * sizeof(int16_t), header.sampleRate);
		}
		else if (extension == ".ogg")
		{
			stb_vorbis* vorbisFile = stb_vorbis_open_filename(path.c_str(), nullptr, nullptr);

			if (!vorbisFile)
				throw std::runtime_error("failed to open vorbis file: " + path);

			stb_vorbis_info vorbisInfo = stb_vorbis_get_info(vorbisFile);
			// Determine the size of the Vorbis data
			int dataSize = stb_vorbis_stream_length_in_samples(vorbisFile) * vorbisInfo.channels * 2; // 16-bit audio

			// Allocate memory for audio data
			short* audioData = new short[dataSize];

			// Read audio data from the Vorbis file
			int samplesRead =
				stb_vorbis_get_samples_short_interleaved(vorbisFile, vorbisInfo.channels, audioData, dataSize);
			if (samplesRead < 0)
			{
				delete[] audioData;
				throw std::runtime_error("Failed to read vorbis data");
			}

			alBufferData(
				_id, (vorbisInfo.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, audioData, dataSize,
				vorbisInfo.sample_rate
			);

			delete[] audioData;
			stb_vorbis_close(vorbisFile);
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
