/* TODO test all functions */
/* --LICENSE-- */
/*
Copyright (c) 2019 Adam McKenney

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#ifndef _USING_ENGINE
#define _USING_ENGINE
#define _ENGINE_VERSION "Alpha 0.0.02"
#define _ENGINE_DEBUG 0		/* Set to 1 for debugging, 0 for none */
#define ENGINE_NAME "Ncurses Engine"

/* Initializes ncurses */
void Init();

/* Allows user defined terminal size  */
void SetMaxYX(const unsigned int, const unsigned int);

/* Creates the UI */
void GenBox(const unsigned int, const unsigned int, const short);

/* Adds a title to the UI */
void PutTitle(const unsigned int, const unsigned int, const char*, const unsigned int);

/* Displays text and waits for user input */
void Pause();

/* Display warnings */
static void Warn(char*);

/* Display errors */
static void Error(char*);

/* Cleans system on exit */
static void CleanUp();

/* Ignores Warnings */
void DisableWarnings();

/* Shows Warnings, Enabled by default */
void EnableWarnings();

/* Suspends the ncurses window, use Maximize to get it back */
void Minimize();

/* Gets the minimized window  */
void Maximize();

/* Sets the default color */
void SetColor(const unsigned short, const unsigned short);

/* Returns the top y cord. of the box  */
unsigned int GetTopY(const unsigned int);

/* Returns the top x cord. of box  */
unsigned int GetTopX(const unsigned int);

/* Overwrites the screen in a certain color */
void SetWindowColor(const unsigned short, const unsigned short);

/* Sets the default title color */
void SetTitleColor(const unsigned short, const unsigned short);

/* Creates a yes/no dialog box */
//void Dialog(char*, char*);


/* -- */

#include <ncurses.h> 				/* Does the grunt work of the user interface */
#include <stdio.h>				/* fprintf() */
#include <stdlib.h>				/* atexit */

static unsigned int Max_y, Max_x;		/* User defined screen size */
static unsigned int Real_Max_y, Real_Max_x;	/* Actual screen size */
static unsigned short UseWarn = 1;		/* When set to 1, Warnings will apear as they are called */
static unsigned short Initialized = 0;	/* Used to track if Init has been ran, only eddited by Init and CleanUp */

void Init(){
/* This is all needed for ncurses.h */

	/* Checks if Init has been called before */
	if(Initialized == 1){
		Warn("In Init, ncurses has already been initialized. Ignoring");
		return;
	}
	Initialized = 1;

	/* Create the screen */
	initscr();

	/* Cleans up terminal on exit */
	atexit(CleanUp);
	
	/* Dont display user input */
	noecho();

	/* Take all data from the keyboard */
	raw();

	/* Make cursor invisable */
	curs_set(0);
	
	/* Checks if terminal supports color */
	if(!has_colors())
		Warn("In Init, Terminal does not support color");

	/* Use color */
	start_color();

	/* These create the COLOR_PAIR(n) that you see in the source */
	/* Used for shadows */
	init_pair(0, COLOR_BLACK, COLOR_BLACK);

	/* Default color of the program, including the box */
	init_pair(1, COLOR_WHITE, COLOR_BLUE);

	/* Default color of the title of the box, default is the inverse of the default color (1) */
	init_pair(2, COLOR_BLUE, COLOR_WHITE);

	/* Default color of Warnings */
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);

	/* Default color of Debugging console and errors (comming soon (tm)) */
	init_pair(4, COLOR_WHITE, COLOR_RED);

	/* Default window color, only used if SetWindowColor is called */
	init_pair(5, COLOR_RED, COLOR_RED);

	/* Determines Max_y and Max_x */
	getmaxyx(stdscr, Real_Max_y, Real_Max_x);

	/* Sets the max term size, this allows user defined max size */
	SetMaxYX(Real_Max_y, Real_Max_x);
}

void SetMaxYX(const unsigned int y, const unsigned int x){
/* Set custom screen size */
	if(Real_Max_y < y)
		Warn("In SetMaxYX, User defined Y is bigger than actual screen size");
	if(Real_Max_x < x)
		Warn("In SetMaxYX, User defined X is bigger than actual screen size");
	Max_y = y;
	Max_x = x;
}

void GenBox(const unsigned int Width, const unsigned int Height, const short UseShadow){
/* Displays a box */
	/* Checks if Init has been called before */
	if(Initialized == 0) Warn("In GenBox, ncurses has not been initialized. Call Init() before use");

	if(Real_Max_y < Height)
		Warn("In GenBox, Height is set higher than screen size");
	if(Real_Max_x < Width)
		Warn("In GenBox, Width is set higher than screen size");
	attron(COLOR_PAIR(1));
	int i, l;
	unsigned int Mid_y = Max_y/2;
	unsigned int Mid_x = Max_x/2;
	for(i = Mid_y-(Height/2); i <= Height+(Mid_y-(Height/2)); i++){
		for(l = Mid_x-(Width/2); l <= Width+(Mid_x-(Width/2)); l++)
			mvaddch(i, l, ' ');
	}
	if(UseShadow){
		/* Add Shadow */
		/* Does the maths to find the bottom left corner of the box */
		int x = Width+1+(Mid_x-(Width/2));
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(0));
		for(i = Mid_y-(Height/2)+1; i <= Height+1+(Mid_y-(Height/2)); i++){
			mvaddch(i, x, ' ');
		}
		i = Height+1+(Mid_y-(Height/2));
		for(l = Mid_x-(Width/2) +1; l <= Width+(Mid_x-(Width/2)); l++)
				mvaddch(i, l, ' ');
		refresh();
		attroff(COLOR_PAIR(0));
		attron(COLOR_PAIR(1));
	}
}

void PutTitle(const unsigned int Width, const unsigned int Height, const char* Msg, const unsigned int Size){
/* Displays a title on the top line of the box */
	/* Checks if Init has been called before */
	if(Initialized == 0) Warn("In PutTitle, ncurses has not been initialized. Call Init() before use");

	int i;
	attron(COLOR_PAIR(2));
	unsigned int Mid_x = Max_x/2;
	const unsigned int Top_y = (Max_y/2) - (Height/2);
	const unsigned int Top_x = Mid_x-(Width/2);
	if(_ENGINE_DEBUG) mvaddch(Top_y+1, Mid_x, '+');
	/* Clears title */
	for(i = Top_x; i <= Width+(Mid_x-(Width/2)); i++){
		mvaddch(Top_y, i, ' ');
	}
	
	/* Prints title on the center of the top of the box */
	mvprintw(Top_y, Mid_x-(Size/2), "%s", Msg); 
	
	attron(COLOR_PAIR(1));
}

void Pause(){
/* TODO make better pause functions */
/* Simple wait functions that waits for a user to press a key */
	/* Checks if Init has been called before */
	if(Initialized == 0) Warn("In Pause, ncurses has not been initialized. Call Init() before use");

	printw(" Press any key to continue...");
	getch();
}

static void Warn(char * Mesg){
/* Display a warning if it is enabled */
	if(UseWarn == 0) return;
	const char Prefix[] = "[Warning] ";
	fprintf(stderr, "%s: %s%s\n", ENGINE_NAME, Prefix, Mesg);
	/* Checks if Init has been called before */
	if(Initialized == 0) return;
	move(0,0);
	attron(COLOR_PAIR(3));
	printw("%s: %s%s\n", ENGINE_NAME, Prefix, Mesg);
	attron(COLOR_PAIR(1));
	if(_ENGINE_DEBUG) getch();
}

static void Error(char * Mesg){
/* Display an error */
	const char Prefix[] = "[Error] ";
	fprintf(stderr, "%s: %s%s\n", ENGINE_NAME, Prefix, Mesg);
	if(Initialized == 0) return;
	move(0,0);
	attron(COLOR_PAIR(4));
	printw("%s: %s%s\n", ENGINE_NAME, Prefix, Mesg);
	attron(COLOR_PAIR(1));
	getch();
}

static void CleanUp(){
/* Destroys window before exit */
	endwin();
	Initialized = 0; //since ncurses is closing, allow it to be restarted
}

void DisableWarnings(){
	UseWarn = 0;
}

void EnableWarnings(){
	UseWarn = 1;
}

void Minimize(){
/* Suspend ncurses */
	if(Initialized == 0) Warn("In Minimize, ncurses has not been initialized. Call Init() before use");
	def_prog_mode();
	endwin();
}

void Maximize(){
/* Resume ncurses */
	if(Initialized == 0) Warn("In Maximize, ncurses has not been initialized. Call Init() before use");
	reset_prog_mode();
	refresh();
}

void SetColor(const unsigned short FColor, const unsigned short BColor){
/* Checks if it is given a valid color, then sets it */
	if(Initialized == 0) Warn("In SetColor, ncurses has not been initialized. Call Init() before use");
	if(FColor > 7 || BColor > 7){
		Error("In SetColor, Color does not exist, colors are 0 - 7");
	} else {
		init_pair(1, FColor, BColor);
	}
}

unsigned int GetTopY(const unsigned int Height){
	unsigned int Mid_y = Max_y/2;
	return Mid_y-(Height/2);
}

unsigned int GetTopX(const unsigned int Width){
	unsigned int Mid_x = Max_x/2;
	return Mid_x-(Width/2);
}

void SetWindowColor(const unsigned short FColor, const unsigned short BColor){
	if(Initialized == 0) Warn("In SetWindowColor, ncurses has not been initialized. Call Init() before use");
	if(FColor > 7 || BColor > 7){
		Error("In SetWindowColor, Color does not exist, colors are 0 - 7");
	} else {
		init_pair(5, FColor, BColor);
		attron(COLOR_PAIR(5));
	 	int y, x, i, l;
		getmaxyx(stdscr, y,x);
	 	for(i = 0; i <= y; i++){
	 		for(l = 0; l <= x; l++)
				mvaddch(i,l, ' ');
		}
		attroff(COLOR_PAIR(5));
		attron(COLOR_PAIR(1));
	}
}

void SetTitleColor(const unsigned short FColor, const unsigned short BColor){
/* Checks if it is given a valid color, then sets it */
	if(Initialized == 0) Warn("In SetTitleColor, ncurses has not been initialized. Call Init() before use");
	if(FColor > 7 || BColor > 7){
		Error("In SetTitleColor, Color does not exist, colors are 0 - 7");
	} else {
		init_pair(2, FColor, BColor);
	}
}

/*
void Dialog(char* Title, char* Prompt){
	return;
}*/
#endif

