#pragma once
#include "CustomScript.h"
#include "OpcodesSystem.h"
#include <set>
#include <algorithm>

enum eGameVersion
{
	GAME_UNKNOWN,
	GAME_V1_0,
	GAME_V1_1,
	GAME_VSTEAM,
	GAME_VSTEAMENC
};

// returned by 0DD5: get_platform opcode
enum ePlatform
{
	PLATFORM_NONE,
	PLATFORM_ANDROID,
	PLATFORM_PSP,
	PLATFORM_IOS,
	PLATFORM_FOS,
	PLATFORM_XBOX,
	PLATFORM_PS2,
	PLATFORM_PS3,
	PLATFORM_MAC,
	PLATFORM_WINDOWS
};

enum OldVirtualKeys
{
	OVK_F1 = 1001,
	OVK_F2,
	OVK_F3,
	OVK_F4,
	OVK_F5,
	OVK_F6,
	OVK_F7,
	OVK_F8,
	OVK_F9,
	OVK_F10,
	OVK_F11,
	OVK_F12,
	OVK_INSERT,
	OVK_DELETE,
	OVK_HOME,
	OVK_END,
	OVK_PRIOR,
	OVK_NEXT,
	OVK_UP,
	OVK_DOWN,
	OVK_LEFT,
	OVK_RIGHT,
	OVK_DIVIDE,
	OVK_MULTIPLY,
	OVK_ADD,
	OVK_SUBTRACT,
	OVK_DECIMAL,
	OVK_NUMPAD1,
	OVK_NUMPAD2,
	OVK_NUMPAD3,
	OVK_NUMPAD4,
	OVK_NUMPAD5,
	OVK_NUMLOCK,
	OVK_NUMPAD6,
	OVK_NUMPAD7,
	OVK_NUMPAD8,
	OVK_NUMPAD9,
	OVK_NUMPAD0,
	OVK_SEPARATOR,
	OVK_SCROLL,
	OVK_PAUSE,
	OVK_BACK,
	OVK_TAB,
	OVK_CAPITAL,
	OVK_RETURN,
	OVK_LSHIFT,
	OVK_RSHIFT,
	OVK_ESC,
	OVK_LCONTROL,
	OVK_RCONTROL,
	OVK_LMENU,
	OVK_RMENU,
	OVK_LWIN,
	OVK_RWIN,
	OVK_APPS
};

struct CVector
{
	float	x, y, z;
};

class CRGBA
{
	BYTE r, g, b, a;
};

struct CTextDrawer
{
	DWORD LetterWidth;
	DWORD LetterHeight;
	CRGBA color;
	BYTE widthAdjustment;
	BYTE centered;
	BYTE drawBox;
	BYTE withBackground;
	DWORD lineWidth;
	DWORD lineHeight;
	CRGBA backgroundBoxColor;
	BYTE proportional;
	BYTE field_1D;
	BYTE alignment;
	BYTE field_1F;
	WORD fontStyle;
	BYTE field_22;
	BYTE field_23;
	float x;
	float y;
#if CLEO_VC
	WORD text[100];
#else
	WORD text[500];
#endif
};

class GtaGame
{
	void InitialiseGameVersion();
	static bool IsChineseVersion();

public:
	
	eGameVersion Version;

	GtaGame();

	eGameVersion GetGameVersion();

	void InitAndPatch();

	static bool IsChinese();

	static void InitScripts_OnGameSaveLoad();
	static void InitScripts_OnGameInit();
	static void InitScripts_OnGameReinit();

	static void OnShutdownGame();

	static void OnGameSaveScripts(int a, int b);

	static void OnMenuDrawing(float x, float y, wchar_t *text);

	struct GamePool
	{
		char *objects;
		unsigned char *flags;
		unsigned int capacity;
		unsigned int count;
	};

	struct tUsedObject
	{
		char name[24];
		int index;
	};

	struct _Scripts
	{
		char *Space;
		tScriptVar *Params;
		unsigned short *pNumOpcodesExecuted;
#if CLEO_VC
		OpcodeHandler OpcodeHandlers[15];
#else
		OpcodeHandler OpcodeHandlers[12];
#endif
		CScript **pActiveScriptsList;
		tUsedObject *usedObjectArray;
		void (__thiscall *AddScriptToList)(CScript *, CScript **);
		void (__thiscall *RemoveScriptFromList)(CScript *, CScript **);
		void (__thiscall *StoreParameters)(CScript *, unsigned int *, unsigned int);
		void (__thiscall *UpdateCompareFlag)(CScript *, bool);
		void *(__thiscall *GetPointerToScriptVariable)(CScript *, unsigned int *, unsigned char);
	} Scripts;

	struct _Text
	{
		wchar_t *(__thiscall *pfGetText)(int, char *);
#if CLEO_VC
		void(__cdecl *TextBox)(const wchar_t *text, bool flag1, bool infinite, bool flag2);
#else
		void(__cdecl *TextBox)(const wchar_t *text, bool flag1);
#endif
		void(__cdecl *StyledText)(const wchar_t *text, unsigned time, unsigned style);
		void(__cdecl *TextLowPriority) (const wchar_t *text, unsigned time, bool flag1, bool flag2);
		void(__cdecl *TextHighPriority) (const wchar_t *text, unsigned time, bool flag1, bool flag2);
		uintptr_t CText;
		CTextDrawer *textDrawers;
		unsigned short *currentTextDrawer;
		char *cheatString;
	} Text;

	struct _Screen
	{
		int *Width;
		int *Height;
	} Screen;

	struct _Font
	{
		void (__cdecl *AsciiToUnicode)(const char *ascii, short *pUni);
		void (__cdecl *PrintString)(float x, float y, wchar_t *text);
		void (__cdecl *SetFontStyle)(int style);
		void (__cdecl *SetScale)(float w, float h);
		void (__cdecl *SetColor)(unsigned int *color);
		void (__cdecl *SetLeftJustifyOn) ();
		void (__cdecl *SetDropShadowPosition)(int position);
		void (__cdecl *SetPropOn)();
	} Font;

	struct _Pools
	{
		GamePool **pPedPool;
		GamePool **pVehiclePool;
		GamePool **pObjectPool;
		uintptr_t *pCPlayerPedPool;
		void* (__thiscall *pfPedPoolGetStruct)(GamePool *pool, int handle);
		void* (__thiscall *pfVehiclePoolGetStruct)(GamePool *pool, int handle);
		void* (__thiscall *pfObjectPoolGetStruct)(GamePool *pool, int handle);
		int (__thiscall *pfPedPoolGetHandle)(GamePool *pool, void *ped);
		int (__thiscall *pfVehiclePoolGetHandle)(GamePool *pool, void *vehicle);
		int (__thiscall *pfObjectPoolGetHandle)(GamePool *pool, void *object);
	} Pools;

	struct _Events
	{
		void (__cdecl *pfInitScripts_OnGameSaveLoad)();
		void (__cdecl *pfInitScripts_OnGameInit)();
		void (__cdecl *pfInitScripts_OnGameReinit)();
		void (__cdecl *pfShutdownGame)();
		void (__cdecl *pfGameSaveScripts)(int, int);
		void (__cdecl *pfDrawInMenu)(float, float, wchar_t *);
	} Events;

	struct _Shadows
	{
		float (__cdecl *StoreShadowToBeRendered)(unsigned char, uintptr_t *, CVector *, float, float, float, float, short, unsigned char, unsigned char, unsigned char, float, bool, float, uintptr_t *, bool);
		uintptr_t **pRwTexture;
		uintptr_t **pRwTexture_shad_car;
		uintptr_t **pRwTexture_shad_ped;
		uintptr_t **pRwTexture_shad_heli;
		uintptr_t **pRwTexture_shad_bike;
		uintptr_t **pRwTexture_shad_rcbaron;
		uintptr_t **pRwTexture_shad_exp;
		uintptr_t **pRwTexture_headlight;
		uintptr_t **pRwTexture_bloodpool_64;
	} Shadows;

	struct _Misc
	{
		// variables
		std::set<FILE *> *openedFiles;
		std::set<void *> *allocatedMemory;
		uintptr_t stVehicleModelInfo;
		uintptr_t activePadState;
		uintptr_t cameraWidescreen;
		uintptr_t currentWeather;

		// functions
		int(__cdecl* pfModelForWeapon)(int eWeaponType);
		char*(__cdecl *pfGetUserDirectory)();
#if CLEO_VC
		void(__cdecl *pfSpawnCar)(unsigned int);
#else
		void(__cdecl *pfSpawnCar)();
#endif
		void(__cdecl* Multiply3x3)(CVector* out, uintptr_t* m, CVector* in);
		void(__cdecl* RwV3dTransformPoints)(CVector*, CVector const*, int, uintptr_t const*);
		bool(__cdecl* pfIsBoatModel)(int mID);
		int(__cdecl *pfCAnimManagerBlendAnimation)(int pRpClump, int dwAnimGroupId, int dwAnimId, float fSpeed);

	} Misc;
};

extern GtaGame game;