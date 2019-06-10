//chagit stupel 209089960

#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wait.h>

#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define DOWN_KEY 's'
#define FLIP_KEY 'w'
#define QUIT_KEY 'q'
#define SIZE 20
#define WRITE_ERROR write(fileno(stderr), "Error in system call\n", strlen("Error in system call\n"));


struct pos {
    int row, column;
} pos;

struct rectangle {
    struct pos left, middle, right; //left, middle and right position
} gameRectangle;

struct gameRectangle; //the rectangle
char table[20][20]; //game table
int running;

/**
 * drawing the board
 */
void DrawingBoard() {
    //clear the board
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            table[i][j] = ' ';
        }
    }
    //draw the walls
    for (i = 0; i < SIZE; i++) {
        table[i][0] = '*';
        table[i][SIZE - 1] = '*';
        //if its the bottom line
        if (i == SIZE - 1) {
            //draw the flore
            for (j = 0; j < SIZE; j++) {
                table[(SIZE - 1)][j] = '*';
            }
        }
    }
}

/**
 * printing board to the screen
 */
//todo put it in the draw rectangle function?
void PrintingBoard() {
    int row, colum;
    for (row = 0; row < SIZE; row++) {
        for (colum = 0; colum < SIZE; colum++) {
            if (write(STDOUT_FILENO, &table[row][colum], sizeof(table[row][colum])) == -1)
                WRITE_ERROR
        }
        if (write(STDOUT_FILENO, "\n", 1) == -1)
            WRITE_ERROR
    }
}

/**
 * put the rectangle in the default place on top of the border
 */
void initializeRectanglePos() {
    //initialize rectangle row place to the top of the border - first line
    gameRectangle.left.row = 0;
    gameRectangle.middle.row = 0;
    gameRectangle.right.row = 0;

    //initialize rectangle column
    gameRectangle.left.column = (SIZE / 2) - 3; // index 7 (place 8)
    gameRectangle.middle.column = (SIZE / 2) - 2; //index 8 (place 9)
    gameRectangle.right.column = (SIZE / 2) - 1; //index 9 (place 10)
}

/**
 * update the positions of the rectangle in the board
 */
void updateRectangleInBoard() {
    //update the left position in the board
    table[gameRectangle.left.row][gameRectangle.left.column] = '-';

    //update the middle position in the board
    table[gameRectangle.middle.row][gameRectangle.middle.column] = '-';

    //update the right position in the board
    table[gameRectangle.right.row][gameRectangle.right.column] = '-';
}

/**
 * remove the rectangle shape from the board
 */
void removeRectangleFromBoard() {
    //remove the left position from the board
    table[gameRectangle.left.row][gameRectangle.left.column] = ' ';

    //remove the middle position from the board
    table[gameRectangle.middle.row][gameRectangle.middle.column] = ' ';

    //remove the right position from the board
    table[gameRectangle.right.row][gameRectangle.right.column] = ' ';
}

/**
 *
 * @param leftPos the place of the left pos
 * @param rightPos the place of the right pos
 * @return if the rectangle can move according to the output
 */
int canMove(int leftPos, int rightPos) {
    if (leftPos > 0 && rightPos < SIZE - 1)
        return 1;
    return 0;
}

/**
 *
 * @param key the key that the user pressed
 * update the place of the rectangle according to the key
 */
void UpdateShapeLocation(int key) {
    //move the rectangle on the board according to the key

    switch (key){
        case RIGHT_KEY: {
            //first check if can move to the place the user wants
            if (canMove(gameRectangle.left.column + 1, gameRectangle.right.column + 1)) {
                removeRectangleFromBoard();
                //update the place of the new rectangle
                gameRectangle.left.column = gameRectangle.left.column + 1;
                gameRectangle.middle.column = gameRectangle.middle.column + 1;
                gameRectangle.right.column = gameRectangle.right.column + 1;
                //puting the new rectangle in the board
                updateRectangleInBoard();
            }
        }
            break;
        case LEFT_KEY: {
            if (canMove(gameRectangle.left.column - 1, gameRectangle.right.column - 1)) {
                removeRectangleFromBoard();
                //update the place of the new rectangle
                gameRectangle.left.column = gameRectangle.left.column - 1;
                gameRectangle.middle.column = gameRectangle.middle.column - 1;
                gameRectangle.right.column = gameRectangle.right.column - 1;
                //puting the new rectangle in the board
                updateRectangleInBoard();
            }
        }
            break;
        case FLIP_KEY: {
            //check if the rectangle is horizontal or vertical
            int isHorizontal;
            if (gameRectangle.right.row == gameRectangle.left.row) {
                isHorizontal = 1;
            } else {
                isHorizontal = 0;
            }

            if(isHorizontal){ //make it vertical
                removeRectangleFromBoard();

                //update the place of the new rectangle
                gameRectangle.left.row = gameRectangle.middle.row-1;
                gameRectangle.right.row = gameRectangle.middle.row+1;

                gameRectangle.left.column = gameRectangle.middle.column;
                gameRectangle.right.column = gameRectangle.middle.column;

                //puting the new rectangle in the board
                updateRectangleInBoard();
            } else { //make it horizontal
                removeRectangleFromBoard();

                //update the place of the new rectangle
                gameRectangle.left.row = gameRectangle.middle.row;
                gameRectangle.right.row = gameRectangle.middle.row;

                gameRectangle.left.column = gameRectangle.middle.column-1;
                gameRectangle.right.column = gameRectangle.middle.column+1;

                //puting the new rectangle in the board
                updateRectangleInBoard();
            }
        }
            break;
        case DOWN_KEY: {
            if (canMove(gameRectangle.left.row + 1, gameRectangle.right.row + 1)) {
                removeRectangleFromBoard();
                //update the place of the new rectangle
                gameRectangle.left.row = gameRectangle.left.row + 1;
                gameRectangle.middle.row = gameRectangle.middle.row + 1;
                gameRectangle.right.row = gameRectangle.right.row + 1;
                //puting the new rectangle in the board
                updateRectangleInBoard();
            } else {  //if we got to the end of the board
                removeRectangleFromBoard();
                initializeRectanglePos();
                updateRectangleInBoard();
            }
        }
            break;
    }

}

/**
 * sig handler for the signal SIGUSR2 - read the input from the user
 */
void usr2_readInput(int sigNum) {
    signal(SIGUSR2, usr2_readInput);
    // read key from stdin
    char key = (char) getchar();
    //check if the user quit from the game
    if (key == QUIT_KEY) {
        exit(0);
    }

    // update the shape location according to the user input key
    UpdateShapeLocation(key);

    // draw the board after the shape move
    PrintingBoard();
}

/**
 * sig handler for the signal ALARM - move the rectangle down
 */
void alarm_moveDown() {

}

int main() {
    running = 1;
    //drawing the table
    DrawingBoard();
    PrintingBoard();
    initializeRectanglePos();
    updateRectangleInBoard();

    //todo check if need to == -1 and print a message
    //define the action of SIGUSR2 and SIGALRM
    // define what to do next time receives SIGUSR2
    signal(SIGUSR2, usr2_readInput);

    // define what to do next time receives SIGALRM
    signal(SIGALRM, alarm_moveDown);

    // set an alarm for one second
    alarm(1);

    // always wait for signals until the user press 'q'
    while (1) { pause(); }
}