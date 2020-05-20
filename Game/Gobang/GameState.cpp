#include "GameState.h"

GameState::GameState()
{
	this->init();
}

GameState::~GameState()
{
}

void GameState::init()
{
	
	initgraph(screenWidth, screenHeight);
	loadimage(&bg, _T("E:\\Wallpaper\\bg.jpg"));
	

	this->map = vector<vector<Node>>(height, vector<Node>(width));
}

void GameState::release()
{
}

void GameState::updateState()
{

}

void GameState::drawMap()
{
	MOUSEMSG msg;
	if (is_player_turn)
		msg = GetMouseMsg();
	else
		AI_play();

	// Interval of board
	int dx = boardWidth / width,
		dy = boardHeight / height;

	// Edge of board
	int left = dx * 3, right  = boardWidth;
	int top  = dy * 3, bottom = boardHeight;

	int my = msg.x - left, mx = msg.y - top;
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {

			// Check Mouse Event
			int diff = 6;
			if (mx > j* dx - diff && mx < j * dx + diff && my > i * dy - diff && my < i * dy + diff && map[i][j].hasNode == false) {

				// Click
				if (msg.mkLButton) {
					map[i][j].color = is_white;
					map[i][j].hasNode = true;
					newNode = { i , j };
					// is_player_turn = !is_player_turn;
					is_white = !is_white;
				}
			}


			// Draw Node
			if (map[i][j].hasNode) {
				if (i == newNode.first && j == newNode.second) {
					setfillcolor(GREEN);
					solidcircle(i * dy + left, j * dx + top, 7);
				}
				else {
					map[i][j].color ? setfillcolor(YELLOW) : setfillcolor(WHITE);
					solidcircle(i * dy + left, j * dx + top, 6);
				}
			}

			// Draw grid
			line(left, top + i * dy, right, top + i * dy);
			line(left + j * dy, top, left + j * dy, bottom);
		}
	}
}

void GameState::drawBG()
{
	setfillstyle(BS_DIBPATTERN, NULL, &bg);
	solidrectangle(200, 700, 700, 100);
	setfillstyle(NULL);
}

int GameState::getWidth()
{
	return this->width;
}

int GameState::getHeight()
{
	return this->height;
}

void GameState::printWinnerInfo()
{
	RECT r = { 1000, 800, 1200, 600 };
	// drawtext(_T("Game Over !"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	if(is_player_turn)
		drawtext(_T("You win!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else
		drawtext(_T("oh~~ You lose~"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

bool GameState::isOver()
{
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			if (map[i][j].hasNode) {
				if (over(i, j, map[i][j].color, 0, 0) ||
					over(i, j, map[i][j].color, 0, 1) ||
					over(i, j, map[i][j].color, 0, 2))
					return true;
			}
		}
	}
	return false;
}

bool GameState::over(int i, int j, bool color, int cnt, int dir) {
	if (cnt == 5) return true;
	if (i < 0 || j < 0 || i >= map.size() || j > map[i].size() ||
		map[i][j].hasNode == false || map[i][j].color != color)
		return false;

	bool res = false;
	map[i][j].hasNode = false;
	if (dir == 0)
		res = over(i, j + 1, color, cnt + 1, dir);
	else if (dir == 1)
		res = over(i + 1, j, color, cnt + 1, dir);
	else if (dir == 2)
		res = over(i + 1, j + 1, color, cnt + 1, dir) || over(i - 1, j + 1, color, cnt + 1, dir);
	map[i][j].hasNode = true;
	return res;
}

void GameState::AI_play()
{
	if (is_player_turn) return;

}
