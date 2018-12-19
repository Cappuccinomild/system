#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>


int **board, **temp;

typedef struct xy {
	int x;
	int y;
} xy;

void mkarr(int, int);
void nextboard(int, int);
int checkarr(int, int);
void printarr(int, int);
void title();
void setboard()
xy moveto(int, int, char);

int main(int ac, char*av[]) {

	int row, col, i, j, k;

	title();
	setting();


	row = atoi(av[1]);
	col = atoi(av[2]);

	mkarr(row, col);

	setboard();

	initscr();
	clear();
	for (k = 0; k < turn; k++) {

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
					start_color();         /* Start color          */
					init_pair(1, COLOR_RED, COLOR_BLACK);

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
		nextboard(row, col);
	}
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
			start_color();         /* Start color */
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


	start_color();			/* Start color 			*/
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
}}