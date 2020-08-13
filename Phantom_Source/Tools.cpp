#include "Tools.h"

void* detourBuffer[3];
const void* cTools::DetourFuncVTable(SIZE_T* src, const BYTE* dest, const DWORD index)
{
	DWORD dwVirtualProtectBackup;
	SIZE_T* const indexPtr = &src[index];
	const void* origFunc = (void*)*indexPtr;
	VirtualProtect(indexPtr, sizeof(SIZE_T), PAGE_EXECUTE_READWRITE, &dwVirtualProtectBackup);
	*indexPtr = (SIZE_T)dest;
	VirtualProtect(indexPtr, sizeof(SIZE_T), dwVirtualProtectBackup, &dwVirtualProtectBackup);
	return origFunc;
}

void cTools::SaveClipboard(char* text)
{
	HGLOBAL global = GlobalAlloc(GMEM_FIXED, strlen(text) + 1); //text size + \0 character
	memcpy(global, text, strlen(text));  //text size + \0 character
	if (OpenClipboard(NULL))
	{
		EmptyClipboard();
		SetClipboardData(CF_TEXT, global);
		CloseClipboard();
	}
}




void FOV() {
	//DWORD64 BASE = (DWORD64)GetModuleHandle(L"GTA5.exe");
	//DWORD64 FOV = BASE + 0x27AF00;
}

void cTools::WriteToMemory(DWORD64 addressToWrite, char* valueToWrite, int byteNum)
{
	unsigned long OldProtection;
	VirtualProtect((LPVOID)(addressToWrite), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);
	memcpy((LPVOID)(addressToWrite), valueToWrite, byteNum);
	VirtualProtect((LPVOID)(addressToWrite), byteNum, OldProtection, NULL);
}
extern HWND GtaHWND;

extern POINT pPoint;
bool cTools::isMouseinRegion(int x1, int y1, int x2, int y2) {
	if (pPoint.x > x1 && pPoint.x < x2 && pPoint.y > y1 && pPoint.y < y2)
		return true;
	else
		return false;
}

MODULEINFO cTools::GetModuleInfoX(char *szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

DWORD cTools::FindPattern(char *pattern, char *mask)
{
	//Get all module related information
	MODULEINFO mInfo = this->GetModuleInfoX("GTA5.exe");

	//Assign our base anaasd module size
	//Having the values right is ESSENTIAL, this makes sure
	//that we don't scan unwanted memory and leading our game to crash
	DWORD64 base = (DWORD64)mInfo.lpBaseOfDll;
	DWORD64 size = (DWORD64)mInfo.SizeOfImage;

	//Get length for our mask, this will allow us to loop through our array
	DWORD64 patternLength = (DWORD64)strlen(mask);

	for (DWORD64 i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (DWORD64 j = 0; j < patternLength; j++)
		{
			//if we have a ? in our mask then we have true by default, 
			//or if the bytes match then we keep searching until finding it or not
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		//found = true, our entire pattern was found
		//return the memory addr so we can write to it
		if (found)
		{
			return i;
		}
	}

	return NULL;
}

const unsigned int cTools::DisasmLengthCheck(const SIZE_T address, const unsigned int jumplength)
{
	DISASM disasm;
	memset(&disasm, 0, sizeof(DISASM));

	disasm.EIP = (UIntPtr)address;
	disasm.Archi = 0x40;

	unsigned int processed = 0;
	while (processed < jumplength)
	{
		const int len = Disasm(&disasm);
		if (len == UNKNOWN_OPCODE)
		{
			++disasm.EIP;
		}
		else
		{
			processed += len;
			disasm.EIP += len;
		}
	}

	return processed;
}

#ifdef _WIN64
struct HookContext
{
	BYTE original_code[64];
	SIZE_T dst_ptr;
	BYTE far_jmp[6];
};
HookContext* presenthook64;

const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength)
{
	// Allocate a memory page that is going to contain executable code.
	MEMORY_BASIC_INFORMATION mbi;
	for (SIZE_T addr = (SIZE_T)src; addr > (SIZE_T)src - 0x80000000; addr = (SIZE_T)mbi.BaseAddress - 1)
	{
		if (!VirtualQuery((LPCVOID)addr, &mbi, sizeof(mbi)))
		{
			break;
		}

		if (mbi.State == MEM_FREE)
		{
			if (presenthook64 = (HookContext*)VirtualAlloc(mbi.BaseAddress, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE))
			{
				break;
			}
		}
	}

	// If allocating a memory page failed, the function failed.
	if (!presenthook64)
	{
		return NULL;
	}

	// Select a pointer slot for the memory page to be freed on unload.
	for (int i = 0; i < sizeof(detourBuffer) / sizeof(void*); ++i)
	{
		if (!detourBuffer[i])
		{
			detourBuffer[i] = presenthook64;
			break;
		}
	}

	// Save original code and apply detour. The detour code is:
	// push rax
	// movabs rax, 0xCCCCCCCCCCCCCCCC
	// xchg rax, [rsp]
	// ret
	BYTE detour[] = { 0x50, 0x48, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x87, 0x04, 0x24, 0xC3 };
	const unsigned int length = Tools->DisasmLengthCheck((SIZE_T)src, jumplength);
	memcpy(presenthook64->original_code, src, length);
	memcpy(&presenthook64->original_code[length], detour, sizeof(detour));
	*(SIZE_T*)&presenthook64->original_code[length + 3] = (SIZE_T)src + length;

	// Build a far jump to the destination function.
	*(WORD*)&presenthook64->far_jmp = 0x25FF;
	*(DWORD*)(presenthook64->far_jmp + 2) = (DWORD)((SIZE_T)presenthook64 - (SIZE_T)src + FIELD_OFFSET(HookContext, dst_ptr) - 6);
	presenthook64->dst_ptr = (SIZE_T)dest;

	// Write the hook to the original function.
	DWORD flOld = 0;
	VirtualProtect(src, 6, PAGE_EXECUTE_READWRITE, &flOld);
	memcpy(src, presenthook64->far_jmp, sizeof(presenthook64->far_jmp));
	VirtualProtect(src, 6, flOld, &flOld);

	// Return a pointer to the original code.
	return presenthook64->original_code;
}
#else
const void* DetourFunc(BYTE* const src, const BYTE* dest, const DWORD length)
{
	BYTE* jump = new BYTE[length + 5];
	for (int i = 0; i < sizeof(detourBuffer) / sizeof(void*); ++i)
	{
		if (!detourBuffer[i])
		{
			detourBuffer[i] = jump;
			break;
		}
	}

	DWORD dwVirtualProtectBackup;
	VirtualProtect(src, length, PAGE_READWRITE, &dwVirtualProtectBackup);

	memcpy(jump, src, length);
	jump += length;

	jump[0] = 0xE9;
	*(DWORD*)(jump + 1) = (DWORD)(src + length - jump) - 5;

	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dest - src) - 5;

	VirtualProtect(src, length, dwVirtualProtectBackup, &dwVirtualProtectBackup);

	return jump - length;
}
#endif

typedef struct _PEB_LDR_DATA {
	UINT8 _PADDING_[12];
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB {
#ifdef _WIN64
	UINT8 _PADDING_[24];
#else
	UINT8 _PADDING_[12];
#endif
	PEB_LDR_DATA* Ldr;
} PEB, *PPEB;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	VOID* DllBase;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _UNLINKED_MODULE
{
	HMODULE hModule;
	PLIST_ENTRY RealInLoadOrderLinks;
	PLIST_ENTRY RealInMemoryOrderLinks;
	PLIST_ENTRY RealInInitializationOrderLinks;
	PLDR_DATA_TABLE_ENTRY Entry;
} UNLINKED_MODULE;

#define UNLINK(x)					\
	(x).Flink->Blink = (x).Blink;	\
	(x).Blink->Flink = (x).Flink;

#define RELINK(x, real)			\
	(x).Flink->Blink = (real);	\
	(x).Blink->Flink = (real);	\
	(real)->Blink = (x).Blink;	\
	(real)->Flink = (x).Flink;

std::vector<UNLINKED_MODULE> UnlinkedModules;

struct FindModuleHandle
{
	HMODULE m_hModule;
	FindModuleHandle(HMODULE hModule) : m_hModule(hModule)
	{
	}
	bool operator() (UNLINKED_MODULE const &Module) const
	{
		return (Module.hModule == m_hModule);
	}
};

#include <algorithm>
void cTools::RelinkModuleToPEB(HMODULE hModule)
{
	std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));

	if (it == UnlinkedModules.end())
	{
		//DBGOUT(TEXT("Module Not Unlinked Yet!"));
		return;
	}

	RELINK((*it).Entry->InLoadOrderLinks, (*it).RealInLoadOrderLinks);
	RELINK((*it).Entry->InInitializationOrderLinks, (*it).RealInInitializationOrderLinks);
	RELINK((*it).Entry->InMemoryOrderLinks, (*it).RealInMemoryOrderLinks);
	UnlinkedModules.erase(it);
}

void cTools::UnlinkModuleFromPEB(HMODULE hModule)
{
	std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));
	if (it != UnlinkedModules.end())
	{
		//DBGOUT(TEXT("Module Already Unlinked!"));
		return;
	}

#ifdef _WIN64
	PPEB pPEB = (PPEB)__readgsqword(0x60);
#else
	PPEB pPEB = (PPEB)__readfsdword(0x30);
#endif

	PLIST_ENTRY CurrentEntry = pPEB->Ldr->InLoadOrderModuleList.Flink;
	PLDR_DATA_TABLE_ENTRY Current = NULL;

	while (CurrentEntry != &pPEB->Ldr->InLoadOrderModuleList && CurrentEntry != NULL)
	{
		Current = CONTAINING_RECORD(CurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (Current->DllBase == hModule)
		{
			UNLINKED_MODULE CurrentModule = { 0 };
			CurrentModule.hModule = hModule;
			CurrentModule.RealInLoadOrderLinks = Current->InLoadOrderLinks.Blink->Flink;
			CurrentModule.RealInInitializationOrderLinks = Current->InInitializationOrderLinks.Blink->Flink;
			CurrentModule.RealInMemoryOrderLinks = Current->InMemoryOrderLinks.Blink->Flink;
			CurrentModule.Entry = Current;
			UnlinkedModules.push_back(CurrentModule);

			UNLINK(Current->InLoadOrderLinks);
			UNLINK(Current->InInitializationOrderLinks);
			UNLINK(Current->InMemoryOrderLinks);

			break;
		}

		CurrentEntry = CurrentEntry->Flink;
	}
}

float cTools::Get3dDistance(Vector3 to, Vector3 from)
{
	return (sqrt(
		((to.x - from.x) * (to.x - from.x)) +
		((to.y - from.y) * (to.y - from.y)) +
		((to.z - from.z) * (to.z - from.z))
	));
}

#include <Windows.h>
#include <stdio.h> 
#include <string> 
#include <sstream> 
#include <iostream>

std::wstring cTools::asciiDecode(const std::string & s)
{
	std::wostringstream    w;
	wchar_t                c;

	for (size_t i = 0; i < s.length(); i++) {
		mbtowc(&c, &s[i], 1);
		w << c;
	}
	return w.str();
}

#ifdef _WIN64
#define PTRMAXVAL ((PVOID)0x000F000000000000)
#else
#define PTRMAXVAL ((PVOID)0xFFF00000)
#endif
BOOLEAN cTools::IsValid(PVOID ptr)
{
	return (ptr >= (PVOID)0x10000) && (ptr < PTRMAXVAL) && ptr != nullptr;
}

bool cTools::isW2SValid(Vector3 p) {
	return ( p.x > 1.0f && p.y > 1.0f );
}

float cTools::GetDistance(float Xx, float Yy, float xX, float yY)
{
	return sqrt((yY - Yy) * (yY - Yy) + (xX - Xx) * (xX - Xx));
}
Vector3 cTools::LerpVector(Vector3 A, Vector3 B, int t)
{
	return A * t + B * (1.f - t);
}



#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

void  cTools::Timer(std::function<void(void)> func, unsigned int interval)
{
	std::thread([func, interval]() {
		while (true)
		{
			func();
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
		}
	}).detach();
}