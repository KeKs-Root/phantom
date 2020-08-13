#ifndef _D3DSLIDER_H
#define _D3DSLIDER_H

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4229 )
#pragma warning( disable : 4099 )
#pragma warning( disable : 4996 )

/////////////////////////

#include <windows.h>
#include <Winuser.h>
#include <stdio.h>
#include <fstream>
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

#include "D3D11Renderer.h"


//////////////////////////

typedef struct {
	int	typ;		// type of menuline, folder, item
	char	*txt;		// text to show
	char	*txt2;		// text to show
	int	opt;		// array of options
	int	*var;		// variable containing current status
	int	maxval;		// maximumvalue,  normally 1  gives  0=off  1=on
} tSLIDE;

class D3DSlider
{
public:
	D3DSlider(char *Name = "", int maxentries = 99, int LX = 15, int LY = 15, int lenght = 10, int size = 1, int itemss = 2, int waabee = 0)
	{
		show = true;
		title = Name;
		maxitems = maxentries;
		noitems = itemss;
		x = LX;
		y = LY;
		totwidth = lenght;
		longe = lenght;
		option = waabee;
		height = size;
		titleheight = totheight = height + 4;
		// allocate menu array
		SLIDE = (tSLIDE **)malloc(4 * maxitems);
		for (int i = 0; i<maxitems; i++) SLIDE[i] = (tSLIDE *)malloc(sizeof(tSLIDE));
	}
	~D3DSlider() {
		for (int i = 0; i<maxitems; i++) free(SLIDE[i]);
		free(SLIDE);
	}


	// position and sizes
	int			x, y;				// current position of the menu
	int			totwidth, totheight;	// total width and height of menu
	int			height;				// height of 1 menuline
	int			longe;				//  how long u want it DUH
	int			titleheight;		// some extra height for a title

									// menu vars
	char		*title;				// some menu title
	int 		noitems;			// number of menu items
	bool		show;				// If menu = NULL are not
	int			items, option, Value;

	tSLIDE		**SLIDE;

	void AddItem(char *txt, int *var, int opt, int maxvalue = 2);

	void Show(D3D11Renderer *renderer);
	DWORD WINAPI Nav(int item);

private:
	int			maxitems;
};

#endif //D3DSlider Class