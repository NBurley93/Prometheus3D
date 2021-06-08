#include "CommandConsole.h"
#include <ShaderManager.h>
#include <Font.h>
#include "Globals.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

bool CommandConsole::mVisible = false;
std::map<std::string, console_command> CommandConsole::mCmdList;
std::vector<std::string> CommandConsole::mConsoleList;
std::vector<std::string> CommandConsole::mHistoryList;
std::map<std::string, std::string> CommandConsole::mCmdDoc;
int CommandConsole::mSelectHistory = 0;
int CommandConsole::mWidth = 0;
int CommandConsole::mHeight = 0;
Sprite2D* CommandConsole::boxSprite = new Sprite2D();

void con_PrintLn(std::string args) {
	CommandConsole::pushList(args);
}

void con_Help(std::string args) {
	CommandConsole::runCommand("println \n");
	for (auto mit = CommandConsole::getDocs().begin(); mit != CommandConsole::getDocs().end(); mit++) {
		CommandConsole::runCommand("println " + mit->first + " " + CommandConsole::getDocs()[mit->first]);
	}
}

void con_Clear(std::string args) {
	CommandConsole::getList().clear();
	CommandConsole::getList().shrink_to_fit();
}

void con_Exit(std::string args) {
	CommandConsole::toggleVisiblility();
}

void CommandConsole::start(int w, int h) {
	CommandConsole::registerCommand("println", con_PrintLn, "[string] - Prints a line to the console");
	CommandConsole::registerCommand("help", con_Help, "- Prints this help list");
	CommandConsole::registerCommand("clear", con_Clear, "- Clears the console output");
	CommandConsole::registerCommand("exit", con_Exit, "- Closes the console");
	mVisible = false;
	mWidth = w;
	mHeight = h;

	boxSprite->Init("./data/textures/black.tga");
}

void CommandConsole::clean() {
	delete boxSprite;
}

void CommandConsole::registerCommand(std::string id, console_command pCmd, std::string helpText) {
	mCmdList[id] = pCmd;
	mCmdDoc[id] = helpText;
	std::printf("Registered %s as a new console command\n", id.c_str());
}

void CommandConsole::runCommand(std::string cmd, bool hisTrack) {
	if (hisTrack) {
		//Track this command in the history
		pushHis(cmd);
	}
	std::string args;
	//First determine if we have any args
	auto bit = cmd.find_first_of(" ");
	if (bit != std::string::npos) {
		//We have specified args, putem in
		args = cmd.substr(bit + 1, cmd.length() - bit);
		cmd = cmd.substr(0, bit);
	}

	auto mit = mCmdList.find(cmd);
	if (mit != mCmdList.end()) {
		mit->second(args);
	}
	else {
		mCmdList.find("println")->second(std::string("\"" + cmd + "\" is an invalid command"));
	}
}

void CommandConsole::pushList(std::string line) {
	mConsoleList.insert(mConsoleList.begin(), line);
}

void CommandConsole::pushHis(std::string line) {
	mHistoryList.insert(mHistoryList.begin(), line);
}

void CommandConsole::drawConsole(Camera& cam, InputManager& input, Font& drawFont) {
	if (mVisible) {
		GLShader* cShdr = ShaderManager::Get()->getShdr("gui_console");
		cShdr->use();

		glUniformMatrix4fv(cShdr->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(cam.getMatrix("ortho")));
		glUniform3f(cShdr->getUniformLocation("drawColor"), 0.0f, 0.0f, 0.0f);
		glUniform1f(cShdr->getUniformLocation("drawAlpha"), 0.5f);

		boxSprite->SetAlpha((float)(128.0f / 255.0f));
		boxSprite->Draw(cam, 0.0f, mHeight - 20.0f, mWidth, 20.0f);

		cShdr->unuse();
		cShdr->use();

		glUniformMatrix4fv(cShdr->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(cam.getMatrix("ortho")));
		glUniform3f(cShdr->getUniformLocation("drawColor"), 0.0f, 0.0f, 0.0f);
		glUniform1f(cShdr->getUniformLocation("drawAlpha"), 0.5f);

		boxSprite->SetAlpha((float)(76.0f / 255.0f));
		boxSprite->Draw(cam, 0.0f, 0.0f, mWidth, mHeight - 20.0f);

		cShdr->unuse();
		cShdr = ShaderManager::Get()->getShdr("gui_text");
		cShdr->use();

		if (Globals::Get()->input_text != "") {
			drawFont.RenderText(*ShaderManager::Get()->getShdr("gui_text"), Globals::Get()->input_text, 2, mHeight - 8.0f, 1.0f);
		}

		for (auto i = 0; i < mConsoleList.size(); i++) {
			drawFont.RenderText(*ShaderManager::Get()->getShdr("gui_text"), mConsoleList[i].c_str(), 2, (mHeight - 15) - 20 - (i * 20), 1.0f);
		}

		cShdr->unuse();
	}
}

void CommandConsole::toggleVisiblility() {
	mVisible = !mVisible;
}

bool CommandConsole::isVisible() {
	return mVisible;
}

std::vector<std::string>& CommandConsole::getList() {
	return mConsoleList;
}

std::map<std::string, std::string>& CommandConsole::getDocs() {
	return mCmdDoc;
}

std::map<std::string, console_command>& CommandConsole::getCommands() {
	return mCmdList;
}

void CommandConsole::historyUp() {
	if (mSelectHistory < mHistoryList.size()) {
		mSelectHistory += 1;
	}
}

void CommandConsole::historyDown() {
	if (mSelectHistory > 0) {
		mSelectHistory -= 1;
	}
}

std::string CommandConsole::getHistory() {
	if (mSelectHistory > 0) {
		return mHistoryList[mSelectHistory - 1];
	}
	else {
		return "";
	}
}