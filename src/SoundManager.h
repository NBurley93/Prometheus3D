#pragma once
#include <string> 
#include <map>

struct SoundFX {
	void* mData;
};

struct SoundMus {
	void* mMusic;
};

class SoundManager {
public:
	static SoundManager* Get();

	bool Init();
	void Shutdown();

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