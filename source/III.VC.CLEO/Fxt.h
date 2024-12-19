#pragma once
#include <cstdint>

typedef uint16_t wchar16_t;

class CustomTextEntry
{
public:
	wchar16_t *m_pText;
	char m_key[8];
	CustomTextEntry *m_pNext;

	CustomTextEntry(char *key, char *text);

	~CustomTextEntry();
};

class CustomText
{
public:
	static void Load();

	static void Unload();

	static wchar_t * __fastcall GetText(int theText, int, char *key);

	static void LoadFxtFile(char *filepath);

	static void Utf8ToUtf16(const char* utf8, wchar16_t* utf16, size_t utf8_len, size_t utf16_len);

	static CustomTextEntry *pCustomTextList;
};