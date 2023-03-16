#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>
using namespace std;

enum Cell : char {
	ZERO = 'O',
	CROSS = 'X',
	EMPTY = '_'
};

struct XYgrid {
	size_t y{0};
	size_t x{0};
};

enum GameState {
	IN_PROGRESS,
	PLAYER_VICTORY,
	AI_VICTORY,
	DRAW
};

#pragma pack(push, 1)
struct GAME {
	Cell** ppField = nullptr;
	const size_t sizeField = 3;
	Cell PLAYER;
	Cell AI;
	size_t TURN = 0;
	GameState currentstate = IN_PROGRESS;
};
#pragma pack(pop)

void clear()
{
	system("cls");
}

int32_t __fastcall getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

void __fastcall ini(GAME& game)
{
	game.ppField = new Cell * [game.sizeField];
	for (size_t i = 0; i < game.sizeField; i++)
	{
		game.ppField[i] = new Cell[game.sizeField];
	}
	for (size_t y = 0; y < game.sizeField; y++)
	{
		for (size_t x = 0; x < game.sizeField; x++)
		{
			game.ppField[y][x] = EMPTY;
		}
	}
	if (getRandomNum(0, 1000) > 500)
	{
		game.PLAYER = CROSS;
		game.AI = ZERO;
		game.TURN = 0;
	}
	else
	{
		game.PLAYER = ZERO;
		game.AI = CROSS;
		game.TURN = 1;
	}

	return;
}

void __fastcall deini(GAME& game)
{
	for (size_t i = 0; i < game.sizeField; i++)
	{
		delete[] game.ppField[i];
	}
	delete[] game.ppField;
	game.ppField = nullptr;
	return;
}

void __fastcall drawfield(GAME& game)
{
	cout << endl << "    ";
	for (size_t x = 0; x < game.sizeField; x++)
	{
		cout << x + 1 << "   ";
	}
	cout << endl;
	for (size_t y = 0; y < game.sizeField; y++)
	{
		cout << y + 1 << " | ";
		for (size_t x = 0; x < game.sizeField; x++)
		{
			cout << game.ppField[y][x] << " | ";
		}
		cout << endl;
	}
	cout << endl << "Player: " << game.PLAYER << endl << "Computer: " << game.AI << endl;
	return;
}

GameState __fastcall getstate(GAME& game)
{
	for (size_t y = 0; y < game.sizeField; y++)
	{
		if (game.ppField[y][0] == game.ppField[y][1] and game.ppField[y][0] == game.ppField[y][2])
		{
			if (game.ppField[y][0] == game.PLAYER)
				return PLAYER_VICTORY;
			
			if (game.ppField[y][0] == game.AI)
				return AI_VICTORY;
		}
	}

	for (size_t x = 0; x < game.sizeField; x++)
	{
		if (game.ppField[0][x] == game.ppField[1][x] and game.ppField[0][x] == game.ppField[2][x])
		{
			if (game.ppField[0][x] == game.PLAYER)
				return PLAYER_VICTORY;

			if (game.ppField[0][x] == game.AI)
				return AI_VICTORY;
		}
	}

	if (game.ppField[0][0] == game.ppField[1][1] and game.ppField[0][0] == game.ppField[2][2])
	{
		if (game.ppField[1][1] == game.PLAYER)
			return PLAYER_VICTORY;

		if (game.ppField[1][1] == game.AI)
			return AI_VICTORY;
	}

	if (game.ppField[2][0] == game.ppField[1][1] and game.ppField[1][1] == game.ppField[0][2])
	{
		if (game.ppField[1][1] == game.PLAYER)
			return PLAYER_VICTORY;

		if (game.ppField[1][1] == game.AI)
			return AI_VICTORY;
	}

	bool draw = true;
	for (size_t y = 0; y < game.sizeField; y++)
	{
		for (size_t x = 0; x < game.sizeField; x++)
		{
			if (game.ppField[y][x] == EMPTY)
			{
				draw = false;
				break;
			}
		}
		if (!draw)
		{
			break;
		}
		if (draw)
		{
			return DRAW;
		}
	}
	return IN_PROGRESS;
}

XYgrid __fastcall getplayerturn(GAME& game)
{
	XYgrid c;
	do
	{
		cout << "Enter x (1...3): ";
		cin >> c.x;
		cout << "Enter y (1...3): ";
		cin >> c.y;
		c.x--;
		c.y--;
	} while (c.x > 2 or c.y > 2 or game.ppField[c.y][c.x] != EMPTY);
	return c;
}

XYgrid __fastcall getaiturn(GAME& game)
{
	if (game.ppField[1][1] == EMPTY)
	{
		return{ 1,1 };
	}

	XYgrid arr[4];
	size_t num = 0;
	if (game.ppField[0][0] == EMPTY)
	{
		arr[num++] = { 0,0 };
	}
	if (game.ppField[2][2] == EMPTY)
	{
		arr[num++] = { 2,2 };
	}
	if (game.ppField[2][0] == EMPTY)
	{
		arr[num++] = { 2,0 };
	}
	if (game.ppField[0][2] == EMPTY)
	{
		arr[num++] = { 0,2 };
	}
	if (num > 0)
	{
		const size_t index = getRandomNum(0, 1000) % num;
		return arr[index];
	}

	if (game.ppField[0][1] == EMPTY)
	{
		arr[num++] = { 0,1 };
	}
	if (game.ppField[2][1] == EMPTY)
	{
		arr[num++] = { 2,1 };
	}
	if (game.ppField[1][0] == EMPTY)
	{
		arr[num++] = { 1,0 };
	}
	if (game.ppField[1][2] == EMPTY)
	{
		arr[num++] = { 1,2 };
	}
	if (num > 0)
	{
		const size_t index = getRandomNum(0, 1000) % num;
		return arr[index];
	}
}

void __fastcall ending(GAME game)
{
	if (game.currentstate == PLAYER_VICTORY)
	{
		cout << "Winner: PLAYER" << endl;
	}
	else if (game.currentstate == AI_VICTORY)
	{
		cout << "Winner: AI" << endl;
	}
	else if (game.currentstate == DRAW)
	{
		cout << "Draw";
	}
	return;
}





int main()
{
	GAME game;
	ini(game);
	clear();
	drawfield(game);
	do {
		if (game.TURN % 2 == 0)
		{
			XYgrid c = getplayerturn(game);
			game.ppField[c.y][c.x] = game.PLAYER;
		}

		else
		{
			XYgrid c = getaiturn(game);
			game.ppField[c.y][c.x] = game.AI;
		}

		clear();
		drawfield(game);
		game.TURN++;
		game.currentstate = getstate(game);

	} while (game.currentstate == IN_PROGRESS);
	ending(game);
	deini(game);
	return 0;
}
