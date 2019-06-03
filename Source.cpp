#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define FIELD_WIDTH 24	//フィールド幅
#define FIELD_HEIGHT 24	//フィールド高さ

enum {//隣のセル確認用
	DIRECTION_UP,
	DIRECTION_UP_LEFT,
	DIRECTION_LEFT,
	DIRECTION_DOWN_LEFT,
	DIRECTION_DOWN,
	DIRECTION_DOWN_RIGHT,
	DIRECTION_RIGHT,
	DIRECTION_UP_RIGHT,
	DIRECTION_MAX
};

int direction[][2]={
	{0,-1},//DIRECTION_UP
	{-1,-1},//DIRECTION_UP_LEFT
	{-1,0},//DIRECTION_LEFT
	{-1,1},//DIRECTION_DOWN_LEFT
	{0,1},//DIRECTION_DOWN
	{1,1},//DIRECTION_DOWN_RIGHT
	{1,0},//DIRECTION_RIGHT
	{1,-1}//DIRECTION_UP_RIGHT
};

enum {//セルの生存状態
	CELL_DEATH,
	CELL_ALIVE,
	CELL_MAX
};


int cell[FIELD_HEIGHT][FIELD_WIDTH][2];
int cursorX, cursorY, layer;	//カーソルのx座標,y座標,描画切り替え用layer
int generationCount;		//世代のカウント

void judgeAliveOfDeath(int _layer) {
	/*
		次の世代の各セルの生死を判定
		@param	_layer	現在のレイヤー番号
		@param	_x,_y	各セルの各方向隣のセル位置のx座標,y座標,フィールド範囲外はループする仕様
		@param	count	各セルの隣のセルの内,生きているセルの数
	*/
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			int count = 0;
			for (int dir = 0; dir < DIRECTION_MAX; dir++) {
				int _x = x, _y = y;
				_x = (FIELD_WIDTH + _x + direction[dir][0]) % FIELD_WIDTH;
				_y = (FIELD_WIDTH + _y + direction[dir][1]) % FIELD_WIDTH;
				if (cell[_y][_x][_layer] == CELL_ALIVE)
					count++;
			}
			if (count == 3)
				cell[y][x][_layer ^ 1] = CELL_ALIVE;
			else if ((cell[y][x][_layer] == CELL_ALIVE) && (count == 2))
				cell[y][x][_layer ^ 1] = CELL_ALIVE;
			else if ((cell[y][x][_layer] == CELL_ALIVE) && (count <= 1))
				cell[y][x][_layer ^ 1] = CELL_DEATH;
			else if ((cell[y][x][_layer] == CELL_ALIVE) && (count >= 4))
				cell[y][x][_layer ^ 1] = CELL_DEATH;
			else cell[y][x][_layer ^ 1] = CELL_DEATH;
		}
	}
}


int main() {
	while (1) {
		//上書き用(Windows API),SetConsoleCursorPosition関数
		COORD coord;
		coord.X = coord.Y = 0;		
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

		//描画処理
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			for (int x = 0; x < FIELD_WIDTH; x++) {
				if ((x == cursorX) && (y == cursorY))
					printf("◎");
				else if (cell[y][x][layer] == CELL_ALIVE)
					printf("■");
				else
					printf("・");
			}
			printf("\n");
		}
		printf("GENERATION:%d\n", generationCount);
		printf("--COMMAND--\n");
		printf("Move:WASD system\n");
		printf("n:Go to the next generation\n");
		printf("c:Clear display\n");

		switch (_getch()) {
		//wasd方式で移動,半角スペースでカーソル位置のセルの生死切り替え,nで世代進行,cで状態クリア
		case 'w':
			cursorY--;
			break;
		case 's':
			cursorY++;
			break;
		case 'a':
			cursorX--;
			break;
		case 'd':
			cursorX++;
			break;
		case ' ':
			cell[cursorY][cursorX][layer] ^= 1;
			break;
		case 'n':
			//次の世代の状態決定
			judgeAliveOfDeath(layer);
			//ターン切り替え
			layer ^= 1;
			//世代進行
			generationCount++;
			break;
		case 'c':
			//セルの状態と世代を初期状態にクリア
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				for (int x = 0; x < FIELD_WIDTH; x++) {
					for(int i = 0; i < 2; i++) 
						cell[y][x][i] = CELL_DEATH;
				}
			}
			generationCount = 0;
			system("cls");//画面クリアDOSコマンド
			break;
		default: break;
		}
	}
}