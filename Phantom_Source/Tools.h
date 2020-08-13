#pragma once
#include <windows.h>
#include <vector>
#include <algorithm>
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

#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "d3d11.lib")

//Get all module related info, this will include the base DLL. 
//and the size of the module
#include <TlHelp32.h>
#include <Psapi.h>

#define BEA_ENGINE_STATIC  /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL    /* specify the usage of a stdcall version of BeaEngine */
#include "BeaEngine\headers\BeaEngine.h"
#pragma comment(lib, "BeaEngineCheetah64.lib")

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
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "d3d11.lib")
using namespace DirectX;
using namespace DirectX::SimpleMath;

class cTools
{
private:
	MODULEINFO GetModuleInfoX(char *szModule);

public:

	DWORD FindPattern(char *pattern, char *mask);
	bool isMouseinRegion(int x1, int y1, int x2, int y2);
	void WriteToMemory(DWORD64 addressToWrite, char* valueToWrite, int byteNum);
	//bool IsGameReadyToFuck();
	const void* DetourFuncVTable(SIZE_T* src, const BYTE* dest, const DWORD index);
	const unsigned int DisasmLengthCheck(const SIZE_T address, const unsigned int jumplength);
	const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength);
	void UnlinkModuleFromPEB(HMODULE hModule);
	void RelinkModuleToPEB(HMODULE hModule);
	void SaveClipboard(char* text);
	std::wstring asciiDecode(const std::string & s);
	float Get3dDistance(Vector3 to, Vector3 from);
	BOOLEAN IsValid(PVOID ptr);
	bool isW2SValid(Vector3 p);
	float GetDistance(float Xx, float Yy, float xX, float yY);
	Vector3 LerpVector(Vector3 A, Vector3 B, int t);
	void Timer(std::function<void(void)> func, unsigned int interval);
};
static cTools *Tools;

extern const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength);
