#pragma once

#include <cstddef>
#include <string>

namespace AGE
{
	class Encoding final
	{
	public:
		Encoding(void) = delete;
		Encoding(const Encoding &other) = delete;
		Encoding(Encoding &&other) = delete;
		Encoding &operator=(const Encoding &other) = delete;
		Encoding &operator=(Encoding &&other) = delete;
		~Encoding(void) = delete;

		static std::string UnicodeToUtf8(const wchar_t *src);
		static int Utf8ToUnicode(const char *src, wchar_t &code);
		static int Utf8ToUnicode(const char *src, wchar_t *dest, std::size_t size);
		static std::size_t Utf8Strlen(const char *str);
		static std::string Utf8Substr(const char *str, int pos, int len);
	};
}