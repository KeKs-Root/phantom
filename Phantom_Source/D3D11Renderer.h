#pragma once

typedef unsigned char uchar;

class ColorX
{
public:
	uchar A, R, G, B;

	ColorX()
	{
		A = R = G = B = 0;
	}

	ColorX(uchar A, uchar R, uchar G, uchar B)
	{
		this->A = A;
		this->R = R;
		this->G = G;
		this->B = B;
	}
};

#include "DxINC\CommonStates.h"
#include "DxINC\DDSTextureLoader.h"
#include "DxINC\DirectXHelpers.h"
#include "DxINC\Effects.h"
#include "DxINC\GamePad.h"
#include "DxINC\GeometricPrimitive.h"
#include "DxINC\Model.h"
#include "DxINC\PrimitiveBatch.h"
#include "DxINC\ScreenGrab.h"
#include "DxINC\SimpleMath.h"
#include "DxINC\SpriteBatch.h"
#include "DxINC\SpriteFont.h"
#include "DxINC\VertexTypes.h"
#include "DxINC\WICTextureLoader.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

#pragma warning (push)
#pragma warning (disable: 4005)
#include <d3d9.h>
#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <D3DX11tex.h>
#include <DXGI.h>

#pragma warning (pop)
#include <stdio.h>
#include <Windows.h>

#include "Helper.h"
#include "D3D11StateSaver.h"
#include "D3D11Shader.h"

#define WHITE            ColorX(255, 255, 255, 255)
#define RED                ColorX(255, 255, 000, 000)
#define GREEN            ColorX(255, 000, 255, 000)
#define BLUE            ColorX(255, 000, 000, 255) 
#define BLACK            ColorX(255, 000, 000, 000)
#define mBLACK            ColorX(100, 000, 000, 205)
#define PURPLE            ColorX(255, 125, 000, 255) 
#define GREY            ColorX(255, 220, 220, 220) 
#define GREYdarker      ColorX(255, 200, 200, 200) 
#define YELLOW            ColorX(255, 255, 255, 000) 
#define ORANGE            ColorX(255, 255, 165, 000)  
#define LightBlue        ColorX(255, 0, 191, 255)
#define CYAN        ColorX(255, 0, 255, 255)

class D3D11Renderer
{
private:
	struct COLOR_VERTEX
	{
		D3DXVECTOR3	Position;
		D3DXCOLOR Color;
	};

	IDXGISwapChain *swapChain = NULL;
	ID3D11Device *device = NULL;
	ID3D11DeviceContext *deviceContext = NULL;
	ID3D11InputLayout *mInputLayout = NULL;
	ID3D11Buffer *mVertexBuffer = NULL;
	ID3D11VertexShader *mVS;
	ID3D11PixelShader *mPS;
	ID3D11BlendState *transparency;

	D3D11StateSaver *stateSaver = NULL;

	bool restoreState = false;

public:
	D3D11Renderer(IDXGISwapChain *SwapChain);
	~D3D11Renderer();


	HMODULE Initialize();
	void DrawCursor(float x, float y, bool Engine_Rect = 0);
	void DrawCross(float X, float Y, float Width, float Height, ColorX dColor, bool Engine_Rect = 0, bool Rainbow = 0);
	void DrawDot(float X, float Y, float Width, float Height, ColorX dColor, bool Center, bool Engine_Rect = 0);
	void FillRect(float x, float y, float w, float h, ColorX color, bool Engine_Rect = 0);
	void DrawBorder(float x, float y, float w, float h, float thickness, ColorX BorderColor);
	void DrawBorder2(float x, float y, float w, float h, float thickness, ColorX BorderColor, bool Engine_Rect = 0);
	void DrawBox(float x, float y, float w, float h, ColorX BoxColor, ColorX BorderColor);
	void DrawAimCross(float x, float y, int Size, ColorX CrossColor);
	void DrawLine(float x1, float y1, float x2, float y2, ColorX color);
	void DrawHealthBar(float x, float y, float w, float health, float max);
	void Circle(int X, int Y, int radius, int numSides, ColorX Circle);
	void DrawCircle(int x0, int y0, float radius, float thickness, ColorX color);
	void DrawArmorBar(float x, float y, float w, float Armor, float max);
	void DrawHealthBar(float x, float y, float w, float h, float health, float max, bool Engine_Rect = 0, bool Side = 0);
	void DrawArmorBar(float x, float y, float w, float h, float Armor, float max, bool Engine_Rect = 0, bool Side = 0);
	void Draw2dText(float x, float y, XMVECTORF32 Colortxt, float scale, bool Center, char *szText, ...);
	BOOLEAN W2S(Vector3 WorldPos, Vector3& ScrPos);
	bool Draw3DBox(Vector3 Position, Vector4 Rotation, ColorX BoxColor);
	float GetWidth();
	float GetHeight();
	void BeginScene();
	void EndScene();
};

