// week4 re-assignment.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <bangtal.h>

#include <time.h>
#include <stdlib.h>

SceneID scene1;
ObjectID puzzle_board[6], init_puzzle[6];
ObjectID startbutton;
ObjectID hint, hintpicture;

const char* puzzle_image[6] = {
	"1_1.png",
	"1-1_1.png",
	"2_1.png",
	"2-1_1.png",
	"3_1.png",
	"3-1_1.png"
};

const int puzzle_x[6] = { 35, 35, 398, 398, 758, 758 };
const int puzzle_y[6] = { 24, 387, 24, 387, 26, 383 }; 

int blank;

TimerID timer;
int mixCount;

int puzzle_index(ObjectID object) { 
	for (int i = 0; i < 6; i++) {
		if (puzzle_board[i] == object) return i;
	}
	return -1;
}

void puzzle_move(int index) {
	ObjectID t = puzzle_board[blank];
	puzzle_board[blank] = puzzle_board[index]; 
	puzzle_board[index] = t; 

	locateObject(puzzle_board[blank], scene1, puzzle_x[blank], puzzle_y[blank]);
	locateObject(puzzle_board[index], scene1, puzzle_x[index], puzzle_y[index]);

	blank = index;
}

bool movable(int index) {
	if (blank < 0 or blank > 6) return false;
	if (blank % 4 != 0 and blank - 1 == index) return true;
	if (blank % 4 != 3 and blank + 1 == index) return true;
	if (blank / 4 != 0 and blank - 4 == index) return true;
	if (blank / 4 != 3 and blank + 4 == index) return true;

	return false;
}

void puzzle_mix() {
	int index;


	do {
		switch (rand() % 4) {
		case0: index = blank - 1; break;
		case1: index = blank + 1; break;
		case2: index = blank - 4; break;
		case3: index = blank + 4; break;
		}
	} while (!movable(index));
	puzzle_move(index);
}

bool completed() {
	for (int i = 0; i < 6; i++) {
		if (puzzle_board[i] != init_puzzle[i])  return false;
	}

	return true;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	int index = puzzle_index(object);
	puzzle_move(index);

	if (object == startbutton) {
		mixCount = 10;

		setTimer(timer, 1.f);
		startTimer(timer);

	}
	else {
		int index = puzzle_index(object);
		if (movable(index))
			puzzle_move(index);

		if (completed()) {
			showMessage("완료됨 !!");
		   }
	}
}

void timerCallback(TimerID timer) {
	mixCount--;

	if (mixCount > 0) {
		puzzle_mix();

		setTimer(timer, 1.f);
		startTimer(timer);
	}
}


int main()
{
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene1 = createScene("스폰지밥 퍼즐 맞추기", "퍼즐배경.png");


	for (int i = 0; i < 6; i++) {
		puzzle_board[i] = createObject(puzzle_image[i]);
		init_puzzle[i] = puzzle_board[i];
		locateObject(puzzle_board[i], scene1, puzzle_x[i], puzzle_y[i]);
		showObject(puzzle_board[i]);
	}

	blank = 5;
	hideObject(puzzle_board[blank]);
	


	startbutton = createObject("시작버튼.png");
	locateObject(startbutton, scene1, 800, 800);
	showObject(startbutton);

	timer = createTimer(1.f);

	hint = createObject("hint.png");
	locateObject(hint, scene1, 670, 668);
	showObject(hint);

	hintpicture = createObject("hintpicture.png");
	locateObject(hintpicture, scene1, 835, 680);
	hideObject(hintpicture);
	
	startGame(scene1);

	return 0;
	


}