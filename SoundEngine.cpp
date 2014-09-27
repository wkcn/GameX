#include "SoundEngine.h"
bool SoundEngine::Init()
{
	unsigned int     version;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS        caps;

	result = FMOD::System_Create(&system);
	if (result != FMOD_OK)return false;
	result = system->getVersion(&version);
	if (result != FMOD_OK)return false;
	if (version < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return false;
	}
	result = system->getDriverCaps(0, &caps, 0, &speakermode);
	if (result != FMOD_OK)return false;
	result = system->setSpeakerMode(speakermode);
	if (result != FMOD_OK)return false;
	if (caps & FMOD_CAPS_HARDWARE_EMULATED)
	{
		result = system->setDSPBufferSize(1024, 10);
		if (result != FMOD_OK)return false;
	}
	int MaxChannel = 8;//°Ë¹ìµÀ
	result = system->init(MaxChannel, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		if (result != FMOD_OK)return false;
		result = system->init(MaxChannel, FMOD_INIT_NORMAL, 0);
		if (result != FMOD_OK)return false;
	}
	return true;
}
SoundEngine::~SoundEngine()
{
	system->close();
	system->release();
	map<string, FMOD::Sound*>::iterator  ptr;
	for (ptr = SoundPool.begin(); ptr != SoundPool.end(); ptr++)
	{
		//safe_release(ptr->second);
	}
}
FMOD::Sound* SoundEngine::ReadMusic(string filename)
{
	if (SoundPool.find(filename) != SoundPool.end())
		return SoundPool[filename];
	FMOD::Sound *sound;
	result = system->createStream(filename.data(), FMOD_HARDWARE, 0, &sound);
	SoundPool[filename] = sound;
	return sound;
}

void SoundEngine::PlaySE(string filename, float volume)
{
	FMOD::Sound *sound;
	FMOD::Channel *channel;
	sound = ReadMusic(filename);
	result = system->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
	result = channel->setVolume((float)volume);
	result = channel->setPaused(false);
}
void SoundEngine::Play(string filename, CHANNEL c, float volume, bool loop)
{
	//if (Channels.find(c) != Channels.end())
	//	Channels[c]->stop();
	Channels[c]->stop();
	FMOD::Sound *sound;
	//FMOD::Channel *channel;
	sound = ReadMusic(filename);
	result = system->playSound(FMOD_CHANNEL_FREE, sound, true, &Channels[c]);
	result = Channels[c]->setVolume((float)volume);
	result = Channels[c]->setPaused(false);
	if (loop)Channels[c]->setMode(FMOD_LOOP_NORMAL);
}
void SoundEngine::StopChannel(CHANNEL c)
{
	Channels[c]->stop();
	/*
	if (Channels.find(c) != Channels.end())
	{
		Channels[c]->stop();
		Channels.clear();
	}
	*/
}
void SoundEngine::StopAllSound()
{
	//for (map<CHANNEL, FMOD::Channel*>::iterator ptr = Channels.begin(); ptr != Channels.end(); ptr++)
	//	StopChannel(ptr->first);

	for (int i = 0; i < 8; i++)
	{
		Channels[i]->stop();
	}

}
void SoundEngine::SetVolume(float volume)
{
	
}
void SoundEngine::Update()
{
	system->update();
}