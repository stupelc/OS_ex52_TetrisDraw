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
    int i, j;
} pos;

struct rectangle {
    struct pos left, middle, right; //left, middle and right position of the rectangle
} gameRectangle;

struct gameRectangle; //the rectangle
char table[SIZE][SIZE]; //game table
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
    gameRectangle.left.i = 0;
    gameRectangle.middle.i = 0;
    gameRectangle.right.i = 0;

    //initialize rectangle column
    gameRectangle.left.j = (SIZE / 2) - 3; // index 7 (place 8)
    gameRectangle.middle.j = (SIZE / 2) - 2; //index 8 (place 9)
    gameRectangle.right.j = (SIZE / 2) - 1; //index 9 (place 10)
}

/**
 * update the positions of the rectangle in the board
 */
void updateRectangleInBoard() {
    //update the left position in the board
    table[gameRectangle.left.i][gameRectangle.left.j] = '-';

    //update the middle position in the board
    table[gameRectangle.middle.i][gameRectangle.middle.j] = '-';

    //update the right position in the board
    table[gameRectangle.right.i][gameRectangle.right.j] = '-';
}

/**
 * remove the rectangle shape from the board
 */
void removeRectangleFromBoard() {
    //remove the left position from the board
    table[gameRectangle.left.i][gameRectangle.left.j] = ' ';

    //remove the middle position from the board
    table[gameRectangle.middle.i][gameRectangle.middle.j] = ' ';

    //remove the right position from the board
    table[gameRectangle.right.i][gameRectangle.right.j] = ' ';
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
    // clear the board and initialize a new one
    system("clear");

    switch (key){
        case RIGHT_KEY: {
            //first check if can move to the place the user wants
            if (canMove(gameRectangle.left.j + 1, gameRectangle.right.j + 1)) {
                removeRectangleFromBoard();
                //update the place of the new rectangle
                gameRectangle.left.j = gameRectangle.left.j + 1;
                gameRectangle.middle.j = gameRectangle.middle.j + 1;
                gameRectangle.right.j = gameRectangle.right.j + 1;
                //puting the new rectangle in the board
                updateRectangleInBoard();
            }
        }
            break;
        case LEFT_KEY: {
            if (canMove(gameRectangle.left.j - 1, gameRectangle.right.j - 1)) {
                removeRectangleFromBoard();
                //update the place of the new rectangle
                gameRectangle.left.j = gameRectangle.left.j - 1;
                gameRectangle.middle.j = gameRectangle.middle.j - 1;
                gameRectangle.right.j = gameRectangle.right.j - 1;
                //puting the new rectangle in the board
                updateRectangleInBoard();
            }
        }
            break;
        case FLIP_KEY: {
            //check if the rectangle is horizontal or vertical
            int isHorizontal;
            if (gameRectangle.right.i == gameRectangle.left.i) {
                isHorizontal = 1;
            } else {
                isHorizontal = 0;
            }

            if(isHorizontal){ //make it vertical
                removeRectangleFromBoard();

                //update the place of the new rectangle
                gameRectangle.left.i = gameRectangle.middle.i-1;
                gameRectangle.right.i = gameRectangle.middle.i+1;

                gameRectangle.left.j = gameRectangle.middle.j;
                gameRectangle.right.j = gameRectangle.middle.j;

                //puting the new rectangle in the board
                updateRectangleInBoard();
            } else { //make it horizontal
                removeRectangleFromBoard();

                //update the place of the new rectangle
                gameRectangle.left.i = gameRectangle.middle.i;
                gameRectangle.right.i = gameRectangle.middle.i;

                gameRectangle.left.j = gameRectangle.middle.j-1;
                gameRectangle.right.j = gameRectangle.middle.j+1;

                //puting the new rectangle in the board
                updateRectangleInBoard();
            }
        }
            break;
        case DOWN_KEY: {
            if (canMove(gameRectangle.left.i + 1, gameRectangle.right.i + 1)) {
                removeRectangleFromBoard();
                //update the place of the new rectangle
                gameRectangle.left.i = gameRectangle.left.i + 1;
                gameRectangle.middle.i = gameRectangle.middle.i + 1;
                gameRectangle.right.i = gameRectangle.right.i + 1;
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
        running = 0;
        if(close(STDIN_FILENO) == -1)
            WRITE_ERROR
        exit(EXIT_SUCCESS);
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
    signal(SIGALRM, alarm_moveDown);

    // clear the board and initialize a new one
    system("clear");

    //move the shape down (like when we press 's'
    if (canMove(gameRectangle.left.i + 1, gameRectangle.right.i + 1)) {
        removeRectangleFromBoard();
        //update the place of the new rectangle
        gameRectangle.left.i = gameRectangle.left.i + 1;
        gameRectangle.middle.i = gameRectangle.middle.i + 1;
        gameRectangle.right.i = gameRectangle.right.i + 1;
        //puting the new rectangle in the board
        updateRectangleInBoard();
    } else {  //if we got to the end of the board
        removeRectangleFromBoard();
        initializeRectanglePos();
        updateRectangleInBoard();
    }

    PrintingBoard();
    alarm(1);
}

int main() {
    // clear the board and initialize a new one
    system("clear");
    running = 1;

    //define the action of SIGUSR2 and SIGALRM
    signal(SIGUSR2, usr2_readInput);
    signal(SIGALRM, alarm_moveDown);

    //drawing the table
    DrawingBoard();
    PrintingBoard();
    initializeRectanglePos();
    updateRectangleInBoard();

    // set an alarm for one second
    alarm(1);

    // always wait for signals until the user press 'q'
    while (running) {
        pause();
    }
}