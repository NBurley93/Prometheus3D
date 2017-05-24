#include "GameMap.h"
#include <fstream>

/*

MAP_FORMAT:

<FILE>
	<HEADER>
		<MAP_NAME>
		<MAP_MESH_PATH>
		<MAP_SUN_POSITION>
	</HEADER>
</FILE>

*/

glm::vec3 convertInputToVec3(std::string input) {
	//First trim off the parentheses

	input = input.substr(1, input.length() - 3);

	float inputs[3];

	for (int i = 0; i < 3; i++) {
		std::string rawInput = input.substr(0, input.find_first_of(","));
		input = input.substr(input.find_first_of(",") + 1, input.length());

		inputs[i] = std::stof(rawInput.c_str());
	}

	return glm::vec3(inputs[0], inputs[1], inputs[2]);
}

void GameMap::Load(std::string mapFile) {
	std::ifstream iMap(mapFile);
	if (iMap.fail()) {
		perror(mapFile.c_str());
		return;
	}

	std::string rawLine;
	std::string key, value;

	std::string mapName;
	glm::vec3 sunPos, sunAmb, sunDiff, sunSpec;
	std::string meshPath;

	while (std::getline(iMap, rawLine)) {
		key = rawLine.substr(0, rawLine.find_first_of("="));
		value = rawLine.substr(rawLine.find_first_of("\"") + 1, rawLine.find_last_of("\"") - rawLine.find_first_of("\"") - 1);

		//Process header entries
		if (key == "name") {
			mapName = value;
		}
		if (key == "mesh") {
			meshPath = "./data/models/" + value;
		}
		if (key == "sun_pos") {
			//Process vec3
			sunPos = convertInputToVec3(value);
		}
		if (key == "sun_ambient") {
			//Process vec3
			sunAmb = convertInputToVec3(value);
		}

		if (key == "sun_diffuse") {
			//Process vec3
			sunDiff = convertInputToVec3(value);
		}
		if (key == "sun_specular") {
			//Process vec3
			sunSpec = convertInputToVec3(value);
		}
	}

	mapMesh = new Model();
	mapMesh->Load(meshPath.c_str());

	//Light definitions
	mapSun.direction = glm::vec4(sunPos.x, sunPos.y, sunPos.z, 0.0f);
	mapSun.ambient = sunAmb;
	mapSun.diffuse = sunDiff;
	mapSun.specular = sunSpec;

	std::printf("Loaded map %s\n", mapName.c_str());
}

void GameMap::Render(GLShader* shdr, bool uTexturing) {
	mapMesh->Translate(glm::vec3(-0.5f, 0.0f, -0.5f));
	mapMesh->Rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mapMesh->Scale(glm::vec3(3.0f, 3.0f, 3.0f));
	mapMesh->Draw(*shdr, uTexturing, true);
}

GameMap::~GameMap() {
	delete mapMesh;
}

Light* GameMap::getSun() {
	return &mapSun;
}

Model* GameMap::getModel() {
	return mapMesh;
}