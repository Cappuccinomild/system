#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>

int **board, **temp;

void mkarr(int, int);
void nextboard(int , int);
int checkarr(int , int);
void printarr(int, int);

int main(int ac, char*av[]){

    int row, col, i, j, k;

    row = atoi(av[1]);
    col = atoi(av[2]);

    mkarr(row, col);
    board[6][6] = 1;
    board[7][7] = 1;
    board[5][5] = 1;
    board[6][5] = 1;

    initscr();
    clear();
    for(k = 0 ; k < 10 ; k ++){

        for(i = 1 ; i < row ; i ++){
            for(j = 1 ; j < col ; j ++){
                move(i, j);
                standout();
                
                if(board[i][j] == 0) {

                    if(has_colors() == FALSE)
                        {   endwin();
                            printf("Your terminal does not support color\n");
                            exit(1);
                        }
                    start_color();         /* Start color          */
                    init_pair(1, COLOR_RED, COLOR_BLACK);

                    attron(COLOR_PAIR(1));
                    addstr(" ");
                    attroff(COLOR_PAIR(1));
                    
                }
                else addstr(" ");
                standend();
            }
            
        }
        refresh();
        sleep(1);
        move(1, 1);
        for(i = 1 ; i < row ; i ++)
            for(j = 1 ; j < col ; j ++)addstr(" ");
        move(1,1);
        nextboard(row, col);
    }
    endwin();
    return 0;
}

void mkarr(int row , int col){

    int i;

    board = (int**)calloc( row + 2 , sizeof(int*) );
    temp = (int**)calloc( row + 2 , sizeof(int*) );
    for(i = 0 ; i < row + 2; i ++){

        board[i] = (int*)calloc(col + 2 ,sizeof(int));
        temp[i] = (int*)calloc(col + 2 ,sizeof(int));
    }

}

void nextboard(int row, int col){

    int i, j;

    for(i = 1 ; i < row+1 ; i ++){
        for(j = 1 ; j < col+1 ; j ++){
            
            switch(checkarr(i, j)){
                
                case 2://���� ����
                    break;

                case 3://��Ƴ�
                    temp[i][j] = 1;
                    break;

                default://�״´�
                    temp[i][j] = 0;
                    break;

            }

        }

    }

    for(i = 1 ; i < row+1 ; i ++){
        for(j = 1 ; j < col+1 ; j ++){
            board[i][j] = temp[i][j];
        }
    }

}

int checkarr(int initI, int initJ){

    int i, j, count = 0;

    for(i = initI - 1;  i <= initI + 1; i ++){

        for(j = initJ - 1 ; j <= initJ + 1 ; j++){

            if(!(i == initI && j == initJ)){

                if( board[i][j] == 1 )count++;


            }

        }

    }
    return count;
}

void printarr(int row, int col){

    int i, j;

    for(i = 1 ; i < row+1 ; i ++){
        for(j = 1; j < col+1 ; j ++){

            printf("%d ", board[i][j]);

        }
        printf("\n");

    }


}