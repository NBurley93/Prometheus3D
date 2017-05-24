#pragma once
#include <map>
#include <vector>
#include <string>
#include <Font.h>
#include <InputManager.h>
#include <Sprite2D.h>

typedef void(*console_command)(std::string); //Function pointer for console command

class CommandConsole {
public:
	static void start(int w, int h);
	static void runCommand(std::string cmd, bool hisTrack = false);
	static void drawConsole(Camera& cam, InputManager& input, Font& drawFont);
	static void toggleVisiblility();
	static void registerCommand(std::string id, console_command pCmd, std::string helpText = "");
	static void pushHis(std::string line);
	static void pushList(std::string line);
	static bool isVisible();
	static void clean();
	
	static std::map<std::string, std::string>& getDocs();
	static std::vector<std::string>& getList();
	static std::map<std::string, console_command>& getCommands();

	static void historyUp();
	static void historyDown();
	static std::string getHistory();

private:
	static bool mVisible;
	static std::map<std::string, console_command> mCmdList;
	static std::map<std::string, std::string> mCmdDoc;
	static std::vector<std::string> mConsoleList;
	static std::vector<std::string> mHistoryList;
	static int mSelectHistory;
	static int mWidth, mHeight;
	static Sprite2D* boxSprite;
};

void con_PrintLn(std::string args);
void con_Help(std::string args);
void con_Clear(std::string args);
void con_Exit(std::string args);