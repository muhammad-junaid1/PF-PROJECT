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
#include<ctime>
#include<fstream>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;

// Global variables for window width and height
int screenWidth = 1250, screenHeight = 600;

// Flag variables
string currMenuItem = "mainMenu";
string hoverMessageText = "";
bool outsideHover = true;
string loadingMessage = "";
bool autoButtonClicked = false;

// Constant values
const int boardRows = 14, boardCols = 24;
const int boardStartX = 150, boardStartY = 520;
const int boardCellSize = 30;

// Grid Arrays
char boardGrid[boardRows][boardCols];
char gameGrid[10][10];
char computerGameGrid[10][10];

/*
	Throughout the program in arrays,
	'0' represents the empty space/cell
	'1' represents presence of battleship
	'/' represents the head of battleship
*/

// 0 -> Row
// 1 -> Col
int hoverShipInfo[2] = {0};

int activeShip[4] = { 0 };
int shipsInOptions[10][4] = {
	// 0 -> ID of ship
	// 1 -> Length of ship
	// 2 -> Row No
	// 3 -> Col No
	{1, 4, 2, 17}, 
	{2, 3, 4, 14}, {3, 3, 4, 18},
	{4, 2, 6, 13}, {5, 2, 6, 16}, {6, 2, 6, 19},
	{7, 1, 8, 14}, {8, 1, 8, 16}, {9, 1, 8, 18}, {10, 1, 8, 20}
};
int shipsInOptionsCopy[10][4] = {
	// 0 -> ID of ship
	// 1 -> Length of ship
	// 2 -> Row No
	// 3 -> Col No
	{1, 4, 2, 17},
	{2, 3, 4, 14}, {3, 3, 4, 18},
	{4, 2, 6, 13}, {5, 2, 6, 16}, {6, 2, 6, 19},
	{7, 1, 8, 14}, {8, 1, 8, 16}, {9, 1, 8, 18}, {10, 1, 8, 20}
};

void SetCanvasSize(int screenWidth, int screenHeight) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, -1, 1); // set the screen size to given width and height.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

//void fillArray(char grid[][10]) {
	//for (int i = 0; i < 10; i++) {
		//for (int k = 0; k < 10; k++) {
			//grid[i][k] = '0';
		//}
	//}
//}

//void fillArray(char grid[][boardCols]) {
	//for (int i = 0; i < boardRows; i++) {
		//for (int k = 0; k < boardCols; k++) {
			//grid[i][k] = '0';
		//}
	//}
//}

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

void resetShipInfo(int ship[]) {
	for (int i = 0; i < 4; i++) {
		ship[i] = 0;
	}
}

void resetGameGrid() {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			gameGrid[i][k] = '0';
		}
	}
}


int getCol(int xAxis, string grid) {
	int col = -1;
	if (grid == "gameGrid") {
		col = (xAxis - boardStartX - (boardCellSize)) / boardCellSize;
	}
	else if (grid == "boardGrid") {
		col = (xAxis - boardStartX) / boardCellSize;
	}

	return col;
}

int getRow(int yAxis, string grid) {
	int row = -1;
	if (grid == "gameGrid") {
		row = ((yAxis)+boardStartY - (boardCellSize)-(screenHeight)) / (boardCellSize);
	}
	else if (grid == "boardGrid") {
		row = ((yAxis - screenHeight) + (boardStartY)+(boardCellSize)) / (boardCellSize);
	}
	return row;
}


bool isShipAlreadyThere(string direction, int lineNo, int start, int length = 0) {
	bool isShipThere = false;
	if (direction == "horizontal") {
		if (length == 0) {
			for (int i = start; i < start + activeShip[1]; i++) {
					if (gameGrid[lineNo][i] == '/' || gameGrid[lineNo][i] == '1') {
						isShipThere = true;
						break;
					}
			}
		}
		else {
			for (int i = start; i < start + length; i++) {
					if (computerGameGrid[lineNo][i] == '/' || computerGameGrid[lineNo][i] == '1') {
						isShipThere = true;
						break;
					} 
			}
		}
	}
	else if (direction == "vertical") {
		if (length == 0) {
			for (int i = start; i < start + activeShip[1]; i++) {
				if (gameGrid[i][lineNo] == '/' || gameGrid[i][lineNo] == '1') {
					isShipThere = true;
					break;
				}
			}
		}
		else {
			for (int i = start; i < start + length; i++) {
					if (computerGameGrid[i][lineNo] == '/' || computerGameGrid[i][lineNo] == '1') {
						isShipThere = true;
						break;
					}
			}
		}
	}
	return isShipThere;
}


// Check, if ship has space/distance of minimum 1 around it
bool shipDistanceOf1(int x, int y, int autoPlaceShipLength = 0) {
	int row = getRow(y, "gameGrid");
	int col = getCol(x, "gameGrid");
	int colStart = col;
	int colEnd;
	if (autoPlaceShipLength) {
		colEnd = col + autoPlaceShipLength - 1;
	}
	else {
		colEnd = col + activeShip[1] - 1;
	}

	bool isShipDistant = true;

	// Upper line
	if (row > 0) {
		for (int i = colStart - 1; i <= colEnd + 1; i++) {
			if (i >= 0 && i <= 9) {
				if ((gameGrid[row - 1][i] == '1' || gameGrid[row - 1][i] == '/')) {
					isShipDistant = false;
					break;
				}
			}
		}
	}

	// Lower line
	if (row < 9) {
		for (int i = colStart - 1; i <= colEnd + 1; i++) {
			if (i >= 0 && i <= 9) {
				if ((gameGrid[row + 1][i] == '1' || gameGrid[row + 1][i] == '/')) {
					isShipDistant = false;
					break;
				}
			}
		}
	}

	// Left 
	if (colStart - 1 >= 0) {
		if (((gameGrid[row][colStart - 1]) == '1' || (gameGrid[row][colStart - 1]) == '/')) {
			isShipDistant = false;
		}
	}

	// Right
	if (colEnd + 1 <= 9) {
		if ((gameGrid[row][colEnd + 1]) == '1' || (gameGrid[row][colEnd + 1]) == '/') {
			isShipDistant = false;
		}
	}

	return isShipDistant;
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
	int yAxis = (screenHeight - boardStartY) + (rowNo * boardCellSize);
	if (grid == "gameGrid") {
		return yAxis + boardCellSize;
	}
	else if (grid == "boardGrid") {
		return yAxis;
	}
}

void copyShipInfo(int info[], int arr[]) {
	for (int i = 0; i < 4; i++) {
		arr[i] = info[i];
	}
}

int arrayForRand[100];
int lengthOfRandArr = 100;

void autoPlaceShips(string grid = "gameGrid") {
	if (grid == "gameGrid") {
		if (activeShip[0]) {
			// Remove the selected ship if any
			for (int i = (activeShip[3]); i < (activeShip[3] + activeShip[1]); i++) {
				boardGrid[activeShip[2]][i] = '0';
			}

			// Remove the active ship if any
			resetShipInfo(activeShip);
		}
	}

	// Iterate through each ship
	int ship = 0;
	while (ship < 10) {
		srand(time(0));
		int randRow = rand() % 10;
		int randCol = rand() % 10;
		int lengthOfShip = shipsInOptions[ship][1];
		int colStart = (randCol - (lengthOfShip - 1));
		int colEnd = randCol;


		// If all of the validations are done, and no error occured, place the ship
		if ((shipDistanceOf1(getXAxis(colStart, "gameGrid"), getYAxis(randRow, "gameGrid"), lengthOfShip)) && (!isShipAlreadyThere("horizontal", randRow, colStart)) && (colStart >= 0)) {

			// Place the battleship
			for (int i = colStart; i <= colEnd; i++) {
				// Head of ship	
				if (i == colStart) {
					gameGrid[randRow][i] = '/';
				}
				else {
					// Rest of the ship
					gameGrid[randRow][i] = '1';
				}
			}
			ship++;
		}
		else {
			continue;
		}
	}

	// Clear the board
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			shipsInOptions[i][k] = 0;
		}
	}

	for (int i = 0; i < boardRows; i++) {
		for (int k = 0; k < boardCols; k++) {
			boardGrid[i][k] = '0';
		}
	}

	loadingMessage = "";

}


void DrawBorderedRect(int sx, int sy, int width, int height, int borderWidth, float* color) {
	// bottom left to bottom right
	DrawLine(sx, sy, sx + width, sy, borderWidth, color);
	// bottom right to top right
	DrawLine(sx + width, sy, sx + width, sy + height, borderWidth, color);
	// top right to top left
	DrawLine(sx + width, sy + height, sx, sy + height, borderWidth, color);
	// top left to bottom left
	DrawLine(sx, sy + height, sx, sy, borderWidth, color);
}

void showGameGrid(char grid[][10], bool myGrid = 0) {
	for (int i = 0; i < 10; i++) {
		for (int k = 0; k < 10; k++) {
			int xAxis = boardStartX + (k * boardCellSize) + boardCellSize;
			int yAxis = boardStartY - (i * boardCellSize) - boardCellSize;
			if (myGrid) {
				xAxis += (boardCellSize * (boardCols-12));
			}
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
			// Ship head
			if (grid[i][k] == '/') {
				DrawTriangle(xAxis + 3, yAxis - boardCellSize + (boardCellSize / 2), xAxis + boardCellSize, yAxis - boardCellSize + boardCellSize + 1, xAxis + boardCellSize, yAxis - boardCellSize, colors[SILVER]);
			}
			// Ship
			if (grid[i][k] == '1') {
				DrawRectangle(xAxis, yAxis + 1 - boardCellSize, boardCellSize + 2, boardCellSize, colors[DARK_SALMON]);
			}
		}
	}
}


void showBoard() {
	int lineWidth = 1;

	for (int i = 0; i < boardRows; i++) {
		for (int k = 0; k < boardCols; k++) {

			int xAxis = boardStartX + (k * boardCellSize);
			int yAxis = boardStartY - (i * boardCellSize);

			if (boardGrid[i][k] != '/' && boardGrid[i][k] != '1') {
				DrawBorderedRect(xAxis, yAxis, boardCellSize, boardCellSize, 1, colors[DIM_GRAY]);
			}
			else {
				// Battleships on the board options
				if (boardGrid[i][k] == '/') {
					DrawTriangle(xAxis + 3, yAxis + (boardCellSize / 2), xAxis + boardCellSize, yAxis + boardCellSize + 1, xAxis + boardCellSize, yAxis, colors[SILVER]);
				}
				if (boardGrid[i][k] == '1') {
					DrawRectangle(xAxis, yAxis + 1, boardCellSize + 2, boardCellSize, colors[DARK_SALMON]);
				}
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


void showBattleButton() {
	// Battle button to start the battle
	DrawRectangle(getXAxis(18, "boardGrid"), screenHeight - getYAxis(11, "boardGrid"), 3.5 * (boardCellSize), boardCellSize * 1.5, colors[BLACK]);
	DrawString(getXAxis(18, "boardGrid") + 25, screenHeight - getYAxis(11, "boardGrid")+15, "Battle!", colors[WHITE]);
}

void autoPlaceButton() {
	// AUTO/RANDOM button to place the ships randomly in the grid
	DrawRectangle(getXAxis(12, "boardGrid"), screenHeight - getYAxis(11, "boardGrid"), 3 * (boardCellSize), boardCellSize * 1.3, colors[DARK_ORANGE]);
	DrawString(getXAxis(12, "boardGrid") + 25, screenHeight - getYAxis(11, "boardGrid") + 15, "Auto", colors[WHITE]);
}

bool areAllShipsPlaced() {
	bool shipsPlaced = true;
	for (int i = 0; i < boardRows; i++) {
		for (int k = 0; k < boardCols; k++) {
			if (boardGrid[i][k] == '1' || boardGrid[i][k] == '/') {
				shipsPlaced = false;
			}
		}
	}
	return shipsPlaced;
}

void startNewGame() {
	glClearColor(mapRanges(120, 0, 255, 0, 1), mapRanges(81, 0, 255, 0, 1), mapRanges(169, 0, 255, 0, 1), 1);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	//bool inputTaken = false;

	//int color;
	//string userName;
	// Take the input data
	//cout << "Enter your name: ";
	//getline(cin, userName);

	//cout << "Color of the battleships: ";
	//cin >> color;

	// ColorNames battleShipColor = static_cast<ColorNames>(color);



	// Draw all the ships in options area
	for (int i = 0; i < 10; i++) {
		if (shipsInOptions[i][0]) {
			setBattleshipInBoardOptions(boardGrid, shipsInOptions[i][2], shipsInOptions[i][3], shipsInOptions[i][1]);
		}
	}

	// Display the board array
	showBoard();
	
	// Display the game grid
	showGameGrid(gameGrid);


	// Hover effect in Game grid
	int hoverShipRow = hoverShipInfo[0];
	int hoverShipCol = hoverShipInfo[1];

	if (!outsideHover) {
		if (hoverShipCol < 0) {
			hoverMessageText = "Out of boundary!";
			DrawBorderedRect(getXAxis(0, "gameGrid"), (screenHeight - getYAxis(hoverShipRow, "gameGrid") - (boardCellSize)), boardCellSize * (activeShip[1]+hoverShipCol), boardCellSize, 3, colors[RED]);
		} else if (isShipAlreadyThere("horizontal", hoverShipRow, hoverShipCol)) {
			hoverMessageText = "Ship is already there!";
			DrawBorderedRect(getXAxis(hoverShipCol, "gameGrid"), (screenHeight - getYAxis(hoverShipRow, "gameGrid") - (boardCellSize)), boardCellSize * activeShip[1], boardCellSize, 3, colors[RED]);
		}
		else if (!shipDistanceOf1(getXAxis(hoverShipCol, "gameGrid"), getYAxis(hoverShipRow, "gameGrid"))) {
			hoverMessageText = "Should have a distance of at least 1 cell!";
			DrawBorderedRect(getXAxis(hoverShipCol, "gameGrid"), (screenHeight - getYAxis(hoverShipRow, "gameGrid") - (boardCellSize)), boardCellSize * activeShip[1], boardCellSize, 3, colors[DARK_CYAN]);
		}
		else {
			if (hoverShipRow != 1000 && hoverShipCol != 1000) {
					DrawBorderedRect(getXAxis(hoverShipCol, "gameGrid"), (screenHeight - getYAxis(hoverShipRow, "gameGrid") - (boardCellSize)), boardCellSize * activeShip[1], boardCellSize, 3, colors[BLACK]);
					hoverMessageText = "";
				}
		}
	}

	// Message Text
	DrawString(getXAxis(5, "boardGrid"), screenHeight - getYAxis(15, "boardGrid"), hoverMessageText, colors[WHITE]);

	DrawString(getXAxis(5, "boardGrid"), screenHeight - getYAxis(15, "boardGrid"), loadingMessage, colors[WHITE]);

	// Show border around the active ship on clicking it
	if (activeShip[0]) {
		DrawBorderedRect(getXAxis(activeShip[3], "boardGrid"), screenHeight - getYAxis(activeShip[2], "boardGrid"), boardCellSize * activeShip[1], boardCellSize, 3, colors[WHITE]);
	}

	// Show/Hide Battle button

	if (areAllShipsPlaced()) {
		showBattleButton();
	}
	
	if (!autoButtonClicked) {
		autoPlaceButton();
	}
}

void showMenu() {
	displayHeading("MAIN MENU");
	DrawRoundRect(70, 195, 300, 215, colors[WHITE], 5);

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

	displayHeading("BALANCE");
}

void showOptions() {
	glClearColor(mapRanges(157, 0, 255, 0, 1), mapRanges(126, 0, 255, 0, 1), mapRanges(143, 0, 255, 0, 1), 1);
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	displayHeading("OPTIONS");
}

void showBattle() {
	showBoard();

	autoPlaceShips("computerGameGrid");
	showGameGrid(computerGameGrid);
	showGameGrid(gameGrid, true);
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
	else if (currMenuItem == "battle") {
		showBattle();
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
	}
	else if (key
		== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
	}
	else if (key
		== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}

	else if (key
		== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
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

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutTimerFunc(100, Timer, 0);
}

void MouseMoved(int x, int y) {
	if (currMenuItem == "startNewGame") {
		if (x >= boardStartX + boardCellSize && x <= boardStartX + boardCellSize + (10 * boardCellSize)
			&& y >= (screenHeight - boardStartY) + boardCellSize && y <= (screenHeight - boardStartY) + boardCellSize + (10 * boardCellSize)) {
			if (activeShip[0]) {
				outsideHover = false;

				int row = getRow(y, "gameGrid");
				int col = getCol(x, "gameGrid");
				int lengthOfShip = activeShip[1];
				int colStart = (col - (lengthOfShip - 1));

				hoverShipInfo[0] = row;
				hoverShipInfo[1] = colStart;

			}
		}
		else {
			// Hide the hover, on going out of the gameGrid 
			outsideHover = true;
			hoverMessageText = "";
		}
}


	glutPostRedisplay();
}

void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) // dealing only with left button
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
		else if (currMenuItem == "startNewGame") {

			// Detect the row and column on clicking the grid, and place the selected battleship
			if (x >= boardStartX + boardCellSize && x <= boardStartX + boardCellSize + (10 * boardCellSize)
				&& y >= (screenHeight - boardStartY) + boardCellSize && y <= (screenHeight - boardStartY) + boardCellSize + (10 * boardCellSize)) {

				int activeShipId = activeShip[0];
				int activeShipLength = activeShip[1];
				int activeShipRow = activeShip[2];
				int activeShipCol = activeShip[3];

				int row = getRow(y, "gameGrid");
				int col = getCol(x, "gameGrid");
				int lengthOfShip = activeShipLength;
				int colStart = (col - (lengthOfShip - 1));
				int colEnd = col;


				// If all of the validations are done, and no error occured, place the ship
				if (activeShipId && (shipDistanceOf1(getXAxis(colStart, "gameGrid"), getYAxis(row, "gameGrid"))) && (!isShipAlreadyThere("horizontal", row, colStart, 0)) && (colStart >= 0)) {
					// Place the battleship
					for (int i = colStart; i <= colEnd; i++) {
						// Head of ship	
						if (i == colStart) {
							gameGrid[row][i] = '/';
						}
						else {
							// Rest of the ship
							gameGrid[row][i] = '1';
						}
					}

					// Remove the selected ship
					for (int i = (activeShipCol); i < (activeShipCol + activeShipLength); i++) {
						boardGrid[activeShipRow][i] = '0';
					}
					resetShipInfo(shipsInOptions[activeShipId-1]);
					resetShipInfo(activeShip);

					outsideHover = true;
				}
			}
			else {
			
				// Get the row, by y-axis, and column by x-axis
				int row = getRow(y, "boardGrid");
				int col = getCol(x, "boardGrid");

				// Set active ship, by clicking on it
				for (int k = 0; k < 10; k++) {
					int ship[4];
					copyShipInfo(shipsInOptions[k], ship);
					if (row == ship[2] && (col >= ship[3] && col < (ship[3] + ship[1]))) {
						copyShipInfo(shipsInOptions[k], activeShip);
					}
				}

				// Detect click on AUTO button
				if (x >= 510 && x <= 600 && y >= 370 && y <= 410) {
					loadingMessage = "Loading...";
					autoButtonClicked = true;
					resetGameGrid();
					autoPlaceShips("gameGrid");
				}

				// Detect click on Battle button
				if (x >= 690 && x <= 790 && y >= 365 && y <= 410 && areAllShipsPlaced()) {
					currMenuItem = "battle";
				}

			}
		}

	}

	glutPostRedisplay();
}

void resize(int width, int height) {
	// we ignore the params and do:
	glutReshapeWindow(screenWidth, screenHeight);
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

	glutReshapeFunc(resize);

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	//glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* Battleship_CPP_ */