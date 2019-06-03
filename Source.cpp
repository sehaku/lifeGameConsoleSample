#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define FIELD_WIDTH 24	//�t�B�[���h��
#define FIELD_HEIGHT 24	//�t�B�[���h����

enum {//�ׂ̃Z���m�F�p
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

enum {//�Z���̐������
	CELL_DEATH,
	CELL_ALIVE,
	CELL_MAX
};


int cell[FIELD_HEIGHT][FIELD_WIDTH][2];
int cursorX, cursorY, layer;	//�J�[�\����x���W,y���W,�`��؂�ւ��player
int generationCount;		//����̃J�E���g

void judgeAliveOfDeath(int _layer) {
	/*
		���̐���̊e�Z���̐����𔻒�
		@param	_layer	���݂̃��C���[�ԍ�
		@param	_x,_y	�e�Z���̊e�����ׂ̃Z���ʒu��x���W,y���W,�t�B�[���h�͈͊O�̓��[�v����d�l
		@param	count	�e�Z���ׂ̗̃Z���̓�,�����Ă���Z���̐�
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
		//�㏑���p(Windows API),SetConsoleCursorPosition�֐�
		COORD coord;
		coord.X = coord.Y = 0;		
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

		//�`�揈��
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			for (int x = 0; x < FIELD_WIDTH; x++) {
				if ((x == cursorX) && (y == cursorY))
					printf("��");
				else if (cell[y][x][layer] == CELL_ALIVE)
					printf("��");
				else
					printf("�E");
			}
			printf("\n");
		}
		printf("GENERATION:%d\n", generationCount);
		printf("--COMMAND--\n");
		printf("Move:WASD system\n");
		printf("n:Go to the next generation\n");
		printf("c:Clear display\n");

		switch (_getch()) {
		//wasd�����ňړ�,���p�X�y�[�X�ŃJ�[�\���ʒu�̃Z���̐����؂�ւ�,n�Ő���i�s,c�ŏ�ԃN���A
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
			//���̐���̏�Ԍ���
			judgeAliveOfDeath(layer);
			//�^�[���؂�ւ�
			layer ^= 1;
			//����i�s
			generationCount++;
			break;
		case 'c':
			//�Z���̏�ԂƐ����������ԂɃN���A
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				for (int x = 0; x < FIELD_WIDTH; x++) {
					for(int i = 0; i < 2; i++) 
						cell[y][x][i] = CELL_DEATH;
				}
			}
			generationCount = 0;
			system("cls");//��ʃN���ADOS�R�}���h
			break;
		default: break;
		}
	}
}