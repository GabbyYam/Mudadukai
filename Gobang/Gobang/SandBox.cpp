#include<iostream>
#include<vector>
#include<conio.h>
#include"GameState.h"

using namespace std;


bool turn = false;

int main() {
	shared_ptr<GameState> gs(new GameState);
	// GameState* gs = new GameState;
	// AIPlayer* ap = new AIPlayer;

	// ap->init();
	gs->init();
	gs->drawBG();
	while (true) {
		
		gs->drawMap();
		if (gs->isOver()) {
			gs->printWinnerInfo();
			system("pause");
		}
	}

	gs->release();

}