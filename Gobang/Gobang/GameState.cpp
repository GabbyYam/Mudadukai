#include "GameState.h"

GameState::GameState()
{
	this->init();
}

GameState::~GameState()
{
	of.close();
}

void GameState::init()
{
	// Window
	initgraph(screenWidth, screenHeight);
	
	// Board grid line
	setlinecolor(WHITE);
	setlinestyle(PS_DASH | PS_ENDCAP_ROUND, 2);

	// Board
	this->map = vector<vector<Node>>(height, vector<Node>(width));

	// logger
	of = ofstream();
	of.open("E:\\log.txt");
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
	if (is_player_turn) msg = GetMouseMsg();

	// Interval of board
	int dx = boardWidth / width,
		dy = boardHeight / height;

	// Edge of board
	int left = dx, right  = boardWidth;
	int top  = dy, bottom = boardHeight;

	int my = msg.x - left, mx = msg.y - top;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			// Check Mouse Event
			int diff = 10;
			if (is_player_turn) {
				if (mx > j* dx - diff && mx < j * dx + diff && my > i * dy - diff && my < i * dy + diff && map[i][j].hasNode == false) {

					// Click and update node state
					if (msg.mkLButton) {
						// newNode = { i , j };
						map[i][j].hasNode = true;
						map[i][j].color = false;
						is_player_turn = false;
						if (isOver()) {
							setfillcolor(RED);
							solidcircle(i * dy + left, j * dx + top, 12);
							return;
						}
					}
				}
			}

			else {
				auto node = AI_play();
				int y = node.first, x = node.second;

				map[y][x].color = true;
				map[y][x].hasNode = true;
				newNode = { y,x };
				is_player_turn = true;
				if (isOver()) {
					setfillcolor(RED);
					solidcircle(y * dy + left, x * dx + top, 12);
					return;
				}
			}

			// Draw Node
			if (map[i][j].hasNode) {

				if (i == newNode.first && j == newNode.second) {
					setfillcolor(YELLOW);
					solidcircle(i * dy + left, j * dx + top, 12);
				}
				else {
					map[i][j].color ? setfillcolor(BLACK) : setfillcolor(WHITE);
					solidcircle(i * dy + left, j * dx + top, 10);
				}
			}
		}
	}
}

void GameState::drawBG()
{
	// Background color
	setbkcolor(0x483d8b);
	cleardevice();
	setbkmode(TRANSPARENT);

	// Background
	bg = IMAGE(600, 1000);
	loadimage(&bg, _T("E:\\Wallpaper\\bk.jpg"));
	setfillstyle(BS_DIBPATTERN, NULL, &bg);
	solidrectangle(800, 100, 1100, 600);
	// solidrectangle(0, 0, 800, 800);

	// Text sytle
	RECT r = { 700, 600, 1200, 700 };
	LOGFONT f;
	gettextstyle(&f);					
	f.lfHeight = 48;					
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	drawtext(_T("Enjoy in Gobang :)"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setfillstyle(NULL);
	
	// Interval of board
	int dx = boardWidth / width,
		dy = boardHeight / height;

	// Edge of board
	int left = dx, right = left + boardWidth;
	int top = dy, bottom = top + boardHeight;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Draw grid
			line(left, top + i * dy, right - left - 6, top + i * dy);
			line(left + j * dy, top, left + j * dy, bottom - top - 6);
		}
	}

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
	RECT r = { 700, 700, 1200, 800 };
	if(!is_player_turn)
		drawtext(_T("You win!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else
		drawtext(_T("You LOSE.."), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

}

bool GameState::isOver()
{
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			if (map[i][j].hasNode) {
				if (over(i, j, map[i][j].color, 0, 0) ||
					over(i, j, map[i][j].color, 0, 1) ||
					over(i, j, map[i][j].color, 0, 2) ||
					over(i, j, map[i][j].color, 0, 3))
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
		res = over(i + 1, j + 1, color, cnt + 1, dir);
	else
		res = over(i - 1, j + 1, color, cnt + 1, dir);
	map[i][j].hasNode = true;
	return res;
}

pair<int, int> GameState::AI_play()
{
	pair<int, int> best_point = { -1, -1 };
	best_point = matchBestShape();

	if (best_point.first == -1 && best_point.second == -1) {
		int y = rand() % height;
		int x = rand() % width;
		while (map[y][x].hasNode) {
			y = rand() % height;
			x = rand() % width;
		}
		best_point = { y,x };
	}

	return best_point;
}

pair<int, int> GameState::matchBestShape()
{
	int score = -1;
	pair<int, int> best = { -1, -1 };
	for (int i = 0; i < map.size() - 1; i++) {
		for (int j = 0; j < map[i].size() - 1; j++) {
			if (!hasNeighbor(i, j))
				continue;

			if (map[i][j].hasNode == false) {
				// Set black node
				map[i][j].hasNode = true;
				map[i][j].color = true;

				// Search
				int t = -1;
				for (int k = 0; k < this->score.size(); k++)
					t = max(t, match(i, j, k, true));

				// Track-back
				map[i][j].hasNode = false;
				map[i][j].color = NULL;

				if (t > score)
					best = { i, j };
			}
		}
	}
	//if(best.first != -1)
	//	of << "New Location : " << best.first << " " << best.second << endl;
	return best;
}

pair<int, int> GameState::matchWorestShape()
{
	pair<int, int> best;
	int score = -1;
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[i].size(); j++) {
			

			if (map[i][j].hasNode == false) {
				// Set black node
				map[i][j].hasNode = true;
				map[i][j].color = true;

				// Search
				int t = -1;
				for (int k = 0; k < this->score.size(); k++)
					t = max(t, match(i, j, k, false));
				map[i][j].hasNode = false;
				map[i][j].color = NULL;

				if (t > score)
					best = { i, j };
			}
		}
	}
}

int GameState::match(int y, int x, int mode, bool color)
{

	int best = -1;
	for (int i = x, idx = 0; i < map[y].size() - 1; i++) {
		if (idx == 4) best = max(best, score[mode][0]);
		if (map[y][i].hasNode && map[y][i].color == color && score[mode][idx + 1] == 1)
			idx++;
		else if (map[y][i].hasNode == false && score[mode][idx + 1] == 0)
			idx++;
		else break;
	}

	for (int i = y, idx = 0; i < map.size() - 1; i++) {
		if (idx == 4) best = max(best, score[mode][0]);
		if (map[i][x].hasNode && map[i][x].color == color && score[mode][idx + 1] == 1)
			idx++;
		if (map[i][x].hasNode == false && score[mode][idx + 1] == 0)
			idx++;
		if (idx == 4) best = max(best, score[mode][0]);
		else break;
	}

	for (int i = y, j = x, idx = 0; i < map.size() - 1 && j < map[y].size() - 1; i++, j++) {
		if (idx == 4) best = max(best, score[mode][0]);
		if (map[i][j].hasNode && map[i][j].color == color && score[mode][idx + 1] == 1)
			idx++;
		if (map[i][j].hasNode == false && score[mode][idx + 1] == 0)
			idx++;
		if (idx == 4) best = max(best, score[mode][0]);
		else break;
	}

	for (int i = y, j = x, idx = 0; i >= 0 && j >= 0; i--, j--) {
		if (idx == 4) best = max(best, score[mode][0]);
		if (map[i][j].hasNode && map[i][j].color == color && score[mode][idx + 1] == 1)
			idx++;
		if (map[i][j].hasNode == false && score[mode][idx + 1] == 0)
			idx++;
		if (idx == 4) best = max(best, score[mode][0]);
		else break;
	}
	//if (best != -1)
	//	of << "This point (" << y << " " << x << ") 's point is " << best << endl;
	return best;
}

bool GameState::hasNeighbor(int i, int j)
{
	vector<int> dirs = { -1, 0, 1, 0, -1 };
	for (int d = 0; d < 4; d++) {
		int y = dirs[d] + i;
		int x = dirs[d + 1] + j;
		if (y < 0 || x < 0 || y >= map.size() || x >= map[0].size() || map[y][x].hasNode)
			continue;
		return true;
	}
	return false;
}
