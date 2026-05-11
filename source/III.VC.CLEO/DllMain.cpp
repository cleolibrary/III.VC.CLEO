#include <Windows.h>
#include "Game.h"
#include "CPatch.h"
#include "Log.h"
#include "CustomOpcodes.h"
#include "CleoVersion.h"
#include "CleoPlugins.h"
#include <memory>

#pragma warning(disable: 26819)
#pragma warning(disable: 26495)
#pragma warning(disable: 26439)
#include "injector\hooking.hpp"

template<uintptr_t addr>
void RwRenderStateSetHook()
{
	using func_hook = injector::function_hooker<addr, void(int, int)>;
	injector::make_static_hook<func_hook>([](func_hook::func_type RwRenderStateSet, int a1, int a2)
	{
		RwRenderStateSet(a1, a2);
		RwRenderStateSet(0xC, a2);
		return;
	});
}

DWORD WINAPI SteamHandlerDllMain(LPVOID)
{
	while (true)
	{
		Sleep(0);
		if (game.GetGameVersion() == GAME_VSTEAM) break;
	}
	RwRenderStateSetHook<0x5786A5>();
	CustomOpcodes::Register();
	CleoPlugins::LoadPlugins();
	return 0;
}

// All those limits must fit in int8_t due to how their cmp instructions are encoded
static constexpr int8_t NUM_SCRIPT_SPRITES = 127; // No restrictions
static constexpr int8_t NUM_INTRO_RECTANGLES = 120; // Must be divisible by 8 due to partial loop unrolling
static constexpr int8_t NUM_INTRO_TEXT_LINES = 48; // Lifted the III limit to match Vice City; no restrictions

std::unique_ptr<std::byte[]>	ScriptSprites;
std::unique_ptr<std::byte[]>	IntroRectangles;
std::unique_ptr<std::byte[]>	IntroTextLines;

#if CLEO_VC
void PatchArrays()
{
	ScriptSprites = std::make_unique<std::byte[]>(4 * NUM_SCRIPT_SPRITES); // sizeof(CSprite2d)
	IntroRectangles = std::make_unique<std::byte[]>(0x18 * NUM_INTRO_RECTANGLES); // sizeof(tScriptRectangle)

	injector::AdjustPointer(0x450B0E, ScriptSprites.get(), 0x8117E8, 0x8117E8 + 0x0); //  -> mov     edi, offset _scriptTextures
	injector::AdjustPointer(0x450C85, ScriptSprites.get(), 0x8117E8, 0x8117E8 + 0x0); //  -> mov     esi, offset _scriptTextures
	injector::AdjustPointer(0x451668, ScriptSprites.get(), 0x8117E8, 0x8117E8 + 0x0); //  -> mov     esi, offset _scriptTextures
	//injector::AdjustPointer(0x451EA1, ScriptSprites.get(), 0x8117E8, 0x8117E8 + 0x0); //  -> push    offset _scriptTextures; object ctor
	//injector::AdjustPointer(0x451EDA, ScriptSprites.get(), 0x8117E8, 0x8117E8 + 0x0); //  -> push    offset _scriptTextures; objects dtor
	injector::AdjustPointer(0x4593C7, ScriptSprites.get(), 0x8117E8, 0x8117E8 + 0x0); //  -> add     ecx, offset _scriptTextures; this
	injector::AdjustPointer(0x5569AD, ScriptSprites.get(), 0x8117E8, 0x8117E8 + 0x0); //  -> add     ecx, offset _scriptTextures; this
	injector::AdjustPointer(0x55ADFC, ScriptSprites.get(), 0x8117E8, 0x8117E8 + 0x0); //  -> add     ecx, offset _scriptTextures; this

	injector::WriteMemory<char>(0x450B1D + 3, NUM_SCRIPT_SPRITES, true);
	injector::WriteMemory<char>(0x450C9B + 3, NUM_SCRIPT_SPRITES, true);
	injector::WriteMemory<char>(0x45167E + 3, NUM_SCRIPT_SPRITES, true); //CMissionCleanup::Process()


	injector::AdjustPointer(0x450125, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[eax], 0
	injector::AdjustPointer(0x450146, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ebx], 0
	injector::AdjustPointer(0x450164, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ecx], 0
	injector::AdjustPointer(0x450183, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ss:_textSprites.active[ebp], 0
	injector::AdjustPointer(0x4501A1, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ebx], 0
	injector::AdjustPointer(0x4501BF, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ecx], 0
	injector::AdjustPointer(0x4501DE, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ss:_textSprites.active[ebp], 0
	injector::AdjustPointer(0x450203, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ebx], 0
	injector::AdjustPointer(0x450A78, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ebx], 0
	//injector::AdjustPointer(0x451E72, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> push    offset _textSprites; object
	//injector::AdjustPointer(0x451EFA, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> push    offset _textSprites; objects
	injector::AdjustPointer(0x45918E, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ss:_textSprites.active[ebp], 1
	injector::AdjustPointer(0x4591FB, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> add     esi, offset _textSprites
	injector::AdjustPointer(0x45929E, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ss:_textSprites.active[ebp], 1
	injector::AdjustPointer(0x459306, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> add     esi, offset _textSprites
	injector::AdjustPointer(0x55690B, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     esi, offset _textSprites
	injector::AdjustPointer(0x556912, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> cmp     byte ptr ss:_textSprites.active[ebp], 0
	injector::AdjustPointer(0x55AD3C, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> mov     ebp, offset _textSprites
	injector::AdjustPointer(0x55AD42, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x0); //  -> cmp     byte ptr ds:_textSprites.active[ebx], 0
	injector::AdjustPointer(0x45012D, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[eax], 0
	injector::AdjustPointer(0x45014D, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x45016B, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ecx], 0
	injector::AdjustPointer(0x45018A, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ss:_textSprites.antialiased[ebp], 0
	injector::AdjustPointer(0x4501A8, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x4501C6, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ecx], 0
	injector::AdjustPointer(0x4501E5, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ss:_textSprites.antialiased[ebp], 0
	injector::AdjustPointer(0x45020A, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x450A93, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x45B07C, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[eax*8], 1
	injector::AdjustPointer(0x45B090, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[eax*8], 0
	injector::AdjustPointer(0x55691F, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> cmp     ss:_textSprites.antialiased[ebp], 0
	injector::AdjustPointer(0x55AD4F, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x1); //  -> cmp     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x450A9B, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x2); //  -> or      ds:_textSprites.textureID[ebx], 0FFFFh
	injector::AdjustPointer(0x459196, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x2); //  -> mov     ss:_textSprites.textureID[ebp], ax
	injector::AdjustPointer(0x4592A6, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x2); //  -> or      ss:_textSprites.textureID[ebp], 0FFFFh
	injector::AdjustPointer(0x55692D, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x2); //  -> mov     dx, ss:_textSprites.textureID[ebp]
	injector::AdjustPointer(0x55AD5D, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x2); //  -> mov     dx, ds:_textSprites.textureID[ebx]
	injector::AdjustPointer(0x450AA2, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x4); //  -> mov     dword ptr ds:_textSprites.pos.x[ebx], 0
	injector::AdjustPointer(0x45919C, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x4); //  -> fstp    dword ptr ss:_textSprites.pos.x[ebp]
	injector::AdjustPointer(0x4592AD, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x4); //  -> fstp    dword ptr ss:_textSprites.pos.x[ebp]
	injector::AdjustPointer(0x556938, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x4); //  -> fld     dword ptr ss:_textSprites.pos.x[ebp]
	injector::AdjustPointer(0x556972, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x4); //  -> fld     dword ptr ss:_textSprites.pos.x[ebp]
	injector::AdjustPointer(0x55AD68, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x4); //  -> fld     dword ptr ds:_textSprites.pos.x[ebx]
	injector::AdjustPointer(0x55ADB2, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x4); //  -> fld     dword ptr ds:_textSprites.pos.x[ebx]
	injector::AdjustPointer(0x450AAC, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x8); //  -> mov     ds:_textSprites.pos.y[ebx], 0
	injector::AdjustPointer(0x4591A6, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x8); //  -> fstp    ss:_textSprites.pos.y[ebp]
	injector::AdjustPointer(0x4592B7, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x8); //  -> fstp    ss:_textSprites.pos.y[ebp]
	injector::AdjustPointer(0x556942, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x8); //  -> fld     ss:_textSprites.pos.y[ebp]
	injector::AdjustPointer(0x55697C, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x8); //  -> fld     ss:_textSprites.pos.y[ebp]
	injector::AdjustPointer(0x55AD75, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x8); //  -> fld     ds:_textSprites.pos.y[ebx]
	injector::AdjustPointer(0x55ADBF, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x8); //  -> fld     ds:_textSprites.pos.y[ebx]
	injector::AdjustPointer(0x450AB6, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0xC); //  -> mov     ds:_textSprites.pos.w[ebx], 0
	injector::AdjustPointer(0x4591BB, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0xC); //  -> fstp    ss:_textSprites.pos.w[ebp]
	injector::AdjustPointer(0x4592C1, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0xC); //  -> fstp    ss:_textSprites.pos.w[ebp]
	injector::AdjustPointer(0x55694C, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0xC); //  -> fld     ss:_textSprites.pos.w[ebp]
	injector::AdjustPointer(0x556986, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0xC); //  -> fld     ss:_textSprites.pos.w[ebp]
	injector::AdjustPointer(0x55AD82, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0xC); //  -> fld     ds:_textSprites.pos.w[ebx]
	injector::AdjustPointer(0x55ADCC, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0xC); //  -> fld     ds:_textSprites.pos.w[ebx]
	injector::AdjustPointer(0x450AC0, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x10); //  -> mov     ds:_textSprites.pos.h[ebx], 0
	injector::AdjustPointer(0x4591CB, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x10); //  -> fstp    ss:_textSprites.pos.h[ebp]
	injector::AdjustPointer(0x4592D6, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x10); //  -> fstp    ss:_textSprites.pos.h[ebp]
	injector::AdjustPointer(0x556956, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x10); //  -> fld     ss:_textSprites.pos.h[ebp]
	injector::AdjustPointer(0x556990, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x10); //  -> fld     ss:_textSprites.pos.h[ebp]
	injector::AdjustPointer(0x55AD8F, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x10); //  -> fld     ds:_textSprites.pos.h[ebx]
	injector::AdjustPointer(0x55ADD9, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x10); //  -> fld     ds:_textSprites.pos.h[ebx]
	injector::AdjustPointer(0x450AD8, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x14); //  -> mov     byte ptr ds:_textSprites.transparentColor.red[ebx], cl
	injector::AdjustPointer(0x450AE2, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x15); //  -> mov     ds:_textSprites.transparentColor.green[ebx], al
	injector::AdjustPointer(0x450AEC, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x16); //  -> mov     ds:_textSprites.transparentColor.blue[ebx], dl
	injector::AdjustPointer(0x450AF2, IntroRectangles.get(), 0x813DF0, 0x813DF0 + 0x17); //  -> mov     ds:_textSprites.transparentColor.alpha[ebx], al

	injector::WriteMemory<char>(0x4501F3 + 3, NUM_INTRO_RECTANGLES, true);
	injector::WriteMemory<char>(0x450AF9 + 2, NUM_INTRO_RECTANGLES, true);
	injector::WriteMemory<char>(0x4501F3 + 3, NUM_INTRO_RECTANGLES, true);
	injector::WriteMemory<char>(0x5569BD + 3, NUM_INTRO_RECTANGLES, true);
	injector::WriteMemory<char>(0x55AE0C + 3, NUM_INTRO_RECTANGLES, true);
}
#else
void PatchArrays()
{
	ScriptSprites = std::make_unique<std::byte[]>(4 * NUM_SCRIPT_SPRITES); // sizeof(CSprite2d)
	IntroRectangles = std::make_unique<std::byte[]>(0x18 * NUM_INTRO_RECTANGLES); // sizeof(tScriptRectangle)
	IntroTextLines = std::make_unique<std::byte[]>(sizeof(CTextDrawer) * NUM_INTRO_TEXT_LINES);

	injector::AdjustPointer(0x438BB9, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > mov     ds:dword_70EA68[ebx], 3EF5C28Fh
	injector::AdjustPointer(0x439106, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > mov     ds:dword_70EA68[ebx], 3EF5C28Fh
																				   //injector::AdjustPointer(0x43EBEC, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > push    offset dword_70EA68
																				   //injector::AdjustPointer(0x43ECDD, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > push    offset dword_70EA68
	injector::AdjustPointer(0x449390, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > fstp    ds:dword_70EA68[eax]
	injector::AdjustPointer(0x44943B, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > add     esi, offset dword_70EA68
	injector::AdjustPointer(0x4496BD, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > add     edi, offset dword_70EA68
	injector::AdjustPointer(0x5084DB, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > mov     esi, offset dword_70EA68
	injector::AdjustPointer(0x508526, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > fmul    ss:dword_70EA68[ebp]
	injector::AdjustPointer(0x50955D, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > mov     esi, offset dword_70EA68
	injector::AdjustPointer(0x5095A7, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > fmul    ss:dword_70EA68[ebp]
	injector::AdjustPointer(0x58A3B1, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > add     eax, offset dword_70EA68
	injector::AdjustPointer(0x58A468, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x0); // > add     eax, offset dword_70EA68
	injector::AdjustPointer(0x438BD7, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x4); // > mov     ds:(dword_70EA68+4)[ebx], 3F8F5C29h
	injector::AdjustPointer(0x439124, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x4); // > mov     ds:(dword_70EA68+4)[ebx], 3F8F5C29h
	injector::AdjustPointer(0x44939C, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x4); // > fstp    ds:(dword_70EA68+4)[eax]
	injector::AdjustPointer(0x50850A, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x4); // > fmul    ss:(dword_70EA68+4)[ebp]
	injector::AdjustPointer(0x50958B, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x4); // > fmul    ss:(dword_70EA68+4)[ebp]
	injector::AdjustPointer(0x438BF2, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x8); // > mov     byte ptr ds:(dword_70EA68+8)[ebx], dl
	injector::AdjustPointer(0x43913F, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x8); // > mov     byte ptr ds:(dword_70EA68+8)[ebx], dl
	injector::AdjustPointer(0x508534, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x8); // > mov     edx, ss:(dword_70EA68+8)[ebp]
	injector::AdjustPointer(0x5095B7, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x8); // > mov     ecx, ss:(dword_70EA68+8)[ebp]
	injector::AdjustPointer(0x438BF8, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x9); // > mov     byte ptr ds:(dword_70EA68+9)[ebx], al
	injector::AdjustPointer(0x439145, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x9); // > mov     byte ptr ds:(dword_70EA68+9)[ebx], al
	injector::AdjustPointer(0x438BFE, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xA); // > mov     byte ptr ds:(dword_70EA68+0Ah)[ebx], cl
	injector::AdjustPointer(0x43914B, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xA); // > mov     byte ptr ds:(dword_70EA68+0Ah)[ebx], cl
	injector::AdjustPointer(0x438C20, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xB); // > mov     byte ptr ds:(dword_70EA68+0Bh)[ebx], al
	injector::AdjustPointer(0x43916D, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xB); // > mov     byte ptr ds:(dword_70EA68+0Bh)[ebx], al
	injector::AdjustPointer(0x438C26, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xC); // > mov     byte ptr ds:(dword_70EA68+0Ch)[ebx], 0
	injector::AdjustPointer(0x439173, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xC); // > mov     byte ptr ds:(dword_70EA68+0Ch)[ebx], 0
	injector::AdjustPointer(0x4494A9, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xC); // > mov     byte ptr ds:(dword_70EA68+0Ch)[eax*4], 1
	injector::AdjustPointer(0x4494C0, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xC); // > mov     byte ptr ds:(dword_70EA68+0Ch)[eax*4], 0
	injector::AdjustPointer(0x508550, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xC); // > cmp     byte ptr ss:(dword_70EA68+0Ch)[ebp], 0
	injector::AdjustPointer(0x5095CB, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xC); // > cmp     byte ptr ss:(dword_70EA68+0Ch)[ebp], 0
	injector::AdjustPointer(0x438C34, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xD); // > mov     byte ptr ds:(dword_70EA68+0Dh)[ebx], 0
	injector::AdjustPointer(0x439181, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xD); // > mov     byte ptr ds:(dword_70EA68+0Dh)[ebx], 0
	injector::AdjustPointer(0x44950C, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xD); // > mov     byte ptr ds:(dword_70EA68+0Dh)[eax*4], 1
	injector::AdjustPointer(0x449523, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xD); // > mov     byte ptr ds:(dword_70EA68+0Dh)[eax*4], 0
	injector::AdjustPointer(0x50857C, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xD); // > cmp     byte ptr ss:(dword_70EA68+0Dh)[ebp], 0
	injector::AdjustPointer(0x5095FC, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xD); // > cmp     byte ptr ss:(dword_70EA68+0Dh)[ebp], 0
	injector::AdjustPointer(0x438C3B, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xE); // > mov     byte ptr ds:(dword_70EA68+0Eh)[ebx], 0
	injector::AdjustPointer(0x439188, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xE); // > mov     byte ptr ds:(dword_70EA68+0Eh)[ebx], 0
	injector::AdjustPointer(0x4495FF, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xE); // > mov     byte ptr ds:(dword_70EA68+0Eh)[eax*4], 1
	injector::AdjustPointer(0x449616, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xE); // > mov     byte ptr ds:(dword_70EA68+0Eh)[eax*4], 0
	injector::AdjustPointer(0x5085CE, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xE); // > cmp     byte ptr ss:(dword_70EA68+0Eh)[ebp], 0
	injector::AdjustPointer(0x50964E, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xE); // > cmp     byte ptr ss:(dword_70EA68+0Eh)[ebp], 0
	injector::AdjustPointer(0x438C42, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xF); // > mov     byte ptr ds:(dword_70EA68+0Fh)[ebx], 0
	injector::AdjustPointer(0x43918F, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xF); // > mov     byte ptr ds:(dword_70EA68+0Fh)[ebx], 0
	injector::AdjustPointer(0x44972B, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xF); // > mov     byte ptr ds:(dword_70EA68+0Fh)[eax*4], 1
	injector::AdjustPointer(0x449742, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xF); // > mov     byte ptr ds:(dword_70EA68+0Fh)[eax*4], 0
	injector::AdjustPointer(0x508601, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xF); // > cmp     byte ptr ss:(dword_70EA68+0Fh)[ebp], 0
	injector::AdjustPointer(0x50967B, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0xF); // > cmp     byte ptr ss:(dword_70EA68+0Fh)[ebp], 0
	injector::AdjustPointer(0x438C49, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x10); // -> mov     ds:(dword_70EA68+10h)[ebx], 43360000h
	injector::AdjustPointer(0x439196, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x10); // -> mov     ds:(dword_70EA68+10h)[ebx], 43360000h
	injector::AdjustPointer(0x449573, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x10); // -> fstp    ds:(dword_70EA68+10h)[eax*4]
	injector::AdjustPointer(0x5085A4, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x10); // -> fmul    ss:(dword_70EA68+10h)[ebp]
	injector::AdjustPointer(0x509624, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x10); // -> fmul    ss:(dword_70EA68+10h)[ebp]
	injector::AdjustPointer(0x438C53, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x14); // -> mov     ds:(dword_70EA68+14h)[ebx], 44200000h
	injector::AdjustPointer(0x4391A0, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x14); // -> mov     ds:(dword_70EA68+14h)[ebx], 44200000h
	injector::AdjustPointer(0x4495BB, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x14); // -> fstp    ds:(dword_70EA68+14h)[eax*4]
	injector::AdjustPointer(0x5085C0, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x14); // -> fmul    ss:(dword_70EA68+14h)[ebp]
	injector::AdjustPointer(0x509640, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x14); // -> fmul    ss:(dword_70EA68+14h)[ebp]
	injector::AdjustPointer(0x438C6E, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x18); // -> mov     byte ptr ds:(dword_70EA68+18h)[ebx], dl
	injector::AdjustPointer(0x4391BB, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x18); // -> mov     byte ptr ds:(dword_70EA68+18h)[ebx], dl
	injector::AdjustPointer(0x5085E7, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x18); // -> mov     ecx, ss:(dword_70EA68+18h)[ebp]
	injector::AdjustPointer(0x509667, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x18); // -> mov     edx, ss:(dword_70EA68+18h)[ebp]
	injector::AdjustPointer(0x438C76, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x19); // -> mov     byte ptr ds:(dword_70EA68+19h)[ebx], al
	injector::AdjustPointer(0x4391C1, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x19); // -> mov     byte ptr ds:(dword_70EA68+19h)[ebx], al
	injector::AdjustPointer(0x438C80, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1A); // -> mov     byte ptr ds:(dword_70EA68+1Ah)[ebx], cl
	injector::AdjustPointer(0x4391CB, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1A); // -> mov     byte ptr ds:(dword_70EA68+1Ah)[ebx], cl
	injector::AdjustPointer(0x438C89, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1B); // -> mov     byte ptr ds:(dword_70EA68+1Bh)[ebx], al
	injector::AdjustPointer(0x4391D1, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1B); // -> mov     byte ptr ds:(dword_70EA68+1Bh)[ebx], al
	injector::AdjustPointer(0x438C91, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1C); // -> mov     byte ptr ds:(dword_70EA68+1Ch)[ebx], 1
	injector::AdjustPointer(0x4391D9, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1C); // -> mov     byte ptr ds:(dword_70EA68+1Ch)[ebx], 1
	injector::AdjustPointer(0x44978E, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1C); // -> mov     byte ptr ds:(dword_70EA68+1Ch)[eax*4], 1
	injector::AdjustPointer(0x4497A5, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1C); // -> mov     byte ptr ds:(dword_70EA68+1Ch)[eax*4], 0
	injector::AdjustPointer(0x508617, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1C); // -> cmp     byte ptr ss:(dword_70EA68+1Ch)[ebp], 0
	injector::AdjustPointer(0x509697, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1C); // -> cmp     byte ptr ss:(dword_70EA68+1Ch)[ebp], 0
	injector::AdjustPointer(0x438C98, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1D); // -> mov     byte ptr ds:(dword_70EA68+1Dh)[ebx], 0
	injector::AdjustPointer(0x4391E2, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1D); // -> mov     byte ptr ds:(dword_70EA68+1Dh)[ebx], 0
	injector::AdjustPointer(0x44F7B6, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1D); // -> mov     byte ptr ds:(dword_70EA68+1Dh)[eax*4], 1
	injector::AdjustPointer(0x44F7D0, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1D); // -> mov     byte ptr ds:(dword_70EA68+1Dh)[eax*4], 0
	injector::AdjustPointer(0x5084F0, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1D); // -> cmp     byte ptr ss:(dword_70EA68+1Dh)[ebp], 0
	injector::AdjustPointer(0x509571, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1D); // -> cmp     byte ptr ss:(dword_70EA68+1Dh)[ebp], 0
	injector::AdjustPointer(0x438C2D, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1E); // -> mov     byte ptr ds:(dword_70EA68+1Eh)[ebx], 0
	injector::AdjustPointer(0x43917A, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1E); // -> mov     byte ptr ds:(dword_70EA68+1Eh)[ebx], 0
	injector::AdjustPointer(0x44F8CD, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1E); // -> mov     byte ptr ds:(dword_70EA68+1Eh)[eax*4], 1
	injector::AdjustPointer(0x44F8E4, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1E); // -> mov     byte ptr ds:(dword_70EA68+1Eh)[eax*4], 0
	injector::AdjustPointer(0x508567, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1E); // -> cmp     byte ptr ss:(dword_70EA68+1Eh)[ebp], 0
	injector::AdjustPointer(0x5095E7, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x1E); // -> cmp     byte ptr ss:(dword_70EA68+1Eh)[ebp], 0
	injector::AdjustPointer(0x438C9F, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x20); // -> mov     ds:(dword_70EA68+20h)[ebx], 2
	injector::AdjustPointer(0x4391E9, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x20); // -> mov     ds:(dword_70EA68+20h)[ebx], 2
	injector::AdjustPointer(0x4497F4, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x20); // -> mov     ds:(dword_70EA68+20h)[ebx*4], eax
	injector::AdjustPointer(0x50862D, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x20); // -> mov     ax, word ptr ss:(dword_70EA68+20h)[ebp]
	injector::AdjustPointer(0x5096AD, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x20); // -> mov     ax, word ptr ss:(dword_70EA68+20h)[ebp]
	injector::AdjustPointer(0x438CA9, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x24); // -> mov     ds:(dword_70EA68+24h)[ebx], 0
	injector::AdjustPointer(0x4391F3, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x24); // -> mov     ds:(dword_70EA68+24h)[ebx], 0
	injector::AdjustPointer(0x44923F, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x24); // -> fstp    ds:(dword_70EA68+24h)[ebx]
	injector::AdjustPointer(0x50868B, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x24); // -> fsub    ss:(dword_70EA68+24h)[ebp]
	injector::AdjustPointer(0x50970A, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x24); // -> fsub    ss:(dword_70EA68+24h)[ebp]
	injector::AdjustPointer(0x58A386, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x24); // -> fstp    ds:(dword_70EA68+24h)[eax]
	injector::AdjustPointer(0x58A437, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x24); // -> fstp    ds:(dword_70EA68+24h)[eax]
	injector::AdjustPointer(0x438CB3, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x28); // -> mov     ds:(dword_70EA68+28h)[ebx], 0
	injector::AdjustPointer(0x4391FD, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x28); // -> mov     ds:(dword_70EA68+28h)[ebx], 0
	injector::AdjustPointer(0x44924B, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x28); // -> fstp    ds:(dword_70EA68+28h)[ebx]
	injector::AdjustPointer(0x50865D, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x28); // -> fsub    ss:(dword_70EA68+28h)[ebp]
	injector::AdjustPointer(0x5096DD, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x28); // -> fsub    ss:(dword_70EA68+28h)[ebp]
	injector::AdjustPointer(0x58A392, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x28); // -> fstp    ds:(dword_70EA68+28h)[eax]
	injector::AdjustPointer(0x58A443, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x28); // -> fstp    ds:(dword_70EA68+28h)[eax]
	injector::AdjustPointer(0x438CC7, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2C); // -> movq    qword ptr ds:(dword_70EA68+2Ch)[eax*2], mm1
	injector::AdjustPointer(0x438D24, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[edx*2], 0
	injector::AdjustPointer(0x43920C, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[edx*2], 0
	injector::AdjustPointer(0x43927B, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[eax*2], 0
	injector::AdjustPointer(0x449288, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[esi*2], di
	injector::AdjustPointer(0x44930A, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[ebx*2], di
	injector::AdjustPointer(0x449328, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[ebp*2], 0
	injector::AdjustPointer(0x5084E3, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2C); // -> cmp     word ptr ss:(dword_70EA68+2Ch)[ebp], 0
	injector::AdjustPointer(0x509564, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2C); // -> cmp     word ptr ss:(dword_70EA68+2Ch)[ebp], 0
	injector::AdjustPointer(0x438D2E, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2E); // -> mov     word ptr ds:(dword_70EA68+2Eh)[edx*2], 0
	injector::AdjustPointer(0x439216, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2E); // -> mov     word ptr ds:(dword_70EA68+2Eh)[edx*2], 0
	injector::AdjustPointer(0x439285, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2E); // -> mov     word ptr ds:(dword_70EA68+2Eh)[eax*2], 0
	injector::AdjustPointer(0x449295, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x2E); // -> mov     word ptr ds:(dword_70EA68+2Eh)[esi*2], bx
	injector::AdjustPointer(0x438D38, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x30); // -> mov     word ptr ds:(dword_70EA68+30h)[edx*2], 0
	injector::AdjustPointer(0x439220, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x30); // -> mov     word ptr ds:(dword_70EA68+30h)[edx*2], 0
	injector::AdjustPointer(0x43928F, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x30); // -> mov     word ptr ds:(dword_70EA68+30h)[eax*2], 0
	injector::AdjustPointer(0x4492A2, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x30); // -> mov     word ptr ds:(dword_70EA68+30h)[esi*2], di
	injector::AdjustPointer(0x438D42, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x32); // -> mov     word ptr ds:(dword_70EA68+32h)[edx*2], 0
	injector::AdjustPointer(0x43922A, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x32); // -> mov     word ptr ds:(dword_70EA68+32h)[edx*2], 0
	injector::AdjustPointer(0x439299, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x32); // -> mov     word ptr ds:(dword_70EA68+32h)[eax*2], 0
	injector::AdjustPointer(0x4492AF, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x32); // -> mov     word ptr ds:(dword_70EA68+32h)[esi*2], bx
	injector::AdjustPointer(0x438CCF, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x34); // -> movq    qword ptr ds:(dword_70EA68+34h)[eax*2], mm1
	injector::AdjustPointer(0x438D4C, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x34); // -> mov     word ptr ds:(dword_70EA68+34h)[edx*2], 0
	injector::AdjustPointer(0x439234, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x34); // -> mov     word ptr ds:(dword_70EA68+34h)[edx*2], 0
	injector::AdjustPointer(0x4492BC, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x34); // -> mov     word ptr ds:(dword_70EA68+34h)[esi*2], di
	injector::AdjustPointer(0x438D56, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x36); // -> mov     word ptr ds:(dword_70EA68+36h)[edx*2], 0
	injector::AdjustPointer(0x43923E, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x36); // -> mov     word ptr ds:(dword_70EA68+36h)[edx*2], 0
	injector::AdjustPointer(0x4492C9, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x36); // -> mov     word ptr ds:(dword_70EA68+36h)[esi*2], bx
	injector::AdjustPointer(0x438D60, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x38); // -> mov     word ptr ds:(dword_70EA68+38h)[edx*2], 0
	injector::AdjustPointer(0x439248, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x38); // -> mov     word ptr ds:(dword_70EA68+38h)[edx*2], 0
	injector::AdjustPointer(0x4492D6, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x38); // -> mov     word ptr ds:(dword_70EA68+38h)[esi*2], di
	injector::AdjustPointer(0x438D6A, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x3A); // -> mov     word ptr ds:(dword_70EA68+3Ah)[edx*2], 0
	injector::AdjustPointer(0x439252, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x3A); // -> mov     word ptr ds:(dword_70EA68+3Ah)[edx*2], 0
	injector::AdjustPointer(0x4492E6, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x3A); // -> mov     word ptr ds:(dword_70EA68+3Ah)[esi*2], bx
	injector::AdjustPointer(0x438CD7, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x3C); // -> movq    qword ptr ds:(dword_70EA68+3Ch)[eax*2], mm1
	injector::AdjustPointer(0x438D74, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x3C); // -> mov     word ptr ds:(dword_70EA68+3Ch)[edx*2], 0
	injector::AdjustPointer(0x438D7E, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x3E); // -> mov     word ptr ds:(dword_70EA68+3Eh)[edx*2], 0
	injector::AdjustPointer(0x438D88, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x40); // -> mov     word ptr ds:(dword_70EA68+40h)[edx*2], 0
	injector::AdjustPointer(0x438D92, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x42); // -> mov     word ptr ds:(dword_70EA68+42h)[edx*2], 0
	injector::AdjustPointer(0x438CDF, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x44); // -> movq    qword ptr ds:(dword_70EA68+44h)[eax*2], mm1
	injector::AdjustPointer(0x438D9C, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x44); // -> mov     word ptr ds:(dword_70EA68+44h)[edx*2], 0
	injector::AdjustPointer(0x438DA6, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x46); // -> mov     word ptr ds:(dword_70EA68+46h)[edx*2], 0
	injector::AdjustPointer(0x438DB0, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x48); // -> mov     word ptr ds:(dword_70EA68+48h)[edx*2], 0
	injector::AdjustPointer(0x438DBA, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x4A); // -> mov     word ptr ds:(dword_70EA68+4Ah)[edx*2], 0
	injector::AdjustPointer(0x438CE7, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x4C); // -> movq    qword ptr ds:(dword_70EA68+4Ch)[eax*2], mm1
	injector::AdjustPointer(0x438DC4, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x4C); // -> mov     word ptr ds:(dword_70EA68+4Ch)[edx*2], 0
	injector::AdjustPointer(0x438DCE, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x4E); // -> mov     word ptr ds:(dword_70EA68+4Eh)[edx*2], 0
	injector::AdjustPointer(0x438DD8, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x50); // -> mov     word ptr ds:(dword_70EA68+50h)[edx*2], 0
	injector::AdjustPointer(0x438DE2, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x52); // -> mov     word ptr ds:(dword_70EA68+52h)[edx*2], 0
	injector::AdjustPointer(0x438CEF, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x54); // -> movq    qword ptr ds:(dword_70EA68+54h)[eax*2], mm1
	injector::AdjustPointer(0x438CF7, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x5C); // -> movq    qword ptr ds:(dword_70EA68+5Ch)[eax*2], mm1
	injector::AdjustPointer(0x438CFF, IntroTextLines.get(), 0x70EA68, 0x70EA68 + 0x64); // -> movq    qword ptr ds:(dword_70EA68+64h)[eax*2], mm1


	injector::WriteMemory<char>(0x438D1D + 2, NUM_INTRO_TEXT_LINES, true);
	injector::WriteMemory<char>(0x5086AD + 3, NUM_INTRO_TEXT_LINES, true);
	injector::WriteMemory<char>(0x439273 + 3, NUM_INTRO_TEXT_LINES, true);
	injector::WriteMemory<char>(0x50972C + 3, NUM_INTRO_TEXT_LINES, true);


	//injector::AdjustPointer(0x43EC4A, ScriptSprites.get(), 0x72B090, 0x72B090 + 0x0); // -> push    offset unk_72B090
	//injector::AdjustPointer(0x43EC7A, ScriptSprites.get(), 0x72B090, 0x72B090 + 0x0); // -> push    offset unk_72B090
	injector::AdjustPointer(0x44D65B, ScriptSprites.get(), 0x72B090, 0x72B090 + 0x0); // -> add     ecx, offset unk_72B090
	injector::AdjustPointer(0x44D709, ScriptSprites.get(), 0x72B090, 0x72B090 + 0x0); // -> mov     esi, offset unk_72B090
	injector::AdjustPointer(0x50874F, ScriptSprites.get(), 0x72B090, 0x72B090 + 0x0); // -> add     ecx, offset unk_72B090
	injector::AdjustPointer(0x5097C6, ScriptSprites.get(), 0x72B090, 0x72B090 + 0x0); // -> add     ecx, offset unk_72B090

	injector::WriteMemory<char>(0x44D71B + 2, NUM_SCRIPT_SPRITES, true);

	injector::AdjustPointer(0x438E0A, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ebx], 0
	injector::AdjustPointer(0x4392B6, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[eax], 0
	injector::AdjustPointer(0x4392D7, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ebx], 0
	injector::AdjustPointer(0x4392F5, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ecx], 0
	injector::AdjustPointer(0x439314, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ss:byte_72D108[ebp], 0
	injector::AdjustPointer(0x439332, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ebx], 0
	injector::AdjustPointer(0x439350, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ecx], 0
	injector::AdjustPointer(0x43936F, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ss:byte_72D108[ebp], 0
	injector::AdjustPointer(0x439394, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ebx], 0
																				   //injector::AdjustPointer(0x43EC1B, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> push    offset byte_72D108
																				   //injector::AdjustPointer(0x43EC9A, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> push    offset byte_72D108
	injector::AdjustPointer(0x44D421, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ss:byte_72D108[ebp], 1
	injector::AdjustPointer(0x44D48D, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> add     edi, offset byte_72D108
	injector::AdjustPointer(0x44D525, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[eax], 1
	injector::AdjustPointer(0x44D58B, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> add     edi, offset byte_72D108
	injector::AdjustPointer(0x5086BC, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     esi, offset byte_72D108
	injector::AdjustPointer(0x5086C2, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> cmp     ss:byte_72D108[ebp], 0
	injector::AdjustPointer(0x50973B, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> mov     esi, offset byte_72D108
	injector::AdjustPointer(0x509742, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x0); // -> cmp     ss:byte_72D108[ebp], 0
	injector::AdjustPointer(0x438E25, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ebx], 0
	injector::AdjustPointer(0x4392BE, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[eax], 0
	injector::AdjustPointer(0x4392DE, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ebx], 0
	injector::AdjustPointer(0x4392FC, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ecx], 0
	injector::AdjustPointer(0x43931B, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ss:(byte_72D108+1)[ebp], 0
	injector::AdjustPointer(0x439339, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ebx], 0
	injector::AdjustPointer(0x439357, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ecx], 0
	injector::AdjustPointer(0x439376, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ss:(byte_72D108+1)[ebp], 0
	injector::AdjustPointer(0x43939B, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ebx], 0
	injector::AdjustPointer(0x44F873, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[eax*8], 1
	injector::AdjustPointer(0x44F88D, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[eax*8], 0
	injector::AdjustPointer(0x5086CF, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> cmp     ss:(byte_72D108+1)[ebp], 0
	injector::AdjustPointer(0x50974F, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x1); // -> cmp     ss:(byte_72D108+1)[ebp], 0
	injector::AdjustPointer(0x438E2D, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x2); // -> or      word ptr ds:(byte_72D108+2)[ebx], 0FFFFh
	injector::AdjustPointer(0x44D429, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x2); // -> mov     word ptr ss:(byte_72D108+2)[ebp], ax
	injector::AdjustPointer(0x44D52D, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x2); // -> or      word ptr ds:(byte_72D108+2)[eax], 0FFFFh
	injector::AdjustPointer(0x5086DD, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x2); // -> cmp     word ptr ss:(byte_72D108+2)[ebp], 0
	injector::AdjustPointer(0x508741, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x2); // -> movsx   ecx, word ptr ss:(byte_72D108+2)[ebp]
	injector::AdjustPointer(0x509759, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x2); // -> cmp     word ptr ss:(byte_72D108+2)[ebp], 0
	injector::AdjustPointer(0x5097B8, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x2); // -> movsx   ecx, word ptr ss:(byte_72D108+2)[ebp]
	injector::AdjustPointer(0x438E34, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x4); // -> mov     dword ptr ds:(byte_72D108+4)[ebx], 0
	injector::AdjustPointer(0x44D42F, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x4); // -> fstp    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x44D534, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x4); // -> fstp    dword ptr ds:(byte_72D108+4)[eax]
	injector::AdjustPointer(0x508703, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x4); // -> push    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x508735, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x4); // -> push    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x50977C, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x4); // -> push    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x5097AC, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x4); // -> push    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x438E3E, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x8); // -> mov     dword ptr ds:(byte_72D108+8)[ebx], 0
	injector::AdjustPointer(0x44D442, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x8); // -> fstp    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x44D53C, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x8); // -> fstp    dword ptr ds:(byte_72D108+8)[eax]
	injector::AdjustPointer(0x5086FD, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x8); // -> push    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x50872F, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x8); // -> push    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x509776, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x8); // -> push    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x5097A6, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x8); // -> push    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x438E48, IntroRectangles.get(), 0x72D108, 0x72D108 + 0xC); // -> mov     dword ptr ds:(byte_72D108+0Ch)[ebx], 0
	injector::AdjustPointer(0x44D457, IntroRectangles.get(), 0x72D108, 0x72D108 + 0xC); // -> fstp    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x44D544, IntroRectangles.get(), 0x72D108, 0x72D108 + 0xC); // -> fstp    dword ptr ds:(byte_72D108+0Ch)[eax]
	injector::AdjustPointer(0x5086F7, IntroRectangles.get(), 0x72D108, 0x72D108 + 0xC); // -> push    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x508729, IntroRectangles.get(), 0x72D108, 0x72D108 + 0xC); // -> push    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x509770, IntroRectangles.get(), 0x72D108, 0x72D108 + 0xC); // -> push    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x5097A0, IntroRectangles.get(), 0x72D108, 0x72D108 + 0xC); // -> push    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x438E52, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x10); //  -> mov     dword ptr ds:(byte_72D108+10h)[ebx], 0
	injector::AdjustPointer(0x44D45D, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x10); //  -> fst     dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x44D54A, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x10); //  -> fst     dword ptr ds:(byte_72D108+10h)[eax]
	injector::AdjustPointer(0x5086F1, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x10); //  -> push    dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x508723, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x10); //  -> push    dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x50976A, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x10); //  -> push    dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x50979A, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x10); //  -> push    dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x438E6A, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x14); //  -> mov     ds:(byte_72D108+14h)[ebx], dl
	injector::AdjustPointer(0x438E74, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x15); //  -> mov     ds:(byte_72D108+15h)[ebx], al
	injector::AdjustPointer(0x438E7E, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x16); //  -> mov     ds:(byte_72D108+16h)[ebx], cl
	injector::AdjustPointer(0x438E84, IntroRectangles.get(), 0x72D108, 0x72D108 + 0x17); //  -> mov     ds:(byte_72D108+17h)[ebx], al


	injector::WriteMemory<char>(0x438E8B + 2, NUM_INTRO_RECTANGLES, true);
	injector::WriteMemory<char>(0x439384 + 3, NUM_INTRO_RECTANGLES, true);
	injector::WriteMemory<char>(0x5097D6 + 3, NUM_INTRO_RECTANGLES, true);

	injector::WriteMemory<char>(0x50875F + 3, NUM_INTRO_RECTANGLES, true);
	injector::WriteMemory<char>(0x5097D6 + 3, NUM_INTRO_RECTANGLES, true);

	game.Text.textDrawers = (CTextDrawer *)IntroTextLines.get();
}
#endif

BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		Log::Initialise("cleo.log");
#if CLEO_VC
		LOGL(LOG_PRIORITY_ALWAYS, "GTA VC CLEO v%d.%d.%d Log File", CLEO_VERSION_MAIN, CLEO_VERSION_MAJOR, CLEO_VERSION_MINOR);

		switch (game.Version)
		{
		case GAME_V1_0:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC v%d.%d", 1, 0);
			RwRenderStateSetHook<0x578737>();
			RwRenderStateSetHook<0x578737 - 0x50>();
			PatchArrays();
			break;
		case GAME_V1_1:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC v%d.%d", 1, 1);
			RwRenderStateSetHook<0x578757>();
			RwRenderStateSetHook<0x578757 - 0x50>();
			break;
		case GAME_VSTEAM:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC steam version");
			RwRenderStateSetHook<0x5786A5>();
			RwRenderStateSetHook<0x5786A5 - 0x7E>();
			break;
		case GAME_VSTEAMENC:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC steam version");
			break;
		default:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC unknown version");
			break;
		}
#else
		LOGL(LOG_PRIORITY_ALWAYS, "GTA 3 CLEO v%d.%d.%d Log File", CLEO_VERSION_MAIN, CLEO_VERSION_MAJOR, CLEO_VERSION_MINOR);

		switch (game.Version)
		{
		case GAME_V1_0:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA III v%d.%d", 1, 0);
			CPatch::SetChar(0x566A15, 0);
			CPatch::Nop(0x566A56, 6);
			CPatch::Nop(0x581C44, 2);
			CPatch::Nop(0x581C52, 6);
			*(unsigned char *)0x604B24 = 0;

			RwRenderStateSetHook<0x51F965>();
			PatchArrays();
			break;
		case GAME_V1_1:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA III v%d.%d", 1, 1);
			CPatch::SetChar(0x566B55, 0);
			CPatch::Nop(0x566B96, 6);
			CPatch::Nop(0x581F84, 2);
			CPatch::Nop(0x581F92, 6);
			*(unsigned char *)0x6043EC = 0;

			RwRenderStateSetHook<0x51FB95>();
			break;
		case GAME_VSTEAM:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA III steam version");
			RwRenderStateSetHook<0x51FB25>();
			break;
		default:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA III unknown version");
			break;
		}
#endif
		if (game.GetGameVersion() == GAME_VSTEAMENC)
		{
#if CLEO_VC
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&SteamHandlerDllMain, NULL, 0, NULL);
#endif
		}
		else
		{
			CustomOpcodes::Register();
			CleoPlugins::LoadPlugins();
		}
	}
	else if(fdwReason == DLL_PROCESS_DETACH)
	{
		CleoPlugins::UnloadPlugins();
		Log::Close();
	}
	return TRUE;
}