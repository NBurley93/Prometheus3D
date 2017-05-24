#include "Game.h"

int main(int argc, char* argv[]) {
	Game* mGame = new Game();
	mGame->Exec();
	delete mGame;
	return 0;
}