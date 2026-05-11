#pragma once
#include <cstdint>

class CustomTextEntry
{
public:
	char m_key[8];
	CustomTextEntry *m_pNext = nullptr;

	// This struct is immediately followed by char16_t text[];

private:
	CustomTextEntry(const char *key);

	// Custom operator new so we allocate space for CustomTextEntry and the text at once
	struct ThisIsntPlacementNewTag {};

	// Tag is needed, so this operator new doesn't look like new(size_t size, size_t align)
	void* operator new(size_t size, ThisIsntPlacementNewTag, size_t textBufferLength)
	{
		return ::operator new(size + (textBufferLength * sizeof(char16_t)));
	}

public:
	static CustomTextEntry* NewTextEntry(const char *key, const char *text, size_t length);
	const wchar_t* GetText() const { return reinterpret_cast<const wchar_t*>(this + 1); }

	void operator delete(void* ptr)
	{
		return ::operator delete(ptr);
	}

	// operator delete matching our operator new
	void operator delete(void* ptr, ThisIsntPlacementNewTag, size_t textBufferLength)
	{
		return ::operator delete(ptr);
	}
};

class CustomText
{
public:
	static void Load();

	static void Unload();

	static const wchar_t * __fastcall GetText(int theText, int, const char *key);

	static void LoadFxtFile(const char *filepath);

	static void Utf8ToUtf16(const char* utf8, char16_t* utf16, size_t utf8_len, size_t utf16_len);

	static CustomTextEntry *pCustomTextList;
};