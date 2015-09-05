#include <Windows.h>
#include "Game.h"
#include "CPatch.h"
#include "Log.h"
#include "CustomOpcodes.h"
#include "CleoVersion.h"
#include "CleoPlugins.h"
#include <vector>
#include "..\injector\hooking.hpp"

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

std::vector<char>	aScriptTextures;
std::vector<char>	aTextSprites;
std::vector<char>	aTextDrawers;

void PatchArrays()
{
	aScriptTextures.resize(4 * 125);
	aTextSprites.resize(0x18 * 125);
	aTextDrawers.resize(0x34C * 20);

	injector::AdjustPointer(0x438BB9, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > mov     ds:dword_70EA68[ebx], 3EF5C28Fh
	injector::AdjustPointer(0x439106, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > mov     ds:dword_70EA68[ebx], 3EF5C28Fh
	//injector::AdjustPointer(0x43EBEC, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > push    offset dword_70EA68
	//injector::AdjustPointer(0x43ECDD, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > push    offset dword_70EA68
	injector::AdjustPointer(0x449390, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > fstp    ds:dword_70EA68[eax]
	injector::AdjustPointer(0x44943B, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > add     esi, offset dword_70EA68
	injector::AdjustPointer(0x4496BD, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > add     edi, offset dword_70EA68
	injector::AdjustPointer(0x5084DB, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > mov     esi, offset dword_70EA68
	injector::AdjustPointer(0x508526, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > fmul    ss:dword_70EA68[ebp]
	injector::AdjustPointer(0x50955D, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > mov     esi, offset dword_70EA68
	injector::AdjustPointer(0x5095A7, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > fmul    ss:dword_70EA68[ebp]
	injector::AdjustPointer(0x58A3B1, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > add     eax, offset dword_70EA68
	injector::AdjustPointer(0x58A468, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x0); // > add     eax, offset dword_70EA68
	injector::AdjustPointer(0x438BD7, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x4); // > mov     ds:(dword_70EA68+4)[ebx], 3F8F5C29h
	injector::AdjustPointer(0x439124, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x4); // > mov     ds:(dword_70EA68+4)[ebx], 3F8F5C29h
	injector::AdjustPointer(0x44939C, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x4); // > fstp    ds:(dword_70EA68+4)[eax]
	injector::AdjustPointer(0x50850A, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x4); // > fmul    ss:(dword_70EA68+4)[ebp]
	injector::AdjustPointer(0x50958B, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x4); // > fmul    ss:(dword_70EA68+4)[ebp]
	injector::AdjustPointer(0x438BF2, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x8); // > mov     byte ptr ds:(dword_70EA68+8)[ebx], dl
	injector::AdjustPointer(0x43913F, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x8); // > mov     byte ptr ds:(dword_70EA68+8)[ebx], dl
	injector::AdjustPointer(0x508534, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x8); // > mov     edx, ss:(dword_70EA68+8)[ebp]
	injector::AdjustPointer(0x5095B7, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x8); // > mov     ecx, ss:(dword_70EA68+8)[ebp]
	injector::AdjustPointer(0x438BF8, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x9); // > mov     byte ptr ds:(dword_70EA68+9)[ebx], al
	injector::AdjustPointer(0x439145, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x9); // > mov     byte ptr ds:(dword_70EA68+9)[ebx], al
	injector::AdjustPointer(0x438BFE, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xA); // > mov     byte ptr ds:(dword_70EA68+0Ah)[ebx], cl
	injector::AdjustPointer(0x43914B, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xA); // > mov     byte ptr ds:(dword_70EA68+0Ah)[ebx], cl
	injector::AdjustPointer(0x438C20, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xB); // > mov     byte ptr ds:(dword_70EA68+0Bh)[ebx], al
	injector::AdjustPointer(0x43916D, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xB); // > mov     byte ptr ds:(dword_70EA68+0Bh)[ebx], al
	injector::AdjustPointer(0x438C26, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xC); // > mov     byte ptr ds:(dword_70EA68+0Ch)[ebx], 0
	injector::AdjustPointer(0x439173, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xC); // > mov     byte ptr ds:(dword_70EA68+0Ch)[ebx], 0
	injector::AdjustPointer(0x4494A9, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xC); // > mov     byte ptr ds:(dword_70EA68+0Ch)[eax*4], 1
	injector::AdjustPointer(0x4494C0, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xC); // > mov     byte ptr ds:(dword_70EA68+0Ch)[eax*4], 0
	injector::AdjustPointer(0x508550, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xC); // > cmp     byte ptr ss:(dword_70EA68+0Ch)[ebp], 0
	injector::AdjustPointer(0x5095CB, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xC); // > cmp     byte ptr ss:(dword_70EA68+0Ch)[ebp], 0
	injector::AdjustPointer(0x438C34, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xD); // > mov     byte ptr ds:(dword_70EA68+0Dh)[ebx], 0
	injector::AdjustPointer(0x439181, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xD); // > mov     byte ptr ds:(dword_70EA68+0Dh)[ebx], 0
	injector::AdjustPointer(0x44950C, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xD); // > mov     byte ptr ds:(dword_70EA68+0Dh)[eax*4], 1
	injector::AdjustPointer(0x449523, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xD); // > mov     byte ptr ds:(dword_70EA68+0Dh)[eax*4], 0
	injector::AdjustPointer(0x50857C, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xD); // > cmp     byte ptr ss:(dword_70EA68+0Dh)[ebp], 0
	injector::AdjustPointer(0x5095FC, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xD); // > cmp     byte ptr ss:(dword_70EA68+0Dh)[ebp], 0
	injector::AdjustPointer(0x438C3B, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xE); // > mov     byte ptr ds:(dword_70EA68+0Eh)[ebx], 0
	injector::AdjustPointer(0x439188, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xE); // > mov     byte ptr ds:(dword_70EA68+0Eh)[ebx], 0
	injector::AdjustPointer(0x4495FF, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xE); // > mov     byte ptr ds:(dword_70EA68+0Eh)[eax*4], 1
	injector::AdjustPointer(0x449616, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xE); // > mov     byte ptr ds:(dword_70EA68+0Eh)[eax*4], 0
	injector::AdjustPointer(0x5085CE, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xE); // > cmp     byte ptr ss:(dword_70EA68+0Eh)[ebp], 0
	injector::AdjustPointer(0x50964E, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xE); // > cmp     byte ptr ss:(dword_70EA68+0Eh)[ebp], 0
	injector::AdjustPointer(0x438C42, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xF); // > mov     byte ptr ds:(dword_70EA68+0Fh)[ebx], 0
	injector::AdjustPointer(0x43918F, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xF); // > mov     byte ptr ds:(dword_70EA68+0Fh)[ebx], 0
	injector::AdjustPointer(0x44972B, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xF); // > mov     byte ptr ds:(dword_70EA68+0Fh)[eax*4], 1
	injector::AdjustPointer(0x449742, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xF); // > mov     byte ptr ds:(dword_70EA68+0Fh)[eax*4], 0
	injector::AdjustPointer(0x508601, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xF); // > cmp     byte ptr ss:(dword_70EA68+0Fh)[ebp], 0
	injector::AdjustPointer(0x50967B, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0xF); // > cmp     byte ptr ss:(dword_70EA68+0Fh)[ebp], 0
	injector::AdjustPointer(0x438C49, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x10); // -> mov     ds:(dword_70EA68+10h)[ebx], 43360000h
	injector::AdjustPointer(0x439196, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x10); // -> mov     ds:(dword_70EA68+10h)[ebx], 43360000h
	injector::AdjustPointer(0x449573, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x10); // -> fstp    ds:(dword_70EA68+10h)[eax*4]
	injector::AdjustPointer(0x5085A4, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x10); // -> fmul    ss:(dword_70EA68+10h)[ebp]
	injector::AdjustPointer(0x509624, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x10); // -> fmul    ss:(dword_70EA68+10h)[ebp]
	injector::AdjustPointer(0x438C53, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x14); // -> mov     ds:(dword_70EA68+14h)[ebx], 44200000h
	injector::AdjustPointer(0x4391A0, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x14); // -> mov     ds:(dword_70EA68+14h)[ebx], 44200000h
	injector::AdjustPointer(0x4495BB, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x14); // -> fstp    ds:(dword_70EA68+14h)[eax*4]
	injector::AdjustPointer(0x5085C0, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x14); // -> fmul    ss:(dword_70EA68+14h)[ebp]
	injector::AdjustPointer(0x509640, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x14); // -> fmul    ss:(dword_70EA68+14h)[ebp]
	injector::AdjustPointer(0x438C6E, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x18); // -> mov     byte ptr ds:(dword_70EA68+18h)[ebx], dl
	injector::AdjustPointer(0x4391BB, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x18); // -> mov     byte ptr ds:(dword_70EA68+18h)[ebx], dl
	injector::AdjustPointer(0x5085E7, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x18); // -> mov     ecx, ss:(dword_70EA68+18h)[ebp]
	injector::AdjustPointer(0x509667, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x18); // -> mov     edx, ss:(dword_70EA68+18h)[ebp]
	injector::AdjustPointer(0x438C76, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x19); // -> mov     byte ptr ds:(dword_70EA68+19h)[ebx], al
	injector::AdjustPointer(0x4391C1, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x19); // -> mov     byte ptr ds:(dword_70EA68+19h)[ebx], al
	injector::AdjustPointer(0x438C80, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1A); // -> mov     byte ptr ds:(dword_70EA68+1Ah)[ebx], cl
	injector::AdjustPointer(0x4391CB, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1A); // -> mov     byte ptr ds:(dword_70EA68+1Ah)[ebx], cl
	injector::AdjustPointer(0x438C89, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1B); // -> mov     byte ptr ds:(dword_70EA68+1Bh)[ebx], al
	injector::AdjustPointer(0x4391D1, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1B); // -> mov     byte ptr ds:(dword_70EA68+1Bh)[ebx], al
	injector::AdjustPointer(0x438C91, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1C); // -> mov     byte ptr ds:(dword_70EA68+1Ch)[ebx], 1
	injector::AdjustPointer(0x4391D9, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1C); // -> mov     byte ptr ds:(dword_70EA68+1Ch)[ebx], 1
	injector::AdjustPointer(0x44978E, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1C); // -> mov     byte ptr ds:(dword_70EA68+1Ch)[eax*4], 1
	injector::AdjustPointer(0x4497A5, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1C); // -> mov     byte ptr ds:(dword_70EA68+1Ch)[eax*4], 0
	injector::AdjustPointer(0x508617, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1C); // -> cmp     byte ptr ss:(dword_70EA68+1Ch)[ebp], 0
	injector::AdjustPointer(0x509697, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1C); // -> cmp     byte ptr ss:(dword_70EA68+1Ch)[ebp], 0
	injector::AdjustPointer(0x438C98, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1D); // -> mov     byte ptr ds:(dword_70EA68+1Dh)[ebx], 0
	injector::AdjustPointer(0x4391E2, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1D); // -> mov     byte ptr ds:(dword_70EA68+1Dh)[ebx], 0
	injector::AdjustPointer(0x44F7B6, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1D); // -> mov     byte ptr ds:(dword_70EA68+1Dh)[eax*4], 1
	injector::AdjustPointer(0x44F7D0, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1D); // -> mov     byte ptr ds:(dword_70EA68+1Dh)[eax*4], 0
	injector::AdjustPointer(0x5084F0, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1D); // -> cmp     byte ptr ss:(dword_70EA68+1Dh)[ebp], 0
	injector::AdjustPointer(0x509571, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1D); // -> cmp     byte ptr ss:(dword_70EA68+1Dh)[ebp], 0
	injector::AdjustPointer(0x438C2D, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1E); // -> mov     byte ptr ds:(dword_70EA68+1Eh)[ebx], 0
	injector::AdjustPointer(0x43917A, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1E); // -> mov     byte ptr ds:(dword_70EA68+1Eh)[ebx], 0
	injector::AdjustPointer(0x44F8CD, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1E); // -> mov     byte ptr ds:(dword_70EA68+1Eh)[eax*4], 1
	injector::AdjustPointer(0x44F8E4, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1E); // -> mov     byte ptr ds:(dword_70EA68+1Eh)[eax*4], 0
	injector::AdjustPointer(0x508567, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1E); // -> cmp     byte ptr ss:(dword_70EA68+1Eh)[ebp], 0
	injector::AdjustPointer(0x5095E7, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x1E); // -> cmp     byte ptr ss:(dword_70EA68+1Eh)[ebp], 0
	injector::AdjustPointer(0x438C9F, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x20); // -> mov     ds:(dword_70EA68+20h)[ebx], 2
	injector::AdjustPointer(0x4391E9, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x20); // -> mov     ds:(dword_70EA68+20h)[ebx], 2
	injector::AdjustPointer(0x4497F4, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x20); // -> mov     ds:(dword_70EA68+20h)[ebx*4], eax
	injector::AdjustPointer(0x50862D, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x20); // -> mov     ax, word ptr ss:(dword_70EA68+20h)[ebp]
	injector::AdjustPointer(0x5096AD, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x20); // -> mov     ax, word ptr ss:(dword_70EA68+20h)[ebp]
	injector::AdjustPointer(0x438CA9, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x24); // -> mov     ds:(dword_70EA68+24h)[ebx], 0
	injector::AdjustPointer(0x4391F3, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x24); // -> mov     ds:(dword_70EA68+24h)[ebx], 0
	injector::AdjustPointer(0x44923F, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x24); // -> fstp    ds:(dword_70EA68+24h)[ebx]
	injector::AdjustPointer(0x50868B, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x24); // -> fsub    ss:(dword_70EA68+24h)[ebp]
	injector::AdjustPointer(0x50970A, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x24); // -> fsub    ss:(dword_70EA68+24h)[ebp]
	injector::AdjustPointer(0x58A386, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x24); // -> fstp    ds:(dword_70EA68+24h)[eax]
	injector::AdjustPointer(0x58A437, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x24); // -> fstp    ds:(dword_70EA68+24h)[eax]
	injector::AdjustPointer(0x438CB3, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x28); // -> mov     ds:(dword_70EA68+28h)[ebx], 0
	injector::AdjustPointer(0x4391FD, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x28); // -> mov     ds:(dword_70EA68+28h)[ebx], 0
	injector::AdjustPointer(0x44924B, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x28); // -> fstp    ds:(dword_70EA68+28h)[ebx]
	injector::AdjustPointer(0x50865D, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x28); // -> fsub    ss:(dword_70EA68+28h)[ebp]
	injector::AdjustPointer(0x5096DD, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x28); // -> fsub    ss:(dword_70EA68+28h)[ebp]
	injector::AdjustPointer(0x58A392, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x28); // -> fstp    ds:(dword_70EA68+28h)[eax]
	injector::AdjustPointer(0x58A443, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x28); // -> fstp    ds:(dword_70EA68+28h)[eax]
	injector::AdjustPointer(0x438CC7, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2C); // -> movq    qword ptr ds:(dword_70EA68+2Ch)[eax*2], mm1
	injector::AdjustPointer(0x438D24, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[edx*2], 0
	injector::AdjustPointer(0x43920C, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[edx*2], 0
	injector::AdjustPointer(0x43927B, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[eax*2], 0
	injector::AdjustPointer(0x449288, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[esi*2], di
	injector::AdjustPointer(0x44930A, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[ebx*2], di
	injector::AdjustPointer(0x449328, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2C); // -> mov     word ptr ds:(dword_70EA68+2Ch)[ebp*2], 0
	injector::AdjustPointer(0x5084E3, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2C); // -> cmp     word ptr ss:(dword_70EA68+2Ch)[ebp], 0
	injector::AdjustPointer(0x509564, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2C); // -> cmp     word ptr ss:(dword_70EA68+2Ch)[ebp], 0
	injector::AdjustPointer(0x438D2E, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2E); // -> mov     word ptr ds:(dword_70EA68+2Eh)[edx*2], 0
	injector::AdjustPointer(0x439216, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2E); // -> mov     word ptr ds:(dword_70EA68+2Eh)[edx*2], 0
	injector::AdjustPointer(0x439285, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2E); // -> mov     word ptr ds:(dword_70EA68+2Eh)[eax*2], 0
	injector::AdjustPointer(0x449295, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x2E); // -> mov     word ptr ds:(dword_70EA68+2Eh)[esi*2], bx
	injector::AdjustPointer(0x438D38, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x30); // -> mov     word ptr ds:(dword_70EA68+30h)[edx*2], 0
	injector::AdjustPointer(0x439220, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x30); // -> mov     word ptr ds:(dword_70EA68+30h)[edx*2], 0
	injector::AdjustPointer(0x43928F, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x30); // -> mov     word ptr ds:(dword_70EA68+30h)[eax*2], 0
	injector::AdjustPointer(0x4492A2, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x30); // -> mov     word ptr ds:(dword_70EA68+30h)[esi*2], di
	injector::AdjustPointer(0x438D42, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x32); // -> mov     word ptr ds:(dword_70EA68+32h)[edx*2], 0
	injector::AdjustPointer(0x43922A, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x32); // -> mov     word ptr ds:(dword_70EA68+32h)[edx*2], 0
	injector::AdjustPointer(0x439299, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x32); // -> mov     word ptr ds:(dword_70EA68+32h)[eax*2], 0
	injector::AdjustPointer(0x4492AF, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x32); // -> mov     word ptr ds:(dword_70EA68+32h)[esi*2], bx
	injector::AdjustPointer(0x438CCF, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x34); // -> movq    qword ptr ds:(dword_70EA68+34h)[eax*2], mm1
	injector::AdjustPointer(0x438D4C, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x34); // -> mov     word ptr ds:(dword_70EA68+34h)[edx*2], 0
	injector::AdjustPointer(0x439234, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x34); // -> mov     word ptr ds:(dword_70EA68+34h)[edx*2], 0
	injector::AdjustPointer(0x4492BC, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x34); // -> mov     word ptr ds:(dword_70EA68+34h)[esi*2], di
	injector::AdjustPointer(0x438D56, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x36); // -> mov     word ptr ds:(dword_70EA68+36h)[edx*2], 0
	injector::AdjustPointer(0x43923E, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x36); // -> mov     word ptr ds:(dword_70EA68+36h)[edx*2], 0
	injector::AdjustPointer(0x4492C9, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x36); // -> mov     word ptr ds:(dword_70EA68+36h)[esi*2], bx
	injector::AdjustPointer(0x438D60, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x38); // -> mov     word ptr ds:(dword_70EA68+38h)[edx*2], 0
	injector::AdjustPointer(0x439248, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x38); // -> mov     word ptr ds:(dword_70EA68+38h)[edx*2], 0
	injector::AdjustPointer(0x4492D6, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x38); // -> mov     word ptr ds:(dword_70EA68+38h)[esi*2], di
	injector::AdjustPointer(0x438D6A, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x3A); // -> mov     word ptr ds:(dword_70EA68+3Ah)[edx*2], 0
	injector::AdjustPointer(0x439252, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x3A); // -> mov     word ptr ds:(dword_70EA68+3Ah)[edx*2], 0
	injector::AdjustPointer(0x4492E6, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x3A); // -> mov     word ptr ds:(dword_70EA68+3Ah)[esi*2], bx
	injector::AdjustPointer(0x438CD7, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x3C); // -> movq    qword ptr ds:(dword_70EA68+3Ch)[eax*2], mm1
	injector::AdjustPointer(0x438D74, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x3C); // -> mov     word ptr ds:(dword_70EA68+3Ch)[edx*2], 0
	injector::AdjustPointer(0x438D7E, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x3E); // -> mov     word ptr ds:(dword_70EA68+3Eh)[edx*2], 0
	injector::AdjustPointer(0x438D88, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x40); // -> mov     word ptr ds:(dword_70EA68+40h)[edx*2], 0
	injector::AdjustPointer(0x438D92, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x42); // -> mov     word ptr ds:(dword_70EA68+42h)[edx*2], 0
	injector::AdjustPointer(0x438CDF, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x44); // -> movq    qword ptr ds:(dword_70EA68+44h)[eax*2], mm1
	injector::AdjustPointer(0x438D9C, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x44); // -> mov     word ptr ds:(dword_70EA68+44h)[edx*2], 0
	injector::AdjustPointer(0x438DA6, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x46); // -> mov     word ptr ds:(dword_70EA68+46h)[edx*2], 0
	injector::AdjustPointer(0x438DB0, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x48); // -> mov     word ptr ds:(dword_70EA68+48h)[edx*2], 0
	injector::AdjustPointer(0x438DBA, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x4A); // -> mov     word ptr ds:(dword_70EA68+4Ah)[edx*2], 0
	injector::AdjustPointer(0x438CE7, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x4C); // -> movq    qword ptr ds:(dword_70EA68+4Ch)[eax*2], mm1
	injector::AdjustPointer(0x438DC4, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x4C); // -> mov     word ptr ds:(dword_70EA68+4Ch)[edx*2], 0
	injector::AdjustPointer(0x438DCE, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x4E); // -> mov     word ptr ds:(dword_70EA68+4Eh)[edx*2], 0
	injector::AdjustPointer(0x438DD8, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x50); // -> mov     word ptr ds:(dword_70EA68+50h)[edx*2], 0
	injector::AdjustPointer(0x438DE2, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x52); // -> mov     word ptr ds:(dword_70EA68+52h)[edx*2], 0
	injector::AdjustPointer(0x438CEF, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x54); // -> movq    qword ptr ds:(dword_70EA68+54h)[eax*2], mm1
	injector::AdjustPointer(0x438CF7, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x5C); // -> movq    qword ptr ds:(dword_70EA68+5Ch)[eax*2], mm1
	injector::AdjustPointer(0x438CFF, &aTextDrawers[0], 0x70EA68, 0x70EA68 + 0x64); // -> movq    qword ptr ds:(dword_70EA68+64h)[eax*2], mm1


	injector::WriteMemory<char>(0x438D1D + 2, 16, true);
	injector::WriteMemory<char>(0x5086AD + 3, 16, true);
	injector::WriteMemory<char>(0x439273 + 3, 16, true);
	injector::WriteMemory<char>(0x50972C + 3, 16, true);


	//injector::AdjustPointer(0x43EC4A, &aScriptTextures[0], 0x72B090, 0x72B090 + 0x0); // -> push    offset unk_72B090
	//injector::AdjustPointer(0x43EC7A, &aScriptTextures[0], 0x72B090, 0x72B090 + 0x0); // -> push    offset unk_72B090
	injector::AdjustPointer(0x44D65B, &aScriptTextures[0], 0x72B090, 0x72B090 + 0x0); // -> add     ecx, offset unk_72B090
	injector::AdjustPointer(0x44D709, &aScriptTextures[0], 0x72B090, 0x72B090 + 0x0); // -> mov     esi, offset unk_72B090
	injector::AdjustPointer(0x50874F, &aScriptTextures[0], 0x72B090, 0x72B090 + 0x0); // -> add     ecx, offset unk_72B090
	injector::AdjustPointer(0x5097C6, &aScriptTextures[0], 0x72B090, 0x72B090 + 0x0); // -> add     ecx, offset unk_72B090

	injector::WriteMemory<char>(0x50875F + 3, 121, true);
	injector::WriteMemory<char>(0x5097D6 + 3, 121, true);


	injector::AdjustPointer(0x438E0A, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ebx], 0
	injector::AdjustPointer(0x4392B6, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[eax], 0
	injector::AdjustPointer(0x4392D7, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ebx], 0
	injector::AdjustPointer(0x4392F5, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ecx], 0
	injector::AdjustPointer(0x439314, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ss:byte_72D108[ebp], 0
	injector::AdjustPointer(0x439332, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ebx], 0
	injector::AdjustPointer(0x439350, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ecx], 0
	injector::AdjustPointer(0x43936F, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ss:byte_72D108[ebp], 0
	injector::AdjustPointer(0x439394, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[ebx], 0
	//injector::AdjustPointer(0x43EC1B, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> push    offset byte_72D108
	//injector::AdjustPointer(0x43EC9A, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> push    offset byte_72D108
	injector::AdjustPointer(0x44D421, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ss:byte_72D108[ebp], 1
	injector::AdjustPointer(0x44D48D, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> add     edi, offset byte_72D108
	injector::AdjustPointer(0x44D525, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     ds:byte_72D108[eax], 1
	injector::AdjustPointer(0x44D58B, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> add     edi, offset byte_72D108
	injector::AdjustPointer(0x5086BC, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     esi, offset byte_72D108
	injector::AdjustPointer(0x5086C2, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> cmp     ss:byte_72D108[ebp], 0
	injector::AdjustPointer(0x50973B, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> mov     esi, offset byte_72D108
	injector::AdjustPointer(0x509742, &aTextSprites[0], 0x72D108, 0x72D108 + 0x0); // -> cmp     ss:byte_72D108[ebp], 0
	injector::AdjustPointer(0x438E25, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ebx], 0
	injector::AdjustPointer(0x4392BE, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[eax], 0
	injector::AdjustPointer(0x4392DE, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ebx], 0
	injector::AdjustPointer(0x4392FC, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ecx], 0
	injector::AdjustPointer(0x43931B, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ss:(byte_72D108+1)[ebp], 0
	injector::AdjustPointer(0x439339, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ebx], 0
	injector::AdjustPointer(0x439357, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ecx], 0
	injector::AdjustPointer(0x439376, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ss:(byte_72D108+1)[ebp], 0
	injector::AdjustPointer(0x43939B, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[ebx], 0
	injector::AdjustPointer(0x44F873, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[eax*8], 1
	injector::AdjustPointer(0x44F88D, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> mov     ds:(byte_72D108+1)[eax*8], 0
	injector::AdjustPointer(0x5086CF, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> cmp     ss:(byte_72D108+1)[ebp], 0
	injector::AdjustPointer(0x50974F, &aTextSprites[0], 0x72D108, 0x72D108 + 0x1); // -> cmp     ss:(byte_72D108+1)[ebp], 0
	injector::AdjustPointer(0x438E2D, &aTextSprites[0], 0x72D108, 0x72D108 + 0x2); // -> or      word ptr ds:(byte_72D108+2)[ebx], 0FFFFh
	injector::AdjustPointer(0x44D429, &aTextSprites[0], 0x72D108, 0x72D108 + 0x2); // -> mov     word ptr ss:(byte_72D108+2)[ebp], ax
	injector::AdjustPointer(0x44D52D, &aTextSprites[0], 0x72D108, 0x72D108 + 0x2); // -> or      word ptr ds:(byte_72D108+2)[eax], 0FFFFh
	injector::AdjustPointer(0x5086DD, &aTextSprites[0], 0x72D108, 0x72D108 + 0x2); // -> cmp     word ptr ss:(byte_72D108+2)[ebp], 0
	injector::AdjustPointer(0x508741, &aTextSprites[0], 0x72D108, 0x72D108 + 0x2); // -> movsx   ecx, word ptr ss:(byte_72D108+2)[ebp]
	injector::AdjustPointer(0x509759, &aTextSprites[0], 0x72D108, 0x72D108 + 0x2); // -> cmp     word ptr ss:(byte_72D108+2)[ebp], 0
	injector::AdjustPointer(0x5097B8, &aTextSprites[0], 0x72D108, 0x72D108 + 0x2); // -> movsx   ecx, word ptr ss:(byte_72D108+2)[ebp]
	injector::AdjustPointer(0x438E34, &aTextSprites[0], 0x72D108, 0x72D108 + 0x4); // -> mov     dword ptr ds:(byte_72D108+4)[ebx], 0
	injector::AdjustPointer(0x44D42F, &aTextSprites[0], 0x72D108, 0x72D108 + 0x4); // -> fstp    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x44D534, &aTextSprites[0], 0x72D108, 0x72D108 + 0x4); // -> fstp    dword ptr ds:(byte_72D108+4)[eax]
	injector::AdjustPointer(0x508703, &aTextSprites[0], 0x72D108, 0x72D108 + 0x4); // -> push    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x508735, &aTextSprites[0], 0x72D108, 0x72D108 + 0x4); // -> push    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x50977C, &aTextSprites[0], 0x72D108, 0x72D108 + 0x4); // -> push    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x5097AC, &aTextSprites[0], 0x72D108, 0x72D108 + 0x4); // -> push    dword ptr ss:(byte_72D108+4)[ebp]
	injector::AdjustPointer(0x438E3E, &aTextSprites[0], 0x72D108, 0x72D108 + 0x8); // -> mov     dword ptr ds:(byte_72D108+8)[ebx], 0
	injector::AdjustPointer(0x44D442, &aTextSprites[0], 0x72D108, 0x72D108 + 0x8); // -> fstp    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x44D53C, &aTextSprites[0], 0x72D108, 0x72D108 + 0x8); // -> fstp    dword ptr ds:(byte_72D108+8)[eax]
	injector::AdjustPointer(0x5086FD, &aTextSprites[0], 0x72D108, 0x72D108 + 0x8); // -> push    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x50872F, &aTextSprites[0], 0x72D108, 0x72D108 + 0x8); // -> push    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x509776, &aTextSprites[0], 0x72D108, 0x72D108 + 0x8); // -> push    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x5097A6, &aTextSprites[0], 0x72D108, 0x72D108 + 0x8); // -> push    dword ptr ss:(byte_72D108+8)[ebp]
	injector::AdjustPointer(0x438E48, &aTextSprites[0], 0x72D108, 0x72D108 + 0xC); // -> mov     dword ptr ds:(byte_72D108+0Ch)[ebx], 0
	injector::AdjustPointer(0x44D457, &aTextSprites[0], 0x72D108, 0x72D108 + 0xC); // -> fstp    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x44D544, &aTextSprites[0], 0x72D108, 0x72D108 + 0xC); // -> fstp    dword ptr ds:(byte_72D108+0Ch)[eax]
	injector::AdjustPointer(0x5086F7, &aTextSprites[0], 0x72D108, 0x72D108 + 0xC); // -> push    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x508729, &aTextSprites[0], 0x72D108, 0x72D108 + 0xC); // -> push    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x509770, &aTextSprites[0], 0x72D108, 0x72D108 + 0xC); // -> push    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x5097A0, &aTextSprites[0], 0x72D108, 0x72D108 + 0xC); // -> push    dword ptr ss:(byte_72D108+0Ch)[ebp]
	injector::AdjustPointer(0x438E52, &aTextSprites[0], 0x72D108, 0x72D108 + 0x10); //  -> mov     dword ptr ds:(byte_72D108+10h)[ebx], 0
	injector::AdjustPointer(0x44D45D, &aTextSprites[0], 0x72D108, 0x72D108 + 0x10); //  -> fst     dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x44D54A, &aTextSprites[0], 0x72D108, 0x72D108 + 0x10); //  -> fst     dword ptr ds:(byte_72D108+10h)[eax]
	injector::AdjustPointer(0x5086F1, &aTextSprites[0], 0x72D108, 0x72D108 + 0x10); //  -> push    dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x508723, &aTextSprites[0], 0x72D108, 0x72D108 + 0x10); //  -> push    dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x50976A, &aTextSprites[0], 0x72D108, 0x72D108 + 0x10); //  -> push    dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x50979A, &aTextSprites[0], 0x72D108, 0x72D108 + 0x10); //  -> push    dword ptr ss:(byte_72D108+10h)[ebp]
	injector::AdjustPointer(0x438E6A, &aTextSprites[0], 0x72D108, 0x72D108 + 0x14); //  -> mov     ds:(byte_72D108+14h)[ebx], dl
	injector::AdjustPointer(0x438E74, &aTextSprites[0], 0x72D108, 0x72D108 + 0x15); //  -> mov     ds:(byte_72D108+15h)[ebx], al
	injector::AdjustPointer(0x438E7E, &aTextSprites[0], 0x72D108, 0x72D108 + 0x16); //  -> mov     ds:(byte_72D108+16h)[ebx], cl
	injector::AdjustPointer(0x438E84, &aTextSprites[0], 0x72D108, 0x72D108 + 0x17); //  -> mov     ds:(byte_72D108+17h)[ebx], al


	injector::WriteMemory<char>(0x438E8B + 2, 121, true);
	injector::WriteMemory<char>(0x439384 + 3, 121, true);
	injector::WriteMemory<char>(0x5097D6 + 3, 121, true);

	game.Text.textDrawers = (CTextDrawer *)&aTextDrawers[0];
}

BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		Log::Initialise("cleo.log");
		LOGL(LOG_PRIORITY_ALWAYS, "GTA 3 CLEO v%d.%d.%d.%d Log File", CLEO_VERSION_MAIN, CLEO_VERSION_MAJOR, 
			CLEO_VERSION_MINOR, CLEO_VERSION_BINARY);

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

		CustomOpcodes::Register();
		CleoPlugins::LoadPlugins();
	}
	else if(fdwReason == DLL_PROCESS_DETACH)
	{
		CleoPlugins::UnloadPlugins();
		Log::Close();
	}
	return TRUE;
}