#include "sdlwavaudiodata.hpp"

SDLWAVAudioData::SDLWAVAudioData(const std::string& fileName, bool streamFromFile)
{
	// TODO: Handle streamFromFile

	SDL_AudioSpec wavSpec;
	Uint8* wavStart;
	Uint32 wavLength;

	if(SDL_LoadWAV(fileName.c_str(), &wavSpec, &wavStart, &wavLength) == NULL)
	{
		// TODO: Proper error handling
//		std::cerr << "Error: " << fileName 
//			<< " could not be loaded as an audio file" << std::endl;
		throw fileName;
	}

	m_pos = wavStart;
	m_start = wavStart;
	m_end = m_start + wavLength;
}

SDLWAVAudioData::~SDLWAVAudioData()
{
	SDL_FreeWAV(m_start);
}

size_t SDLWAVAudioData::GenerateSamples(float* stream, size_t streamLength, 
		size_t pos, const SampleInfo& info)
{
	m_pos = m_start + pos;
	Uint32 lengthLeft = (Uint32)(m_end - m_pos);

	if(lengthLeft == 0)
	{
		return (size_t)-1;
	}

	Uint32 length = (Uint32)streamLength;
	length = (length > lengthLeft ? lengthLeft : length);

	// TODO: Make this more general
	Sint16* samples = (Sint16*)m_pos;

	float factor = (float)info.volume * 1.0f/32768.0f;
	for(Uint32 i = 0; i < length; i++)
	{
		stream[i] = (*samples) * factor;
		samples++;
	}

	m_pos = (Uint8*)samples;
	return (size_t)(m_pos - m_start);
}

size_t SDLWAVAudioData::GetAudioLength()
{
	return (size_t)(m_end - m_start);
}

