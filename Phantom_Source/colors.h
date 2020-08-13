#ifndef _COLORS_H_
#define _COLORS_H_

#pragma once

#include <d3d9.h>

#define d3d9WHITE(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)
#define d3d9BLACK(alpha)         D3DCOLOR_ARGB(alpha, 0, 0, 0)
#define d3d9RED(alpha)           D3DCOLOR_ARGB(alpha, 255, 0, 0)
#define d3d9GREEN(alpha)         D3DCOLOR_ARGB(alpha, 0, 128, 0)
#define d3d9LAWNGREEN(alpha)     D3DCOLOR_ARGB(alpha, 124, 252, 0)
#define d3d9BLUE(alpha)          D3DCOLOR_ARGB(alpha, 0, 200, 255)
#define d3d9DEEPSKYBLUE(alpha)   D3DCOLOR_ARGB(alpha, 0, 191, 255)
#define d3d9SKYBLUE(alpha)       D3DCOLOR_ARGB(alpha, 0, 122, 204)
#define d3d9YELLOW(alpha)        D3DCOLOR_ARGB(alpha, 255, 255, 0)
#define d3d9ORANGE(alpha)        D3DCOLOR_ARGB(alpha, 255, 165, 0)
#define d3d9DARKORANGE(alpha)    D3DCOLOR_ARGB(alpha, 255, 140, 0)
#define d3d9PURPLE(alpha)        D3DCOLOR_ARGB(alpha, 125, 0, 255)
#define d3d9CYAN(alpha)          D3DCOLOR_ARGB(alpha, 0, 255, 255)
#define d3d9PINK(alpha)          D3DCOLOR_ARGB(alpha, 255, 20, 147)
#define d3d9GRAY(alpha)          D3DCOLOR_ARGB(alpha, 128, 128, 128)
#define d3d9DARKGRAY(alpha)      D3DCOLOR_ARGB(alpha, 73, 73, 73)
#define d3d9DARKERGRAY(alpha)    D3DCOLOR_ARGB(alpha, 31, 31, 31)

#endif /* _COLORS_H_ */