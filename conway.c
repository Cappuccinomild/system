#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include <signal.h>

int **board, **temp;
int turn;
double speed;

typedef struct xy {
	int x;
	int y;
} xy;

void mkarr(int, int);
void nextboard(int, int);
int checkarr(int, int);
void printarr(int, int);
void title();
void setting();
void setboard();
xy moveto(int, int, char);
void printboard(int, int);
char end();
void inthandler();
void resize();

int main(int ac, char*av[]) {

	int row, col, i, j, k;
	char ch = 'r';

	struct sigaction newhandler;	/* new settings        */
	sigset_t	 blocked;	/* set of blocked sigs */

	/* load these two members first */
	newhandler.sa_handler = inthandler;	/* handler function    */

	newhandler.sa_flags = SA_RESTART;

	/* then build the list of blocked signals */
	sigemptyset(&blocked);			/* clear all bits      */
	sigaddset(&blocked, SIGQUIT);		/* add SIGQUIT to list */
	newhandler.sa_mask = blocked;		/* store blockmask     */

	if (sigaction(SIGINT, &newhandler, NULL) == -1)
		perror("sigaction");

	title();//타이틀 화면 출력
	do {
		setting();//세팅 화면 출력 (턴 수, 초)

		//창 크기 변화 대응
		row = LINES;
		col = COLS;

		mkarr(row, col);//보드 동적할당


		setboard();//세포 입력

		for (k = 0; k < turn; k++) {
			printboard(row, col);
			nextboard(row, col);
		}

		//end
		ch = end();
	} while (ch != 'q');
	endwin();
	return 0;
}

void mkarr(int row, int col) {

	int i;

	board = (int**)calloc(row + 2, sizeof(int*));
	temp = (int**)calloc(row + 2, sizeof(int*));
	for (i = 0; i < row + 2; i++) {

		board[i] = (int*)calloc(col + 2, sizeof(int));
		temp[i] = (int*)calloc(col + 2, sizeof(int));
	}

}

void nextboard(int row, int col) {

	int i, j;

	for (i = 1; i < row + 1; i++) {
		for (j = 1; j < col + 1; j++) {

			switch (checkarr(i, j)) {

			case 2://상태 유지
				temp[i][j] = board[i][j];
				break;

			case 3://살아남
				temp[i][j] = 1;
				break;

			default://죽는다
				temp[i][j] = 0;
				break;

			}

		}

	}

	for (i = 1; i < row + 1; i++) {
		for (j = 1; j < col + 1; j++) {
			board[i][j] = temp[i][j];
		}
	}

}

int checkarr(int initI, int initJ) {

	int i, j, count = 0;

	for (i = initI - 1; i <= initI + 1; i++) {

		for (j = initJ - 1; j <= initJ + 1; j++) {

			if (!(i == initI && j == initJ)) {

				if (board[i][j] == 1)count++;


			}

		}

	}
	return count;
}

void printarr(int row, int col) {

	int i, j;

	for (i = 1; i < row + 1; i++) {
		for (j = 1; j < col + 1; j++) {

			printf("%d ", board[i][j]);

		}
		printf("\n");

	}


}

void title() {//메인화면 출력
	int i, j, row = 24, col = 80;
	char arr[24][80] = { "********************************************************************************",
						"*                                                                              *",
						"*                                                              *               *",
						"*       ********  ******** **     * *   *   * ******** *     *                 *",
						"*       *         *      * * *    * *   *   * *      *  *   *                  *",
						"*       *         *      * *  *   * *   *   * *      *   * *                   *",
						"*       *         *      * *   *  * *   *   * ********    *                    *",
						"*       *         *      * *    * * *   *   * *      *    *                    *",
						"*       ********  ******** *     ** **** **** *      *    *                    *",
						"*                                                                              *",
						"*                                                                              *",
						"*    ****** ****** ******* ******    ****** ******    **  **  ******  ******   *",
						"*    *      *    * *  *  * *         *    * *         **      *       *        *",
						"*    *  *** ****** *  *  * ******    *    * ******    **  **  ******  ******   *",
						"*    *    * *    * *  *  * *         *    * *         **  **  *       *        *",
						"*    ****** *    * *  *  * ******    ****** *         **  **  *       ******   *",
						"*                                                                              *",
						"*                                                                              *",
						"*                                                                              *",
						"*                             press the key!                                   *",
						"*                                                                              *",
						"*                                                                              *",
						"*                                                                              *",
						"********************************************************************************" };

	initscr();
	clear();

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			move(i, j);
			standout();
			start_color();			/* Start color */
			init_pair(1, COLOR_BLACK, COLOR_WHITE);

			attron(COLOR_PAIR(1));
			addch(arr[i][j]);
			attroff(COLOR_PAIR(1));
			standend();
		}
	}
	refresh();
	getch();
	endwin();

}

void setting() {
	int i, j, row = 24, col = 80;
	char input;
	char input_turn[40], input_speed[40];
	char arr[24][80] = { "********************************************************************************",
						"*                                                                              *",
						"*                                                                              *",
						"*            ********  ******** ******* ******* ** *     *  ******             *",
						"*            *         *           *       *       **    *  *                  *",
						"*            ********  *******     *       *    ** * *   *  *  ***             *",
						"*                   *  *           *       *    ** *  *  *  *    *             *",
						"*            ********  ********    *       *    ** *    **  ******             *",
						"*                                                                              *",
						"*                                                                              *",
						"*                                                                              *",
						"*                          Set turn :                                          *",
						"*                                                                              *",
						"*                                                                              *",
						"*                                                                              *",
						"*                                                                              *",
						"*                          Set Speed(sec) :                                    *",
						"*                                                                              *",
						"*                                                                              *",
						"*                                                                              *",
						"*                                                                              *",
						"*                                                                              *",
						"*                                                                              *",
						"********************************************************************************" };

	initscr();

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			move(i, j);
			standout();
			start_color();			/* Start color */
			init_pair(1, COLOR_BLACK, COLOR_WHITE);

			attron(COLOR_PAIR(1));
			addch(arr[i][j]);
			attroff(COLOR_PAIR(1));
			standend();
		}
	}
	do {
		//턴 횟수 입력
		move(11, 38);
		addstr("                     ");//clear input
		move(11, 38);
		echo();
		getstr(input_turn);

		//대기시간 입력
		move(16, 44);
		addstr("                     ");//clear input
		move(16, 44);
		echo();
		getstr(input_speed);

		turn = atoi(input_turn);
		speed = atof(input_speed) * 1000000;//마이크로초단위를 초단위로

	} while ((turn <= 0 || speed <= 0));

	refresh();
	endwin();

}

void setboard() {
	int i, j, row = LINES, col = COLS;
	char ch;
	xy result;

	initscr();
	clear();
	keypad(stdscr, TRUE);
	noecho();

	//print message
	move(9, 27);
	addstr("Press Space To Set Board!");
	move(10, 27);
	addstr("Press Enter To Start Game");
	refresh();
	sleep(3);
	move(9, 27);
	addstr("                         ");
	move(10, 27);
	addstr("                         ");
	refresh();
	//init board
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			move(i, j);
			addch(' ');
		}
	}

	//start set board
	ch = '\0';
	move(0, 0);
	i = 0;
	j = 0;

	if (has_colors() == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}


	start_color();			/* Start color */
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	while (1) {

		ch = getch();
		if (ch == '\n')
			break;

		if (ch == ' ') {

			if (board[i + 1][j + 1] == 1) {
				board[i + 1][j + 1] = 0;

				standout();
				attron(COLOR_PAIR(2));
				addch(' ');
				attroff(COLOR_PAIR(2));
				standend();

			}

			else {

				standout();


				attron(COLOR_PAIR(1));
				addch(' ');
				attroff(COLOR_PAIR(1));
				standend();
				board[i + 1][j + 1] = 1;
			}

		}

		else {
			result = moveto(i, j, ch);

			move(result.x, result.y);
			i = result.x;
			j = result.y;
		}
		refresh();
	}

	endwin();

}

xy moveto(int x, int y, char key) {
	xy result;
	switch (key) {

	case 'w':
		x--;
		break;

	case 's':
		x++;
		break;

	case 'a':
		y--;
		break;

	case 'd':
		y++;
		break;

	}
	if (x < 0) {
		x = LINES - 1;
	}
	else if (x > LINES - 1) {
		x = 0;
	}

	if (y > COLS - 1)
		y = 0;
	else if (y < 0)
		y = COLS - 1;

	result.x = x;
	result.y = y;

	return result;
}

void printboard(int row, int col) {
	int i, j;

	initscr();
	clear();
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			move(i, j);
			standout();

			if (board[i + 1][j + 1] == 0) {

				if (has_colors() == FALSE)
				{
					endwin();
					printf("Your terminal does not support color\n");
					exit(1);
				}
				start_color();			/* Start color */

				init_pair(2, COLOR_BLACK, COLOR_RED);
				attron(COLOR_PAIR(2));
				addstr(" ");
				attroff(COLOR_PAIR(2));

			}
			else {
				addstr(" ");
			}
			standend();
		}

	}
	refresh();
	usleep(speed);
	move(0, 0);
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)addstr(" ");

	move(1, 1);

}

char end() {

	move(9, 27);
	addstr("Game End!");
	move(10, 27);
	addstr("restart : press any key     exit : q");
	refresh();

	return getch();

}

void inthandler() {

	setting();

}

void resize() {//resize

	int i;

	board = (int**)realloc(board, sizeof(int*) * LINES);
	temp = (int**)realloc(temp, sizeof(int*) * LINES);

	for (i = 0; i < LINES; i++) {
		board[i] = (int*)realloc(board[i], sizeof(int) * COLS);
		temp[i] = (int*)realloc(temp[i], sizeof(int) * COLS);
	}

}