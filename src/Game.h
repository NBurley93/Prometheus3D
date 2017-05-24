#pragma once
#include <Window.h>
#include "GameState.h"
#include <Camera.h>
#include <InputManager.h>
#include "LightManager.h"
#include <Material.h>
#include <Font.h>
#include <Model.h>
#include <map>
#include <Sprite2D.h>
#include <Skybox.h>
#include "Player.h"
#include <Timer.h>
#include "GameMap.h"
#include <Renderer.h>
#include <ParticleEmitter.h>
#include "SoundManager.h"
#include <time.h>
#include <stdlib.h>

const float ORBIT_RADIUS = 12.0f;
const float CAM_RADIUS = 5.0f;
const float CAM_HEIGHT = 3.0f;
const GLuint SHADOW_FACTOR = 4096;
const float SHADOW_SAMPLES = 64.0f;
const float SHADOW_FAR = 1024.0f;
const int STEP_DELAY = 40;

void con_glVersion(std::string args);
void con_version(std::string args);
void con_toggleShadow(std::string args);
void con_toggleWireframe(std::string args);
void con_debugShadows(std::string args);
void con_toggleNormalMapping(std::string args);
void con_toggleAnimations(std::string args);
void con_modVal(std::string args);
void con_listVal(std::string args);

class Game {
public:
	void Exec();
private:
	void Init();
	void Run();

	void Update();
	void Render();
	void Close();

	//Render methods
	void RenderScene(GLShader* shdr, bool texture);
	void RenderFPArms();
	void RenderGUI();
	void RenderShadowMapping();
	void RenderLightSources();

	void playRandFootstep();

	GameState mGameState;
	Window* mWindow;
	Model* mFPArms;
	Model* mModel;
	Model* lightSrc;
	Model* mWell;
	Model* mCutout;
	InputManager* mInput;
	Camera* mCamera;
	Skybox* mSkybox;
	Player* mPlayer;
	Timer mFPSTimer;
	Timer mCapTimer;
	int countedFrames;
	Renderer* mRenderer;

	ParticleEmitter* testEmit;

	GameMap* mTestMap;

	Font testFont;
	Font consoleFont;

	GLuint mFBO;
	GLuint mDepthMap;

	float far;

	float cutoutRotate;

	int footStepSoundTimer;

	float mLastFrame;
	float mDT;
};