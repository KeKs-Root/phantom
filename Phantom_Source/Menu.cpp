#include "Menu.h"
#include "D3D11Renderer.h"
#include "Tools.h"
#include "Classes.h"

bool Aimbot_Menu = 1;
bool Npc_Menu = 0;
bool Weapon_Menu = 0;
bool Radar_Menu = 0;
bool Visual_Menu = 0;
bool Other_Menu = 0;

bool Save = 0;
bool Load = 0;
bool Crash = 0;

#define MenuBack      ColorX(120, 51, 51, 51) 
#define MenuIsEnabled      ColorX(100, 38, 38, 38) 
#define OrangeRedX      ColorX(255, 255, 69, 000) 
#define CyanOutline      ColorX(150, 0, 255, 255)

D3DMenu *pMenu = nullptr;
D3D11Renderer *renderer = nullptr;

void DrawButton(float x, float y, float w, float h, char *txt, bool *Option) {
	renderer->DrawBox(x, y, w, h, ColorX(50, 0, 0, 0), CyanOutline);
	renderer->Draw2dText(x + (w / 2.0f), y + (h / 2.0f), (Tools->isMouseinRegion(x, y, x + w, y + h) ? Colors::OrangeRed : Colors::White), 0.5f, true, txt);
	if (*Option)renderer->FillRect(x, y, 8, h, ColorX(150, 0, 255, 255));
	static bool SendKey = false;
	if (GetKeyState(VK_LBUTTON) & 0x800){
	if (Tools->isMouseinRegion(x, y, x + w, y + h) && SendKey)
		SendKey = false, Aimbot_Menu = 0, Npc_Menu = 0, Weapon_Menu = 0, Radar_Menu = 0, Visual_Menu = 0, Other_Menu = 0, *Option = true;

	} else {
		SendKey = true;
	}
}

void DrawButtonHotKey(float x, float y, float w, float h, BYTE *HotKey, char *txt, char *txt2) {
	renderer->DrawBox(x, y, w, h, ColorX(50, 0, 0, 0), CyanOutline);
	renderer->Draw2dText(x + w/2, y - h/2, Colors::White, 0.5f, true, txt2);
	renderer->Draw2dText(x + (w / 2.0f), y + (h / 2.0f), (Tools->isMouseinRegion(x, y, x + w, y + h) ? Colors::OrangeRed : Colors::White), 0.5f, true, txt);
	if (Tools->isMouseinRegion(x, y, x + w, y + h))
		if (GetKeyState(VK_LBUTTON) & 0x800){
			*HotKey = 0;
		} else {
			if (*HotKey == 0){
				for (BYTE i = 0; i < 0xFF; i++)
					if(i != 255)
						if (GetKeyState(i) & 0x800)
							*HotKey = i;
			}
		}

}

void D3DMenu::AddItem(char *txt, int *var, char **opt, int maxval)
{
	renderer->Draw2dText(pMenu->x + 115.0f, pMenu->y + 15.0f + ((float)pMenu->noitems * 20.0f), Colors::White, 0.5f, false, txt);

	static bool SendKey = false;
	if (GetKeyState(VK_LBUTTON) & 0x800) {
		if (Tools->isMouseinRegion(pMenu->x + 280.0f, pMenu->y + 10.0f + ((float)pMenu->noitems * 20.0f), pMenu->x + 350.0f, pMenu->y + 10.0f + ((float)pMenu->noitems * 20.0f) + 20.0f) && *var > 0 && SendKey == true)
			SendKey = false, *var -= 1;
		if (Tools->isMouseinRegion(pMenu->x + 350.0f, pMenu->y + 10.0f + ((float)pMenu->noitems * 20.0f), pMenu->x + 420.0f, pMenu->y + 10.0f + ((float)pMenu->noitems * 20.0f) + 20.0f) && *var < maxval && SendKey == true)
			SendKey = false, *var += 1;
	} else {
		SendKey = true;
	}
	renderer->Draw2dText(pMenu->x + 300.0f, pMenu->y + 15.0f + ((float)pMenu->noitems * 20.0f), Colors::White, 0.5f, true, "<-");
	renderer->Draw2dText(pMenu->x + 350.0f, pMenu->y + 15.0f + ((float)pMenu->noitems * 20.0f), !*var ? Colors::Red : Colors::Lime, 0.5f, true, (char *)opt[*var]);
	renderer->Draw2dText(pMenu->x + 400.0f, pMenu->y + 15.0f + ((float)pMenu->noitems * 20.0f), Colors::White, 0.5f, true, "->");

	//renderer->DrawLine(pMenu->x + 117.0f, pMenu->y + 27.0f + ((float)pMenu->noitems * 20.0f), pMenu->x + 420.0f, pMenu->y + 27.0f + ((float)pMenu->noitems * 20.0f), WHITE);
	pMenu->noitems++;
}

void D3DMenu::AddintRange(char *txt, int *var, int min, int max, int Step)
{
	renderer->Draw2dText(pMenu->x + 115.0f, pMenu->y + 15.0f + ((float)pMenu->noitems * 20.0f), Colors::White, 0.5f, false, txt);

	static bool SendKey = false;
	if (GetKeyState(VK_LBUTTON) & 0x800) {
		if (Tools->isMouseinRegion(pMenu->x + 280.0f, pMenu->y + 10.0f + ((float)pMenu->noitems * 20.0f), pMenu->x + 350.0f, pMenu->y + 10.0f + ((float)pMenu->noitems * 20.0f) + 20.0f) && *var > min && SendKey == true)
			SendKey = false, *var -= Step;
		if (Tools->isMouseinRegion(pMenu->x + 350.0f, pMenu->y + 10.0f + ((float)pMenu->noitems * 20.0f), pMenu->x + 420.0f, pMenu->y + 10.0f + ((float)pMenu->noitems * 20.0f) + 20.0f) && *var < max && SendKey == true)
			SendKey = false, *var += Step;
	} else {
		SendKey = true;
	}
	renderer->Draw2dText(pMenu->x + 300.0f, pMenu->y + 15.0f + ((float)pMenu->noitems * 20.0f), Colors::White, 0.5f, true, "<<");
	renderer->Draw2dText(pMenu->x + 350.0f, pMenu->y + 15.0f + ((float)pMenu->noitems * 20.0f), !*var ? Colors::Red : Colors::Lime, 0.5f, true, "%d", *var);
	renderer->Draw2dText(pMenu->x + 400.0f, pMenu->y + 15.0f + ((float)pMenu->noitems * 20.0f), Colors::White, 0.5f, true, ">>");

	//renderer->DrawLine(pMenu->x + 117.0f, pMenu->y + 27.0f + ((float)pMenu->noitems * 20.0f), pMenu->x + 420.0f, pMenu->y + 27.0f + ((float)pMenu->noitems * 20.0f), WHITE);
	pMenu->noitems++;
}

char *opt_OffOn[] = { ("Off"), ("On") };
void D3DMenu::AddOnOff(char *txt, int *var)
{
	this->AddItem(txt, var, opt_OffOn, 1);
}

extern POINT pPoint;
extern float ScreenHight;
extern float ScreenWidth;

static bool AddColorSlider(char* strText, uchar *Color, bool *ColorControl) {
	int iMax = 255;
	float x = pMenu->x + 115.0f;
	float y = pMenu->y + 15.0f + float(((int)pMenu->noitems * 2) * 40.0f);
	renderer->FillRect(x - 1, y - 1, 40 + 2, 40 + 2, MenuIsEnabled);
	renderer->FillRect(x, y, 40, 40, ColorX(Color[0], Color[1], Color[2], Color[3]));

		for (auto i = 0; i < 4; i++) {
				static int ClickedOn = -1;
				static Vector2 ClickOnPos(0, 0);
				float step = (iMax / iMax);
				float draw = (step * Color[i]);
				int TempH = y + ((i) * 10.0f);

				if (Tools->isMouseinRegion(x + 40.0f, TempH, x + 40.0f + iMax, TempH + 10.0f) && ClickedOn == -1)
					ClickedOn = i, ClickOnPos = Vector2(pPoint.x, pPoint.y);
				
				if(GetKeyState(VK_LBUTTON) & 0x800){
					if (ClickOnPos.y != 0)pPoint.y = ClickOnPos.y;
					if(Tools->isMouseinRegion(x + 40.0f - 100.0f, TempH, x + 40.0f + iMax + 100.0f, TempH + 10.0f)) {
						if (-((x + 40.0f) - pPoint.x) <= iMax && -((x + 40.0f) - pPoint.x) >= 0 && ClickedOn != -1)Color[ClickedOn] = -((x + 40.0f) - pPoint.x);
						if (pPoint.x >(x + 40.0f + iMax))Color[ClickedOn] = 255;
						if (pPoint.x < (x + 40.0f))Color[ClickedOn] = 0;
					}
				} else {
					ClickedOn = -1;
				}
				renderer->FillRect(x + 40 + 1, TempH, 255 + 1, 10.0f, MenuIsEnabled);
				renderer->FillRect(x + 40, TempH, draw + 2, 10.0f, (i + 1 == 1 ? ORANGE : i+ 1 == 2 ? RED : i+ 1 == 3 ? GREEN : i+ 1 == 4 ? BLUE : BLACK));
				renderer->Draw2dText(x + 40 + (iMax / 2.0f), TempH + 2.5f, Colors::White, 0.38f, true, "%d", Color[i]);
		}
	renderer->Draw2dText(x - 2.0f, y - 10, Colors::White, 0.4f, false, strText);

	pMenu->noitems++;
	return true;
}

struct Options {
public:
	bool Engine_Draw = false;

	//Player Menu
	//int God_Mode = false;
	//int Super_Jump = false;
	int Crosshair = false;
	int CrosshairSize = 4;
	float CrosshairColor[3];
	//int Never_Wanted = false;
	//int Always_Wanted = false;
	//int Visible = false;
	//int Regdoll = false;
	//float Run_Speed = 1.0f;
	//float Swim_Speed = 1.0f;

	//Aimbot Menu
	int Aimbot_Toggle = false;
	int Aimbot_Type = false;
	int Aimbot_AutoFire = false;
	int Aimbot_Bone = 0;
	int Aimbot_Smooth_factor = 1000.0f;
	int Aimbot_SmoothDelay = 20.0f;
	int Aimbot_radius = 50.0f;
	BYTE HotKeyAim = 0;

	//Weapon Menu
	int No_Recoil = false;
	int No_Spread = false;


	int Vehicle_Speed_Enabled = false;
	int Vehicle_Speed_Add = 10.0f;
	BYTE Hotkey_speed = 0;

	//Npc ESP
	int Npc_Max_Distance = 500.0f;
	int Npc_Name_Esp = false;
	int Npc_Box_Esp = 0;
	int Npc_Box_Esp_Type = 0;
	int Npc_Health_Esp = 0;
	int Npc_Weapon_Esp = false;
	int Npc_Distance_Esp = false;
	int ESP_Status = false;
	int Npc_Bone_Esp = false;
	int Npc_veh_esp = false;

	// Object ESP
	int Weed_ESP = false;

	XMVECTORF32 ColCivilian = Colors::OrangeRed;
	XMVECTORF32 ColCop = Colors::DeepSkyBlue;
	XMVECTORF32 ColSwat = Colors::Red;

	//2d Radar
	int Toggle_2D_Radar = false;
	int Center2d = false;
	int Health2dRadar = false;

	int RadarSize = 1;
	int RadarZoom = 1;

	float CrossColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float BgColor[4] = { 0.0f, 0.0f, 0.0f, 0.2f };
	Vector2 RadarLoc = Vector2(400, 500);

	char LastSave[132] = "Last: Unknown";

};
Options Hacks;

char	*opt_Save[] = { ("Off"), ("Saved") };
char	*opt_Load[] = { ("Off"), ("Loaded") };
char	*opt_HealthEsp[] = { ("Off"), ("TOP"), ("SIDE"), ("TXT") };
char	*opt_BoxEsp[] = { ("Off"), ("2D"), ("3D"), ("FILL") };

char	*opt_Aimbot_Toggle[] = { ("Off"),("Hotkey") };
char	*opt_Aimbot_Type[] = { ("NONE"), ("Trigger"), ("Assist") };
char	*opt_Veh_Types[] = { ("Off"), ("On") };
BYTE HotKeyAim = 0;
BYTE Hotkey_speed = 0;
extern float MouseWheel;

bool GiveWeaponB = false;





bool MakeMenu(void) {
	try {
		renderer->FillRect(pMenu->x - 1.0f, pMenu->y - 25.0f, pMenu->width + 1.0f, 25.0f, ColorX(150, 0, 255, 255));
		renderer->Draw2dText(pMenu->x + (pMenu->width / 2.0f), pMenu->y - (25.0f / 2.0f), Colors::White, 0.5f, true, "pornHook");
		
		
		//Draw Background
		renderer->FillRect(pMenu->x, pMenu->y, pMenu->width, pMenu->hight, MenuBack);

		int ButtonCount = 0;
		DrawButton(pMenu->x, pMenu->y + float(ButtonCount * 35.0f), 108, 35, "Aim", &Aimbot_Menu); ButtonCount++;
		DrawButton(pMenu->x, pMenu->y + float(ButtonCount * 35.0f), 108, 35, "ESP", &Npc_Menu); ButtonCount++;
		DrawButton(pMenu->x, pMenu->y + float(ButtonCount * 35.0f), 108, 35, "WEAPON", &Weapon_Menu); ButtonCount++;
		DrawButton(pMenu->x, pMenu->y + float(ButtonCount * 35.0f), 108, 35, "RADAR", &Radar_Menu); ButtonCount++;
		DrawButton(pMenu->x, pMenu->y + float(ButtonCount * 35.0f), 108, 35, "VISUAL", &Visual_Menu); ButtonCount++;
		DrawButton(pMenu->x, pMenu->y + float(ButtonCount * 35.0f), 108, 35, "OTHER", &Other_Menu); ButtonCount++;

		if (Aimbot_Menu) {
			pMenu->AddItem("Toggle", &Hacks.Aimbot_Toggle, opt_Aimbot_Toggle, 1);
			pMenu->AddItem("Type", &Hacks.Aimbot_Type, opt_Aimbot_Type, 3);
			pMenu->AddintRange("Smooth Delay", &Hacks.Aimbot_SmoothDelay, 0, 1000, 10);
			pMenu->AddintRange("Smooth Factor", &Hacks.Aimbot_Smooth_factor, 1, 1000, 10);
			pMenu->AddintRange("Aim Radius", &Hacks.Aimbot_radius, 10, 1000, 10);
			pMenu->AddOnOff("Auto Fire", &Hacks.Aimbot_AutoFire);

			char cAimHotKey[230] = "0";
			sprintf_s(cAimHotKey, (Hacks.HotKeyAim ? "0x%x" : "None.."), Hacks.HotKeyAim);
			DrawButtonHotKey(pMenu->x + (pMenu->width / 3.5f), pMenu->y + 155, 120, 30, &Hacks.HotKeyAim, cAimHotKey, "Aim key");
		}
		if (Weapon_Menu) {
			pMenu->AddOnOff("No Recoil", &Hacks.No_Recoil);
			pMenu->AddOnOff("No Spread", &Hacks.No_Spread);
		}

		if (Npc_Menu) {
				pMenu->AddOnOff("Enable", &Hacks.ESP_Status);
				pMenu->AddintRange("Max Distance", &Hacks.Npc_Max_Distance, 50, 1000, 50);
				pMenu->AddOnOff("Weapon", &Hacks.Npc_Weapon_Esp);
				pMenu->AddOnOff("Distance", &Hacks.Npc_Distance_Esp);
				pMenu->AddOnOff("Bone", &Hacks.Npc_Bone_Esp);
				pMenu->AddItem("Health", &Hacks.Npc_Health_Esp, opt_HealthEsp, 3);
				pMenu->AddItem("Box", &Hacks.Npc_Box_Esp_Type, opt_BoxEsp, 3);
				pMenu->AddOnOff("Vehicle ESP", &Hacks.Npc_veh_esp);

		}
		if (Radar_Menu) {
			pMenu->AddOnOff("Toggle", &Hacks.Toggle_2D_Radar);
			pMenu->AddOnOff("Health", &Hacks.Health2dRadar);
			pMenu->AddOnOff("Center", &Hacks.Center2d);
			pMenu->AddintRange("Size", &Hacks.RadarSize, 1, 15, 1);
			pMenu->AddintRange("Zoom", &Hacks.RadarZoom, 1, 6, 1);
		}
		if ( Visual_Menu ) {
			pMenu->AddOnOff("Weed ESP", &Hacks.Weed_ESP);
		}
		if (Other_Menu) {
			DrawButton(pMenu->x + 120.0f, pMenu->y + 30.0f, 100, 40,"Kill Game", &Crash), Other_Menu = 1;
		}
		return true;
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	
}
