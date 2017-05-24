#pragma once
#include <Light.h>
#include <Model.h>
#include <string>

class GameMap {
public:
	void Load(std::string mapFile);
	void Render(GLShader* shdr, bool uTexturing);
	~GameMap();

	Light* getSun();
	Model* getModel();
private:
	Model* mapMesh;
	Light mapSun;
};