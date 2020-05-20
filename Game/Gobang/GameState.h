#pragma once
#include<iostream>
#include<vector>
#include<graphics.h>
using namespace std;

struct Node {
	bool color = false;
	bool hasNode = false;
};

class GameState
{
public:
	GameState();
	~GameState();

	// init param
	void init();
	void release();

	// update info & draw map
	void updateState();
	void drawMap();
	void drawBG();

	// getter
	int  getWidth();
	int  getHeight();

	// Game-over Info
	void printWinnerInfo();

	// isover
	bool isOver();
	bool over(int, int, bool, int, int);

private:
	bool is_player_turn = true;
	bool is_white = false;

	const int width = 31;
	const int height = 31;
	const int boardWidth = 700;
	const int boardHeight = 700;

	const int screenWidth = 1200;
	const int screenHeight = 800;

	std::pair<int, int> newNode = { -1, -1 };
	vector<vector<Node>> map;

	IMAGE bg;

	enum WINNER {
		AI, Player, God
	};


private:
	// CP-turn
	void AI_play();

	// Player-turn
	void parseInput();


};

