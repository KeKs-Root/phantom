#pragma once
#include "Classes.h"
#include "Tools.h"

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
#include "Tools.h"
#include <plog/Log.h>

#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "Winmm.lib")

#include <type_traits>

#pragma section(".text")
__declspec( allocate( ".text" ) )const unsigned char jmp_rbx_0[] = {0xFF, 0x23};
__declspec( allocate( ".text" ) )const unsigned char jmp_rbx_2[] = {0x41, 0xFF, 0x24, 0x24};

namespace detail {
	extern "C" void* _spoofer_stub();

	template <typename Ret, typename... Args>
	static inline auto shellcode_stub_helper(
		const void* shell,
		Args... args
	) -> Ret {
		auto fn = ( Ret(*)( Args... ) )( shell );
		return fn(args...);
	}

	template <std::size_t Argc, typename>
	struct argument_remapper {
		// At least 5 params
		template<
			typename Ret,
			typename First,
			typename Second,
			typename Third,
			typename Fourth,
			typename... Pack
		>
			static auto do_call(
				const void* shell,
				void* shell_param,
				First first,
				Second second,
				Third third,
				Fourth fourth,
				Pack... pack
			) -> Ret {
			return shellcode_stub_helper<
				Ret,
				First,
				Second,
				Third,
				Fourth,
				void*,
				void*,
				Pack...
			>(
				shell,
				first,
				second,
				third,
				fourth,
				shell_param,
				nullptr,
				pack...
				);
		}
	};

	template <std::size_t Argc>
	struct argument_remapper<Argc, std::enable_if_t<Argc <= 4>> {
		// 4 or less params
		template<
			typename Ret,
			typename First = void*,
			typename Second = void*,
			typename Third = void*,
			typename Fourth = void*
		>
			static auto do_call(
				const void* shell,
				void* shell_param,
				First first = First{},
				Second second = Second{},
				Third third = Third{},
				Fourth fourth = Fourth{}
			) -> Ret {
			return shellcode_stub_helper<
				Ret,
				First,
				Second,
				Third,
				Fourth,
				void*,
				void*
			>(
				shell,
				first,
				second,
				third,
				fourth,
				shell_param,
				nullptr
				);
		}
	};
}


template <typename Ret, typename... Args>
static inline auto spoof_call(
	const void* trampoline,
	Ret(*fn)( Args... ),
	Args... args
) -> Ret {
	struct shell_params {
		const void* trampoline;
		void* function;
		void* rbx;
	};

	shell_params p{trampoline, reinterpret_cast<void*>( fn )};
	using mapper = detail::argument_remapper<sizeof...( Args ), void>;
	return mapper::template do_call<Ret, Args...>((const void*)&detail::_spoofer_stub, &p, args...);
}


#pragma pack(push, 1)
struct PVector3 {
	PVector3(float x, float y, float z) :
		x(x), y(y), z(z),
		_paddingx(0), _paddingy(0), _paddingz(0) { }

	float x;
	DWORD _paddingx;
	float y;
	DWORD _paddingy;
	float z;
	DWORD _paddingz;
};
#pragma pack(pop)

using namespace DirectX;
using namespace DirectX::SimpleMath;

#pragma region Bones
#define SKEL_ROOT 0x0
#define SKEL_Pelvis 0x2e28
#define SKEL_L_Thigh 0xe39f
#define SKEL_L_Calf 0xf9bb
#define SKEL_L_Foot 0x3779
#define SKEL_L_Toe0 0x83c
#define IK_L_Foot 0xfedd
#define PH_L_Foot 0xe175
#define MH_L_Knee 0xb3fe
#define SKEL_R_Thigh 0xca72
#define SKEL_R_Calf 0x9000
#define SKEL_R_Foot 0xcc4d
#define SKEL_R_Toe0 0x512d
#define IK_R_Foot 0x8aae
#define PH_R_Foot 0x60e6
#define MH_R_Knee 0x3fcf
#define RB_L_ThighRoll 0x5c57
#define RB_R_ThighRoll 0x192a
#define SKEL_Spine_Root 0xe0fd
#define SKEL_Spine0 0x5c01
#define SKEL_Spine1 0x60f0
#define SKEL_Spine2 0x60f1
#define SKEL_Spine3 0x60f2
#define SKEL_L_Clavicle 0xfcd9
#define SKEL_L_UpperArm 0xb1c5
#define SKEL_L_Forearm 0xeeeb
#define SKEL_L_Hand 0x49d9
#define SKEL_L_Finger00 0x67f2
#define SKEL_L_Finger01 0xff9
#define SKEL_L_Finger02 0xffa
#define SKEL_L_Finger10 0x67f3
#define SKEL_L_Finger11 0x1049
#define SKEL_L_Finger12 0x104a
#define SKEL_L_Finger20 0x67f4
#define SKEL_L_Finger21 0x1059
#define SKEL_L_Finger22 0x105a
#define SKEL_L_Finger30 0x67f5
#define SKEL_L_Finger31 0x1029
#define SKEL_L_Finger32 0x102a
#define SKEL_L_Finger40 0x67f6
#define SKEL_L_Finger41 0x1039
#define SKEL_L_Finger42 0x103a
#define PH_L_Hand 0xeb95
#define IK_L_Hand 0x8cbd
#define RB_L_ForeArmRoll 0xee4f
#define RB_L_ArmRoll 0x1470
#define MH_L_Elbow 0x58b7
#define SKEL_R_Clavicle 0x29d2 //schulter oder auch Schlüsselbein
#define SKEL_R_UpperArm 0x9d4d
#define SKEL_R_Forearm 0x6e5c
#define SKEL_R_Hand 0xdead
#define SKEL_R_Finger00 0xe5f2
#define SKEL_R_Finger01 0xfa10
#define SKEL_R_Finger02 0xfa11
#define SKEL_R_Finger10 0xe5f3
#define SKEL_R_Finger11 0xfa60
#define SKEL_R_Finger12 0xfa61
#define SKEL_R_Finger20 0xe5f4
#define SKEL_R_Finger21 0xfa70
#define SKEL_R_Finger22 0xfa71
#define SKEL_R_Finger30 0xe5f5
#define SKEL_R_Finger31 0xfa40
#define SKEL_R_Finger32 0xfa41
#define SKEL_R_Finger40 0xe5f6
#define SKEL_R_Finger41 0xfa50
#define SKEL_R_Finger42 0xfa51
#define PH_R_Hand 0x6f06
#define IK_R_Hand 0x188e
#define RB_R_ForeArmRoll 0xab22
#define RB_R_ArmRoll 0x90ff
#define MH_R_Elbow 0xbb0
#define SKEL_Neck_1 0x9995
#define SKEL_Head 0x796e
#define IK_Head 0x322c
#define FACIAL_facialRoot 0xfe2c
#define FB_L_Brow_Out_000 0xe3db
#define FB_L_Lid_Upper_000 0xb2b6
#define FB_L_Eye_000 0x62ac
#define FB_L_CheekBone_000 0x542e
#define FB_L_Lip_Corner_000 0x74ac
#define FB_R_Lid_Upper_000 0xaa10
#define FB_R_Eye_000 0x6b52
#define FB_R_CheekBone_000 0x4b88
#define FB_R_Brow_Out_000 0x54c
#define FB_R_Lip_Corner_000 0x2ba6
#define FB_Brow_Centre_000 0x9149
#define FB_UpperLipRoot_000 0x4ed2
#define FB_UpperLip_000 0xf18f
#define FB_L_Lip_Top_000 0x4f37
#define FB_R_Lip_Top_000 0x4537
#define FB_Jaw_000 0xb4a0
#define FB_LowerLipRoot_000 0x4324
#define FB_LowerLip_000 0x508f
#define FB_L_Lip_Bot_000 0xb93b
#define FB_R_Lip_Bot_000 0xc33b
#define FB_Tongue_000 0xb987
#define RB_Neck_1 0x8b93
#define IK_Root 0xdd1c
#define STOMACH           0x60F0
#define BONETAG_HEAD		0x796E 
#define BONETAG_CHEST		0x60F2
#pragma endregion

#include "D3D11Renderer.h"
extern D3D11Renderer *renderer;

typedef void*( __cdecl* GetBoneFromMask )( CObject* pThis, Vector4& vBonePos, WORD dwMask );
typedef void*( __cdecl* GetBoneFromMask2 )( CObject* pThis, Vector4& vBonePos, WORD dwMask );
static GetBoneFromMask GetBone1;
static GetBoneFromMask2 GetBone2;

typedef CPlayerAngles*( __fastcall* GetTPSCamera_t )( LPVOID pECX, LPVOID pEDX ); // If NULL is returned, then We are on First Person. 
static CPlayerAngles* pPlayerAngles = 0;

static Vector3 GetBonePosition1(CObject* pThis, WORD Mask) {
	Vector4 BonePos(0, 0, 0, 0);
	Vector3 BonePosOut(0, 0, 0);

	if ( reinterpret_cast<void*>( GetBone1(pThis, BonePos, Mask) ) ) {
		BonePosOut.x = BonePos.x;
		BonePosOut.y = BonePos.y;
		BonePosOut.z = BonePos.z;
		return BonePosOut;
	}
	return Vector3(0, 0, 0);
}

static Vector3 GetBonePosition2(CObject* pThis, WORD Mask) {
	__declspec( align( 16 ) )Vector4 BonePos(0, 0, 0, 0);
	Vector3 BonePosOut(0, 0, 0);

	if ( reinterpret_cast<void*>( GetBone2(pThis, BonePos, Mask) ) ) {
		BonePosOut.x = BonePos.x;
		BonePosOut.y = BonePos.y;
		BonePosOut.z = BonePos.z;
		return BonePosOut;
	}
	return Vector3(0, 0, 0);
}



bool BoneEsp(CObject* pObj, ColorX BoneColor) {
	//HEAD NECK
	Vector3 firstBone, secondBone = Vector3(0, 0, 0);
	Vector3 firstBoneScreen, secondBoneScreen = Vector3(0, 0, 0);
	Vector3 leftShoulderTmp, chestTmp, stomachTmp = Vector3(0, 0, 0);

	//HEAD-NECK
	firstBone = GetBonePosition2(pObj, SKEL_Head); secondBone = GetBonePosition2(pObj, SKEL_Neck_1);
	if ( renderer->W2S(firstBone, firstBoneScreen) && renderer->W2S(secondBone, secondBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}

	//CHEST-NECK
	firstBone = GetBonePosition2(pObj, BONETAG_CHEST);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		chestTmp = firstBoneScreen;
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//LEFT SHOLDER - NECK
	firstBone = GetBonePosition2(pObj, SKEL_L_UpperArm);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		leftShoulderTmp = firstBoneScreen;
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//RIGHT SHOLDER - NECK
	firstBone = GetBonePosition2(pObj, SKEL_R_UpperArm);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//RIGHT SHOLDER - ELBOW RIGHT
	secondBone = GetBonePosition2(pObj, MH_R_Elbow);
	if ( renderer->W2S(secondBone, secondBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//ELBOW DESN - HAND-ELBOW
	firstBone = GetBonePosition2(pObj, SKEL_R_Hand);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}

	////HAND-ELBOW - HAND
	//if (GetBone2(pObj, secondBone, BONETAG_RIGHTHAND)) {
	//	if (W2S(secondBone, secondBoneScreen)) {
	//		renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, Color(255, 0, 255, 0));
	//	}
	//}
	////HAND - FINGER
	//if (GetBone2(pObj, firstBone, BONETAG_RIGHTHANDMIDDLE)) {
	//	if (W2S(firstBone, firstBoneScreen)) {
	//		renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, Color(255, 0, 255, 0));
	//	}
	//}

	//LEFT SHOLDER - ELBOW LEFT
	secondBone = GetBonePosition2(pObj, MH_L_Elbow);
	if ( renderer->W2S(secondBone, secondBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && leftShoulderTmp.x > 2.0f && secondBoneScreen.y > 2.0f && leftShoulderTmp.y > 2.0f )
			renderer->DrawLine(leftShoulderTmp.x, leftShoulderTmp.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//ELBOW LEFT - HAND-ELBOW
	firstBone = GetBonePosition2(pObj, SKEL_L_Hand);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	////HAND - ELBOW - HAND
	//if (GetBone2(pObj, secondBone, BONETAG_LEFTHAND)) {
	//	if (W2S(secondBone, secondBoneScreen)) {
	//		renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, Color(255, 0, 255, 0));
	//	}
	//}
	////HAND - FINGER
	//if (GetBone2(pObj, firstBone, BONETAG_LEFTHANDMIDDLE)) {
	//	if (W2S(firstBone, firstBoneScreen)) {
	//		renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, Color(255, 0, 255, 0));
	//	}
	//}

	//CHEST - STOMACH
	firstBone = GetBonePosition2(pObj, STOMACH);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		stomachTmp = firstBoneScreen;
		if ( chestTmp.x > 2.0f && firstBoneScreen.x > 2.0f && chestTmp.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, chestTmp.x, chestTmp.y, BoneColor);
	}
	//STOMACH - RIGHT THIG 
	secondBone = GetBonePosition2(pObj, SKEL_R_Thigh);
	if ( renderer->W2S(secondBone, secondBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//RIGHT THIG - RIGHT CALF
	firstBone = GetBonePosition2(pObj, SKEL_R_Calf);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//RIGHT CALF - R FOOT BACK
	secondBone = GetBonePosition2(pObj, SKEL_R_Foot);
	if ( renderer->W2S(secondBone, secondBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//RIGHT THIG - RIGHT CALF
	firstBone = GetBonePosition2(pObj, PH_R_Foot);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}

	//STOMACH - LEFT THIG 
	secondBone = GetBonePosition2(pObj, SKEL_L_Thigh);
	if ( renderer->W2S(secondBone, secondBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && stomachTmp.x > 2.0f && secondBoneScreen.y > 2.0f && stomachTmp.y > 2.0f )
			renderer->DrawLine(stomachTmp.x, stomachTmp.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//RIGHT THIG - LEFT CALF
	firstBone = GetBonePosition2(pObj, SKEL_L_Calf);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//RIGHT CALF - LEFT FOOT BACK
	secondBone = GetBonePosition2(pObj, SKEL_L_Foot);
	if ( renderer->W2S(secondBone, secondBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	//RIGHT THIG - LEFT CALF
	firstBone = GetBonePosition2(pObj, PH_L_Foot);
	if ( renderer->W2S(firstBone, firstBoneScreen) ) {
		if ( secondBoneScreen.x > 2.0f && firstBoneScreen.x > 2.0f && secondBoneScreen.y > 2.0f && firstBoneScreen.y > 2.0f )
			renderer->DrawLine(firstBoneScreen.x, firstBoneScreen.y, secondBoneScreen.x, secondBoneScreen.y, BoneColor);
	}
	return true;
}

void Barrel(Vector3 HeadPos, Vector4 Rotation, ColorX Color, const float trailLen = 10.0f) {
	float rot = acosf(Rotation.x) * 180.0f / D3DX_PI;
	if ( asinf(Rotation.y) * 180.0f / D3DX_PI < 0 ) rot *= -1.0f;

	Vector3 forwardVec;
	float ForwardDirection = XMConvertToRadians(rot + 90.0f);
	forwardVec.x = cosf(ForwardDirection);
	forwardVec.y = sinf(ForwardDirection);
	forwardVec.z = Rotation.z;

	Vector3 dir(forwardVec.x, forwardVec.y, forwardVec.z);
	Vector3 screenOrigin, screenTrail, trailEnd;

	renderer->W2S(HeadPos, screenOrigin);

	Vector3 trail(dir.x * trailLen, dir.y * trailLen, dir.z * trailLen);
	trailEnd = trail + HeadPos;

	renderer->W2S(trailEnd, screenTrail);

	if ( screenOrigin.x > 0.0f && screenOrigin.y > 0.0f && screenTrail.x > 0.0f && screenTrail.y > 0.0f )
		renderer->DrawLine(screenOrigin.x, screenOrigin.y, screenTrail.x, screenTrail.y, Color);
}

bool DrawBoundingBox(Vector3 Top2d, Vector3 Bottom2d, ColorX Color) {
	float H = abs(Top2d.y - Bottom2d.y);
	float W = H / 4.0f;

	int Size = 1;
	renderer->FillRect(Top2d.x - W, Top2d.y, W * 2, Size, Color); // TOP
	renderer->FillRect(Top2d.x - W, Top2d.y + H, W * 2 + Size, Size, Color); //BOTTOM
	renderer->FillRect(Top2d.x - W, Top2d.y, Size, H, Color); //Left side
	renderer->FillRect(Top2d.x + W, Top2d.y, Size, H, Color); //Right side

	return true;
}

typedef CPlayerAngles*( __fastcall* GetTPSCamera_t )( LPVOID pECX, LPVOID pEDX ); // If NULL is returned, then We are on First Person.
GetTPSCamera_t oGetTPSCamera = NULL;
DWORD64 pECX_TPS = 0;

bool GetPlayerAngles() {
	if ( Tools->IsValid(addr.pGameCamManager) && Tools->IsValid(addr.pGameCamManager->pMyCameraManagerAngles) && Tools->IsValid(addr.pGameCamManager->pMyCameraManagerAngles->pMyCameraAngles) ) {
		DWORD64 qwEDX = addr.pGameCamManager->pMyCameraManagerAngles->pMyCameraAngles->pTPSCamEDX;
		if ( *(DWORD64*)( pECX_TPS ) && Tools->IsValid(&qwEDX) ) {
			pPlayerAngles = ( oGetTPSCamera(( LPVOID )*(DWORD64*)( pECX_TPS ), (LPVOID)qwEDX) );
			if ( Tools->IsValid(pPlayerAngles) ) {
				pPlayerAngles = (CPlayerAngles*)( (LPVOID)( DWORD64(pPlayerAngles) + 0x390 ) );
			} else {
				pPlayerAngles = addr.pGameCamManager->pMyCameraManagerAngles->pMyCameraAngles->pMyFPSAngles;
			}
		}
	}
	return true;
}

void SetAngle(Vector3 Target, int smooth = 1) {
	if ( Tools->IsValid(pPlayerAngles) ) {
		Vector3 MyPos = pPlayerAngles->Crosshair_Location;
		float Distance = Tools->Get3dDistance(MyPos, Target);
		Vector3 Out(( Target.x - MyPos.x ) / Distance, ( Target.y - MyPos.y ) / Distance, ( Target.z - MyPos.z ) / Distance);
		pPlayerAngles->ViewAngles2 = Out;
	}
}

Vector2 WorldToRadar(Vector3 Location, FLOAT RadarX, FLOAT RadarY, float RadarSize, float RadarZoom) {
	Vector2 Coord(0, 0);
	Vector2 Return(0, 0);
	if ( Tools->IsValid(addr.localPlayer->_ObjectNavigation) ) {
		if ( Tools->IsValid(addr.pGameCamManager) && Tools->IsValid(addr.pGameCamManager->pMyCameraManagerAngles) && Tools->IsValid(addr.pGameCamManager->pMyCameraManagerAngles->pMyCameraAngles)
			&& Tools->IsValid(addr.pGameCamManager->pMyCameraManagerAngles->pMyCameraAngles->pMyFPSAngles) ) {
			CPlayerAngles* pPlayerAngles = ( addr.pGameCamManager->pMyCameraManagerAngles->pMyCameraAngles->pMyFPSAngles );

			float rot = acosf(pPlayerAngles->ViewAngles.x) * 180.0f / D3DX_PI;
			if ( asinf(pPlayerAngles->ViewAngles.y) * 180.0f / D3DX_PI < 0.0f ) rot *= -1.0f;

			Vector2 forwardVec(0, 0);
			float ForwardDirection = XMConvertToRadians(rot + 90.0f);
			forwardVec.x = cos(ForwardDirection);
			forwardVec.y = sin(ForwardDirection);

			FLOAT CosYaw = forwardVec.x;
			FLOAT SinYaw = forwardVec.y;

			FLOAT DeltaX = Location.x - addr.localPlayer->fPosition.x;
			FLOAT DeltaY = Location.y - addr.localPlayer->fPosition.y;

			FLOAT LocationX = ( DeltaY * CosYaw - DeltaX * SinYaw ) / RadarZoom;
			FLOAT LocationY = ( DeltaX * CosYaw + DeltaY * SinYaw ) / RadarZoom;

			if ( LocationX > RadarSize / 2.0f - 2.5f )
				LocationX = RadarSize / 2.0f - 2.5f;
			else if ( LocationX < -( RadarSize / 2.0f - 2.5f ) )
				LocationX = -( RadarSize / 2.0f - 2.5f );

			if ( LocationY > RadarSize / 2.0f - 2.5f )
				LocationY = RadarSize / 2.0f - 2.5f;
			else if ( LocationY < -( RadarSize / 2.0f - 2.5f ) )
				LocationY = -( RadarSize / 2.0f - 2.5f );

			Return.x = -LocationX + RadarX;
			Return.y = -LocationY + RadarY;
		}
	}
	return Return;
}

typedef void(__cdecl* DisableAllControls_t)( unsigned int index );
static DisableAllControls_t DisableAllControls;
typedef void(__cdecl* SetVehicleForwardSpeed_t)( CVehicleManager vehicle, float Speed );
static SetVehicleForwardSpeed_t SetVehicleForwardSpeed;
extern bool firstTime;

typedef void(__fastcall* SetEntityVisible_t)( int entity, BOOLEAN toggle, BOOLEAN unk );
static SetEntityVisible_t SetEntityVisible;



typedef void(__fastcall* tSET_VEHICLE_NUMBER_PLATE_TEXT)( int vehicle, char *plateText );
static tSET_VEHICLE_NUMBER_PLATE_TEXT SET_VEHICLE_NUMBER_PLATE_TEXT;

typedef void(__fastcall* tDRAW_RECT)( float x, float y, float width, float height, int r, int g, int b, int a );
static tDRAW_RECT oDRAW_RECT;

typedef void(__fastcall* tDranw_line)( PVector3 From, PVector3 To, int r, int g, int b, int a );
static tDranw_line oDRAW_LINE = nullptr;

void draw_rect(float x, float y, float width, float height, int r, int g, int b, int a) { oDRAW_RECT(( x + width * 0.5f ), y + ( height * 0.5f ), width, height, r, g, b, a); }

void draw_rect2(float x, float y, float width, float height, ColorX Color) {
	float Width = renderer->GetWidth(); float Hight = renderer->GetHeight();
	draw_rect(x / Width, y / Hight, width / Width, height / Hight, Color.R, Color.G, Color.B, Color.A);
}

typedef BOOLEAN(__fastcall* WorldToScreen_t)( Vector3 * WorldPos, float* x, float* y );
extern WorldToScreen_t World2Screen;
typedef bool(__cdecl* fpApplyForceToEntity)( DWORD entity, int forceType, PVector3* pos, PVector3* rot, int unk0, bool isRel, bool unk2, bool highForce, bool unk3, bool unk4 );
static fpApplyForceToEntity ApplyForceToEntity = nullptr;

using namespace std;
#include <fstream>
#include <iostream>

DWORD64 LocalPlayerWrapper_address = 0;

typedef DWORD(__cdecl* tSTART_SHAPE_TEST_CAPSULE)( PVector3 From, PVector3 To, float radius, IntersectOptions flags, DWORD entity, int p9 );
tSTART_SHAPE_TEST_CAPSULE _START_SHAPE_TEST_CAPSULE = nullptr;

typedef int(__cdecl* t_GET_RAYCAST_RESULT)( DWORD Handle, bool* hit, PVector3* endCoords, PVector3* surfaceNormal, DWORD* entityHit );
static t_GET_RAYCAST_RESULT _GET_RAYCAST_RESULT = nullptr;


typedef bool(__stdcall *Is_Dlc_Present) ( std::uint64_t hash, bool a2 );
Is_Dlc_Present pIs_Dlc_Present = NULL;
Is_Dlc_Present	Is_DLC_present;


bool VisiblePeds[1024];
bool invisiblePeds[1024];

extern void ApplyHacks();


typedef DWORD64(__fastcall* UpdateObjectNavigation_t)( CObject* pObj, Vector3* Pos, bool returnPointer );
static UpdateObjectNavigation_t UpdateObjectNavigation = nullptr;


typedef bool(__cdecl*	fpisPlayerFreeAiming)( DWORD ped );
static fpisPlayerFreeAiming IsPlayerFreeAiming = nullptr;
fpisPlayerFreeAiming	OGIsPlayerFreeAiming = nullptr;

typedef void(__fastcall* t_ChangePos)( CObject* pObj, PVector3 Pos, bool a3, bool a4, bool a5 );
static t_ChangePos ChangePos = nullptr;



DWORD wakeAt = 0;
void WaitGame(DWORD ms) {
	wakeAt = timeGetTime() + ms;
}




extern float ScreenWidth;
extern float ScreenHight;
extern float ScreenCenterX;
extern float ScreenCenterY;

DWORD64 LocalPlayerWrapper_pattern;

float aimfov = 0.0f;
float radiusx = 0.0f;
float radiusy = 0.0f;
bool IsVisMenu = false;

extern void VehSpeedHack();





void onTick() {
	try {
		WaitGame(100);
		aimfov = 20.0f;
		radiusx = aimfov * ( ScreenCenterX / 100.0f );
		radiusy = aimfov * ( ScreenCenterY / 100.0f );

		if ( Tools->IsValid(&LocalPlayerWrapper_address) )
			addr.LocalPlayerWrapper = *(CWorld**)( LocalPlayerWrapper_address ); //48 8B 05 ? ? ? ? 45 0F C6 C?

		if ( Tools->IsValid(addr.LocalPlayerWrapper) )
			addr.localPlayer = ( addr.LocalPlayerWrapper->getLocalPlayer() );

		if ( Tools->IsValid(addr.localPlayer) ) {
			ApplyHacks();

			
			//if GiveAllWeapons(), GiveWeaponB = false;


			if ( Tools->IsValid(addr.ReplayInterface) && Tools->IsValid(addr.ReplayInterface->ped_interface) ) {
				for ( int i = 0; i < addr.ReplayInterface->ped_interface->max_peds; i++ ) {
					auto tmpEntity = reinterpret_cast<CObject*>( addr.ReplayInterface->ped_interface->get_ped(i) );
					if ( Tools->IsValid(tmpEntity) ) {
						if ( tmpEntity->IsVisible() == true ) {
							static PVector3 EmtVec(0, 0, 0);
							static RaycastResult RaycastResult;
							PVector3 HitCord(0, 0, 0);
							auto tmpEntityHead = GetBonePosition2(tmpEntity, SKEL_Head);
							auto tmpLocalHead = GetBonePosition2(addr.localPlayer, SKEL_R_Hand);
							DWORD Raycast = _START_SHAPE_TEST_CAPSULE(PVector3(tmpLocalHead.x, tmpLocalHead.y, tmpLocalHead.z), PVector3(tmpEntityHead.x, tmpEntityHead.y, tmpEntityHead.z), 0.0f, IntersectMap, (DWORD)0, 7);
							_GET_RAYCAST_RESULT(Raycast, &RaycastResult.DidHitAnything, &HitCord, &EmtVec, &RaycastResult.HitEntity);
							if ( RaycastResult.DidHitAnything ) {
								VisiblePeds[i] = false;
							} else {
								VisiblePeds[i] = true;
							}
							invisiblePeds[i] = false;

						} else {
							invisiblePeds[i] = true;
						}
					}
				}
			}
		}
	} catch ( const std::exception& e ) {
		//CLog::error((char*)e.what());
	} catch ( ... ) {
		//CLog::error("Unknown exception");
	}

}

void WriteToMemory(DWORD64 addressToWrite, char* valueToWrite, int byteNum) {
	unsigned long OldProtection;
	VirtualProtect((LPVOID)( addressToWrite ), byteNum, PAGE_EXECUTE_READWRITE, &OldProtection);
	memcpy((LPVOID)( addressToWrite ), valueToWrite, byteNum);
	VirtualProtect((LPVOID)( addressToWrite ), byteNum, OldProtection, NULL);
}

bool Refresh = false;
DWORD64 FrameCount = 0x0;
bool __stdcall hookIs_Dlc_Present(std::uint64_t hash, bool a2) {
	//if ( firstTime == false ) {
		static uint64_t    last = 0;
		uint64_t        cur = *(uint64_t*)( FrameCount );
		if ( last != cur ) {
			last = cur;
			onTick();
		}
	//}
	return  pIs_Dlc_Present(hash, a2);
}

static void InitGameFunctions() {

	/* NO COPY PASTETE */

}

void VehSpeed(float fSpeed) {
	if ( Tools->IsValid(addr.localPlayer->VehicleManager) ) {
		SetVehicleForwardSpeed(*(CVehicleManager*)addr.localPlayer->VehicleManager, fSpeed);
	}
}

float Get3dDistance2d(float x, float y, Vector3 from) {
	return ( sqrt(
		( ( x - from.x ) * ( x - from.x ) ) +
		( ( y - from.y ) * ( y - from.y ) )
	) );
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
	//int Fast_Reload = false;
	//int Super_Velocity = false;
	//int Super_Dmg = false;
	//int Super_Impulse = false;
	//int Unlimted_Ammo = false;
	//int Sniper_Range = false;

	//Vehicle Menu
	//int Veh_God_Mode = false;
	//float Gravity = 0.0f;
	//float Veh_Impulse = 0.1f;
	//int Veh_Impulse = 1;

	//int SpeedOmeter = false;
	//float VehColorPrimary[3];
	//float VehColorSecondary[3];
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

	float CrossColor[4] = {1.0f, 0.0f, 0.0f, 1.0f};
	float BgColor[4] = {0.0f, 0.0f, 0.0f, 0.2f};
	Vector2 RadarLoc = Vector2(400, 500);

	char LastSave[132] = "Last: Unknown";

};
extern Options Hacks;

/*
void VehSpeedHack() {
	if (Tools->IsValid(addr.localPlayer) && Tools->IsValid(addr.localPlayer->VehicleManager)) { //Vehcile Speed hack
		bool bUp = GetKeyState(VK_NUMPAD9) & 0x800;
		bool bStop = GetKeyState(VK_NUMPAD6) & 0x800;
		bool bDown = GetKeyState(VK_NUMPAD3) & 0x800;
		if (bUp || bDown || bStop) {
			float speed = *(float*)(addr.BASE + 0x24D4E30); //0x722A4E
			if (bUp) {
				speed += speed * float(Hacks.Veh_Impulse * 0.01f);
				VehSpeed(speed);
			}
			if (bStop) {
				speed = 0.0f;
				VehSpeed(speed);
			}
			if (bDown) {
				if (speed > 1.0f)
					speed -= speed * float(Hacks.Veh_Impulse * 0.01f),
					VehSpeed(speed);
			}
		}
	}
}*/

void ApplyHacks() {
	if ( Tools->IsValid(addr.localPlayer) && Tools->IsValid(addr.localPlayer->PlayerInfo) && Tools->IsValid(addr.localPlayer->WeaponManager) ) {
		//if (Hacks.Never_Wanted || Hacks.Always_Wanted)addr.localPlayer->PlayerInfo->SetWanted((Hacks.Never_Wanted ? 0 : 5));
		//if (Hacks.God_Mode)addr.localPlayer->GodModeToggle = true; // God Mode On
		//if (!Hacks.God_Mode)addr.localPlayer->GodModeToggle = false; // God Mode Off

		addr.localPlayer->NoRecoil();
		addr.localPlayer->NoSpread();

		if ( Hacks.No_Recoil )
			addr.localPlayer->NoRecoil();
		if ( Hacks.No_Spread )
			addr.localPlayer->NoSpread();
		//if (Hacks.Fast_Reload)
		//	addr.localPlayer->FastRelod();
		//if (Hacks.Super_Impulse)
		//	addr.localPlayer->SuperImpulse();
		//if (Hacks.Super_Dmg)
		//	addr.localPlayer->SuperDamage();
		//if (Hacks.Sniper_Range)
		//	addr.localPlayer->SniperRange();
		//if (GetKeyState(VK_DELETE) & 0x800) {
		//	addr.localPlayer->PlayerInfo->SetWanted(0);
		//}
		//if (GetKeyState(VK_ADD) & 0x800) { // Heal Yourself
		//	addr.localPlayer->ResetHealth();
		//}
	}
}
