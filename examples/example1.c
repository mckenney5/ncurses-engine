#include <stdio.h>
#include "../src/nengine.h"

int main(){

	/* Constant Vars to make the code more readable */
	const int BOX_WIDTH = 51;
	const int BOX_HEIGHT = 11;
	const int SPACE = 1;
	const int PAD = 2;
	const char TITLE[] = "Example Title";

	/* Init must be called before any other function in engine.h */
	Init();
	const unsigned int Top_y = GetTopY(BOX_HEIGHT);
	const unsigned int Top_x = GetTopX(BOX_WIDTH);

	/* Sets default menu color to black text on a white background */
	SetColor(COLOR_BLACK, COLOR_WHITE);

	/* Sets the title color */
	SetTitleColor(COLOR_BLACK, COLOR_WHITE);

	/* Sets up the UI which includes a box with a title, add a shadow to the box, and set the window color */
	SetWindowColor(COLOR_BLUE, COLOR_BLUE);
	GenBox(BOX_WIDTH, BOX_HEIGHT, 1);
	PutTitle(BOX_WIDTH, BOX_HEIGHT, TITLE, sizeof(TITLE)-1);
	int y = Top_y+2; /* Plus 2 is added so the program does not overwrite the title */
	int x = Top_x+PAD;

	/* Display Choices */
	mvprintw(y, x, "Example text with a pad of 2 and a space of 2");
	mvprintw(y+=SPACE, x, "A space of 2 allows the text to be on top of one");
	mvprintw(y+=SPACE, x, "another.");
	mvprintw(y+=SPACE*4, x, "Also note that shadow is on");
	mvprintw(y, x+47, "-->");
	getch();
	return 0;
}
