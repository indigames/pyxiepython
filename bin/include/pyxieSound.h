#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <map>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

namespace pyxie
{
	static ALenum soundErr;

	#define AL_CHECK(...) \
	{ \
		if ((soundErr = alGetError()) != AL_NO_ERROR) { \
			pyxie_printf("OpenAL error: %x at %s(%i)", soundErr, __FILE__, __LINE__); \
			check_error(); \
			return errUnknown; \
		} \
	}

	#define EXCEPTION(...) \
	{ \
		LOG_ERROR(__VA_ARGS__); \
		return errUnknown; \
	}

	typedef struct {
		char  riff[4];//'RIFF'
		uint32_t riffSize;
		char  wave[4];//'WAVE'
		char  fmt[4];//'fmt '
		uint32_t fmtSize;
		uint16_t format;
		uint16_t channels;
		uint32_t samplesPerSec;
		uint32_t bytesPerSec;
		uint16_t blockAlign;
		uint16_t bitsPerSample;
		char  data[4];//'data'
		uint32_t dataSize;
	} BasicWAVEHeader;

	typedef struct {
		char* name;
		ALuint bufferId;
	} SoundBuffer;

    class PYXIE_EXPORT pyxieSound
    {
	public:

		// python public api
		pyxieSound();
		~pyxieSound();
		void init();
		void release();
		void play(char* filename, bool loop = false);
		void stop(char* filename);
		void stopAllSound();
		ALuint load(char* filename);
		void unload(char* filename);
		void unloadUnused();
		void setPositon(char* filename, float x, float y, float z);
		void setPitch(char* filename, float pitch);
		void setGain(char* filename, float gain);
		void setRolloff(char* filename, float rolloff);

		// Easy device init/deinit functions. InitAL returns true on success.
		bool initAL();
		void closeAL(void);

		// Buffer handling
		int addBuffer(char* fnameptr);
		int releaseBuffer(int bufferId);

		// Source handling
		int addSource(int bufferId);
		int releaseSource(int sourceId);
		int setPositon(int sourceId, float x, float y, float z);
		int setPitch(int sourceId, float pitch);
		int setGain(int sourceId, float gain);
		int setRolloff(int sourceId, float rolloff);
		int play(int sourceId, int loop);
		int stop(int sourceId);

		// Listener handling
		int setListenerPosition(float x, float y, float z);
		int setListenerOrientation(float xAt, float yAt, float zAt, float xUp, float yUp, float zUp);

		// Helpers
		char* readWav(char* filename, BasicWAVEHeader* header);
		ALuint createBufferFromWave(char*, BasicWAVEHeader);
		int check_error();
	protected:
		// Utilities
		bool isInitialized() { return initialized; }
		ALuint loadSound(char* filename);
		void releaseAllSound();
		ALuint getSourceId(char* filename);


	private:
		bool initialized;
		std::map<ALuint, SoundBuffer> soundSourcesDict;
    };
} // namespace pyxie