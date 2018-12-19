#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int **board, **temp;

void mkarr(int, int);
void nextboard(int, int);
int checkarr(int, int);
void printarr(int, int);
void title();

int main(int ac, char*av[]) {

	int row, col, i;

	row = atoi(av[1]);
	col = atoi(av[2]);

	mkarr(row, col);
	board[6][6] = 1;
	board[7][7] = 1;
	board[5][5] = 1;
	board[6][5] = 1;

	for (i = 0; i < 10; i++) {
		nextboard(row, col);
		sleep(1);
		printarr(row, col);
	}

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
