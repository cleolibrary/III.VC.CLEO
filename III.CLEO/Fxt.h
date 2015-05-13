#pragma once

class CustomTextEntry
{
public:
	wchar_t *m_pText;
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

	static CustomTextEntry *pCustomTextList;
};