#pragma once
#include "Defines.h"
#include "fmod.hpp"

typedef int CHANNEL;

class SoundEngine{

private:
	FMOD::System    *system;
	FMOD_RESULT      result;
	map<string, FMOD::Sound*> SoundPool;
	//map<CHANNEL, FMOD::Channel*> Channels;
	FMOD::Channel* Channels[8];// °ËÍ¨µÀ
private:
	FMOD::Sound* ReadMusic(string filename);
public:
	bool Init();
	void Update();
	void PlaySE(string filename, float volume);
	void Play(string filename, CHANNEL c, float volume, bool loop);
	//void SetSEVolume(float volume);
	//void SetBGMVolume(float volume);
	void SetVolume(float volume);
	void StopAllSound();
	void StopChannel(CHANNEL c);
	~SoundEngine();
};