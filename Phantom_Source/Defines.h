//#pragma once
//typedef void*(__fastcall* GetBoneFromMask)(CObject* pThis, Vector4& vBonePos, WORD dwMask);
//typedef void*(__fastcall* GetBoneFromMask2)(CObject* pThis, Vector4& vBonePos, WORD dwMask);
//static GetBoneFromMask GetBone1;
//static GetBoneFromMask2 GetBone2;
//#define D3DX_PI 3.14159265
//#include <windows.h>
//#include <vector>
//#include <algorithm>
//HWND GtaHWND;
//
//typedef struct _PEB_LDR_DATA {
//	UINT8 _PADDING_[12];
//	LIST_ENTRY InLoadOrderModuleList;
//	LIST_ENTRY InMemoryOrderModuleList;
//	LIST_ENTRY InInitializationOrderModuleList;
//} PEB_LDR_DATA, *PPEB_LDR_DATA;
//
//typedef struct _PEB {
//#ifdef _WIN64
//	UINT8 _PADDING_[24];
//#else
//	UINT8 _PADDING_[12];
//#endif
//	PEB_LDR_DATA* Ldr;
//} PEB, *PPEB;
//
//typedef struct _LDR_DATA_TABLE_ENTRY {
//	LIST_ENTRY InLoadOrderLinks;
//	LIST_ENTRY InMemoryOrderLinks;
//	LIST_ENTRY InInitializationOrderLinks;
//	VOID* DllBase;
//} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;
//
//typedef struct _UNLINKED_MODULE
//{
//	HMODULE hModule;
//	PLIST_ENTRY RealInLoadOrderLinks;
//	PLIST_ENTRY RealInMemoryOrderLinks;
//	PLIST_ENTRY RealInInitializationOrderLinks;
//	PLDR_DATA_TABLE_ENTRY Entry;
//} UNLINKED_MODULE;
//
//#define UNLINK(x)					\
//	(x).Flink->Blink = (x).Blink;	\
//	(x).Blink->Flink = (x).Flink;
//
//#define RELINK(x, real)			\
//	(x).Flink->Blink = (real);	\
//	(x).Blink->Flink = (real);	\
//	(real)->Blink = (x).Blink;	\
//	(real)->Flink = (x).Flink;
//
//std::vector<UNLINKED_MODULE> UnlinkedModules;
//
//struct FindModuleHandle
//{
//	HMODULE m_hModule;
//	FindModuleHandle(HMODULE hModule) : m_hModule(hModule)
//	{
//	}
//	bool operator() (UNLINKED_MODULE const &Module) const
//	{
//		return (Module.hModule == m_hModule);
//	}
//};
//
//void RelinkModuleToPEB(HMODULE hModule)
//{
//	std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));
//
//	if (it == UnlinkedModules.end())
//	{
//		//DBGOUT(TEXT("Module Not Unlinked Yet!"));
//		return;
//	}
//
//	RELINK((*it).Entry->InLoadOrderLinks, (*it).RealInLoadOrderLinks);
//	RELINK((*it).Entry->InInitializationOrderLinks, (*it).RealInInitializationOrderLinks);
//	RELINK((*it).Entry->InMemoryOrderLinks, (*it).RealInMemoryOrderLinks);
//	UnlinkedModules.erase(it);
//}
//
//void UnlinkModuleFromPEB(HMODULE hModule)
//{
//	std::vector<UNLINKED_MODULE>::iterator it = std::find_if(UnlinkedModules.begin(), UnlinkedModules.end(), FindModuleHandle(hModule));
//	if (it != UnlinkedModules.end())
//	{
//		//DBGOUT(TEXT("Module Already Unlinked!"));
//		return;
//	}
//
//#ifdef _WIN64
//	PPEB pPEB = (PPEB)__readgsqword(0x60);
//#else
//	PPEB pPEB = (PPEB)__readfsdword(0x30);
//#endif
//
//	PLIST_ENTRY CurrentEntry = pPEB->Ldr->InLoadOrderModuleList.Flink;
//	PLDR_DATA_TABLE_ENTRY Current = NULL;
//
//	while (CurrentEntry != &pPEB->Ldr->InLoadOrderModuleList && CurrentEntry != NULL)
//	{
//		Current = CONTAINING_RECORD(CurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
//		if (Current->DllBase == hModule)
//		{
//			UNLINKED_MODULE CurrentModule = { 0 };
//			CurrentModule.hModule = hModule;
//			CurrentModule.RealInLoadOrderLinks = Current->InLoadOrderLinks.Blink->Flink;
//			CurrentModule.RealInInitializationOrderLinks = Current->InInitializationOrderLinks.Blink->Flink;
//			CurrentModule.RealInMemoryOrderLinks = Current->InMemoryOrderLinks.Blink->Flink;
//			CurrentModule.Entry = Current;
//			UnlinkedModules.push_back(CurrentModule);
//
//			UNLINK(Current->InLoadOrderLinks);
//			UNLINK(Current->InInitializationOrderLinks);
//			UNLINK(Current->InMemoryOrderLinks);
//
//			break;
//		}
//
//		CurrentEntry = CurrentEntry->Flink;
//	}
//}
//
//const char* BlackList_Windows[] = { "Cheat Engine 6.4", "Cheat Engine 6.5", "Cheat Engine 6.6", "Cheat Engine 6.7" };
//const char* BlackList_Class[] = { "Window", "UPP-CLASS-W", "QtQWindowIcon", "WinDbgFrameClass", "OLLYDBG" };
//void Guard_Blacklist() {
//	for (int i = 0; i < sizeof(BlackList_Windows); i++) {
//		if(FindWindow(NULL, BlackList_Windows[i]))ExitThread(-1); //Kill Window
//	}
//	for (int i = 0; i < sizeof(BlackList_Class); i++) {
//		if (FindWindow(BlackList_Class[i], NULL))ExitThread(-1); //Kill Window
//	}
//	Sleep(1000); //Slow it down (Cpu Usage saver)
//}
//
//void SaveClipboard(char* text)
//{
//	HGLOBAL global = GlobalAlloc(GMEM_FIXED, strlen(text) + 1); //text size + \0 character
//	memcpy(global, text, strlen(text));  //text size + \0 character
//	if (OpenClipboard(NULL))
//	{
//		EmptyClipboard();
//		SetClipboardData(CF_TEXT, global);
//		CloseClipboard();
//	}
//}
//#include "D3D11Renderer.h"
//D3D11Renderer *renderer;
//
//static BOOLEAN GetBonePosition2(CObject* pThis, Vector3 *BonePosOut, WORD Mask)
//{
//	__declspec(align(16)) Vector4 BonePos(0, 0, 0, 0);
//
//	if (static_cast<bool>(GetBone2(pThis, BonePos, Mask)))
//	{
//		BonePosOut->x = BonePos.x;
//		BonePosOut->y = BonePos.y;
//		BonePosOut->z = BonePos.z;
//			return TRUE;
//	}
//	return FALSE;
//}
//
//using namespace std::chrono;
//void FPSCheck(std::string& str)
//{
//	static high_resolution_clock::time_point lastTime;
//	static int ticks = 0;
//	auto now = high_resolution_clock::now();
//	auto secs = duration_cast<seconds>(now - lastTime);
//	ticks++;
//	if (secs.count() >= 1)
//	{
//		str = std::to_string(ticks / secs.count());
//		ticks = 0;
//		lastTime = now;
//	}
//}
//
//float Get3dDistance(Vector3 to, Vector3 from)
//{
//	return (sqrt(
//		((to.x - from.x) * (to.x - from.x)) +
//		((to.y - from.y) * (to.y - from.y)) +
//		((to.z - from.z) * (to.z - from.z))
//	));
//}
//
//float GetDistance2d(float Xx, float Yy, float xX, float yY)
//{
//	return sqrt((yY - Yy) * (yY - Yy) + (xX - Xx) * (xX - Xx));
//}
//
//DWORD64 baseAddress;
//
//#pragma region Bones
//	#define SKEL_ROOT 0x0
//	#define SKEL_Pelvis 0x2e28
//	#define SKEL_L_Thigh 0xe39f
//	#define SKEL_L_Calf 0xf9bb
//	#define SKEL_L_Foot 0x3779
//	#define SKEL_L_Toe0 0x83c
//	#define IK_L_Foot 0xfedd
//	#define PH_L_Foot 0xe175
//	#define MH_L_Knee 0xb3fe
//	#define SKEL_R_Thigh 0xca72
//	#define SKEL_R_Calf 0x9000
//	#define SKEL_R_Foot 0xcc4d
//	#define SKEL_R_Toe0 0x512d
//	#define IK_R_Foot 0x8aae
//	#define PH_R_Foot 0x60e6
//	#define MH_R_Knee 0x3fcf
//	#define RB_L_ThighRoll 0x5c57
//	#define RB_R_ThighRoll 0x192a
//	#define SKEL_Spine_Root 0xe0fd
//	#define SKEL_Spine0 0x5c01
//	#define SKEL_Spine1 0x60f0
//	#define SKEL_Spine2 0x60f1
//	#define SKEL_Spine3 0x60f2
//	#define SKEL_L_Clavicle 0xfcd9
//	#define SKEL_L_UpperArm 0xb1c5
//	#define SKEL_L_Forearm 0xeeeb
//	#define SKEL_L_Hand 0x49d9
//	#define SKEL_L_Finger00 0x67f2
//	#define SKEL_L_Finger01 0xff9
//	#define SKEL_L_Finger02 0xffa
//	#define SKEL_L_Finger10 0x67f3
//	#define SKEL_L_Finger11 0x1049
//	#define SKEL_L_Finger12 0x104a
//	#define SKEL_L_Finger20 0x67f4
//	#define SKEL_L_Finger21 0x1059
//	#define SKEL_L_Finger22 0x105a
//	#define SKEL_L_Finger30 0x67f5
//	#define SKEL_L_Finger31 0x1029
//	#define SKEL_L_Finger32 0x102a
//	#define SKEL_L_Finger40 0x67f6
//	#define SKEL_L_Finger41 0x1039
//	#define SKEL_L_Finger42 0x103a
//	#define PH_L_Hand 0xeb95
//	#define IK_L_Hand 0x8cbd
//	#define RB_L_ForeArmRoll 0xee4f
//	#define RB_L_ArmRoll 0x1470
//	#define MH_L_Elbow 0x58b7
//	#define SKEL_R_Clavicle 0x29d2 //schulter oder auch Schlüsselbein
//	#define SKEL_R_UpperArm 0x9d4d
//	#define SKEL_R_Forearm 0x6e5c
//	#define SKEL_R_Hand 0xdead
//	#define SKEL_R_Finger00 0xe5f2
//	#define SKEL_R_Finger01 0xfa10
//	#define SKEL_R_Finger02 0xfa11
//	#define SKEL_R_Finger10 0xe5f3
//	#define SKEL_R_Finger11 0xfa60
//	#define SKEL_R_Finger12 0xfa61
//	#define SKEL_R_Finger20 0xe5f4
//	#define SKEL_R_Finger21 0xfa70
//	#define SKEL_R_Finger22 0xfa71
//	#define SKEL_R_Finger30 0xe5f5
//	#define SKEL_R_Finger31 0xfa40
//	#define SKEL_R_Finger32 0xfa41
//	#define SKEL_R_Finger40 0xe5f6
//	#define SKEL_R_Finger41 0xfa50
//	#define SKEL_R_Finger42 0xfa51
//	#define PH_R_Hand 0x6f06
//	#define IK_R_Hand 0x188e
//	#define RB_R_ForeArmRoll 0xab22
//	#define RB_R_ArmRoll 0x90ff
//	#define MH_R_Elbow 0xbb0
//	#define SKEL_Neck_1 0x9995
//	#define SKEL_Head 0x796e
//	#define IK_Head 0x322c
//	#define FACIAL_facialRoot 0xfe2c
//	#define FB_L_Brow_Out_000 0xe3db
//	#define FB_L_Lid_Upper_000 0xb2b6
//	#define FB_L_Eye_000 0x62ac
//	#define FB_L_CheekBone_000 0x542e
//	#define FB_L_Lip_Corner_000 0x74ac
//	#define FB_R_Lid_Upper_000 0xaa10
//	#define FB_R_Eye_000 0x6b52
//	#define FB_R_CheekBone_000 0x4b88
//	#define FB_R_Brow_Out_000 0x54c
//	#define FB_R_Lip_Corner_000 0x2ba6
//	#define FB_Brow_Centre_000 0x9149
//	#define FB_UpperLipRoot_000 0x4ed2
//	#define FB_UpperLip_000 0xf18f
//	#define FB_L_Lip_Top_000 0x4f37
//	#define FB_R_Lip_Top_000 0x4537
//	#define FB_Jaw_000 0xb4a0
//	#define FB_LowerLipRoot_000 0x4324
//	#define FB_LowerLip_000 0x508f
//	#define FB_L_Lip_Bot_000 0xb93b
//	#define FB_R_Lip_Bot_000 0xc33b
//	#define FB_Tongue_000 0xb987
//	#define RB_Neck_1 0x8b93
//	#define IK_Root 0xdd1c
//	#define STOMACH           0x60F0
//	#define BONETAG_HEAD		0x796E 
//	#define BONETAG_CHEST		0x60F2
//#pragma endregion
//
//#define WHITE            ColorX(255, 255, 255, 255)
//#define RED                ColorX(255, 255, 000, 000)
//#define GREEN            ColorX(255, 000, 255, 000)
//#define BLUE            ColorX(255, 000, 000, 255) 
//#define BLACK            ColorX(255, 000, 000, 000)
//#define mBLACK            ColorX(100, 000, 000, 205)
//#define PURPLE            ColorX(255, 125, 000, 255) 
//#define GREY            ColorX(255, 220, 220, 220) 
//#define GREYdarker      ColorX(255, 200, 200, 200) 
//#define YELLOW            ColorX(255, 255, 255, 000) 
//#define ORANGE            ColorX(255, 255, 165, 000)  
//#define LightBlue        ColorX(255, 0, 191, 255)
//
//#include <TlHelp32.h>
//#include <Psapi.h>
//
////Get all module related info, this will include the base DLL. 
////and the size of the module
//
//
//
//
//MODULEINFO GetModuleInfoX(char *szModule)
//{
//	MODULEINFO modinfo = { 0 };
//	HMODULE hModule = GetModuleHandle(szModule);
//	if (hModule == 0)
//		return modinfo;
//	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
//	return modinfo;
//}
//
//
//DWORD FindPattern(char *pattern, char *mask)
//{
//	//Get all module related information
//	MODULEINFO mInfo = GetModuleInfoX("GTA5.exe");
//
//	//Assign our base anaasd module size
//	//Having the values right is ESSENTIAL, this makes sure
//	//that we don't scan unwanted memory and leading our game to crash
//	DWORD64 base = (DWORD64)mInfo.lpBaseOfDll;
//	DWORD64 size = (DWORD64)mInfo.SizeOfImage;
//
//	//Get length for our mask, this will allow us to loop through our array
//	DWORD64 patternLength = (DWORD64)strlen(mask);
//
//	for (DWORD64 i = 0; i < size - patternLength; i++)
//	{
//		bool found = true;
//		for (DWORD64 j = 0; j < patternLength; j++)
//		{
//			//if we have a ? in our mask then we have true by default, 
//			//or if the bytes match then we keep searching until finding it or not
//			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
//		}
//
//		//found = true, our entire pattern was found
//		//return the memory addr so we can write to it
//		if (found)
//		{
//			return i;
//		}
//	}
//
//	return NULL;
//}
//
////LRESULT __stdcall HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
////WNDPROC OldWndProc = nullptr;
////
////LRESULT __stdcall HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
////{
////	//ImGuiIO& io = ImGui::GetIO();
////	switch (uMsg)
////	{
////	case WM_LBUTTONDOWN:
////		io.MouseDown[0] = true;
////		return true;
////	case WM_LBUTTONUP:
////		io.MouseDown[0] = false;
////		return true;
////	case WM_RBUTTONDOWN:
////		io.MouseDown[1] = true;
////		return true;
////	case WM_RBUTTONUP:
////		io.MouseDown[1] = false;
////		return true;
////	case WM_MBUTTONDOWN:
////		io.MouseDown[2] = true;
////		return true;
////	case WM_MBUTTONUP:
////		io.MouseDown[2] = false;
////		return true;
////	case WM_MOUSEWHEEL:
////		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
////		return true;
////	case WM_MOUSEMOVE:
////		io.MousePos.x = (signed short)(lParam);
////		io.MousePos.y = (signed short)(lParam >> 16);
////		return true;
////	case WM_KEYDOWN:
////		if (wParam < 256)
////			io.KeysDown[wParam] = 1;
////		return true;
////	case WM_KEYUP:
////		if (wParam < 256)
////			io.KeysDown[wParam] = 0;
////		return true;
////	case WM_CHAR:
////		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
////		if (wParam > 0 && wParam < 0x10000)
////			io.AddInputCharacter((unsigned short)wParam);
////		return true;
////	}
////
////	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
////}
////
////void InitializeWnd()
////{
////	if (GtaHWND)OldWndProc = (WNDPROC)SetWindowLongPtr(GtaHWND, GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
////}
////
////void RestoreWnd()
////{
////	SetWindowLongPtr(GtaHWND, GWLP_WNDPROC, (LONG_PTR)OldWndProc);
////}
//
//void WriteToMemory(DWORD64 addressToWrite, char* valueToWrite, int byteNum)
//{
//	unsigned long OldProtection;
//	VirtualProtect((LPVOID)(addressToWrite), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);
//	memcpy((LPVOID)(addressToWrite), valueToWrite, byteNum);
//	VirtualProtect((LPVOID)(addressToWrite), byteNum, OldProtection, NULL);
//}
//
//bool isMouseinRegion(int x1, int y1, int x2, int y2) {
//	POINT cPos;
//	GetCursorPos(&cPos); ScreenToClient(GtaHWND, &cPos);
//	if (cPos.x > x1 && cPos.x < x2 && cPos.y > y1 && cPos.y < y2)
//		return true;
//	else
//		return false;
//}
//
//enum ExplosionTypes
//{
//	EXPLOSION_GRENADE,
//	EXPLOSION_GRENADELAUNCHER,
//	EXPLOSION_STICKYBOMB,
//	EXPLOSION_MOLOTOV,
//	EXPLOSION_ROCKET,
//	EXPLOSION_TANKSHELL,
//	EXPLOSION_HI_OCTANE,
//	EXPLOSION_CAR,
//	EXPLOSION_PLANE,
//	EXPLOSION_PETROL_PUMP,
//	EXPLOSION_BIKE,
//	EXPLOSION_DIR_STEAM,
//	EXPLOSION_DIR_FLAME,
//	EXPLOSION_DIR_WATER_HYDRANT,
//	EXPLOSION_DIR_GAS_CANISTER,
//	EXPLOSION_BOAT,
//	EXPLOSION_SHIP_DESTROY,
//	EXPLOSION_TRUCK,
//	EXPLOSION_BULLET,
//	EXPLOSION_SMOKEGRENADELAUNCHER,
//	EXPLOSION_SMOKEGRENADE,
//	EXPLOSION_BZGAS,
//	EXPLOSION_FLARE,
//	EXPLOSION_GAS_CANISTER,
//	EXPLOSION_EXTINGUISHER,
//	EXPLOSION_PROGRAMMABLEAR,
//	EXPLOSION_TRAIN,
//	EXPLOSION_BARREL,
//	EXPLOSION_PROPANE,
//	EXPLOSION_BLIMP,
//	EXPLOSION_DIR_FLAME_EXPLODE,
//	EXPLOSION_TANKER,
//	EXPLOSION_PLANE_ROCKET,
//	EXPLOSION_VEHICLE_BULLET,
//	EXPLOSION_GAS_TANK,
//	EXPLOSION_BIRD_CRAP
//};
//
//enum WeaponHashes
//{
//	WEAPON_ADVANCEDRIFLE = 0xAF113F99,
//	WEAPON_AIRSTRIKE_ROCKET = 0x13579279,
//	WEAPON_ANIMAL = 0xF9FBAEBE,
//	WEAPON_APPISTOL = 0x22D8FE39,
//	WEAPON_ASSAULTRIFLE = 0xBFEFFF6D,
//	WEAPON_ASSAULTSHOTGUN = 0xE284C527,
//	WEAPON_ASSAULTSMG = 0xEFE7E2DF,
//	WEAPON_TEC9 = 0xDB1AA450,
//	WEAPON_DOUBLEBARRELED = 0xDB1AA450,
//	WEAPON_BALL = 0x23C9F95C,
//	WEAPON_BARBED_WIRE = 0x48E7B178,
//	WEAPON_BAT = 0x958A4A8F,
//	WEAPON_BLEEDING = 0x8B7333FB,
//	WEAPON_BOTTLE = 0xF9E6AA4B,
//	WEAPON_BRIEFCASE = 0x88C78EB7,
//	WEAPON_BRIEFCASE_02 = 0x01B79F17,
//	WEAPON_BULLPUPRIFLE = 0x7F229F94,
//	WEAPON_COMPACTRIFLE = 0x624FE830,
//	WEAPON_BULLPUPSHOTGUN = 0x9D61E50F,
//	WEAPON_BZGAS = 0xA0973D5E,
//	WEAPON_CARBINERIFLE = 0x83BF0278,
//	WEAPON_COMBATMG = 0x7FD62962,
//	WEAPON_PARACHUTE = 0xFBAB5776,
//	WEAPON_COMBATPDW = 0xA3D4D34,
//	WEAPON_COMBATPISTOL = 0x5EF9FEC4,
//	WEAPON_COUGAR = 0x08D4BE52,
//	WEAPON_CROWBAR = 0x84BD7BFD,
//	WEAPON_DAGGER = 0x92A27487,
//	WEAPON_DIGISCANNER = 0xFDBADCED,
//	WEAPON_DROWNING = 0xFF58C4FB,
//	WEAPON_DROWNING_IN_VEHICLE = 0x736F5990,
//	WEAPON_ELECTRIC_FENCE = 0x92BD4EBB,
//	WEAPON_EXHAUSTION = 0x364A29EC,
//	WEAPON_EXPLOSION = 0x2024F4E8,
//	WEAPON_FALL = 0xCDC174B0,
//	WEAPON_FIRE = 0xDF8E89EB,
//	WEAPON_FIREEXTINGUISHER = 0x060EC506,
//	WEAPON_FIREWORK = 0x7F7497E5,
//	WEAPON_FLARE = 0x497FACC3,
//	WEAPON_FLAREGUN = 0x47757124,
//	WEAPON_GARBAGEBAG = 0xE232C28C,
//	WEAPON_GOLFCLUB = 0x440E4788,
//	WEAPON_GRENADE = 0x93E220BD,
//	WEAPON_GRENADELAUNCHER = 0xA284510B,
//	WEAPON_GRENADELAUNCHER_SMOKE = 0x4DD2DC56,
//	WEAPON_GUSENBERG = 0x61012683,
//	WEAPON_HAMMER = 0x4E875F73,
//	WEAPON_HANDCUFFS = 0xD04C944D,
//	WEAPON_HEAVYPISTOL = 0xD205520E,
//	WEAPON_HEAVYSHOTGUN = 0x3AABBBAA,
//	WEAPON_HEAVYSNIPER = 0x0C472FE2,
//	WEAPON_HELI_CRASH = 0x145F1012,
//	WEAPON_HIT_BY_WATER_CANNON = 0xCC34325E,
//	WEAPON_HOMINGLAUNCHER = 0x63AB0442,
//	WEAPON_KNIFE = 0x99B507EA,
//	WEAPON_KNUCKLE = 0xD8DF3C3C,
//	WEAPON_MARKSMANPISTOL = 0xDC4DB296,
//	WEAPON_MARKSMANRIFLE = 0xC734385A,
//	WEAPON_MG = 0x9D07F764,
//	WEAPON_HATCHET = 0xF9DCBF2D,
//	WEAPON_FLASHLIGHT = 0x8BB05FD7,
//	WEAPON_MACHINEPISTOL = 0xDB1AA450,
//	WEAPON_MACHETE = 0xDD5DF8D9,
//	WEAPON_SWITCHBLADE = 0xDFE37640,
//	WEAPON_REVOLVER = 0xC1B3C3D1,
//	WEAPON_MICROSMG = 0x13532244,
//	WEAPON_MINIGUN = 0x42BF8A85,
//	WEAPON_MOLOTOV = 0x24B17070,
//	WEAPON_MUSKET = 0xA89CB99E,
//	WEAPON_NIGHTSTICK = 0x678B81B1,
//	WEAPON_PASSENGER_ROCKET = 0x166218FF,
//	WEAPON_PETROLCAN = 0x34A67B97,
//	WEAPON_PISTOL = 0x1B06D571,
//	WEAPON_PISTOL50 = 0x99AEEB3B,
//	WEAPON_PROXMINE = 0xAB564B93,
//	WEAPON_PUMPSHOTGUN = 0x1D073A89,
//	WEAPON_RAILGUN = 0x6D544C99,
//	WEAPON_RAMMED_BY_CAR = 0x07FC7D7A,
//	WEAPON_REMOTESNIPER = 0x33058E22,
//	WEAPON_RPG = 0xB1CA77B1,
//	WEAPON_RUN_OVER_BY_CAR = 0xA36D413E,
//	WEAPON_SAWNOFFSHOTGUN = 0x7846A318,
//	WEAPON_SMG = 0x2BE6766B,
//	WEAPON_SMOKEGRENADE = 0xFDBC8A50,
//	WEAPON_SNIPERRIFLE = 0x05FC3C11,
//	WEAPON_SNOWBALL = 0x787F0BB,
//	WEAPON_SNSPISTOL = 0xBFD21232,
//	WEAPON_SPECIALCARBINE = 0xC0A3098D,
//	WEAPON_STICKYBOMB = 0x2C3731D9,
//	WEAPON_STINGER = 0x687652CE,
//	WEAPON_STUNGUN = 0x3656C8C1,
//	WEAPON_UNARMED = 0xA2719263,
//	WEAPON_VEHICLE_ROCKET = 0xBEFDC581,
//	WEAPON_VINTAGEPISTOL = 0x083839C4
//};