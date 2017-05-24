#pragma once
#include <SDL2/SDL_mixer.h>
#include <string> 
#include <map>

struct SoundFX {
	Mix_Chunk* mData;
};

struct SoundMus {
	Mix_Music* mMusic;
};

class SoundManager {
public:
	static SoundManager* Get();

	void addSFX(std::string path);
	void addMus(std::string path);

	void playSFX(std::string path);
	void playMus(std::string path, bool loop);

	void stopAll();

	void Clean();
private:
	static SoundManager* mInst;
	std::map<std::string, SoundFX*> mSoundCache;
	std::map<std::string, SoundMus*> mMusicCache;
};