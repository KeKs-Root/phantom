#pragma once

#include <Windows.h>
#include <stdio.h> 
#include <string> 
#include <sstream> 
#include <iostream>

#pragma warning ( disable: 4102 ) 
#pragma warning ( disable: 4311 ) 
#pragma warning ( disable: 4312 ) 

#pragma warning ( disable: 4244 ) 
#pragma warning ( disable: 4996 ) 

static std::wstring asciiDecode(const std::string & s)
{
	std::wostringstream    w;
	wchar_t                c;

	for (size_t i = 0; i < s.length(); i++) {
		mbtowc(&c, &s[i], 1);
		w << c;
	}
	return w.str();
}

struct CVector3 {
public:
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float m[3];
	};
};

#pragma pack(push, 1)
struct screenReso
{
	uint32_t w, h;
};

class CViewPort
{
public:
	char _0x0000[0x24C];
	float fViewMatrix[0x10];
};//Size=0x028C
#pragma pack(pop)
static CViewPort* m_viewPort;


class GameViewMatrices;
class CPlayerInfo;
class CPlayers;
class CInventory;
class CWeaponManager;
class CObjectWrapper;
class CObject;
class CVehicleManager;
class CAttacker;
class CVehicleHandling;
class CVehicleColorOptions;
class CVehicleColors;
class CBoneManager;

class CObjectNavigation
{
public:
	char pad_0[0x20];
	Vector4 Rotation;
	char pad_1[0x20];
	Vector3 Position;

	void WritePos(Vector3 Pos) {
		*(Vector3 *)(((DWORD64)this + 0x50)) = Pos;
	}

};
#define IsValidPtr(x) (x && !IsBadReadPtr(&x, sizeof(void*) && x != nullptr && x > nullptr))

class CReplayInterfacePed
{
private:
	class CPedList
	{
	private:
		struct Ped
		{
			CObject* ped;
			char _pad0[0x8];
		};

	public:
		Ped peds[256];
	};

public:
	char _pad0[0x100];			// 0000 - 0100
	CPedList* ped_list;			// 0100 - 0108
	int max_peds;				// 0108 - 010C
	char _pad1[0x4];			// 010C - 0110
	int number_of_peds;		// 0110 - 0114

	CObject* get_ped(const unsigned int& index)
	{
		return (index <= max_peds && IsValidPtr(ped_list->peds[index].ped) ? ped_list->peds[index].ped : nullptr);
	}
};

class CReplayInterfaceVehicle
{
private:
	class VehicleHandle
	{
	public:
		CVehicleManager * vehicle; //0x0000 
		__int32 handle; //0x0008
		char _pad0[0x4];
	};//Size=0x0588

	class CVehicleList
	{
	public:
		VehicleHandle vehicles[0x100];//0x0000 
	};//Size=0x0490

public:
	char _0x0000[0x180];
	CVehicleList* vehicle_list; //0x0180
	__int32 max_vehicles; //0x0188 
	char _0x010C[0x4];
	__int32 number_of_vehicles; //0x0190 
	char _0x0114[0x34];

	CVehicleManager* get_vehicle(const unsigned int& index)
	{
		return (index <= max_vehicles && IsValidPtr(vehicle_list->vehicles[index].vehicle) ? vehicle_list->vehicles[index].vehicle : nullptr);
	}
};

class CPickup
{
public:
	char pad_0x0000[0x30]; //0x0000
	CObjectNavigation* pCNavigation; //0x0030 
	char pad_0x0038[0x58]; //0x0038
	Vector3 v3VisualPos; //0x0090 
	char pad_0x009C[0x3F4]; //0x009C
	__int32 iValue; //0x0490 
	char pad_0x0494[0xC4]; //0x0494

}; //Size=0x0558

class CPickupHandle
{
public:
	CPickup * pCPickup; //0x0000 
	__int32 iHandle; //0x0008 
	char pad_0x000C[0x4]; //0x000C

}; //Size=0x0010

class CPickupList
{
public:
	CPickupHandle pickups[73]; //0x0000 

}; //Size=0x0490

class CPickupInterface
{
public:
	char pad_0x0000[0x100]; //0x0000
	CPickupList* pCPickupList; //0x0100 
	__int32 iMaxPickups; //0x0108 
	char pad_0x010C[0x4]; //0x010C
	__int32 iCurPickups; //0x0110 

	CPickup* get_pickup(const int& index)
	{
		if (index < iMaxPickups)
			return pCPickupList->pickups[index].pCPickup;
		return nullptr;
	}
}; //Size=0x0114

class CObjectInterface
{
private:
	class CObjectHandle
	{
	public:
		CObject * pCObject; //0x0000 
		__int32 iHandle; //0x0008 
		char pad_0x000C[0x4]; //0x000C

	}; //Size=0x0010

	class CObjectList
	{
	public:
		CObjectHandle ObjectList[2300]; //0x0000 

	}; //Size=0x8FC0

public:
	char pad_0x0000[0x158]; //0x0000
	CObjectList* pCObjectList; //0x0158 
	__int32 iMaxObjects; //0x0160 
	char pad_0x0164[0x4]; //0x0164
	__int32 iCurObjects; //0x0168 
	char pad_0x016C[0x5C]; //0x016C

	CObject* get_object(const unsigned int& index)
	{
		return (index < iMaxObjects && IsValidPtr(pCObjectList->ObjectList[index].pCObject) ? pCObjectList->ObjectList[index].pCObject : nullptr && IsValidPtr(pCObjectList));
	}
	bool Remove_object(const unsigned int& index)
	{
		if (index < iMaxObjects && IsValidPtr(pCObjectList->ObjectList[index].pCObject) ? pCObjectList->ObjectList[index].pCObject : nullptr && IsValidPtr(pCObjectList)) {
			pCObjectList->ObjectList[index].iHandle = 0;
		}
		return true;
	}


}; //Size=0x01C8

class CReplayInterface
{
public:
	void* game_interface;
	void* camera_interface;
	CReplayInterfaceVehicle* vehicle_interface;
	CReplayInterfacePed* ped_interface;
	CPickupInterface* pickup_interface;
	CObjectInterface* object_interface;
};

class CVehicleManager
{
public:
	char pad_0x0000[0x30]; //0x0000
	CObjectNavigation* _ObjectNavigation; //0x0030 
	char pad_0x0038[0x10]; //0x0038
	CVehicleColorOptions* CVehicleColorOptions; //0x0048 
	char pad_0x0050[0x40]; //0x0050
	Vector3 fPosition; //0x0090 
	char pad_0x009C[0xED]; //0x009C
	BYTE btGodMode; //0x0189 
	char pad_0x018A[0xF6]; //0x018A
	float fHealth; //0x0280 
	char pad_0x0284[0x1C]; //0x0284
	float fHealthMax; //0x02A0 
	char pad_0x02A4[0x4]; //0x02A4
	CAttacker* CAttacker; //0x02A8 
	char pad_0x02B0[0x59C]; //0x02B0
	float fHealth2; //0x084C 
	char pad_0x0850[0x28]; //0x0850
	CVehicleHandling* CVehicleHandling; //0x0878 
	char pad_0x0880[0x3]; //0x0880
	BYTE btBulletproofTires; //0x0883 
	char pad_0x0884[0x2F8]; //0x0884
	float fGravity; //0x0B7C 

	float VehSpeed() {
		return (float)(*(float *)((DWORD64)this + 0xA30));
	}
	//CModelInfo *ModelInfo() {
	//	return (*(CModelInfo**)((DWORD64)this + 0x20));
	//}


	bool isGod()
	{
		return (btGodMode & 0x01) ? true : false;
	}

	bool hasBulletproofTires()
	{
		return (btBulletproofTires & 0x20) ? true : false;
	}
}; //Size=0x0B80

class CModelInfo
{
public:
	char pad_0x0000[0x270]; //0x0000
	BYTE Type; //0x0270 
	char pad_0x0271[0xF]; //0x0271

	DWORD GetHash() {
		return (*(DWORD*)((DWORD64)this + 0x18));
	}

}; //Size=0x0280

class CWeaponInfo
{
public:
	char pad_0000[96]; //0x0000
	void* _AmmoInfo; //0x0060 //70
	char pad_0068[12]; //0x0068
	float Spread; //0x0074 //84
	char pad_0078[56]; //0x0078
	float Damage; //0x00B0 
	char pad_00B4[36]; //0x00B4
	float MinImpulse; //0x00D8 
	float MaxImpulse; //0x00DC
	char pad_00E0[52]; //0x00E0
	float Velocity; //0x0114 
	int32_t BulitsPerShoot; //0x0118 
	float MultiBulitSpread; //0x011C 
	char pad_0120[12]; //0x0120
	float ReloadTimeMultiplier; //0x012C
	char pad_0130[4]; //0x0130
	float TimeToShoot; //0x0134 //144
	char pad_0138[404]; //0x0138
	//float RecoilFirstPerson; //0x02CC 
	char pad_02D0[8]; //0x02D0
	float RecoilThirdPerson; //0x02D8 //2E8
	char pad_02DC[772]; //0x02DC
	char* szWeaponName; //0x05E0 //5F0
	char pad_05E8[40]; //0x05E8

	void SniperRange() {
		*(float*)((DWORD64)this + 0x28C) = 1750.0f; //MinRange
		*(float*)((DWORD64)this + 0x29C) = 1750.0f; //MaxRange
	}
	void NoRecoil() {
		*(float*)((DWORD64)this + 0x2E8) = 0.0f;
	}
	void NoSpread() {
		*(float*)((DWORD64)this + 0x84) = 0.0f; //Spread
		*(float*)((DWORD64)this + 0x11C) = 0.0f; //Multi Bulit spread
	}
	void NoReload() {
		*(float*)((DWORD64)this + 0x12C) = 22.0f;
	}
	void SuperImpulse() {
		*(float*)((DWORD64)this + 0xD8) = 9999.0f;
		*(float*)((DWORD64)this + 0xDC) = 9999.0f;
	}
	void SuperDmg() {
		*(float*)((DWORD64)this + 0xB0) = 9999.0f;
	}
	char *GetSzWeaponName() {
		return (*reinterpret_cast<char **>((DWORD64)this + 0x5F0));
	}

	//0x1D4 = Visual shoot animation radius
}; //Size: 0x0610
static CWeaponInfo* m_pCWeaponInfo = nullptr;
static CWeaponInfo m_CWeapon;


//class CWeaponManager
//{
//public:
//	char pad_0000[32]; //0x0000
//	CWeaponInfo* _WeaponInfo; //0x0020
//	char pad_0028[96]; //0x0028
//	void* _CurrentWeapon; //0x0088
//	char pad_0090[272]; //0x0090
//	Vector3 Impact; //0x01A0
//	char pad_01AC[24]; //0x01AC
//
//	Vector3 GetImpact2() {
//		return (Vector3)(*(Vector3 *)((DWORD64)this + 0x1B0));
//	}
//}; //Size: 0x01C4


class CWeaponManager
{
public:
	char pad_0000[32]; //0x0000
	class CWeaponInfo* _WeaponInfo; //0x0020
	char pad_0028[96]; //0x0028
	void* _CurrentWeapon; //0x0088
	char pad_0090[272]; //0x0090
	Vector3 Aiming_at_Cord; //0x01A0
	char pad_01AC[4]; //0x01AC
	Vector3 Last_Impact_Cord; //0x01B0
	char pad_01BC[44]; //0x01BC
}; //Size: 0x01E8


class CObject
{
public:
	char pad_0x0000[0x2C]; //0x0000
	BYTE btInvisibleSP; //0x002C 
	char pad_0x002D[0x1]; //0x002D
	BYTE btFreezeMomentum; //0x002E 
	char pad_0x002F[0x1]; //0x002F
	CObjectNavigation* _ObjectNavigation; //0x0030 
	char pad_0x0038[0x58]; //0x0038
	Vector3 fPosition; //0x0090 
	char pad_0x009C[0xED]; //0x009C
	BYTE GodModeToggle; //0x0189 
	char pad_0x018A[0xF6]; //0x018A
	float HP; //0x0280 
	char pad_0x0284[0x1C]; //0x0284
	float MaxHP; //0x02A0 
	char pad_0x02A4[0x4]; //0x02A4
	CAttacker* CAttacker; //0x02A8 
	char pad_0x02B0[0x70]; //0x02B0
	Vector3 v3Velocity; //0x0320 
	char pad_0x032C[0x9FC]; //0x032C
	CVehicleManager* VehicleManager; //0x0D28 
	char pad_0x0D30[0x378]; //0x0D30
	BYTE btNoRagdoll; //0x10A8 
	char pad_0x10A9[0xF]; //0x10A9
	CPlayerInfo* PlayerInfo; //0x10B8 
	char pad_0x10C0[0x8]; //0x10C0
	CWeaponManager* WeaponManager; //0x10C8 
	char pad_0x10D0[0x31C]; //0x10D0
	BYTE btSeatBelt; //0x13EC 
	char pad_0x13ED[0xB]; //0x13ED
	BYTE btSeatbeltWindshield; //0x13F8 
	char pad_0x13F9[0x72]; //0x13F9
	BYTE btIsInVehicle; //0x146B 
	char pad_0x146C[0x44]; //0x146C
	float Armor; //0x14B0 
	char pad_0x14B4[0x3C]; //0x14B4
	CVehicleManager* VehicleManager2; //0x14F0 

	bool IsVisible() {
		return (*(BYTE *)((DWORD64)this + 0xAC) == 255 ? true : false);
	}


	void ResetHealth() {
		this->HP = this->MaxHP;
		*(float*)((DWORD64)this + 0x14B8) = 100.0f;
	}

	void NoRecoil() {
		if (Tools->IsValid(WeaponManager))
			if (Tools->IsValid(WeaponManager->_WeaponInfo))
				WeaponManager->_WeaponInfo->NoRecoil();
	}
	void NoSpread() {
		if (Tools->IsValid(WeaponManager))
			if (Tools->IsValid(WeaponManager->_WeaponInfo))
				WeaponManager->_WeaponInfo->NoSpread();
	}
	void FastRelod() {
		if (Tools->IsValid(WeaponManager))
			if (Tools->IsValid(WeaponManager->_WeaponInfo))
				WeaponManager->_WeaponInfo->NoReload();
	}
	void SuperImpulse() {
		if (Tools->IsValid(WeaponManager))
			if (Tools->IsValid(WeaponManager->_WeaponInfo))
				WeaponManager->_WeaponInfo->SuperImpulse();
	}
	void SuperDamage() {
		if (Tools->IsValid(WeaponManager))
			if (Tools->IsValid(WeaponManager->_WeaponInfo))
				WeaponManager->_WeaponInfo->SuperDmg();
	}
	void SniperRange() {
		if (Tools->IsValid(WeaponManager))
			if (Tools->IsValid(WeaponManager->_WeaponInfo))
				WeaponManager->_WeaponInfo->SniperRange();
	}

	float GetArmor() {
		return (*(float *)((DWORD64)this + 0x14B8));
	}

	bool GetGodModeToggle()
	{
		return (GodModeToggle & 0x01) ? true : false;
	}

	bool isInvisSP()
	{
		return (btInvisibleSP & 0x01) ? true : false;
	}

	bool hasFrozenMomentum()
	{
		return (btFreezeMomentum & 0x02) ? true : false;
	}

	bool canBeRagdolled()
	{
		return (btNoRagdoll & 0x20) ? false : true;
	}

	bool hasSeatbelt()
	{
		return (btSeatBelt & 0x01) ? true : false;
	}

	bool hasSeatbeltWindshield()
	{
		return (btSeatbeltWindshield & 0x10) ? true : false;
	}

	WORD PedType()
	{
		return (*(WORD *)((DWORD64)this + 0x10A8) << 11 >> 25);
	}

	void SetPedType(int Type)
	{
		*(WORD *)(((DWORD64)this + 0x10A8) << 11 >> 25) = Type;
	}

	bool IsInVehicle()
	{
		return (btIsInVehicle & 0x10) ? false : true;
	}

	CModelInfo *ModelInfo() {
		return (*(CModelInfo**)((DWORD64)this + 0x20));
	}

	void SetPosClass(Vector3 Pos) {
		if (Tools->IsValid(_ObjectNavigation))
			fPosition = Pos, _ObjectNavigation->WritePos(Pos);
	}
	//CBoneManager *BoneManager() {
	//	return (*(CBoneManager**)((DWORD64)this + 0x180));
	//}

	//Vector3 HeadPos() {
	//	return (*(Vector3 *)((DWORD64)this + addToBone));
	//}

}; //Size=0x14F8

class CAttacker
{
public:
	CObject * CPed0; //0x0000 
	char pad_0x0008[0x10]; //0x0008
	CObject* CPed1; //0x0018 
	char pad_0x0020[0x10]; //0x0020
	CObject* CPed2; //0x0030 

}; //Size=0x0038

class CWantedData
{
public:
	char pad_0x0000[0x1C]; //0x0000
	float fWantedCanChange; //0x001C 
	char pad_0x0020[0x10]; //0x0020
	Vector3 v3WantedCenterPos; //0x0030 
	char pad_0x003C[0x4]; //0x003C
	Vector3 v3WantedCenterPos2; //0x0040 
	char pad_0x004C[0x38]; //0x004C
	BYTE btFlag0; //0x0084 
	BYTE btFlag1; //0x0085 
	BYTE btFlag2; //0x0086 
	BYTE btFlag3; //0x0087 
	char pad_0x0088[0xC]; //0x0088
	DWORD dwWantedLevelFake; //0x0094 
	DWORD dwWantedLevel; //0x0098 

}; //Size=0x00AC

class CPlayerInfo
{
public:
	char pad_0x0000[0x34]; //0x0000
	int32_t iInternalIP; //0x0034 
	int16_t iInternalPort; //0x0038 
	char pad_0x003A[0x2]; //0x003A
	int32_t iRelayIP; //0x003C 
	int16_t iRelayPort; //0x0040 
	char pad_0x0042[0x2]; //0x0042
	int32_t iExternalIP; //0x0044 
	int16_t iExternalPort; //0x0048 
	char pad_0x004A[0x32]; //0x004A
	char sName[20]; //0x007C 
	char pad_0x0090[0x4]; //0x0090
	int32_t iTeam; //0x0094 
	char pad_0x0098[0xB0]; //0x0098
	float fSwimSpeed; //0x0148 
	float fRunSpeed; //0x014C 
	char pad_0x0150[0x68]; //0x0150
	uint32_t ulState; //0x01B8 
	char pad_0x01BC[0xC]; //0x01BC
	CObject* pCPed; //0x01C8 
	char pad_0x01D0[0x29]; //0x01D0
	BYTE btFrameFlags; //0x01F9 
	char pad_0x01FA[0x566]; //0x01FA
	CWantedData CWantedData; //0x0760 
	char pad_0x07FC[0x464]; //0x07FC
	float fStamina; //0x0C60 
	float fStaminaMax; //0x0C64 
	char pad_0x0C68[0x10]; //0x0C68
	float fDamageMod; //0x0C78 
	char pad_0x0C7C[0x4]; //0x0C7C
	float fMeleeDamageMod; //0x0C80 
	char pad_0x0C84[0x4]; //0x0C84
	float fVehicleDamageMod; //0x0C88 
	char pad_0x0C8C[0x1B4]; //0x0C8C
	BYTE btAiming; //0x0E40 
	char pad_0x0E41[0x7]; //0x0E41

	void SetWanted(int lvl) {
		*(__int32 *)((DWORD64)this + 0x814) = lvl,
			*(__int32 *)((DWORD64)this + 0x818) = lvl;
	}
	__int32 GetWanted() {
		return *(__int32 *)((DWORD64)this + 0x814);
	}


};

class CPrimaryAmmoCount
{
public:
	char pad_0x0000[0x18]; //0x0000
	__int32 AmmoCount; //0x0018 

	void InfGunAmmo() {
		AmmoCount = 999;
	}

};//Size=0x0040

class CAmmoCount
{
public:
	CPrimaryAmmoCount * _PrimaryAmmoCount; //0x0000 
	char pad_0x0008[0x38]; //0x0008

};//Size=0x0040

class CAmmoInfo
{
public:
	char pad_0x0000[0x8]; //0x0000
	CAmmoCount* _AmmoCount; //0x0008 
	char pad_0x0010[0x18]; //0x0010
	__int32 AmmoMax; //0x0028 
	char pad_0x002C[0x40C]; //0x002C

}; //Size=0x0438

class CWeaponObject
{
public:
	char _0x0000[8];
	CAmmoInfo* m_pAmmoInfo; //0x0008 
	char _0x0010[16];
	__int32 m_iAmmo; //0x0020 
	char _0x0024[20];

};//Size=0x0038

class CInventory
{
public:
	char _0x0000[72];
	CWeaponObject** WeaponList; //0x0048 

	CWeaponObject* getWeapon(int index) {
		return (CWeaponObject*)WeaponList[index];
	}
};//Size=0x0050

class CObjectWrapper
{
public:
	char _0x0000[60];
	__int16 TeamId; //0x003C 
	char _0x0040[104];
	CPlayerInfo* playerInfo; //0x00A8 
	char _0x00B0[64];

	__int16 GetTeamId() {
		return ((TeamId) << 0 >> 0 << 0);
	}

};//Size=0x00F0

class CPlayers
{
public:
	char _0x0000[376];
	__int32 numPlayersOnline; //0x0178 
	char _0x017C[4];
	CObjectWrapper* ObjectWrapperList[256]; //0x0180 

	CObject* getPlayer(int index) {
		if (Tools->IsValid(ObjectWrapperList[index]) && Tools->IsValid(ObjectWrapperList[index]->playerInfo) && Tools->IsValid(ObjectWrapperList[index]->playerInfo->pCPed))
			return (CObject*)ObjectWrapperList[index]->playerInfo->pCPed;
		return NULL;
	}

	CPlayerInfo* getPlayerInfo(int index) {
		if (Tools->IsValid(ObjectWrapperList[index]) && Tools->IsValid(ObjectWrapperList[index]->playerInfo))
			return (CPlayerInfo*)ObjectWrapperList[index]->playerInfo;
		return NULL;
	}

	char* getPlayerName(int index) {
		if (Tools->IsValid(ObjectWrapperList[index]) && Tools->IsValid(ObjectWrapperList[index]->playerInfo))
			return (char*)ObjectWrapperList[index]->playerInfo->sName;
		return NULL;
	}

	int GetNumPlayersOnline() {
		if (numPlayersOnline > 1 && numPlayersOnline <= 0x20)
			return numPlayersOnline;
		else
			return 0;
	}
};


class CWorld
{
public:
	char _0x0000[8];
	CObject* pLocalPlayer; //0x0008 

	CObject* getLocalPlayer() {
		if ( Tools->IsValid(pLocalPlayer) ) {
			return pLocalPlayer;
		} else {
			return false;
		}
	}
};//Size=0x0016

class CGameCameraAngles;
class CCameraManagerAngles;
class CCameraAngles;
class CPlayerAngles;

class CGameCameraAngles
{
public:
	CCameraManagerAngles * pMyCameraManagerAngles; //0x0000  
	char _0x0008[56];

};//Size=0x0040 

class CCameraManagerAngles
{
public:
	CCameraAngles * pMyCameraAngles; //0x0000  

};//Size=0x0008 

class CCameraAngles
{
public:
	char _0x0000[960];
	CPlayerAngles* pMyFPSAngles; //0x03C0  
	char _0x03C8[16];
	__int64 pTPSCamEDX; //0x03D8  // This is passed as second parameter to oGetTPSCamera 
	char _0x03E0[40];

};//Size=0x0408 

class CPlayerCameraData
{
public:
	char _0x0000[48];
	float Fov_Zoom; //0x0030 
	char _0x0034[36];
	__int32 m_ZoomState; //0x0058 1 = Normal, 0 = Zoom, 10000+ = FPS

};//Size=0x0548

class CPlayerAngles
{
public:
	char pad_0x0000[0x10]; //0x0000
	CPlayerCameraData* CamData; //0x0010 
	__int32 UnknownCounter; //0x0018 
	char pad_0x001C[0x14]; //0x001C
	Vector3 ViewAngles; //0x0030 
	char pad_0x003C[0x4]; //0x003C
	Vector3 ViewAngles2; //0x0040 
	char pad_0x004C[0x14]; //0x004C
	Vector3 Crosshair_Location; //0x0060 
	char pad_0x006C[0x3D4]; //0x006C

	void SetFOV(float v)
	{
		*(float *)((DWORD64)this + 0x90) = v;
	}

}; //Size=0x0440

class CVehicleColors
{
public:
	char pad_0x0000[0xA4]; //0x0000
	BYTE btPrimaryBlue; //0x00A4 
	BYTE btPrimaryGreen; //0x00A5 
	BYTE btPrimaryRed; //0x00A6 
	BYTE btPrimaryAlpha; //0x00A7 
	BYTE btSecondaryBlue; //0x00A8 
	BYTE btSecondaryGreen; //0x00A9 
	BYTE btSecondaryRed; //0x00AA 
	BYTE btSecondaryAlpha; //0x00AB 

	void SetColor(float Primary[2], float Secondary[2]) {
		btPrimaryRed = BYTE(Primary[0] * 255);
		btPrimaryGreen = BYTE(Primary[1] * 255);
		btPrimaryBlue = BYTE(Primary[2] * 255);

		btSecondaryRed = BYTE(Secondary[0] * 255);
		btSecondaryGreen = BYTE(Secondary[1] * 255);
		btSecondaryBlue = BYTE(Secondary[2] * 255);
	}

}; //Size=0x00AC

class CVehicleColorOptions
{
public:
	char pad_0x0000[0x20]; //0x0000
	CVehicleColors* CVehicleColor; //0x0020 

}; //Size=0x0028

class CVehicleHandling
{
public:
	char pad_0x0000[0xC]; //0x0000
	float fMass; //0x000C 
	char pad_0x0010[0x10]; //0x0010
	Vector3 v3CentreOfMassOffset; //0x0020 
	char pad_0x002C[0x4]; //0x002C
	Vector3 v3InertiaMult; //0x0030 
	char pad_0x003C[0x4]; //0x003C
	float fPercentSubmerged; //0x0040 
	char pad_0x0044[0x8]; //0x0044
	float fAcceleration; //0x004C 
	BYTE btInitialDriveGears; //0x0050 
	char pad_0x0051[0x3]; //0x0051
	float fDriveInertia; //0x0054 
	float fClutchChangeRateScaleUpShift; //0x0058 
	float fClutchChangeRateScaleDownShift; //0x005C 
	float fInitialDriveForce; //0x0060 
	char pad_0x0064[0x8]; //0x0064
	float fBrakeForce; //0x006C 
	char pad_0x0070[0x4]; //0x0070
	float fBrakeBiasFront; //0x0074 
	char pad_0x0078[0x4]; //0x0078
	float fHandBrakeForce; //0x007C 
	char pad_0x0080[0x8]; //0x0080
	float fTractionCurveMax; //0x0088 
	char pad_0x008C[0x4]; //0x008C
	float fTractionCurveMin; //0x0090 
	char pad_0x0094[0xC]; //0x0094
	float fTractionSpringDeltaMax; //0x00A0 
	char pad_0x00A4[0x4]; //0x00A4
	float fLowSpeedTractionLossMult; //0x00A8 
	float fCamberStiffnesss; //0x00AC 
	float fTractionBiasFront; //0x00B0 
	float fTwoMinus_fTractionBiasFront; //0x00B4 
	float fTractionLossMult; //0x00B8 
	float fSuspensionForce; //0x00BC 
	float fSuspensionCompDamp; //0x00C0 
	float fSuspensionReboundDamp; //0x00C4 
	float fSuspensionUpperLimit; //0x00C8 
	float fSuspensionLowerLimit; //0x00CC 
	char pad_0x00D0[0xC]; //0x00D0
	float fAntiRollBarForce; //0x00DC 
	char pad_0x00E0[0x8]; //0x00E0
	float fRollCentreHeightFront; //0x00E8 
	float fRollCentreHeightRear; //0x00EC 
	float fCollisionDamageMult; //0x00F0 
	float fWeaponDamageMult; //0x00F4 
	float fDeformationDamageMult; //0x00F8 
	float fEngineDamageMult; //0x00FC 
	float fPetrolTankVolume; //0x0100 
	float fOilVolume; //0x0104 

}; //Size=0x0108



enum ePedType
{
	PEDTYPE_PLAYER_0,                // michael 
	PEDTYPE_PLAYER_1,                // franklin 
	PEDTYPE_NETWORK_PLAYER,            // mp character 
	PEDTYPE_PLAYER_2,                // trevor 
	PEDTYPE_CIVMALE,
	PEDTYPE_CIVFEMALE,
	PEDTYPE_COP,
	PEDTYPE_GANG_ALBANIAN,
	PEDTYPE_GANG_BIKER_1,
	PEDTYPE_GANG_BIKER_2,
	PEDTYPE_GANG_ITALIAN,
	PEDTYPE_GANG_RUSSIAN,
	PEDTYPE_GANG_RUSSIAN_2,
	PEDTYPE_GANG_IRISH,
	PEDTYPE_GANG_JAMAICAN,
	PEDTYPE_GANG_AFRICAN_AMERICAN,
	PEDTYPE_GANG_KOREAN,
	PEDTYPE_GANG_CHINESE_JAPANESE,
	PEDTYPE_GANG_PUERTO_RICAN,
	PEDTYPE_DEALER,
	PEDTYPE_MEDIC,
	PEDTYPE_FIREMAN,
	PEDTYPE_CRIMINAL,
	PEDTYPE_BUM,
	PEDTYPE_PROSTITUTE,
	PEDTYPE_SPECIAL,
	PEDTYPE_MISSION,
	PEDTYPE_SWAT,
	PEDTYPE_ANIMAL,
	PEDTYPE_ARMY
};

enum ExplosionTypes
{
	EXPLOSION_GRENADE,
	EXPLOSION_GRENADELAUNCHER,
	EXPLOSION_STICKYBOMB,
	EXPLOSION_MOLOTOV,
	EXPLOSION_ROCKET,
	EXPLOSION_TANKSHELL,
	EXPLOSION_HI_OCTANE,
	EXPLOSION_CAR,
	EXPLOSION_PLANE,
	EXPLOSION_PETROL_PUMP,
	EXPLOSION_BIKE,
	EXPLOSION_DIR_STEAM,
	EXPLOSION_DIR_FLAME,
	EXPLOSION_DIR_WATER_HYDRANT,
	EXPLOSION_DIR_GAS_CANISTER,
	EXPLOSION_BOAT,
	EXPLOSION_SHIP_DESTROY,
	EXPLOSION_TRUCK,
	EXPLOSION_BULLET,
	EXPLOSION_SMOKEGRENADELAUNCHER,
	EXPLOSION_SMOKEGRENADE,
	EXPLOSION_BZGAS,
	EXPLOSION_FLARE,
	EXPLOSION_GAS_CANISTER,
	EXPLOSION_EXTINGUISHER,
	EXPLOSION_PROGRAMMABLEAR,
	EXPLOSION_TRAIN,
	EXPLOSION_BARREL,
	EXPLOSION_PROPANE,
	EXPLOSION_BLIMP,
	EXPLOSION_DIR_FLAME_EXPLODE,
	EXPLOSION_TANKER,
	EXPLOSION_PLANE_ROCKET,
	EXPLOSION_VEHICLE_BULLET,
	EXPLOSION_GAS_TANK,
	EXPLOSION_BIRD_CRAP
};

enum WeaponHashes
{
	WEAPON_ADVANCEDRIFLE = 0xAF113F99,
	WEAPON_AIRSTRIKE_ROCKET = 0x13579279,
	WEAPON_ANIMAL = 0xF9FBAEBE,
	WEAPON_APPISTOL = 0x22D8FE39,
	WEAPON_ASSAULTRIFLE = 0xBFEFFF6D,
	WEAPON_ASSAULTSHOTGUN = 0xE284C527,
	WEAPON_ASSAULTSMG = 0xEFE7E2DF,
	WEAPON_TEC9 = 0xDB1AA450,
	WEAPON_DOUBLEBARRELED = 0xDB1AA450,
	WEAPON_BALL = 0x23C9F95C,
	WEAPON_BARBED_WIRE = 0x48E7B178,
	WEAPON_BAT = 0x958A4A8F,
	WEAPON_BLEEDING = 0x8B7333FB,
	WEAPON_BOTTLE = 0xF9E6AA4B,
	WEAPON_BRIEFCASE = 0x88C78EB7,
	WEAPON_BRIEFCASE_02 = 0x01B79F17,
	WEAPON_BULLPUPRIFLE = 0x7F229F94,
	WEAPON_COMPACTRIFLE = 0x624FE830,
	WEAPON_BULLPUPSHOTGUN = 0x9D61E50F,
	WEAPON_BZGAS = 0xA0973D5E,
	WEAPON_CARBINERIFLE = 0x83BF0278,
	WEAPON_COMBATMG = 0x7FD62962,
	WEAPON_PARACHUTE = 0xFBAB5776,
	WEAPON_COMBATPDW = 0xA3D4D34,
	WEAPON_COMBATPISTOL = 0x5EF9FEC4,
	WEAPON_COUGAR = 0x08D4BE52,
	WEAPON_CROWBAR = 0x84BD7BFD,
	WEAPON_DAGGER = 0x92A27487,
	WEAPON_DIGISCANNER = 0xFDBADCED,
	WEAPON_DROWNING = 0xFF58C4FB,
	WEAPON_DROWNING_IN_VEHICLE = 0x736F5990,
	WEAPON_ELECTRIC_FENCE = 0x92BD4EBB,
	WEAPON_EXHAUSTION = 0x364A29EC,
	WEAPON_EXPLOSION = 0x2024F4E8,
	WEAPON_FALL = 0xCDC174B0,
	WEAPON_FIRE = 0xDF8E89EB,
	WEAPON_FIREEXTINGUISHER = 0x060EC506,
	WEAPON_FIREWORK = 0x7F7497E5,
	WEAPON_FLARE = 0x497FACC3,
	WEAPON_FLAREGUN = 0x47757124,
	WEAPON_GARBAGEBAG = 0xE232C28C,
	WEAPON_GOLFCLUB = 0x440E4788,
	WEAPON_GRENADE = 0x93E220BD,
	WEAPON_GRENADELAUNCHER = 0xA284510B,
	WEAPON_GRENADELAUNCHER_SMOKE = 0x4DD2DC56,
	WEAPON_GUSENBERG = 0x61012683,
	WEAPON_HAMMER = 0x4E875F73,
	WEAPON_HANDCUFFS = 0xD04C944D,
	WEAPON_HEAVYPISTOL = 0xD205520E,
	WEAPON_HEAVYSHOTGUN = 0x3AABBBAA,
	WEAPON_HEAVYSNIPER = 0x0C472FE2,
	WEAPON_HELI_CRASH = 0x145F1012,
	WEAPON_HIT_BY_WATER_CANNON = 0xCC34325E,
	WEAPON_HOMINGLAUNCHER = 0x63AB0442,
	WEAPON_KNIFE = 0x99B507EA,
	WEAPON_KNUCKLE = 0xD8DF3C3C,
	WEAPON_MARKSMANPISTOL = 0xDC4DB296,
	WEAPON_MARKSMANRIFLE = 0xC734385A,
	WEAPON_MG = 0x9D07F764,
	WEAPON_HATCHET = 0xF9DCBF2D,
	WEAPON_FLASHLIGHT = 0x8BB05FD7,
	WEAPON_MACHINEPISTOL = 0xDB1AA450,
	WEAPON_MACHETE = 0xDD5DF8D9,
	WEAPON_SWITCHBLADE = 0xDFE37640,
	WEAPON_REVOLVER = 0xC1B3C3D1,
	WEAPON_MICROSMG = 0x13532244,
	WEAPON_MINIGUN = 0x42BF8A85,
	WEAPON_MOLOTOV = 0x24B17070,
	WEAPON_MUSKET = 0xA89CB99E,
	WEAPON_NIGHTSTICK = 0x678B81B1,
	WEAPON_PASSENGER_ROCKET = 0x166218FF,
	WEAPON_PETROLCAN = 0x34A67B97,
	WEAPON_PISTOL = 0x1B06D571,
	WEAPON_PISTOL50 = 0x99AEEB3B,
	WEAPON_PROXMINE = 0xAB564B93,
	WEAPON_PUMPSHOTGUN = 0x1D073A89,
	WEAPON_RAILGUN = 0x6D544C99,
	WEAPON_RAMMED_BY_CAR = 0x07FC7D7A,
	WEAPON_REMOTESNIPER = 0x33058E22,
	WEAPON_RPG = 0xB1CA77B1,
	WEAPON_RUN_OVER_BY_CAR = 0xA36D413E,
	WEAPON_SAWNOFFSHOTGUN = 0x7846A318,
	WEAPON_SMG = 0x2BE6766B,
	WEAPON_SMOKEGRENADE = 0xFDBC8A50,
	WEAPON_SNIPERRIFLE = 0x05FC3C11,
	WEAPON_SNOWBALL = 0x787F0BB,
	WEAPON_SNSPISTOL = 0xBFD21232,
	WEAPON_SPECIALCARBINE = 0xC0A3098D,
	WEAPON_STICKYBOMB = 0x2C3731D9,
	WEAPON_STINGER = 0x687652CE,
	WEAPON_STUNGUN = 0x3656C8C1,
	WEAPON_UNARMED = 0xA2719263,
	WEAPON_VEHICLE_ROCKET = 0xBEFDC581,
	WEAPON_VINTAGEPISTOL = 0x083839C4
};

typedef struct
{
	int Result = 0;
	bool DidHitAnything = false;
	bool DidHitEntity = false;
	DWORD HitEntity = (DWORD)0;
	Vector3 HitCoords;
} RaycastResult;

enum IntersectOptions
{
	IntersectEverything = -1,
	IntersectMap = 1,
	IntersectMissionEntityAndTrain = 2,
	IntersectPeds1 = 4,
	IntersectPeds2 = 8,
	IntersectVehicles = 10,
	IntersectObjects = 16,
	IntersectVegetation = 256
};

struct Addresses {
public:
	CWorld* LocalPlayerWrapper = nullptr; //48 8B 05 ? ? ? ? 45 0F C6 C9
	CPlayers* playerList = nullptr; //48 8B 0D ? ? ? ? E8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B CF
	CObject* localPlayer = nullptr; //Just Define Local Player as CObject no Sig needded
	CGameCameraAngles* pGameCamManager = nullptr; //? ? ? ? ? ? ? 4A 8B 1C F0 48 85
	CReplayInterface * ReplayInterface = nullptr;
	DWORD64 BASE = 0;
	DWORD64 OBS = 0;
};
static Addresses addr;

#define WHITE            ColorX(255, 255, 255, 255)
#define RED                ColorX(255, 255, 000, 000)
#define GREEN            ColorX(255, 000, 255, 000)
#define LIME            ColorX(255, 000, 255, 000)
#define BLUE            ColorX(255, 000, 000, 255) 
#define BLACK            ColorX(255, 000, 000, 000)
#define mBLACK            ColorX(100, 000, 000, 205)
#define PURPLE            ColorX(255, 125, 000, 255) 
#define GREY            ColorX(255, 220, 220, 220) 
#define GREYdarker      ColorX(255, 200, 200, 200) 
#define YELLOW            ColorX(255, 255, 255, 000) 
#define ORANGE            ColorX(255, 255, 165, 000)  
#define DarkOrange            ColorX(255, 255, 140, 0)  
 
#define LightBlue        ColorX(255, 0, 191, 255)
#define CYAN        ColorX(255, 0, 255, 255)