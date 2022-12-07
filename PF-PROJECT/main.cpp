//============================================================================
// Name        : .cpp
// Author      : FAST CS Department
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Battleship.
//============================================================================

#ifndef Battleship_CPP_
#define Battleship_CPP_
#include "util.h"
#include <iostream>
#include<string>
#include<fstream>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;

// Global variables for window width and height
int width = 1020, height = 700;

// Menu FLag variables
string currMenuItem = "mainMenu";

// Constant values
const int boardRows = 14, boardCols = 24;
const int boardStartX = 150, boardStartY = 520;
const int boardCellSize = 30;


void SetCanvasSize(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int xI = 400, yI = 400;

void drawObject() {
	DrawSquare(xI, yI, 20, colors[WHITE]);
	glutPostRedisplay();
}


bool flag = true;
void moveObject() {

	if (xI > 10 && flag) {
		xI -= 10;
		cout << "going left";
		if (xI < 100)

			flag = false;

	}
	else if (xI < 1010 && !flag) {
		cout << "go right";
		xI += 10;
		if (xI > 900)
			flag = true;
	}
}


void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}

	cout << x << ", " << y << endl;

	glutPostRedisplay();
}

/*
 * Main Canvas drawing function.
 * */


 // Map the rgb color to open gl color components
double mapRanges(double number, double inMin, double inMax, double outMin, double outMax) {
	return (number - inMin) * (outMax - outMin) / ((inMax - inMin) * 1.0) + outMin;
}

void init() {
	glClearColor(mapRanges(65, 0, 255, 0, 1), mapRanges(105, 0, 255, 0, 1), mapRanges(225, 0, 255, 0, 1), 1);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
}

void fillGameGridArr(char grid[][10]) {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			//grid[i][k] = '0';
			if (i == 2 && k >= 3 && k <= 5) {
				grid[i][k] = '1';
			}
		}
	}
}

void displayHeading(string text) {
	DrawString(50, height - 130, text, colors[WHITE]);
	DrawLine(50, height - 150, 200, height - 150, 3, colors[GRAY]);
}

void showLeaderBoard() {
	glClearColor(mapRanges(43, 0, 255, 0, 1), mapRanges(174, 0, 255, 0, 1), mapRanges(102, 0, 255, 0, 1), 1);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	ifstream readHighScores;

	readHighScores.open("./files/highscores.txt", ios::app);
	if (readHighScores.is_open()) {
		string leaderBoardNames[10];
		string leaderBoardScores[10];
		string line;
		int count = 0;
		while (getline(readHighScores, line)) {
			leaderBoardNames[count] = line;
			getline(readHighScores, line);
			leaderBoardScores[count] = line;
			count++;
		}

		int totalRecords = count;

		// Show the leaderboard

		displayHeading("LEADERBOARD");

		if (totalRecords) {
			for (int i = 0; i < totalRecords; i++) {
				string output = leaderBoardNames[i] + "   " + leaderBoardScores[i];
				DrawString(50, height - 220 - (i * 40), output, colors[WHITE]);
			}
		}
		else {
			// If the leaderboard is empty
			DrawString(50, height - 220, "LEADERBOARD IS EMPTY", colors[RED]);
		}
	}
	else {
		MessageBox(NULL, L"'highscores.txt' file couldn't be opened", L"Error", MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
}

void showGameGrid(char gridArr[][10] = {0}, bool isEmpty = 1) {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			int xAxis = boardStartX + (k * boardCellSize) + boardCellSize;
			int yAxis = boardStartY - (i * boardCellSize) -boardCellSize;
			if (i == 0) {
				DrawLine((xAxis), (yAxis), xAxis+boardCellSize, (yAxis), 2, colors[WHITE]);
			}
			if (k == 0) {
				DrawLine((xAxis), (yAxis), xAxis, (yAxis - boardCellSize), 2, colors[WHITE]);
			}
			if (i == (9)) {
				DrawLine((xAxis), (yAxis-boardCellSize), xAxis+boardCellSize, (yAxis-boardCellSize), 2, colors[WHITE]);
			}
			if (k == 9) {
				DrawLine((xAxis+boardCellSize), (yAxis), xAxis+boardCellSize, (yAxis-boardCellSize), 2, colors[WHITE]);
			}
			if (!isEmpty) {
				if (gridArr[i][k] == '1') {
					DrawSquare(xAxis, yAxis, boardCellSize, colors[RED]);
				}
			}
		}
	}
}

void startNewGame() {
	glClearColor(mapRanges(120, 0, 255, 0, 1), mapRanges(81, 0, 255, 0, 1), mapRanges(169, 0, 255, 0, 1), 1);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	bool inputTaken = false;

	//int color;
	//string userName;
	// Take the input data
	//cout << "Enter your name: ";
	//getline(cin, userName);

	//cout << "Color of the battleships: ";
	//cin >> color;

	// ColorNames battleShipColor = static_cast<ColorNames>(color);
	

	int lineWidth = 1;
	for (int i = 0; i < boardRows; i++) {
		for (int k = 0; k < boardCols; k++) {

			int xAxis = boardStartX + (k * boardCellSize);
			int yAxis = boardStartY - (i * boardCellSize);

				// For last column
				if (k == (boardCols - 1)) {
					DrawLine(xAxis + boardCellSize, yAxis, xAxis + boardCellSize, yAxis - boardCellSize, 1, colors[DIM_GRAY]);
				}

				// For last row
				if (i == (boardRows - 1)) {
					DrawLine(xAxis, yAxis - boardCellSize, xAxis + boardCellSize, yAxis - boardCellSize, 1, colors[DIM_GRAY]);
				}

				// to bottom
				DrawLine(xAxis, yAxis, xAxis, yAxis - boardCellSize, 1, colors[DIM_GRAY]);
				// to right
				DrawLine(xAxis, yAxis, xAxis + boardCellSize, yAxis, 1, colors[DIM_GRAY]);
		}
	}

	char gameGrid[10][10];
	fillGameGridArr(gameGrid);
	showGameGrid(gameGrid, 0);


}

void viewBalance() {
	glClearColor(mapRanges(156, 0, 255, 0, 1), mapRanges(37, 0, 255, 0, 1), mapRanges(77, 0, 255, 0, 1), 1);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	DrawString(50, 500, "Balance", colors[WHITE]);
}

void options() {
	glClearColor(mapRanges(250, 0, 255, 0, 1), mapRanges(112, 0, 255, 0, 1), mapRanges(112, 0, 255, 0, 1), 1);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	DrawString(50, 500, "Options", colors[WHITE]);
}

void GameDisplay()/**/ {
	init();

	// Menu
	if (currMenuItem == "mainMenu") {
		displayHeading("MAIN MENU");
		DrawRoundRect(50, 200, 200, 300, colors[WHITE], 5);

		DrawString(60, height - 250, "View Leaderboard", colors[BLACK]);
		DrawString(60, height - 290, "Start New Game", colors[BLACK]);
		DrawString(60, height - 330, "View Balance", colors[BLACK]);
		DrawString(60, height - 370, "Options", colors[BLACK]);
	}
	else if (currMenuItem == "leaderboard") {
		showLeaderBoard();
	}
	else if (currMenuItem == "startNewGame") {
		startNewGame();
	}
	else if (currMenuItem == "viewBalance") {
		viewBalance();
	}
	else if (currMenuItem == "options") {
		options();
	}

	// Main Menu Button
	DrawRectangle(width - 130, height - 65, 85, 35, colors[RED]);
	DrawString(width - 120, height - 55, "Main Menu", colors[WHITE]);

	//glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters

	glutSwapBuffers(); // do not modify this line..

}



/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	if (key
		== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
		xI -= 10;

	}
	else if (key
		== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
		xI += 10;
	}
	else if (key
		== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
		yI += 10;
	}

	else if (key
		== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
		yI -= 10;
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}


/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here
	moveObject();

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) {
	cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) {
	//cout << x << " " << y << endl;
	glutPostRedisplay();
}

void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		cout << x << ", " << y << endl;

		if (x >= 1190 && x <= 1305 && y >= 30 && y <= 65) {
			currMenuItem = "mainMenu";
		}

		if (currMenuItem == "mainMenu") {
			if (x >= 70 && x <= 330) {
				// Leaderboard button
				if (y >= 230 && y <= 260) {
					currMenuItem = "leaderboard";
				}
				else if (y >= 270 && y <= 300) {
					currMenuItem = "startNewGame";
				}
				else if (y >= 310 && y <= 340) {
					currMenuItem = "viewBalance";
				}
				else if (y >= 350 && y <= 380) {
					currMenuItem = "options";
				}
			}
		}

	}

	glutPostRedisplay();
}

/*
 * our gateway main function
 * */
int main(int argc, char* argv[]) {

	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 10); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Battleship Game (SE-F-2660)"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...

	glutMouseFunc(MouseClicked);
	//glutPassiveMotionFunc(MouseMoved); // Mouse
	//glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* Battleship_CPP_ */
