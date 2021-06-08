#include "SoundManager.h"
#include <SDL_mixer.h>

SoundManager* SoundManager::mInst = nullptr;

bool SoundManager::Init() {
	if (Mix_Init(MIX_INIT_OGG) < 0) {
		std::printf("%s\n", Mix_GetError());
		return false;
	}
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_VolumeMusic(10);
	return true;
}

void SoundManager::Shutdown() {
	Mix_CloseAudio();
	Mix_Quit();
}

SoundManager* SoundManager::Get() {
	if (mInst == nullptr) {
		mInst = new SoundManager();
	}
	return mInst;
}

void SoundManager::addSFX(std::string path) {
	auto mit = mSoundCache.find(path);
	if (mit == mSoundCache.end()) {
		//New sound
		SoundFX* mSnd = new SoundFX();
		mSnd->mData = Mix_LoadWAV(path.c_str());
		if (mSnd->mData == NULL) {
			//Fail
			std::printf("Failed to load sound: %s\n", path.c_str());
		}
		mSoundCache[path] = mSnd;
		std::printf("Loaded SFX %s\n", path.c_str());
	}
	else {
		//Replace existing
		SoundFX* mSnd = mit->second;
		Mix_FreeChunk(reinterpret_cast<Mix_Chunk*>(mSnd->mData)); //Free previous stuff
		mSnd->mData = Mix_LoadWAV(path.c_str());
		if (mSnd->mData == NULL) {
			//Fail
			std::printf("Failed to load sound: %s\n", path.c_str());
		}
	}
}

void SoundManager::addMus(std::string path) {
	auto mit = mMusicCache.find(path);
	if (mit == mMusicCache.end()) {
		//New Music
		SoundMus* mSnd = new SoundMus();
		mSnd->mMusic = Mix_LoadMUS(path.c_str());
		if (mSnd->mMusic == NULL) {
			std::printf("Failed to load music: %s\n", path.c_str());
		}
		mMusicCache[path] = mSnd;
		std::printf("Loaded music %s\n", path.c_str());
	}
	else {
		SoundMus* mSnd = mit->second;
		Mix_FreeMusic(reinterpret_cast<Mix_Music*>(mSnd->mMusic));
		mSnd->mMusic = Mix_LoadMUS(path.c_str());
		if (mSnd->mMusic == NULL) {
			std::printf("Failed to load music: %s\n", path.c_str());
		}
	}
}

void SoundManager::playMus(std::string path, bool loop) {
	auto mit = mMusicCache.find(path);
	if (mit != mMusicCache.end()) {
		//Found it
		int lval = 0;
		if (loop) {
			lval = -1;
		}
		if (Mix_PlayMusic(reinterpret_cast<Mix_Music*>(mit->second->mMusic), lval) == -1) {
			std::printf("mixer_playfailure: %s\n", Mix_GetError());
		}
		std::printf("boom\n");
	}
}

void SoundManager::playSFX(std::string path) {
	auto mit = mSoundCache.find(path);
	if (mit != mSoundCache.end()) {
		//Found it
		Mix_PlayChannel(-1, reinterpret_cast<Mix_Chunk*>(mit->second->mData), 0);
	}
}

void SoundManager::stopAll() {
	//Stops looping music
	Mix_HaltMusic();
}

void SoundManager::Clean() {
	//Clean sfx
	for (auto mit = mSoundCache.begin(); mit != mSoundCache.end(); mit++) {
		Mix_FreeChunk(reinterpret_cast<Mix_Chunk*>(mit->second->mData));
		delete mit->second;
	}
	mSoundCache.clear();

	for (auto mit = mMusicCache.begin(); mit != mMusicCache.end(); mit++) {
		Mix_FreeMusic(reinterpret_cast<Mix_Music*>(mit->second->mMusic));
		delete mit->second;
	}
	mMusicCache.clear();
}