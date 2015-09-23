#include <cassert>
#include <algorithm>

#include "Path.hpp"

namespace AGE
{
	std::string Path::DirectoryName(const char *str)
	{
		assert(str != nullptr && "Invalid string");
		const char *s = std::strrchr(str, '/');
		if (s == nullptr)
		{
			s = std::strrchr(str, '\\');
		}
		if (s == nullptr)
		{
			return std::string();
		}
		else
		{
			return std::string(str).substr(0, static_cast<int>(s - str + 1));
		}
	}

	std::string Path::BaseName(const char *str)
	{
		assert(str != nullptr && "Invalid string");
		const char *s = std::strrchr(str, '/');
		if (s == nullptr)
		{
			s = std::strrchr(str, '\\');
		}
		if (s == nullptr)
		{
			return std::string(str);
		}
		else
		{
			return std::string(s + 1);
		}
	}

	std::string Path::PathName(const char *str)
	{
		assert(str != nullptr && "Invalid string");
		std::string returnValue;
		const char *s = str;
		while (true)
		{
			if (*s == '/' && !returnValue.empty() && returnValue[static_cast<int>(returnValue.size()) - 1] == '/')
			{
				++s;
			}
			else if (!std::strncmp(s, "./", 2))
			{
				s += 2;
			}
			else if (!std::strncmp(s, "../", 3))
			{
				s += 3;
				if (returnValue.find('/') != returnValue.rfind('/'))
				{
					returnValue.pop_back();
					std::size_t pos = returnValue.rfind('/');
					if (pos != std::string::npos)
					{
						returnValue.erase(pos + 1, returnValue.size() - pos - 1);
					}
				}
				else
				{
					if (returnValue.size() < 3 || returnValue[1] != ':' || returnValue[2] != '/')
					{
						returnValue.append("../");
					}
				}
			}
			else
			{
				const char *slash = std::strchr(s, '/');
				if (slash == nullptr)
				{
					break;
				}
				returnValue.append(s, slash - s + 1);
				s = slash + 1;
			}
		}
		return returnValue;
	}

	std::string Path::AbsoluteName(const char *path, const char *str)
	{
		assert(path != nullptr && str != nullptr && "Invalid string");
		std::string returnValue;
		const char *s = str;
		if (*s == '\0' || *(s + 1) != ':')
		{
			returnValue = path;
		}
		returnValue += str;
		return returnValue;
	}

	std::string Path::RelativeName(const char *path, const char *str)
	{
		assert(path != nullptr && str != nullptr && "Invalid string");
		const char *p = path;
		const char *s = str;
		while (*p && *p == *s)
		{
			++p;
			++s;
		}
		while (p > path && *p != '/')
		{
			--p;
		}
		while (s > str && *s != '/')
		{
			--s;
		}
		if (*p != '/' || *s != '/')
		{
			return std::string(str);
		}
		std::string returnValue;
		while (*p)
		{
			if (*++p == '/')
			{
				returnValue += "../";
			}
		}
		returnValue += s + 1;
		return returnValue;
	}

	std::string Path::Extension(const char *str, const char *ext)
	{
		assert(str != nullptr && ext != nullptr && "Invalid string");
		std::string returnValue(str);
		std::size_t pos = returnValue.rfind('.');
		if (pos != std::string::npos && static_cast<int>(pos) > static_cast<int>(returnValue.rfind('/'))
			&& static_cast<int>(pos) > static_cast<int>(returnValue.rfind('\\')))
		{
			returnValue = returnValue.substr(static_cast<int>(pos));
			std::transform(returnValue.begin(), returnValue.end(), returnValue.begin(), tolower);
			if (returnValue != ext)
			{
				returnValue.erase(pos, returnValue.size() - pos);
				returnValue += '.';
				returnValue += ext;
			}
		}
		else
		{
			returnValue += '.';
			returnValue += ext;
		}
		return returnValue;
	}
}