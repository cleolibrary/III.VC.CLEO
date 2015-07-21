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
	WORD text[100];
};

class GtaGame
{
	void InitialiseGameVersion();
	public: void InitAndPatch();

public:

	eGameVersion Version;

	GtaGame();

	eGameVersion GetGameVersion();

	static void InitScripts_OnGameSaveLoad();
	static void InitScripts_OnGameInit();
	static void InitScripts_OnGameReinit();

	static void OnShutdownGame();

	static void OnGameSaveScripts(int a, int b);

	static void OnMenuDrawing(float x, float y, wchar_t *text);

	struct GamePool{
		char *objects;
		unsigned char *flags;
		unsigned int capacity;
		unsigned int count;
	};

	struct _Scripts{
		char *Space;
		tScriptVar *Params;
		unsigned short *pNumOpcodesExecuted;
		OpcodeHandler OpcodeHandlers[15];
		CScript **pActiveScriptsList;
		void (__thiscall *AddScriptToList)(CScript *, CScript **);
		void (__thiscall *RemoveScriptFromList)(CScript *, CScript **);
		void (__thiscall *StoreParameters)(CScript *, unsigned int *, unsigned int);
		void (__thiscall *UpdateCompareFlag)(CScript *, bool);
		void *(__thiscall *GetPointerToScriptVariable)(CScript *, unsigned int *, unsigned char);
	} Scripts;
	struct _Text{
		wchar_t *(__thiscall *pfGetText)(int, char *);
		void(__cdecl *TextBox)(const wchar_t *text, bool flag1, bool infinite, bool flag2);
		void(__cdecl *StyledText)(const wchar_t *text, unsigned time, unsigned style);
		void(__cdecl *TextLowPriority) (const wchar_t *text, unsigned time, bool flag1, bool flag2);
		void(__cdecl *TextHighPriority) (const wchar_t *text, unsigned time, bool flag1, bool flag2);
		uintptr_t CText;
		CTextDrawer *textDrawers;
		unsigned short *currentTextDrawer;
		char *cheatString;
	} Text;
	struct _Screen{
		int *Width;
		int *Height;
	} Screen;
	struct _Font{
		void (__cdecl *AsciiToUnicode)(const char *ascii, short *pUni);
		void (__cdecl *PrintString)(float x, float y, wchar_t *text);
		void (__cdecl *SetFontStyle)(int style);
		void (__cdecl *SetScale)(float w, float h);
		void (__cdecl *SetColor)(unsigned int *color);
		void (__cdecl *SetLeftJustifyOn) ();
		void (__cdecl *SetDropShadowPosition)(int position);
		void (__cdecl *SetPropOn)();
	} Font;
	struct _Pools{
		GamePool **pPedPool;
		GamePool **pVehiclePool;
		GamePool **pObjectPool;
		void* (__thiscall *pfPedPoolGetStruct)(GamePool *pool, int handle);
		void* (__thiscall *pfVehiclePoolGetStruct)(GamePool *pool, int handle);
		void* (__thiscall *pfObjectPoolGetStruct)(GamePool *pool, int handle);
		int (__thiscall *pfPedPoolGetHandle)(GamePool *pool, void *ped);
		int (__thiscall *pfVehiclePoolGetHandle)(GamePool *pool, void *vehicle);
		int (__thiscall *pfObjectPoolGetHandle)(GamePool *pool, void *object);
	} Pools;
	struct _Events{
		void (__cdecl *pfInitScripts_OnGameSaveLoad)();
		void (__cdecl *pfInitScripts_OnGameInit)();
		void (__cdecl *pfInitScripts_OnGameReinit)();
		void (__cdecl *pfShutdownGame)();
		void (__cdecl *pfGameSaveScripts)(int, int);
		void (__cdecl *pfDrawInMenu)(float, float, wchar_t *);
	} Events;

	struct _Shadows{
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

	struct _Misc{
		std::set<FILE *> *openedFiles;
		std::set<void *> *allocatedMemory;
		uintptr_t stVehicleModelInfo;
		uintptr_t activePadState;
		int(__cdecl *pfModelForWeapon)(int eWeaponType);
		uintptr_t cameraWidescreen;
		uintptr_t currentWeather;
		char*(__cdecl *pfGetUserDirectory)();
		void(__cdecl *pfSpawnCar)(unsigned int);
		int(__cdecl *pfCAnimManagerBlendAnimation)(int pRpClump, int dwAnimGroupId, int dwAnimId, float fSpeed);
	} Misc;
};

extern GtaGame game;