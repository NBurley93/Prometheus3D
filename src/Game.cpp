#include "Game.h"
#include "CommandConsole.h"
#include "Globals.h"
#include "builddate.h"
#include "buildnum.h"
#include "revision.h"
#include <ShaderManager.h>
#include <TextureManager.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define VER_MAJOR 0
#define VER_MINOR 0

void Game::Exec() {
  Init();
  Run();
}

void Game::Init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    std::printf("%s\n", SDL_GetError());
    exit(-1);
  }
  SDL_GL_LoadLibrary(NULL);

  if (SoundManager::Get()->Init()) {
    SoundManager::Get()->addMus("./data/sounds/music/breaktherules.ogg");

    for (int i = 1; i <= 17; i++) {
      SoundManager::Get()->addSFX("./data/sounds/footsteps/concrete_ct_" +
                                  std::to_string(i) + ".wav");
    }

    SoundManager::Get()->addSFX("./data/sounds/landing/land1.wav");
    SoundManager::Get()->addSFX("./data/sounds/landing/land2.wav");
    SoundManager::Get()->addSFX("./data/sounds/landing/land3.wav");
    SoundManager::Get()->addSFX("./data/sounds/landing/land4.wav");
  }

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  mWindow = new Window();
  mWindow->CreateWindow(1920, 1080, "Prometheus");

  SDL_GL_SetSwapInterval(0);
  SDL_ShowCursor(SDL_FALSE);

  srand(time(NULL));

  CommandConsole::start(mWindow->GetWidth(), mWindow->GetHeight());
  CommandConsole::registerCommand(
      "get_glversion", con_glVersion,
      "- Prints the OpenGL context version information");
  CommandConsole::registerCommand("get_engine_version", con_version,
                                  "- Prints engine version");
  CommandConsole::registerCommand("toggle_shadows", con_toggleShadow,
                                  "- Toggles shadow mapping");
  CommandConsole::registerCommand("toggle_wireframe", con_toggleWireframe,
                                  "- Toggles wireframe");
  CommandConsole::registerCommand("debug_shadows", con_debugShadows,
                                  "- Toggles debug view for shadow mapping");
  CommandConsole::registerCommand(
      "toggle_normal_mapping", con_toggleNormalMapping,
      "- Toggles use of normal maps (STUB - DOES NOTHING)");
  CommandConsole::registerCommand("toggle_animations", con_toggleAnimations,
                                  "- Toggles use of animations");
  CommandConsole::registerCommand(
      "modify_value", con_modVal,
      "- Modify a global value, use listval to show modifiable values");
  CommandConsole::registerCommand("list_values", con_listVal,
                                  "- Lists all modifiable values");

  // Shaders
  GLShader::enableDebugOutput();
  ShaderManager::Get()->getShdr("gui_text");
  ShaderManager::Get()->getShdr("color");
  ShaderManager::Get()->getShdr("light");
  ShaderManager::Get()->getShdr("light_anim");
  ShaderManager::Get()->getShdr("nolight_texture");
  ShaderManager::Get()->getShdr("sprite_2d");
  ShaderManager::Get()->getShdr("skybox");
  ShaderManager::Get()->getShdr("gui_console");
  ShaderManager::Get()->getShdr("simple_depth");

  Globals::Get()->input_text = "";

  mRenderer = new Renderer();

  // Models
  mFPArms = new Model();
  mFPArms->Load("./data/models/fp_torch.dae");
  mFPArms->GetMesh(1)->AddTexture("diffuse", "./data/textures/fp/diffuse.tga");
  mFPArms->GetMesh(0)->AddTexture("diffuse",
                                  "./data/textures/fp/diffuse_torch.tga");

  mWell = new Model();
  mWell->Load("./data/models/well.dae");
  mWell->GetMesh(0)->AddTexture("diffuse",
                                "./data/textures/well/well_diffuse.tga");
  mWell->GetMesh(0)->AddTexture("specular",
                                "./data/textures/well/well_specular.tga");
  mWell->GetMesh(1)->AddTexture("diffuse",
                                "./data/textures/well/water_diffuse.tga");

  mCutout = new Model();
  mCutout->Load("./data/models/cardboard_cutout.dae");
  mCutout->GetMesh(0)->AddTexture("diffuse",
                                  "./data/textures/cardboard/house.tga");
  mCutout->GetMesh(1)->AddTexture(
      "diffuse", "./data/textures/cardboard/cardboard_edge.tga");

  mModel = new Model();
  mModel->Load("./data/models/cyborg_walk.dae");

  // Animation deathAnim;
  // deathAnim.name = "death";
  // deathAnim.priority = 2;
  // deathAnim.startTime = 1;
  // deathAnim.endTime = 32;

  // mModel->AddAnimation(deathAnim);
  // mModel->PlayAnimation(*mModel->FindAnimation("death"), true, false);

  mModel->GetMesh(0)->AddTexture("diffuse",
                                 "./data/textures/cyborg/diffuse.tga");

  // Sphere mesh for the light source
  lightSrc = new Model();
  lightSrc->Load("./data/models/sphere.dae");

  // Camera definitions
  mCamera = new Camera();
  mCamera->defineFOV(110.0f);
  mCamera->init(mWindow->GetWidth(), mWindow->GetHeight());

  mPlayer = new Player();
  mPlayer->attachCamera(mCamera);
  mPlayer->Init(glm::vec3(0.0f, 0.0f, 0.0f));

  mInput = new InputManager();

  glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // Magenta clear color

  mDT = 0.0f; // Delta time

  cutoutRotate = 0.0f;

  // Opengl logo sprite
  mRenderer->CacheSprite(
      TextureManager::Get()->getTexture("./data/textures/opengl_logo.tga"));

  // Testing booleans
  Globals::Get()->uLighting = true;
  Globals::Get()->uNormalMapping = true;
  Globals::Get()->uTexturing = true;
  Globals::Get()->uWireframe = false;
  Globals::Get()->uBBOX = false;
  Globals::Get()->uShadows = true;
  Globals::Get()->uDebugShadows = false;
  Globals::Get()->shadowPCFSamples = SHADOW_SAMPLES;
  Globals::Get()->uAnimation = false;

  // Skybox
  mSkybox = new Skybox();
  mSkybox->loadCubemap("./data/textures/cubemaps/sky_mystic");
  mSkybox->Init();

  mTestMap = new GameMap();
  mTestMap->Load("./data/maps/debug.map");
  Model *mWorld = mTestMap->getModel();
  mWorld->GetMesh(0)->AddTexture("diffuse",
                                 "./data/textures/brick/diffuse.tga");
  mWorld->GetMesh(0)->AddTexture("specular",
                                 "./data/textures/brick/specular.tga");

  LightManager::Get()->Lights[0] = *mTestMap->getSun();
  LightManager::Get()->Lights[0].enabled = true;

  LightManager::Get()->Lights[1] = Light();
  // LightManager::Get()->Lights[1].enabled = true;
  LightManager::Get()->Lights[1].ambient = glm::vec3(0.1f);
  LightManager::Get()->Lights[1].diffuse = glm::vec3(1.0f);
  LightManager::Get()->Lights[1].specular = glm::vec3(0.8f);

  // FBO
  glGenFramebuffers(1, &mFBO);

  // Shadow depth map setup
  glGenTextures(1, &mDepthMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthMap);
  for (GLuint i = 0; i < 6; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_FACTOR, SHADOW_FACTOR, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // glEnables
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_STENCIL_TEST);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  // Fonts
  testFont.Init("./data/fonts/arial.ttf", 24);
  testFont.setColor(glm::vec3(228, 244, 53));

  consoleFont.Init("./data/fonts/arial.ttf", 12);
  consoleFont.setColor(glm::vec3(255, 255, 255));

  mGameState = GameState::GSPLAY; // Initial state
  SoundManager::Get()->playMus("./data/sounds/music/breaktherules.ogg", true);
  mFPSTimer.start();
  footStepSoundTimer = STEP_DELAY;
  countedFrames = 0;
}

void Game::Run() {
  // Fetch the performance counter from SDL to help construct delta-time and
  // loop
  mLastFrame = 0.0f;
  int frameTicks = 0;

  while (mGameState != GameState::GSEXIT) {
    mCapTimer.start();
    float avgFPS = countedFrames / (mFPSTimer.getTicks() / 1000.0f);
    if (avgFPS > 2000000) {
      avgFPS = 0;
    }
    if (avgFPS > 0) {
      mDT = (float)1000 / avgFPS;
    } else {
      mDT = 0.0f;
    }
    Update();
    Render();
    ++countedFrames;
    frameTicks = mCapTimer.getTicks();
    if (frameTicks < (1000 / 60)) {
      SDL_Delay((1000 / 60) - frameTicks);
    }
  }
  Close();
}

void Game::Update() {
  // SDL event handling
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      mGameState = GameState::GSEXIT;
      break;

    case SDL_KEYDOWN:
      mInput->pressKey(e.key.keysym.sym);
      break;

    case SDL_KEYUP:
      mInput->releaseKey(e.key.keysym.sym);
      break;

    case SDL_MOUSEMOTION:
      mInput->setMouse((float)e.motion.x, (float)e.motion.y);
      break;

    case SDL_TEXTINPUT:
      Globals::Get()->input_text += e.text.text;
    }
  }

  // Update step

  if (mInput->isKeyDown(SDLK_ESCAPE)) {
    // Exit
    mGameState = GameState::GSEXIT;
  }

  if (mInput->isKeyPressed(SDLK_y) && (!CommandConsole::isVisible())) {
    // Toggle wireframe for the mesh
    Globals::Get()->uWireframe = !Globals::Get()->uWireframe;
  }

  if (mInput->isKeyPressed(SDLK_BACKQUOTE)) {
    Globals::Get()->input_text = "";
    CommandConsole::toggleVisiblility(); // Console mechanically works, still
                                         // need to adapt it for OpenGL
  }

  if (mInput->isKeyPressed(SDLK_RETURN)) {
    if (CommandConsole::isVisible()) {
      CommandConsole::runCommand(Globals::Get()->input_text, true);
      Globals::Get()->input_text = "";
    }
  }

  if ((mInput->isKeyDown(SDLK_BACKSPACE) &&
       Globals::Get()->input_text.length() > 0)) {
    Globals::Get()->input_text.pop_back();
  }

  // Handle console history
  if (mInput->isKeyPressed(SDLK_UP) && CommandConsole::isVisible()) {
    CommandConsole::historyUp();
    Globals::Get()->input_text = CommandConsole::getHistory();
  }
  if (mInput->isKeyPressed(SDLK_DOWN) && CommandConsole::isVisible()) {
    CommandConsole::historyDown();
    Globals::Get()->input_text = CommandConsole::getHistory();
  }

  if (!CommandConsole::isVisible()) {
    mPlayer->Update(mDT, mInput);
  }

  cutoutRotate += 1.0f;
  if (cutoutRotate > 360.0f) {
    cutoutRotate = 0.0f;
  }

  int TRUE_DELAY = STEP_DELAY;
  if (mPlayer->Sprinting()) {
    TRUE_DELAY /= 2;
  }

  if (mPlayer->Crouching()) {
    TRUE_DELAY *= 2;
  }

  if (mPlayer->Moving()) {
    footStepSoundTimer += 1;
    if (footStepSoundTimer >= TRUE_DELAY) {
      // Play random footstep
      playRandFootstep();
      footStepSoundTimer = 0;
    }
  } else {
    footStepSoundTimer = STEP_DELAY;
  }

  glm::vec3 pp = mCamera->getPosition() + (mCamera->getDirection() * 12.0f);
  LightManager::Get()->Lights[1].direction =
      glm::vec4(pp.x, pp.y + 2.0f, pp.z, 0.0f);

  mInput->update(); // Update the input manager
}

void Game::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
          GL_STENCIL_BUFFER_BIT); // Clear buffers

  RenderShadowMapping();

  // Skybox rendering
  mSkybox->Render(*mCamera);

  RenderLightSources();

  // Render scene as normal
  GLShader *shdr = ShaderManager::Get()->getShdr("light");
  shdr->use();

  glUniformMatrix4fv(shdr->getUniformLocation("view"), 1, false,
                     glm::value_ptr(mCamera->getMatrix("view")));
  glUniformMatrix4fv(shdr->getUniformLocation("proj"), 1, false,
                     glm::value_ptr(mCamera->getMatrix("projection")));
  glUniform1f(shdr->getUniformLocation("material.shininess"), 128.0f);
  glActiveTexture(GL_TEXTURE31);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthMap);

  for (int i = 0; i < MAX_LIGHTS; i++) {
    Light mLight = LightManager::Get()->Lights[i];
    mLight.toShader(*shdr, i);
  }

  glUniform1i(shdr->getUniformLocation("shadowMap"), 31);
  glUniform1f(shdr->getUniformLocation("far_plane"), far);
  glUniform1f(shdr->getUniformLocation("numSamples"),
              Globals::Get()->shadowPCFSamples);
  glUniform1i(shdr->getUniformLocation("uShadows"), Globals::Get()->uShadows);
  glUniform1i(shdr->getUniformLocation("uDebug"),
              Globals::Get()->uDebugShadows);

  if (Globals::Get()->uWireframe) {
    // Enable wireframe rendering if toggled
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  RenderScene(shdr, true);

  // Render well
  mWell->Scale(glm::vec3(0.8f));
  mWell->Translate(glm::vec3(128.0f, 0.0f, 128.0f));
  mWell->Rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  mWell->Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  mWell->Draw(*shdr, true, true, true);

  // Render cutout
  mCutout->Translate(glm::vec3(0.0f, 56.0f, -98.0f));
  mCutout->Rotate(glm::radians(cutoutRotate), glm::vec3(0.0f, 1.0f, 0.0f));
  mCutout->Draw(*shdr, true, true, true);

  shdr->unuse();

  shdr = ShaderManager::Get()->getShdr("light_anim");
  shdr->use();

  glUniformMatrix4fv(shdr->getUniformLocation("view"), 1, false,
                     glm::value_ptr(mCamera->getMatrix("view")));
  glUniformMatrix4fv(shdr->getUniformLocation("proj"), 1, false,
                     glm::value_ptr(mCamera->getMatrix("projection")));
  glUniform1f(shdr->getUniformLocation("material.shininess"), 128.0f);
  glActiveTexture(GL_TEXTURE31);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthMap);

  for (int i = 0; i < MAX_LIGHTS; i++) {
    Light mLight = LightManager::Get()->Lights[i];
    mLight.toShader(*shdr, i);
  }

  glUniform1i(shdr->getUniformLocation("shadowMap"), 31);
  glUniform1f(shdr->getUniformLocation("far_plane"), far);
  glUniform1f(shdr->getUniformLocation("numSamples"),
              Globals::Get()->shadowPCFSamples);
  glUniform1i(shdr->getUniformLocation("uAnimation"),
              Globals::Get()->uAnimation);
  glUniform1i(shdr->getUniformLocation("uShadows"), Globals::Get()->uShadows);
  glUniform1i(shdr->getUniformLocation("uDebug"),
              Globals::Get()->uDebugShadows);

  glm::vec3 ppos = mPlayer->getPosition();
  glm::vec3 plook = mCamera->getDirection();
  float mod = 35.0f;
  mModel->Translate(
      glm::vec3(ppos.x - (plook.x * mod), ppos.y, ppos.z - (plook.z * mod)));
  mModel->Rotate(glm::radians(mCamera->get_hAngle() - 90.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f));
  mModel->Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  mModel->Draw(*shdr, true, true, true);

  shdr->unuse();

  // FP arms
  RenderFPArms();

  // GUI text and sprites + console
  RenderGUI();

  mWindow->SwapBuffers(); // Swap the buffers
}

void Game::RenderLightSources() {
  GLShader *shdr = ShaderManager::Get()->getShdr("color");

  // Render the light source sphere
  shdr->use();
  for (int i = 0; i < MAX_LIGHTS; i++) {
    Light mLight = LightManager::Get()->Lights[i];
    if (mLight.enabled) {
      lightSrc->Translate(mLight.direction);
      lightSrc->Scale(glm::vec3(2.0f, 2.0f, 2.0f));

      glUniformMatrix4fv(shdr->getUniformLocation("view"), 1, false,
                         glm::value_ptr(mCamera->getMatrix("view")));
      glUniformMatrix4fv(shdr->getUniformLocation("proj"), 1, false,
                         glm::value_ptr(mCamera->getMatrix("projection")));
      glUniform3f(shdr->getUniformLocation("drawColor"), 1.0f, 1.0f, 1.0f);

      lightSrc->Draw(*shdr);
    }
  }
  shdr->unuse();
}

void Game::RenderShadowMapping() {
  // First render the shadow map
  GLShader *shdr = ShaderManager::Get()->getShdr("simple_depth");
  shdr->use();

  // Cubemap transforms
  for (int i = 0; i < MAX_LIGHTS; i++) {
    // Shadowmap passes
    Light mLight = LightManager::Get()->Lights[i];
    if (mLight.enabled) {
      GLfloat aspect = (GLfloat)SHADOW_FACTOR / (GLfloat)SHADOW_FACTOR;
      GLfloat near = 1.0f;
      far = SHADOW_FAR;
      glm::mat4 shadowProj =
          glm::perspective(glm::radians(90.0f), aspect, near, far);
      std::vector<glm::mat4> shadowTransforms;
      glm::vec3 lightPos =
          glm::vec3(mLight.direction.x, mLight.direction.y, mLight.direction.z);

      // Allow us to look in all directions
      shadowTransforms.push_back(
          shadowProj * glm::lookAt(lightPos,
                                   lightPos + glm::vec3(1.0, 0.0, 0.0),
                                   glm::vec3(0.0, -1.0, 0.0)));
      shadowTransforms.push_back(
          shadowProj * glm::lookAt(lightPos,
                                   lightPos + glm::vec3(-1.0, 0.0, 0.0),
                                   glm::vec3(0.0, -1.0, 0.0)));
      shadowTransforms.push_back(
          shadowProj * glm::lookAt(lightPos,
                                   lightPos + glm::vec3(0.0, 1.0, 0.0),
                                   glm::vec3(0.0, 0.0, 1.0)));
      shadowTransforms.push_back(
          shadowProj * glm::lookAt(lightPos,
                                   lightPos + glm::vec3(0.0, -1.0, 0.0),
                                   glm::vec3(0.0, 0.0, -1.0)));
      shadowTransforms.push_back(
          shadowProj * glm::lookAt(lightPos,
                                   lightPos + glm::vec3(0.0, 0.0, 1.0),
                                   glm::vec3(0.0, -1.0, 0.0)));
      shadowTransforms.push_back(
          shadowProj * glm::lookAt(lightPos,
                                   lightPos + glm::vec3(0.0, 0.0, -1.0),
                                   glm::vec3(0.0, -1.0, 0.0)));

      // Uniforms
      for (GLuint i = 0; i < 6; ++i) {
        glUniformMatrix4fv(shdr->getUniformLocation("shadowMatrices[" +
                                                    std::to_string(i) + "]"),
                           1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
      }
      glUniform1f(shdr->getUniformLocation("far_plane"), far);
      glUniform3fv(shdr->getUniformLocation("lightPos"), 1, &lightPos[0]);

      glViewport(0, 0, SHADOW_FACTOR, SHADOW_FACTOR);
      glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
      glClear(GL_DEPTH_BUFFER_BIT);

      // Shadow map

      // Render the model

      glCullFace(GL_FRONT);

      // Render well
      mWell->Scale(glm::vec3(0.8f));
      mWell->Translate(glm::vec3(128.0f, 0.0f, 128.0f));
      mWell->Rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
      mWell->Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      mWell->Draw(*shdr, false, true, true);

      // Render cutout
      glDisable(GL_CULL_FACE);
      mCutout->Translate(glm::vec3(0.0f, 56.0f, -98.0f));
      mCutout->Rotate(glm::radians(cutoutRotate), glm::vec3(0.0f, 1.0f, 0.0f));
      mCutout->Draw(*shdr, false, true, true);
      glEnable(GL_CULL_FACE);

      RenderScene(shdr, false);

      glm::vec3 ppos = mPlayer->getPosition();
      glm::vec3 plook = mCamera->getDirection();
      float mod = 35.0f;
      mModel->Translate(glm::vec3(ppos.x - (plook.x * mod), ppos.y,
                                  ppos.z - (plook.z * mod)));
      mModel->Rotate(glm::radians(mCamera->get_hAngle() - 90.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));
      mModel->Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      mModel->Draw(*shdr, false, true, true);
      glCullFace(GL_BACK);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glViewport(0, 0, mWindow->GetWidth(), mWindow->GetHeight());
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthMap);
    }
  }
  shdr->unuse();
}

void Game::RenderFPArms() {
  // FPArms
  GLShader *shdr = ShaderManager::Get()->getShdr("light");
  shdr->use();
  glm::vec3 pl = mCamera->getPosition();
  float s = 0.4f;
  pl.y = pl.y - 2.0f;
  if (mPlayer->Moving()) {
    if (mPlayer->Sprinting()) {
      pl.y += cos(SDL_GetTicks() * 0.008f) * 0.5f;
      pl.x += cos(SDL_GetTicks() * 0.008f) * 0.5f;
    } else {
      pl.y += cos(SDL_GetTicks() * 0.005f) * 0.5f;
      pl.x += cos(SDL_GetTicks() * 0.005f) * 0.5f;
    }
  } else {
    pl.y += cos(SDL_GetTicks() * 0.0007f) * 0.1f;
    // pl.x += sin(SDL_GetTicks() * 0.0007f) * 0.5f;
  }
  mFPArms->Translate(pl);
  mFPArms->Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::vec2 ang = mCamera->getAngles();
  mFPArms->Rotate(glm::radians(ang.x - 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  mFPArms->Rotate(glm::radians(-ang.y - 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  mFPArms->Scale(glm::vec3(s, s, s));

  glUniformMatrix4fv(shdr->getUniformLocation("view"), 1, false,
                     glm::value_ptr(mCamera->getMatrix("view")));
  glUniformMatrix4fv(shdr->getUniformLocation("proj"), 1, false,
                     glm::value_ptr(mCamera->getMatrix("projection")));
  glClear(GL_DEPTH_BUFFER_BIT); // Clear the depth buffer so that the fp arms
                                // are always drawn on top
  mFPArms->Draw(*shdr);
  shdr->unuse();
}

void Game::RenderScene(GLShader *shdr, bool texture) {
  mTestMap->Render(shdr, texture);
}

void Game::RenderGUI() {
  glPolygonMode(
      GL_FRONT_AND_BACK,
      GL_FILL); // Guarentee that normal rendering, not wireframe is set

  // Draw 2D gui elements
  glDisable(GL_DEPTH_TEST); // Wouldn't make much sense for 2D ortho elements to
                            // z-fight
  mRenderer->RenderSprite(
      TextureManager::Get()->getTexture("./data/textures/opengl_logo.tga"),
      *mCamera, (mWindow->GetWidth() - 8) - 256, 8, 256, 128, 0.4f);

  glm::vec3 p = mPlayer->getPosition();
  glm::vec2 a = mCamera->getAngles();

  // GUI text
  ShaderManager::Get()->getShdr("gui_text")->use();

  glUniformMatrix4fv(ShaderManager::Get()
                         ->getShdr("gui_text")
                         ->getUniformLocation("projection"),
                     1, false, glm::value_ptr(mCamera->getMatrix("ortho")));

  testFont.RenderText(*ShaderManager::Get()->getShdr("gui_text"),
                      "CS465 Final Project", 8.0f, 24.0f, 1.0f);
  testFont.RenderText(*ShaderManager::Get()->getShdr("gui_text"),
                      "FPS: " + std::to_string((int)(1000 / mDT)) +
                          " (Ms/s: " + std::to_string(mDT) + ")",
                      8.0f, 48.0f, 1.0f);

  ShaderManager::Get()->getShdr("gui_text")->unuse();

  CommandConsole::drawConsole(*mCamera, *mInput, consoleFont);

  glEnable(GL_DEPTH_TEST);
}

void Game::Close() {
  CommandConsole::clean();
  TextureManager::Get()->Clean();
  ShaderManager::Get()->Clean();
  SoundManager::Get()->Clean();
  SoundManager::Get()->Shutdown();

  delete mTestMap;

  glDeleteTextures(1, &mDepthMap);
  glDeleteFramebuffers(1, &mFBO);
  delete mRenderer;
  delete mPlayer;
  delete mSkybox;
  delete mInput;
  delete mCamera;
  delete lightSrc;
  delete mModel;
  delete mWell;
  delete mCutout;
  delete mFPArms;
  mWindow->DestroyWindow();
  delete mWindow;
  SDL_Quit();
}

void con_glVersion(std::string args) {
  // Print the opengl context info
  std::string vendorStr, verStr, glslStr;

  verStr = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  glslStr =
      reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
  CommandConsole::runCommand("println OpenGL " + verStr);
  CommandConsole::runCommand("println GLSL " + glslStr);
}

void con_version(std::string args) {
  CommandConsole::runCommand("println Version: " + std::to_string(VER_MAJOR) +
                             "." + std::to_string(VER_MINOR) + "r" +
                             std::to_string(B_REVISION) + " build" +
                             std::to_string(BUILD_NUM) + " " + BUILD_DT);
}

void con_toggleShadow(std::string args) {
  CommandConsole::runCommand("println Toggling shadows...");
  Globals::Get()->uShadows = !Globals::Get()->uShadows;
}

void con_toggleWireframe(std::string args) {
  CommandConsole::runCommand("println Toggling wireframe...");
  Globals::Get()->uWireframe = !Globals::Get()->uWireframe;
}

void con_debugShadows(std::string args) {
  CommandConsole::runCommand("println Toggling shadow debugging...");
  Globals::Get()->uDebugShadows = !Globals::Get()->uDebugShadows;
}

void con_toggleNormalMapping(std::string args) {
  CommandConsole::runCommand("println Toggling normal maps...");
  Globals::Get()->uNormalMapping = !Globals::Get()->uNormalMapping;
}

void con_toggleAnimations(std::string args) {
  CommandConsole::runCommand("println Toggling animations...");
  Globals::Get()->uAnimation = !Globals::Get()->uAnimation;
}

void con_modVal(std::string args) {
  // Change a mod value

  // Fetch the mod val
  std::string var, input;

  if (args.find_first_of(" ") == std::string::npos) {
    // Bad value
    CommandConsole::runCommand("println No input provided");
    return;
  }

  var = args.substr(0, args.find_first_of(" "));
  input = args.substr(args.find_first_of(" ") + 1,
                      args.length() - args.find_first_of(" "));

  bool good = false;
  // Process the inputs
  if (var == "pcf_samples") {
    // Shadow samples
    int s = std::stoi(input);
    Globals::Get()->shadowPCFSamples = s;
    good = true;
  }

  if (good) {
    CommandConsole::runCommand("println " + var + " set to " + input);
  } else {
    CommandConsole::runCommand(
        "println The value '" + var +
        "' is either invalid, or was provided an invalid input");
  }
}

void con_listVal(std::string args) {
  // List all moddable values
  CommandConsole::runCommand("println \n");
  CommandConsole::runCommand("println pcf_samples");
}

void Game::playRandFootstep() {
  int rNumber = rand() % 17 + 1;
  SoundManager::Get()->playSFX("./data/sounds/footsteps/concrete_ct_" +
                               std::to_string(rNumber) + ".wav");
}