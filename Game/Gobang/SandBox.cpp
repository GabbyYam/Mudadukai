#include<iostream>
#include<vector>
#include<xmemory>
#include"GameState.h"

using namespace std;


bool turn = false;

int main() {
	shared_ptr<GameState> gs(new GameState);
	// GameState* gs = new GameState;
	// AIPlayer* ap = new AIPlayer;

	// ap->init();
	gs->init();
	setbkcolor(YELLOW);
	setbkmode(0);
	while (true) {
		
		gs->drawBG();
		gs->drawMap();
		if (gs->isOver()) {
			gs->printWinnerInfo();
			system("pause");
		}
	}
	gs->printWinnerInfo();
	// ap->release();
	gs->release();
}