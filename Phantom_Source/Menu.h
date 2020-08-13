#ifndef _MENU_H
#define _MENU_H
#ifndef D3DFONT_RIGHT
#define D3DFONT_RIGHT	0x0008
#endif
#ifndef D3DFONT_SHADOW
#define D3DFONT_SHADOW	0x0010
#endif

#define MENUGROUP		1
#define MENUGROUP2		4
#define MENUTEXT		2
#define MENUITEM		3

#define MCOLOR_TITLE	0xffff0000
#define MCOLOR_CURRENT	0xff4d79ff//0094F0 Blue
#define MCOLOR_GROUP	0xffffff00
#define MCOLOR_TEXT		0xff4d79ff //Blue
#define MCOLOR_OFF		0xffffff00
#define MCOLOR_ON		0xff4d79ff//Blue
////////////////////////

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4229 )
#pragma warning( disable : 4099 )
#pragma warning( disable : 4996 )
#include <Windows.h>

typedef struct {
	int  typ;		  // type of menuline, folder, item
	char *txt;	  // text to show
	char **opt;	  // array of options
	int  *var;	  // variable containing current status
	int  maxval;    // maximumvalue,  normally 1  gives  0=off  1=on
} tMENU;

class D3DMenu
{
public:
	// position and sizes
	int			x = 500, y =200;				// current position of the menu
	float width = 450;
	float hight = 212;

	// menu vars
	int 		noitems = 0;			// number of menu items
	bool visible = 0;

	tMENU		**MENU;
	void AddItem(char *txt, int *var, char **opt, int maxval = 1);
	void AddOnOff(char *txt, int *var);
	void AddintRange(char *txt, int *var, int min, int max, int Step);

private:

};
#endif /* Menu.h */
extern bool MakeMenu(void);