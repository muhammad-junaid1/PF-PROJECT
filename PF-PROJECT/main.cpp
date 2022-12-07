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
int screenWidth = 1250, screenHeight = 600;

// Menu FLag variables
string currMenuItem = "mainMenu";

// Constant values
const int boardRows = 14, boardCols = 24;
const int boardStartX = 150, boardStartY = 520;
const int boardCellSize = 30;

/*
	Throughou the program in arrays,
	'0' represents the empty space/cell
	'1' represents presence of battleship
	'/' represents the head of battleship

*/


void SetCanvasSize(int screenWidth, int screenHeight) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, -1, 1); // set the screen size to given width and height.
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

void fillArray(char grid[][10]) {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			grid[i][k] = '0';
		}
	}
}

void fillArray(char grid[][boardCols]) {
	for (int i = 0; i < boardRows; i++) {
		for (int k = 0; k < boardCols; k++) {
			grid[i][k] = '0';
		}
	}
}

void displayHeading(string text) {
	DrawString(70, screenHeight - 130, text, colors[WHITE]);
	DrawLine(70, screenHeight - 150, 200, screenHeight - 150, 3, colors[GRAY]);
}

void showLeaderBoard() {
	glClearColor(mapRanges(33, 0, 255, 0, 1), mapRanges(133, 0, 255, 0, 1), mapRanges(38, 0, 255, 0, 1), 1);
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
				DrawString(50, screenHeight - 220 - (i * 40), output, colors[WHITE]);
			}
		}
		else {
			// If the leaderboard is empty
			DrawString(70, screenHeight - 220, "LEADERBOARD IS EMPTY", colors[TOMATO]);
		}
	}
	else {
		MessageBox(NULL, L"'highscores.txt' file couldn't be opened", L"Error", MB_OK | MB_ICONEXCLAMATION);
		exit(1);
	}
}

void showGameGrid(char gridArr[][10] = { 0 }, bool isEmpty = 1) {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			int xAxis = boardStartX + (k * boardCellSize) + boardCellSize;
			int yAxis = boardStartY - (i * boardCellSize) - boardCellSize;
			if (i == 0) {
				DrawLine((xAxis), (yAxis), xAxis + boardCellSize, (yAxis), 2, colors[WHITE]);

				// Label for x-axis
				DrawString(xAxis + (boardCellSize / 2) - 8, yAxis + (boardCellSize / 2) - 5, to_string(k + 1), colors[WHITE]);
			}
			if (k == 0) {
				DrawLine((xAxis), (yAxis), xAxis, (yAxis - boardCellSize), 2, colors[WHITE]);

				// Label for y-axis
				string letter = "";
				letter += char('A' + i);
				DrawString(xAxis - (boardCellSize / 2) - 8, yAxis - (boardCellSize)+10, letter, colors[WHITE]);
			}
			if (i == (9)) {
				DrawLine((xAxis), (yAxis - boardCellSize), xAxis + boardCellSize, (yAxis - boardCellSize), 2, colors[WHITE]);
			}
			if (k == 9) {
				DrawLine((xAxis + boardCellSize), (yAxis), xAxis + boardCellSize, (yAxis - boardCellSize), 2, colors[WHITE]);
			}
			if (!isEmpty) {
				if (gridArr[i][k] == '1') {
					DrawSquare(xAxis, yAxis, boardCellSize - 2, colors[RED]);
				}
			}
		}
	}
}

int getXAxis(int colNo, string grid) {
	int xAxis = boardStartX + (colNo * boardCellSize);
	if (grid == "gameGrid") {
		return xAxis + boardCellSize;
	}
	else if (grid == "boardGrid") {
		return xAxis;
	}
}

int getYAxis(int rowNo, string grid) {
	int yAxis = (screenHeight - boardStartY) + (rowNo * boardCellSize) - boardCellSize;
	if (grid == "gameGrid") {
		return yAxis + boardCellSize;
	}
	else if (grid == "boardGrid") {
		return yAxis;
	}
}

void showBoard(char grid[][boardCols]) {
	int lineWidth = 1;

	for (int i = 0; i < boardRows; i++) {
		for (int k = 0; k < boardCols; k++) {

			int xAxis = boardStartX + (k * boardCellSize);
			int yAxis = boardStartY - (i * boardCellSize);

			// For last column
			if (k == (boardCols - 1)) {
				DrawLine(xAxis + boardCellSize, yAxis, xAxis + boardCellSize, yAxis - boardCellSize, lineWidth, colors[DIM_GRAY]);
			}

			// For last row
			if (i == (boardRows - 1)) {
				DrawLine(xAxis, yAxis - boardCellSize, xAxis + boardCellSize, yAxis - boardCellSize, lineWidth, colors[DIM_GRAY]);
			}

			// to bottom
			DrawLine(xAxis, yAxis, xAxis, yAxis - boardCellSize, lineWidth, colors[DIM_GRAY]);
			// to right
			DrawLine(xAxis, yAxis, xAxis + boardCellSize, yAxis, lineWidth, colors[DIM_GRAY]);

			// Battleships on the board options
			if (grid[i][k] == '/') {
				DrawTriangle(xAxis+3, yAxis + (boardCellSize / 2), xAxis + boardCellSize, yAxis + boardCellSize+1, xAxis + boardCellSize, yAxis, colors[SILVER]);
			}
			if (grid[i][k] == '1') {
				DrawRectangle(xAxis, yAxis+1, boardCellSize+2, boardCellSize, colors[LIGHT_GRAY]);
			}
		}
	}
}

void setBattleshipInBoardOptions(char grid[][boardCols], int row, int col, int length) {
	for (int i = col; i < col + length; i++) {
		if (i == col) {
			grid[row][i] = '/';
		}
		else {
			grid[row][i] = '1';
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


	char boardGrid[boardRows][boardCols];
	fillArray(boardGrid);

	// 1 Battleship of length 4
	setBattleshipInBoardOptions(boardGrid, 2, 17, 4);

	// 2 Battleships of length 3
	setBattleshipInBoardOptions(boardGrid, 4, 14, 3);
	setBattleshipInBoardOptions(boardGrid, 4, 18, 3);

	// 3 Battleships of length 2
	setBattleshipInBoardOptions(boardGrid, 6, 13, 2);
	setBattleshipInBoardOptions(boardGrid, 6, 16, 2);
	setBattleshipInBoardOptions(boardGrid, 6, 19, 2);

	// 4 Battleships of length 1
	setBattleshipInBoardOptions(boardGrid, 8, 14, 1);
	setBattleshipInBoardOptions(boardGrid, 8, 16, 1);
	setBattleshipInBoardOptions(boardGrid, 8, 18, 1);
	setBattleshipInBoardOptions(boardGrid, 8, 20, 1);


	showBoard(boardGrid);
	
	char gameGrid[10][10];
	fillArray(gameGrid);
	showGameGrid();


}

void showMenu() {
	displayHeading("MAIN MENU");
	DrawRoundRect(70, 50, 300, 350, colors[WHITE], 5);

	DrawRectangle(80, screenHeight - 255, 280, 40, colors[ROYAL_BLUE]);
	DrawString(90, screenHeight - 240, "View Leaderboard", colors[WHITE]);
	DrawRectangle(80, screenHeight - 300, 280, 40, colors[ROYAL_BLUE]);
	DrawString(90, screenHeight - 285, "Start New Game", colors[WHITE]);
	DrawRectangle(80, screenHeight - 345, 280, 40, colors[ROYAL_BLUE]);
	DrawString(90, screenHeight - 330, "View Balance", colors[WHITE]);
	DrawRectangle(80, screenHeight - 390, 280, 40, colors[ROYAL_BLUE]);
	DrawString(90, screenHeight - 375, "Options", colors[WHITE]);
}

void viewBalance() {
	glClearColor(mapRanges(156, 0, 255, 0, 1), mapRanges(37, 0, 255, 0, 1), mapRanges(77, 0, 255, 0, 1), 1);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	DrawString(50, 500, "Balance", colors[WHITE]);
}

void showOptions() {
	glClearColor(mapRanges(157, 0, 255, 0, 1), mapRanges(126, 0, 255, 0, 1), mapRanges(143, 0, 255, 0, 1), 1);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	DrawString(50, 500, "Options", colors[WHITE]);
}

void GameDisplay()/**/ {
	init();

	// Menu
	if (currMenuItem == "mainMenu") {
		showMenu();
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
		showOptions();
	}

	// Main Menu Button
	DrawRectangle(screenWidth - 210, screenHeight - 60, 115, 35, colors[RED]);
	DrawString(screenWidth - 200, screenHeight - 50, "Main Menu", colors[WHITE]);

	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters

	glutSwapBuffers(); // do not modify this line..

}


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

		if (x >= screenWidth - 210 && x <= (screenWidth - 210 + 115) && y >= (screenHeight - (screenHeight - 60) - 35) && y <= (screenHeight - (screenHeight - 60))) {
			currMenuItem = "mainMenu";
		}

		if (currMenuItem == "mainMenu") {
			if (x >= 80 && x <= 360) {
				// Leaderboard button
				if (y >= (screenHeight - (screenHeight - 255) - 40) && y <= screenHeight - (screenHeight - 255)) {
					currMenuItem = "leaderboard";
				}
				else if (y >= (screenHeight - (screenHeight - 300) - 40) && y <= screenHeight - (screenHeight - 300)) {
					currMenuItem = "startNewGame";
				}
				else if (y >= (screenHeight - (screenHeight - 345) - 40) && y <= screenHeight - (screenHeight - 345)) {
					currMenuItem = "viewBalance";
				}
				else if (y >= (screenHeight - (screenHeight - 390) - 40) && y <= screenHeight - (screenHeight - 390)) {
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
	glutInitWindowPosition(50, 30); // set the initial position of our window
	glutInitWindowSize(screenWidth, screenHeight); // set the size of our window
	glutCreateWindow("Battleship Game (SE-F-2660)"); // set the title of our game window
	SetCanvasSize(screenWidth, screenHeight);

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
