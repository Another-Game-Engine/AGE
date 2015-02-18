#include "Encoding.hpp"

namespace AGE
{
	std::string Encoding::UnicodeToUtf8(const wchar_t *src)
	{
		std::string returnValue;
		const wchar_t *s = src;
		while (*s)
		{
			wchar_t code = *s++;
			if (code < 0x80)
			{
				returnValue += code & 0x7f;
			}
			else if (code < 0x800)
			{
				returnValue += 0xc0 | (code >> 6);
				returnValue += 0x80 | (code & 0x3f);
			}
			else if (code < 0x10000)
			{
				returnValue += 0xe0 | (code >> 12);
				returnValue += 0x80 | ((code >> 6) & 0x3f);
				returnValue += 0x80 | (code & 0x3f);
			}
		}
		return returnValue;
	}

	int Encoding::Utf8ToUnicode(const char *src, wchar_t &code)
	{
		const unsigned char *s = reinterpret_cast<const unsigned char *>(src);
		code = *s;
		if (code > 0xe0)
		{
			code = static_cast<wchar_t>(*s++ & 0x0f) << 12;
			if (*s != '\0')
			{
				code |= static_cast<wchar_t>(*s++ & 0x3f) << 6;
			}
			if (*s != '\0')
			{
				code |= static_cast<wchar_t>(*s++ & 0x3f);
			}
		}
		else if (code > 0xc0)
		{
			code = static_cast<wchar_t>(*s++ & 0x1f) << 6;
			if (*s != '\0')
			{
				code |= static_cast<wchar_t>(*s++ & 0x3f);
			}
		}
		else
		{
			++s;
		}
		return static_cast<int>(s - reinterpret_cast<const unsigned char *>(src));
	}

	int Encoding::Utf8ToUnicode(const char *src, wchar_t *dest, std::size_t size)
	{
		const char *s = src;
		wchar_t *d = dest;
		while (*s && --size)
		{
			s += Utf8ToUnicode(s, *d++);
		}
		*d = '\0';
		return static_cast<int>(d - dest);
	}

	std::size_t Encoding::Utf8Strlen(const char *str)
	{
		const unsigned char *s = reinterpret_cast<const unsigned char *>(str);
		std::size_t length = 0;
		while (*s)
		{
			wchar_t code = *s++;
			if (code > 0xe0)
			{
				if (*s != '\0')
				{
					++s;
				}
				if (*s != '\0')
				{
					++s;
				}
			}
			else if (code > 0xc0)
			{
				if (*s != '\0')
				{
					++s;
				}
			}
			++length;
		}
		return length;
	}

	std::string Encoding::Utf8Substr(const char *str, int pos, int len)
	{
		std::string returnValue;
		const unsigned char *s = reinterpret_cast<const unsigned char *>(str);
		std::size_t length = 0;
		while (*s)
		{
			if (length == static_cast<unsigned int>(pos))
			{
				str = reinterpret_cast<const char *>(s);
			}
			wchar_t code = *s++;
			if (code > 0xe0)
			{
				if (*s != '\0')
				{
					++s;
				}
				if (*s != '\0')
				{
					++s;
				}
			}
			else if (code > 0xc0)
			{
				if (*s != '\0')
				{
					++s;
				}
			}
			++length;
		}
		s = reinterpret_cast<const unsigned char *>(str);
		if (static_cast<std::size_t>(len) == std::string::npos)
		{
			len = static_cast<int>(length)-pos;
		}
		for (int i = 0; i < len; ++i)
		{
			int added = pos + i;
			if (added < 0)
			{
				returnValue += ' ';
			}
			else if (added < static_cast<int>(length))
			{
				wchar_t code = *s++;
				returnValue += static_cast<char>(code);
				if (code > 0xe0)
				{
					if (*s != '\0')
					{
						returnValue += *s++;
					}
					if (*s != '\0')
					{
						returnValue += *s++;
					}
				}
				else if (code > 0xc0)
				{
					if (*s != '\0')
					{
						returnValue += *s++;
					}
				}
			}
			else
			{
				returnValue += ' ';
			}
		}
		return returnValue;
	}
}
