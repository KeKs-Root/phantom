#pragma once
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
//#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "d3d11.lib")

#define BEA_ENGINE_STATIC  /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL    /* specify the usage of a stdcall version of BeaEngine */
#include "BeaEngine\headers\BeaEngine.h"
#pragma comment(lib, "BeaEngineCheetah64.lib")

#include "D3D11Renderer.h"
#include "Tools.h"
#include "Menu.h"
#include "Classes.h"
#include "Game.h"

#include <Windows.h>
#include <d3d9.h>
#include <iostream>
#include <d3dx9.h>
#include <dwmapi.h> 
#include <TlHelp32.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#pragma comment (lib, "Dwmapi.lib")
#pragma comment(lib, "dwmapi.lib")

#include "Overlay.h"

#include <Windows.h>
#include "MinHook.h"
#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

HWND GtaHWND = NULL;
float ScreenCenterX = 0;
float ScreenCenterY = 0;

bool firstTime = true;

extern bool Aimbot_Menu;
extern bool Npc_Menu;
extern bool Weapon_Menu;
extern bool Radar_Menu;
extern bool Visual_Menu;
extern bool Other_Menu;


extern bool Save;
extern bool Load;
extern bool Crash;

float ScreenHight = 0.0f;
float ScreenWidth = 0.0f;

DWORD smoothAimTime = timeGetTime();

const MARGINS Margin = {0, 0, 700, 700}; // Change
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 pd3D9Device; 
LPD3DXFONT pFont;

#include <chrono>

typedef HRESULT(__stdcall *D3D11PresentHook) ( IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags );
typedef HRESULT(__fastcall *D3D11DrawIndexedHook) ( ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation );
typedef HRESULT(__fastcall *D3D11ClearRenderTargetViewHook) ( ID3D11DeviceContext* pContext, ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4] );

ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;

DWORD_PTR* pSwapChainVtable = NULL;
DWORD_PTR* pDeviceContextVTable = NULL;

D3D11PresentHook phookD3D11Present = NULL;
D3D11DrawIndexedHook phookD3D11DrawIndexed = NULL;
D3D11ClearRenderTargetViewHook phookD3D11ClearRenderTargetView = NULL;

SpriteFont* m_font = NULL;
SpriteBatch* m_batch = NULL;
std::unique_ptr<SpriteBatch>						 DX11SpriteBatch = NULL;
std::unique_ptr<BasicEffect>						 DX11BatchEffects = NULL;
std::unique_ptr<PrimitiveBatch<VertexPositionColor>> DX11Batch = NULL;
#include "Purista.h"
#define MenuBack      ColorX(255, 51, 51, 51) 
#define MenuIsEnabled      ColorX(255, 38, 38, 38) 

extern D3DMenu *pMenu;
extern D3D11Renderer *renderer;



void DrawHelthEsp(float w_health, CObject * tmpObject, Vector3 Top2d, float BoxWidth, float BoxHight, bool Side = 0) {
	if ( !Side ) {
		renderer->DrawHealthBar(Top2d.x - ( BoxWidth > w_health ? BoxWidth / 2 : ( w_health / 2 ) ), Top2d.y - 8.0f, ( BoxWidth > w_health ? BoxWidth : w_health ), 3, tmpObject->HP - 100.0f, tmpObject->MaxHP - 100.0f, Hacks.Engine_Draw);
		if ( tmpObject->GetArmor() > 0.0f )renderer->DrawArmorBar(Top2d.x - ( BoxWidth > w_health ? BoxWidth / 2 : ( w_health / 2 ) ), Top2d.y - 16.0f, ( BoxWidth > w_health ? BoxWidth : w_health ), 3, tmpObject->GetArmor(), 100.0f, Hacks.Engine_Draw);
	} else {
		renderer->DrawHealthBar(Top2d.x + ( BoxWidth / 2 ) + 4, Top2d.y, 3, BoxHight, tmpObject->HP - 100.0f, tmpObject->MaxHP - 100.0f, Hacks.Engine_Draw, ( Hacks.Npc_Health_Esp == 2 ? 1 : 0 ));
		if ( tmpObject->GetArmor() > 0.0f )renderer->DrawArmorBar(Top2d.x + ( BoxWidth / 2 ) + 10, Top2d.y, 3, BoxHight, tmpObject->GetArmor(), 100.0f, Hacks.Engine_Draw, ( Hacks.Npc_Health_Esp == 2 ? 1 : 0 ));
	}
}

bool PlayerEsp() {
	try {
		XMVECTORF32 ColorTxt = Colors::Red;
		static __int16 LocalTeamId = 0;
		float fClosestPos = 9999.0f;
		float fClosestPosCross = 0;
		for (int i = 0; i < 0x20; i++) {
			ColorX ColorObj(0, 0, 0, 0);
			bool IsEnemy[0x20];
			auto tmpObject = reinterpret_cast<CObject*>(addr.playerList->getPlayer(i));
			if (Tools->IsValid(addr.playerList->ObjectWrapperList[i]) && Tools->IsValid(addr.playerList->ObjectWrapperList[i]->playerInfo)) {

				if (addr.playerList->ObjectWrapperList[i]->playerInfo == addr.localPlayer->PlayerInfo)
					LocalTeamId = addr.playerList->ObjectWrapperList[i]->GetTeamId();

				if (Tools->IsValid(tmpObject) && Tools->IsValid(tmpObject->_ObjectNavigation) && Tools->IsValid(tmpObject->PlayerInfo) && tmpObject->HP > 1.0f && tmpObject != addr.localPlayer && tmpObject->HP > 100.0f) {
					float Distance = Tools->Get3dDistance(tmpObject->fPosition, addr.localPlayer->fPosition);

					if (addr.playerList->ObjectWrapperList[i]->GetTeamId() != LocalTeamId)
						ColorObj = IsVisPedOnline[i] ? RED : ORANGE, ColorTxt = IsVisPedOnline[i] ? Colors::Red : Colors::Orange, IsEnemy[i] = true; //Enemy
					if (addr.playerList->ObjectWrapperList[i]->GetTeamId() == LocalTeamId)
						ColorObj = IsVisPedOnline[i] ? BLUE : GREEN, ColorTxt = IsVisPedOnline[i] ? Colors::Blue : Colors::Lime, IsEnemy[i] = false; //Friend

					Vector3 Top2d(0, 0, 0);
					Vector3	Bottom2d(0, 0, 0);
					Vector3 Top(tmpObject->fPosition.x, tmpObject->fPosition.y, tmpObject->fPosition.z + 0.85f);
					Vector3 Bottom(tmpObject->fPosition.x, tmpObject->fPosition.y, tmpObject->fPosition.z - 1.0f);
					if (renderer->W2S(Top, Top2d) && renderer->W2S(Bottom, Bottom2d)) {
						float BoxHight = abs(Top2d.y - Bottom2d.y);
						float BoxWitdth = BoxHight / 2.0f;
						auto Top = Vector3(tmpObject->fPosition.x, tmpObject->fPosition.y, tmpObject->fPosition.z + 0.85f);
						auto Bottom = Vector3(tmpObject->fPosition.x, tmpObject->fPosition.y, tmpObject->fPosition.z - 1.0f);

						renderer->Draw2dText(Top2d.x, Top2d.y + 50.0f, Colors::BlueViolet, 0.6f, true, tmpObject->IsVisible() ? "True" : "False");

						//if (Hacks.Player_HeadDot_Esp && BoxHight > 5.0f)renderer->DrawDot(Head2d.x, Head2d.y, 2, 2, ColorObj, true);
						if (Hacks.Player_Barrel_Esp && BoxHight > 5.0f)
							Barrel(Top, tmpObject->_ObjectNavigation->Rotation, ColorObj, 5.0f);

						if (Hacks.Player_Bone_Esp && BoxHight > 5.0f)BoneEsp(tmpObject, ColorObj);
						if (Hacks.Player_Box_Esp == 1 && Top2d.x > 1.0f && Top2d.y > 1.0f && BoxHight > 5.0f)DrawBoundingBox(Top2d, Bottom2d, ColorObj);
						if (Hacks.Player_Health_Esp == 1)DrawHelthEsp(50.0f, tmpObject, Top2d, BoxWitdth, BoxHight);
						if (Hacks.Player_Health_Esp == 2)DrawHelthEsp(50.0f, tmpObject, Top2d, BoxWitdth, BoxHight, true);

						if (Hacks.Player_Health_Esp && (tmpObject->GetGodModeToggle() > NULL || tmpObject->HP > 500.0f))
							renderer->DrawLine(Top2d.x - (50.0f / 2.0f), Top2d.y - 8.0f, Top2d.x + (50.0f / 2.0f), Top2d.y - 5.0f, RED),
							renderer->DrawLine(Top2d.x - (50.0f / 2.0f), Top2d.y - (8.0f - 3.0f), Top2d.x + (50.0f / 2.0f), Top2d.y - 8.0f, RED);

						if (Tools->IsValid(tmpObject->WeaponManager))
							if (Tools->IsValid(tmpObject->WeaponManager->_WeaponInfo))
								if (Hacks.Player_Weapon_Esp)renderer->Draw2dText(Bottom2d.x, Bottom2d.y + (Hacks.Player_Name_Esp ? 15.0f : 5.0f), ColorTxt, 0.4f, true, tmpObject->WeaponManager->_WeaponInfo->szWeaponName);

						if (Hacks.Player_Distance_Esp)renderer->Draw2dText(Bottom2d.x, Bottom2d.y + (Hacks.Player_Weapon_Esp ? (Hacks.Player_Name_Esp ? 25.0f : 15.0f) : (Hacks.Player_Name_Esp ? 15.0f : 5.0f)), ColorTxt, 0.4f, true, "[%1.0fm]", Distance);
						if (Hacks.Player_Box_Esp == 2 && BoxHight > 5.0f)renderer->Draw3DBox(tmpObject->fPosition, tmpObject->_ObjectNavigation->Rotation, ColorObj); //3D Npc Box Esp

						if (Hacks.Player_Name_Esp) {
							renderer->Draw2dText(Bottom2d.x, Bottom2d.y + 5.0f, ColorTxt, 0.4f, true, tmpObject->PlayerInfo->sName);
							if ((tmpObject->PlayerInfo->GetWanted() > 0)) {
								renderer->Draw2dText(Top2d.x, Top2d.y - (tmpObject->Armor == 0.0f ? 15.0f : 23.0f), ColorTxt, 0.4f, true, "*%d*", tmpObject->PlayerInfo->GetWanted());
							}
						}
					}
					float RadarSize = 125.0f + (float(Hacks.RadarSize) * 25.0f);
					float RadarZoom = 0.2f + (float(Hacks.RadarZoom) * 0.1f);
					if (Hacks.Toggle_2D_Radar == true) {
						Vector2 RadarPos = WorldToRadar(tmpObject->fPosition, Hacks.RadarLoc.x, Hacks.RadarLoc.y, RadarSize, RadarZoom);
						renderer->DrawDot(RadarPos.x, RadarPos.y, 4, 4, ColorObj, true, Hacks.Engine_Draw);
						if (Hacks.Health2dRadar)renderer->DrawHealthBar(RadarPos.x - 10, RadarPos.y + 5, 20, 3, tmpObject->HP - 100.0f, tmpObject->MaxHP - 100.0f, Hacks.Engine_Draw);
					}
				}
			}
		}
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (...)
	{
		//CLog::error("Unknown exception");
	}
	return true;
}


bool NPC() {
	float fClosestPos = 9999.0f;
	float fClosestPosCross = 9999.0f;
	try {
		if ( Tools->IsValid(addr.ReplayInterface) && Tools->IsValid(addr.ReplayInterface->ped_interface) )
			for ( int i = 0; i < addr.ReplayInterface->ped_interface->max_peds; i++ ) {
				Vector3 Top2d = Vector3(0, 0, 0), Bottom2d = Vector3(0, 0, 0);
				float BoxHight = 0.0f, BoxWidth = 0.0f;
				auto ColorEsp = ColorX(0, 0, 0, 0);
				auto TxtColor = Colors::DimGray;
				char NameEsp[10] = "UNKNOWN";

				auto tmpObject = reinterpret_cast<CObject*>( addr.ReplayInterface->ped_interface->get_ped(i) );
				if ( Tools->IsValid(tmpObject) && ( tmpObject->HP > 100.0f ) && tmpObject != addr.localPlayer && Tools->IsValid(tmpObject->_ObjectNavigation) && !Tools->IsValid(tmpObject->PlayerInfo) ) {
					if ( ( tmpObject->IsVisible() == true ) && ( !invisiblePeds[i] ) ) {
						auto Distance = Tools->Get3dDistance(addr.localPlayer->fPosition, tmpObject->fPosition);
						if ( Distance < Hacks.Npc_Max_Distance ) {
							if ( VisiblePeds[i] ) {
								ColorEsp = YELLOW;
								TxtColor = Colors::Yellow;
							} else {
								ColorEsp = DarkOrange;
								TxtColor = Colors::Orange;
							}

							auto Top = Vector3(tmpObject->fPosition.x, tmpObject->fPosition.y, tmpObject->fPosition.z + 0.85f);
							auto Bottom = Vector3(tmpObject->fPosition.x, tmpObject->fPosition.y, tmpObject->fPosition.z - 1.0f);

							float RadarSize = 125.0f + ( float(Hacks.RadarSize) * 25.0f );
							float RadarZoom = 0.2f + ( float(Hacks.RadarZoom) * 0.1f );
							if ( Hacks.Toggle_2D_Radar ) {
								Vector2 RadarPos = WorldToRadar(tmpObject->fPosition, Hacks.RadarLoc.x, Hacks.RadarLoc.y, RadarSize, RadarZoom);
								if ( Hacks.ESP_Status )renderer->DrawDot(RadarPos.x, RadarPos.y, 4, 4, ColorEsp, true, Hacks.Engine_Draw), renderer->DrawHealthBar(RadarPos.x - 10, RadarPos.y + 5, 20, 3, tmpObject->HP - 100.0f, tmpObject->MaxHP - 100.0f, Hacks.Engine_Draw);;
							}


							if ( renderer->W2S(Top, Top2d) && renderer->W2S(Bottom, Bottom2d) ) {
								if ( Tools->isW2SValid(Top2d) && Tools->isW2SValid(Bottom2d) ) {
									BoxHight = abs(Top2d.y - Bottom2d.y);
									BoxWidth = BoxHight / 2.0f;


									if ( Hacks.Npc_Box_Esp_Type == 2 && BoxWidth > 2.0f ) { renderer->Draw3DBox(tmpObject->fPosition, tmpObject->_ObjectNavigation->Rotation, ColorEsp); } //3D Npc Box Esp
									if ( Hacks.Npc_Box_Esp_Type == 3 && BoxWidth > 2.0f ) { renderer->FillRect(Top2d.x - ( BoxWidth / 2.0f ), Top2d.y, BoxWidth, BoxHight, ColorX(35, ColorEsp.R, ColorEsp.G, ColorEsp.B), Hacks.Engine_Draw); } //Fill Npc Box Esp
									if ( Hacks.Npc_Box_Esp_Type == 1 && BoxWidth > 2.0f ) { DrawBoundingBox(Top2d, Bottom2d, ColorEsp); }//2D Npc Box Esp

									if ( Hacks.Npc_Name_Esp ) {
										if ( Hacks.Npc_Health_Esp == 1 ) { renderer->Draw2dText(Top2d.x, Top2d.y - ( tmpObject->GetArmor() < 1.0f ? 18.0f : 24.0f ), TxtColor, 0.32f, true, NameEsp); }
										if ( Hacks.Npc_Health_Esp == 0 || Hacks.Npc_Health_Esp == 2 ) { renderer->Draw2dText(Top2d.x, Top2d.y - 8.0f, TxtColor, 0.32f, true, NameEsp); }
									}
									if ( Hacks.Npc_Health_Esp == 1 || Hacks.Npc_Health_Esp == 2 ) { DrawHelthEsp(50.0f, tmpObject, Top2d, BoxWidth, BoxHight, Hacks.Npc_Health_Esp == 1 ? false : true); } //Top Health

									if ( Hacks.Npc_Health_Esp == 3 ) { renderer->Draw2dText(Top2d.x, Top2d.y + 5.0f, TxtColor, 0.5f, true, "%1.0f", ( tmpObject->HP - 100.0f ) + tmpObject->GetArmor()); }

									if ( Hacks.Npc_Bone_Esp && BoxWidth > 2.0f )BoneEsp(tmpObject, ColorEsp);




									if ( Tools->IsValid(tmpObject->WeaponManager) ) {
										if ( Tools->IsValid(tmpObject->WeaponManager->_WeaponInfo) ) {
											if ( Hacks.Npc_Weapon_Esp ) {
												renderer->Draw2dText(Top2d.x, Bottom2d.y + 5.0f, TxtColor, 0.34f, true, tmpObject->WeaponManager->_WeaponInfo->GetSzWeaponName());
											}
										}
									}


									if ( Hacks.Npc_Distance_Esp ) {
										renderer->Draw2dText(Top2d.x, Bottom2d.y + ( Hacks.Npc_Weapon_Esp ? 15.0f : 5.0f ), TxtColor, 0.34f, true, "[%1.0fm]", Distance);
									}

								}
							}

							if ( Hacks.Npc_veh_esp ) {
								if ( Tools->IsValid(tmpObject->VehicleManager) ) {
									auto vehObj = (CVehicleManager*)tmpObject->VehicleManager;
									if ( Tools->IsValid(vehObj) ) {
										Vector3 ObjectPos2D(0, 0, 0);
										if ( renderer->W2S(vehObj->fPosition, ObjectPos2D) ) {
											if ( Tools->isW2SValid(ObjectPos2D) ) {
												renderer->Draw2dText(ObjectPos2D.x, ObjectPos2D.y + 15.0f, Colors::Red, 0.7f, true, "%1.0f HP", vehObj->fHealth);
											}
										}
									}
								}
							}
						}
					}
				}
			}
	} catch ( const std::exception &e ) {
	} catch ( ... ) {
	}

	return true;
}

void Aimbot() {
	float aimTargetDist = 99999.0f;
	if ( Tools->IsValid(addr.ReplayInterface) && Tools->IsValid(addr.ReplayInterface->ped_interface) )
		for ( int i = 0; i < addr.ReplayInterface->ped_interface->max_peds; i++ ) {
			auto TxtColor = Colors::Yellow;
			auto tmpObject = reinterpret_cast<CObject*>( addr.ReplayInterface->ped_interface->get_ped(i) );
			if ( Tools->IsValid(tmpObject) && ( tmpObject->HP > 100.0f ) && tmpObject != addr.localPlayer && Tools->IsValid(tmpObject->_ObjectNavigation) && !Tools->IsValid(tmpObject->PlayerInfo) ) {
				if ( ( tmpObject->IsVisible() == true ) && ( !invisiblePeds[i] ) ) {
					auto Distance = Tools->Get3dDistance(addr.localPlayer->fPosition, tmpObject->fPosition);
					if ( Distance < Hacks.Npc_Max_Distance ) {
						if ( VisiblePeds[i] && ( invisiblePeds[i] == false ) ) { // Change once hook fixed
							Vector3 Head2d(0, 0, 0);
							Vector3 Head4d = GetBonePosition1(tmpObject, SKEL_Head);
							if ( renderer->W2S(Head4d, Head2d) ) {
								Head4d = Head4d;
								Head2d = Head2d;

								if ( Tools->IsValid(pPlayerAngles) ) {
									Vector3 Crosshair3D = pPlayerAngles->Crosshair_Location;
									Vector3 Head3D = GetBonePosition1(tmpObject, SKEL_Head);
									Vector3 Crosshair2D(0, 0, 0);
									Vector3 Entity2d(0, 0, 0);
									if ( renderer->W2S(Crosshair3D, Crosshair2D) && renderer->W2S(Head3D, Entity2d) ) {
										float AimDistance = Tools->GetDistance(Crosshair2D.x, Crosshair2D.y, Entity2d.x, Entity2d.y);
										if ( Hacks.Aimbot_Toggle == 1 ) {
											if ( Hacks.Aimbot_Type == 2 ) {
												if ( AimDistance < Hacks.Aimbot_radius ) {
													if ( ( AimDistance < aimTargetDist ) ) {
														aimTargetDist = AimDistance;
														renderer->DrawAimCross(Entity2d.x, Entity2d.y, 24, RED);
														if ( GetKeyState(Hacks.HotKeyAim) & 0x800 ) {
															if ( timeGetTime() - smoothAimTime > Hacks.Aimbot_SmoothDelay ) {
																smoothAimTime = timeGetTime();
																Vector3 Out(( Head3D.x - Crosshair3D.x ) / Distance, ( Head3D.y - Crosshair3D.y ) / Distance, ( Head3D.z - Crosshair3D.z ) / Distance);


																float threshold = 0.5f / Hacks.Aimbot_Smooth_factor;
																if ( ( ( Out.x - pPlayerAngles->ViewAngles2.x ) > threshold ) || ( ( pPlayerAngles->ViewAngles2.x - Out.x ) > threshold ) ) {
																	if ( pPlayerAngles->ViewAngles2.x > Out.x ) {
																		pPlayerAngles->ViewAngles2.x -= threshold;
																	} else if ( pPlayerAngles->ViewAngles2.x < Out.x ) {
																		pPlayerAngles->ViewAngles2.x += threshold;
																	}
																}
																if ( ( ( Out.y - pPlayerAngles->ViewAngles2.y ) > threshold ) || ( ( pPlayerAngles->ViewAngles2.y - Out.y ) > threshold ) ) {
																	if ( pPlayerAngles->ViewAngles2.y > Out.y ) {
																		pPlayerAngles->ViewAngles2.y -= threshold;
																	} else if ( pPlayerAngles->ViewAngles2.y < Out.y ) {
																		pPlayerAngles->ViewAngles2.y += threshold;
																	}
																}
																if ( ( ( Out.z - pPlayerAngles->ViewAngles2.z ) > threshold ) || ( ( pPlayerAngles->ViewAngles2.z - Out.z ) > threshold ) ) {
																	if ( pPlayerAngles->ViewAngles2.z > Out.z ) {
																		pPlayerAngles->ViewAngles2.z -= threshold;
																	} else if ( pPlayerAngles->ViewAngles2.z < Out.z ) {
																		pPlayerAngles->ViewAngles2.z += threshold;
																	}
																}
															}
														}
													}
												}
											} else if ( Hacks.Aimbot_Type == 1 ) {
												if ( AimDistance < 25 ) {
													renderer->Draw2dText(Entity2d.x, Entity2d.y + 30.0f, TxtColor, 0.50f, true, "%s", "Trigger");
												}
											}

										}

									}
								}
							}
						}
					}
				}
			}
		}
}





using namespace std::chrono;
void FPSCheck(std::string& str) {
	static high_resolution_clock::time_point lastTime;
	static int ticks = 0;
	auto now = high_resolution_clock::now();
	auto secs = duration_cast<seconds>( now - lastTime );
	ticks++;
	if ( secs.count() >= 1 ) {
		str = std::to_string(( ticks / secs.count() ) / 2);
		ticks = 0;
		lastTime = now;
	}
}
using namespace std;

string Fps = "0";
void DrawFps() {
	FPSCheck(Fps);
	renderer->Draw2dText(1, 6.0f, Colors::White, 0.5f, false, "FPS: %s", Fps); //Draw Fps
}

bool DrawResultion(XMVECTORF32 Colortxt) {
	XMFLOAT2 origin(0, 0);
	char logbuf[32];
	sprintf_s(logbuf, "Resolution: %1.0fx%1.0f", renderer->GetWidth(), renderer->GetHeight());

	XMVECTOR size = m_font->MeasureString(asciiDecode(logbuf).c_str());
	float sizeX = XMVectorGetX(size);
	float sizeY = 0;
	origin = XMFLOAT2(sizeX, sizeY);
	m_font->DrawString(DX11SpriteBatch.get(), asciiDecode(logbuf).c_str(), Vector2(renderer->GetWidth() - 1.0f, 0), Colortxt, 0.f, origin, 0.5f, SpriteEffects_None);
	return false;
}

LRESULT __stdcall HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDPROC OldWndProc = nullptr;


using OriginalFn = int(*)( void*, void*, bool );
OriginalFn oReturn;

float MouseWheel = 0.0;
POINT pPoint;
bool LMouse, LMouseU = false;
LRESULT __stdcall HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch ( uMsg ) {
		case WM_LBUTTONDOWN:
			LMouse = true;
			return true;
		case WM_LBUTTONUP:
			LMouseU = false;
			return true;
		case WM_MOUSEWHEEL:
			MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
			return true;
		case WM_MOUSEMOVE:
			pPoint.x = (signed short)( lParam );
			pPoint.y = (signed short)( lParam >> 16 );
			return true;
	}
	return CallWindowProc(OldWndProc, hWnd, uMsg, wParam, lParam);
}

void InitializeWnd() {
	if ( GtaHWND )OldWndProc = (WNDPROC)SetWindowLongPtr(GtaHWND, GWLP_WNDPROC, (LONG_PTR)HookedWndProc);
}

void RestoreWnd() {
	SetWindowLongPtr(GtaHWND, GWLP_WNDPROC, (LONG_PTR)OldWndProc);
}

HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	try {
#pragma region InitialzeFunktions
		if ( firstTime ) {


			addr.BASE = (DWORD64)GetModuleHandle("GTA5.exe");

			pSwapChain->GetDevice(__uuidof( pDevice ), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
			m_font = new SpriteFont(pDevice, PuristaFont, sizeof(PuristaFont));
			DX11SpriteBatch.reset(new SpriteBatch(pContext));
			DX11Batch.reset(new PrimitiveBatch<VertexPositionColor>(pContext));
			renderer = new D3D11Renderer(pSwapChain);
			HMODULE D3DModule = renderer->Initialize();
			InitializeWnd();
			firstTime = false;


			Tools->UnlinkModuleFromPEB(D3DModule);


		}
#pragma endregion

		if ( GetAsyncKeyState(VK_INSERT) & 1 ) {
			pMenu->visible = !pMenu->visible;
			if ( pMenu->visible )InitializeWnd();
			if ( !pMenu->visible )RestoreWnd();
			//
		}
		if ( Crash == true ) {
			exit(1);
		}

		if ( Tools->IsValid(&LocalPlayerWrapper_address) ) {
			addr.LocalPlayerWrapper = *(CWorld**)( LocalPlayerWrapper_address ); //48 8B 05 ? ? ? ? 45 0F C6 C?
		}

		if ( Tools->IsValid(addr.LocalPlayerWrapper) ) {
			addr.localPlayer = ( addr.LocalPlayerWrapper->getLocalPlayer() );
		}
		if ( Hacks.Aimbot_Toggle > 0 ) {
			GetPlayerAngles();
		}
		renderer->BeginScene();
		DrawResultion(Colors::White);
		if ( pMenu == NULL ) {
			pMenu = new D3DMenu();
			if ( pMenu->visible == 0 )
				pMenu->visible = 1;
		} else {
			DrawResultion(Colors::White);
			if ( Tools->IsValid(addr.localPlayer) && Tools->IsValid(addr.localPlayer->PlayerInfo) ) {

				if ( Hacks.ESP_Status ) NPC();


				if ( Hacks.Aimbot_Toggle > 0 ) {
					Aimbot();
				}

				if ( Hacks.Weed_ESP ) {
					WeedESP();
				}
			}

			IsVisMenu = pMenu->visible;
			if ( pMenu->visible ) {
				ScreenWidth = renderer->GetWidth();
				ScreenHight = renderer->GetHeight();

				ScreenCenterX = ScreenWidth / 2.0f;
				ScreenCenterY = ScreenHight / 2.0f;

				static bool LockMenuPos = false;
				static int DifferenceX = 0, DifferenceY = 0;
				if ( GetKeyState(VK_LBUTTON) & 0x800 ) {
					if ( Tools->isMouseinRegion(pMenu->x - 1.0f, pMenu->y - 25.0f, pMenu->x + pMenu->width, pMenu->y) || LockMenuPos ) {
						LockMenuPos = true;
						pMenu->x = pPoint.x - DifferenceX,
							pMenu->y = pPoint.y - DifferenceY;
					}
				} else {
					LockMenuPos = false;
					DifferenceX = ( pPoint.x - pMenu->x );
					DifferenceY = ( pPoint.y - pMenu->y );
				}

				pMenu->noitems = 0;
				if ( pMenu->noitems == 0 )
					MakeMenu();

				renderer->DrawCross(pPoint.x, pPoint.y, Hacks.CrosshairSize, Hacks.CrosshairSize, ColorX(255, Hacks.CrosshairColor[0] * 255, Hacks.CrosshairColor[1] * 255, Hacks.CrosshairColor[2] * 255), Hacks.Engine_Draw, true);
			}
		}
		float RadarSize = 125.0f + ( float(Hacks.RadarSize) * 25.0f );
		float RadarZoom = 0.2f + ( float(Hacks.RadarZoom) * 0.1f );

		if ( Radar_Menu )
			//renderer->Draw2dText(Hacks.RadarLoc.x - ( RadarSize / 2 ) - 2, Hacks.RadarLoc.y - ( RadarSize / 2 ) - 12.0f, Colors::WhiteSmoke, 0.4f, false, "Size: %1.0f Zoom: %1.1f", RadarSize, RadarZoom);

			if ( Hacks.Toggle_2D_Radar ) {
				renderer->FillRect(Hacks.RadarLoc.x - ( RadarSize / 2 ), Hacks.RadarLoc.y - ( RadarSize / 2 ), RadarSize, RadarSize, ColorX(Hacks.BgColor[3] * 255, Hacks.BgColor[0] * 255, Hacks.BgColor[1] * 255, Hacks.BgColor[2] * 255)),
					renderer->DrawCross(Hacks.RadarLoc.x, Hacks.RadarLoc.y, RadarSize / 2, RadarSize / 2, ColorX(Hacks.CrossColor[3] * 255, Hacks.CrossColor[0] * 255, Hacks.CrossColor[1] * 255, Hacks.CrossColor[2] * 255)),
					renderer->DrawLine(Hacks.RadarLoc.x, Hacks.RadarLoc.y, Hacks.RadarLoc.x - ( RadarSize / 2 ), Hacks.RadarLoc.y - ( RadarSize / 2 ), ColorX(Hacks.CrossColor[3] * 255, Hacks.CrossColor[0] * 255, Hacks.CrossColor[1] * 255, Hacks.CrossColor[2] * 255)),
					renderer->DrawLine(Hacks.RadarLoc.x, Hacks.RadarLoc.y, Hacks.RadarLoc.x + ( RadarSize / 2 ), Hacks.RadarLoc.y - ( RadarSize / 2 ), ColorX(Hacks.CrossColor[3] * 255, Hacks.CrossColor[0] * 255, Hacks.CrossColor[1] * 255, Hacks.CrossColor[2] * 255));
			}

		if ( Hacks.Center2d )Hacks.RadarLoc = Vector2(ScreenCenterX, ScreenCenterY);
		if ( Tools->isMouseinRegion(Hacks.RadarLoc.x - ( RadarSize / 2 ), Hacks.RadarLoc.y - ( RadarSize / 2 ), Hacks.RadarLoc.x + ( RadarSize / 2 ), Hacks.RadarLoc.y + ( RadarSize / 2 )) && GetKeyState(VK_LBUTTON) & 0x800 ) {
			if ( Radar_Menu )Hacks.RadarLoc = Vector2(pPoint.x, pPoint.y);
		}
		if ( Hacks.Crosshair )
			renderer->DrawCross(ScreenCenterX, ScreenCenterY, Hacks.CrosshairSize, Hacks.CrosshairSize, ColorX(255, Hacks.CrosshairColor[0] * 255, Hacks.CrosshairColor[1] * 255, Hacks.CrosshairColor[2] * 255), Hacks.Engine_Draw, true);

		if ( Hacks.Aimbot_Toggle > 0 ) {
			renderer->DrawCircle(ScreenCenterX, ScreenCenterY, Hacks.Aimbot_radius, 1.0f, ORANGE);
		}



		renderer->EndScene();
		//}
	} catch ( const std::exception &e ) {
	}

	return phookD3D11Present(pSwapChain, SyncInterval, Flags);
}


bool Ready() {
	if ( GetModuleHandleA("d3d11.dll") != NULL && GetModuleHandleA("dxgi.dll") != NULL && GetModuleHandleA("DNSAPI.dll") != NULL ) return true;
	return false;
}


bool InitHook = false;
DWORD __stdcall InitializeHook(HINSTANCE hInstance) {
	while ( InitHook == false ) {
		if ( Ready() ) {

			if ( MH_Initialize() != MH_OK ) {
				return 1;
			}

			RECT rc;

			GetWindowRect(GtaHWND, &rc);
			ScreenWidth = rc.right - rc.left;
			ScreenHight = rc.bottom - rc.top;

			memset(VisiblePeds, 0, sizeof(VisiblePeds));
			memset(invisiblePeds, 0, sizeof(invisiblePeds));

			
			PVOID* pSwapChainVtable = NULL;
			DWORD64 Swapchain_Pattern = addr.BASE + Tools->FindPattern("\x48\x8B\x3D\x00\x00\x00\x00\x48\x8D\x54\x24\x00\x48\x8B\x07\x48\x8B\xCF\xFF\x50\x60", "xxx????xxxx?xxxxxxxxx");
			auto SwapChain = (DWORD64*)( Swapchain_Pattern + *(DWORD*)( Swapchain_Pattern + 0x3 ) + 0x7 );
			LPVOID* lppSwapChain = reinterpret_cast<LPVOID*>( SwapChain );
			pSwapChainVtable = *reinterpret_cast<PVOID**>( *lppSwapChain );

			InitGameFunctions();
			// Reason for Crashes is the SwapChain present hook!
			//phookD3D11Present = (D3D11PresentHook)DetourFunc64((BYTE*)pSwapChainVtable[8], (BYTE*)hookD3D11Present, 16);
			pIs_Dlc_Present = (Is_Dlc_Present)DetourFunc64((BYTE* const)Is_DLC_present, (const BYTE*)hookIs_Dlc_Present, (const unsigned int)16);

			// Create a hook for MessageBoxW, in disabled state.
			if ( MH_CreateHook((BYTE*)( pSwapChainVtable[8] ), hookD3D11Present, reinterpret_cast<void**>( &phookD3D11Present )) != MH_OK ) {
				return 1;
			}

			// Enable the hook for MessageBoxW.
			if ( MH_EnableHook(pSwapChainVtable[8]) != MH_OK ) {
				return 1;
			}

			// 48 89 5C 24 20 56 48 83 EC 30 0F B6 05 D3 9B 07 00 48 8B F2
			// \x48\x89\x5C\x24\x20\x56\x48\x83\xEC\x30\x0F\xB6\x05\xD3\x9B\x07\x00\x48\x8B\xF2
			// xxxxxxxxxxxxxxxx?xxx

			/*addr.OBS = (DWORD64)GetModuleHandle("graphics-hook64.dll");
			DWORD64 obs_pattern = ( addr.OBS + Tools->FindPattern("\x48\x89\x5C\x24\x20\x56\x48\x83\xEC\x30\x0F\xB6\x05\xD3\x9B\x07\x00\x48\x8B\xF2", "xxxxxxxxxxxxxxxx?xxx") );

			if ( Tools->IsValid(*(OriginalFn**)obs_pattern) ) {
			}*/

			InitHook = true;

			return NULL;
		}
	}
	return false;
}



extern void* detourBuffer[3];
BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
	GtaHWND = FindWindow("174", NULL); 
	if ( GtaHWND == NULL )GtaHWND = FindWindow("grcWindow", NULL);
	addr.BASE = (DWORD64)GetModuleHandle("GTA5.exe");


	if ( dwReason == DLL_PROCESS_ATTACH ) {
		srand(time(NULL));
		Tools = new cTools();
		Tools->UnlinkModuleFromPEB(hModule);
		memset(detourBuffer, 0, sizeof(detourBuffer));
		InitializeHook(hModule);
		//CreateThread(NULL, 0, InitializeHook, hModule, 0, NULL);

	} else if ( dwReason == DLL_PROCESS_DETACH ) {
		Sleep(1000);
		RestoreWnd();
		TerminateThread(InitializeHook, 0);
		//TerminateThread(ClockThread, 0);
		for ( int i = 0; i < sizeof(detourBuffer) / sizeof(void*); ++i ) {
			if ( detourBuffer[i] ) {
#ifdef _WIN64
				VirtualFree(detourBuffer[i], 0, MEM_RELEASE);
#else
				delete[] detourBuffer[i];
#endif
			}
		}
	}
	return TRUE;
}