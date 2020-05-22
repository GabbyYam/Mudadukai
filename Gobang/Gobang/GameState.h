#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<fstream>
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

	const int width = 15;
	const int height = 15;
	const int boardWidth = 700;
	const int boardHeight = 700;

	const int screenWidth = 1200;
	const int screenHeight = 800;

	std::pair<int, int> newNode = { -1, -1 };
	vector<vector<Node>> map;

	IMAGE bg;

	vector<vector<int>> score = {
	{50  , 1, 1, 0, 0, 0},
	{50  , 0, 1, 1, 0, 0},
	{50  , 0, 0, 1, 1, 0},
	{50  , 0, 0, 0, 1, 1},
	{1000, 0, 1, 1, 1, 0},
	{500, 1, 1, 0, 1, 0},
	{500 , 0, 0, 1, 1, 1},
	{500 , 1, 1, 1, 0, 0},
	// 4
	{5000, 0, 1, 1, 1, 1},
	{5000, 1, 0, 1, 1, 1},
	{5000, 1, 1, 0, 1, 1},
	{5000, 1, 1, 1, 0, 1},
	{5000, 1, 1, 1, 1, 0},
	// 5
	{999999 , 1, 1, 1, 1, 1}
	};

	ofstream of;


private:
	// CP-turn
	pair<int, int> AI_play();

	pair<int, int> matchBestShape();
	pair<int, int> matchWorestShape();
	int match(int, int, int, bool);

	bool hasNeighbor(int, int);

};

