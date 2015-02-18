#include <cstdarg>

#include "Endianness.hpp"
#include "Stream.hpp"

namespace AGE
{
	namespace Engine
	{
		Stream::operator bool(void) const
		{
			return !error;
		}

		std::size_t Stream::readToken(char *str, std::size_t size) const
		{
			if (str)
			{
				char c = '\0';
				char *s = str;
				while (static_cast<std::size_t>(s - str) < size)
				{
					if (read(&c, sizeof(char), 1) != 1)
					{
						break;
					}
					if (std::strchr(" \t\n\r", c))
					{
						continue;
					}
					if (c != '"')
					{
						*s++ = c;
					}
					break;
				}
				if (c == '"')
				{
					while (static_cast<std::size_t>(s - str) < size)
					{
						if (read(&c, sizeof(char), 1) != 1)
						{
							break;
						}
						if (c == '"' && *(s - 1) != '\\')
						{
							break;
						}
						*s++ = c;
					}
				}
				else
				{
					while (static_cast<std::size_t>(s - str) < size)
					{
						if (read(&c, sizeof(char), 1) != 1)
						{
							break;
						}
						if (std::strchr(" \t\n\r", c))
						{
							break;
						}
						*s++ = c;
					}
				}
				*s = '\0';
				return static_cast<std::size_t>(s - str);
			}
			else
			{
				return 0;
			}
		}

		std::size_t Stream::readLine(char *str, std::size_t size) const
		{
			if (str)
			{
				char c = '\0';
				char *s = str;
				while (static_cast<std::size_t>(s - str) < size)
				{
					if (read(&c, sizeof(char), 1) != 1)
					{
						break;
					}
					if (c != '\r')
					{
						*s++ = c;
					}
					if (c == '\n')
					{
						break;
					}
				}
				*s = '\0';
				return static_cast<std::size_t>(s - str);
			}
			else
			{
				return 0;
			}
		}

		std::string Stream::readToken(void) const
		{
			char buffer[BufferSize] = { '\0' };
			readToken(buffer, sizeof(buffer) - 1);
			return buffer[0] ? std::string(buffer) : std::string();
		}

		std::string Stream::readLine(void) const
		{
			char buffer[BufferSize] = { '\0' };
			readLine(buffer, sizeof(buffer) - 1);
			return buffer[0] ? std::string(buffer) : std::string();
		}

		bool Stream::puts(const char *str) const
		{
			if (str)
			{
				const std::size_t length = std::strlen(str);
				return length > 0 ? write(str, sizeof(char), length) == length : false;
			}
			else
			{
				return false;
			}
		}

		bool Stream::puts(const std::string &str) const
		{
			const std::size_t length = str.size();
			return length > 0 ? write(&str[0], sizeof(char), length) == length : false;
		}

		bool Stream::vprintf(const char *format, va_list argptr) const
		{
			if (format)
			{
				char buffer[BufferSize];
				_vsnprintf_s(buffer, sizeof(buffer), format, argptr);
				buffer[sizeof(buffer) - 1] = '\0';
				return puts(buffer);
			}
			else
			{
				return false;
			}
		}

		bool Stream::printf(const char *format, ...) const
		{
			if (format)
			{
				va_list argptr;
				va_start(argptr, format);
				const bool returnValue = vprintf(format, argptr);
				va_end(argptr);
				return returnValue;
			}
			else
			{
				return false;
			}
		}

		bool Stream::read(std::string &value) const
		{
			unsigned int length;
			if (read(length))
			{
				if (length)
				{
					value.resize(length);
					const bool returnValue = read(&value[0], length);
					error |= !returnValue;
					return returnValue;
				}
				else
				{
					return true;
				}
			}
			else
			{
				error = true;
				return false;
			}
		}

		bool Stream::read(std::string *str, std::size_t size) const
		{
			if (str)
			{
				unsigned int length;
				if (!read(length) || length == 0 || length > size)
				{
					error = true;
					return false;
				}
				for (unsigned int i = 0; i < length; ++i)
				{
					if (!read(str[i]))
					{
						error = true;
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		bool Stream::write(const std::string &value) const
		{
			if (!value.empty())
			{
				const bool returnValue = write(value.size()) && write(&value[0], value.size());
				error |= !returnValue;
				return returnValue;
			}
			else
			{
				return true;
			}
		}

		bool Stream::write(const std::string *value, std::size_t size) const
		{
			if (value)
			{
				if (size)
				{
					if (write(size))
					{
						for (unsigned int i = 0; i < size; ++i)
						{
							if (!write(value[i]))
							{
								error = true;
								return false;
							}
						}
						return true;
					}
					else
					{
						error = true;
						return false;
					}
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}

		bool Stream::readLittle(std::string &value) const
		{
			unsigned int length;
			if (readLittle(length))
			{
				if (length)
				{
					value.resize(length);
					bool returnValue = readLittle(&value[0], length);
					error |= !returnValue;
					return returnValue;
				}
				else
				{
					return true;
				}
			}
			else
			{
				error = true;
				return false;
			}
		}

		bool Stream::readLittle(std::string *str, std::size_t size) const
		{
			if (str)
			{
				unsigned int length;
				if (!readLittle(length) || length == 0 || length > size)
				{
					error = true;
					return false;
				}
				for (unsigned int i = 0; i < length; ++i)
				{
					if (!readLittle(str[i]))
					{
						error = true;
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		bool Stream::writeLittle(const std::string &value) const
		{
			if (!value.empty())
			{
				bool returnValue = writeLittle(value.size()) && writeLittle(&value[0], value.size());
				error |= !returnValue;
				return returnValue;
			}
			else
			{
				return true;
			}
		}

		bool Stream::writeLittle(const std::string *value, std::size_t size) const
		{
			if (value)
			{
				if (size)
				{
					if (writeLittle(size))
					{
						for (unsigned int i = 0; i < size; ++i)
						{
							if (!writeLittle(value[i]))
							{
								error = true;
								return false;
							}
						}
						return true;
					}
					else
					{
						error = true;
						return false;
					}
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}

		bool Stream::readBig(std::string &value) const
		{
			unsigned int length;
			if (readBig(length))
			{
				if (length)
				{
					value.resize(length);
					bool returnValue = readBig(&value[0], length);
					error |= !returnValue;
					return returnValue;
				}
				else
				{
					return true;
				}
			}
			else
			{
				error = true;
				return false;
			}
		}

		bool Stream::readBig(std::string *str, std::size_t size) const
		{
			if (str)
			{
				unsigned int length;
				if (!readBig(length) || length == 0 || length > size)
				{
					error = true;
					return false;
				}
				for (unsigned int i = 0; i < length; ++i)
				{
					if (!readBig(str[i]))
					{
						error = true;
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		bool Stream::writeBig(const std::string &value) const
		{
			if (!value.empty())
			{
				bool returnValue = writeBig(value.size()) && writeBig(&value[0], value.size());
				error |= !returnValue;
				return returnValue;
			}
			else
			{
				return true;
			}
		}

		bool Stream::writeBig(const std::string *value, std::size_t size) const
		{
			if (value)
			{
				if (size)
				{
					if (writeBig(size))
					{
						for (unsigned int i = 0; i < size; ++i)
						{
							if (!writeBig(value[i]))
							{
								error = true;
								return false;
							}
						}
						return true;
					}
					else
					{
						error = true;
						return false;
					}
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}

		const Stream &Stream::operator<<(bool value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(char value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(std::int8_t value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(std::uint8_t value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(std::int16_t value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(std::uint16_t value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(std::int32_t value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(std::uint32_t value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(std::int64_t value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(std::uint64_t value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(float value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(double value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(long double value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator<<(const char *value) const
		{
			if (value)
			{
				std::size_t size = std::strlen(value);
				write(size) && write(value, size);
			}
			return *this;
		}

		const Stream &Stream::operator<<(const std::string &value) const
		{
			write(value);
			return *this;
		}

		const Stream &Stream::operator>>(bool &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(char &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(std::int8_t &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(std::uint8_t &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(std::int16_t &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(std::uint16_t &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(std::int32_t &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(std::uint32_t &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(std::int64_t &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(std::uint64_t &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(float &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(double &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(long double &value) const
		{
			read(value);
			return *this;
		}

		const Stream &Stream::operator>>(char *value) const
		{
			if (value)
			{
				std::size_t size;
				if (read(size) && read(value, size))
				{
					value[size] = '\0';
				}
			}
			return *this;
		}

		const Stream &Stream::operator>>(std::string &value) const
		{
			read(value);
			return *this;
		}

#define DECLARE_READ_DIRECT(Type, Name) \
    bool Stream::Name(Type &dest) const \
    { \
        const bool returnValue = read(&dest, sizeof(Type), 1) == 1; \
        error |= !returnValue; \
        return returnValue; \
    }

#define DECLARE_READ_REVERSE(Type, Name) \
    bool Stream::Name(Type &dest) const \
    { \
	    Type buffer = 0; \
	    const bool returnValue = read(&buffer, sizeof(Type), 1) == 1; \
	    error |= !returnValue; \
	    unsigned char *s = reinterpret_cast<unsigned char *>(&buffer); \
	    unsigned char *d = reinterpret_cast<unsigned char *>(&dest) + sizeof(Type) - 1; \
	    for (unsigned int i = 0; i < sizeof(Type); ++i) \
        { \
	        *d-- = *s++; \
        } \
	    return returnValue; \
    }

#define DECLARE_READ_ARRAY_DIRECT(Type, Name) \
	bool Stream::Name(Type *dest, std::size_t size) const \
    { \
	    const bool returnValue = read(dest, sizeof(Type), size) == size; \
	    error |= !returnValue; \
	    return returnValue; \
    }

#define DECLARE_READ_ARRAY_REVERSE(Type, Name) \
	bool Stream::Name(Type *dest, std::size_t size) const \
    { \
	    const bool returnValue = read(dest, sizeof(Type), size) == size; \
	    error |= !returnValue; \
	    for (unsigned int i = 0; i < size; ++i) \
        { \
	        Type buffer = dest[i]; \
	        unsigned char *s = reinterpret_cast<unsigned char *>(&buffer); \
	        unsigned char *d = reinterpret_cast<unsigned char *>(&dest[i]) + sizeof(Type) - 1; \
	        for (unsigned int j = 0; j < sizeof(Type); ++j) \
            { \
	            *d-- = *s++; \
            } \
        } \
	    return returnValue; \
    }

#define DECLARE_WRITE_DIRECT(Type, Name) \
    bool Stream::Name(const Type &value) const \
    { \
		const bool returnValue = write(&value, sizeof(Type), 1) == 1; \
		error |= !returnValue; \
		return returnValue; \
    }

#define DECLARE_WRITE_REVERSE(Type, Name) \
	bool Stream::Name(const Type &value) const \
    { \
		Type buffer; \
		const unsigned char *s = reinterpret_cast<const unsigned char *>(&value); \
		unsigned char *d = reinterpret_cast<unsigned char *>(&const_cast<Type &>(buffer)) + sizeof(Type) - 1; \
		for (unsigned int i = 0; i < sizeof(Type); ++i) \
        { \
			*d-- = *s++; \
        } \
		const bool returnValue = write(&buffer, sizeof(Type), 1) == 1; \
		error |= !returnValue; \
		return returnValue; \
    }

#define DECLARE_WRITE_ARRAY_DIRECT(Type, Name) \
	bool Stream::Name(const Type *src, std::size_t size) const \
    { \
		const bool returnValue = write(src, sizeof(Type), size) == size; \
		error |= !returnValue; \
		return returnValue; \
    }

#define DECLARE_WRITE_ARRAY_REVERSE(Type, Name) \
	bool Stream::Name(const Type *src, std::size_t size) const \
    { \
		for (unsigned int i = 0; i < size; ++i) \
        { \
			Type buffer; \
			const unsigned char *s = reinterpret_cast<const unsigned char *>(&src[i]); \
			unsigned char *d = reinterpret_cast<unsigned char *>(&buffer) + sizeof(Type) - 1; \
			for (unsigned int j = 0; j < sizeof(Type); ++j) \
            { \
				*d-- = *s++; \
            } \
			if (write(&buffer, sizeof(Type), 1) != 1) \
            { \
				error = true; \
				return false; \
            } \
        } \
		return true; \
    }

#if defined(AGE_LITTLE_ENDIAN)

DECLARE_READ_DIRECT(bool, read)
DECLARE_WRITE_DIRECT(bool, write)
DECLARE_READ_ARRAY_DIRECT(bool, read)
DECLARE_WRITE_ARRAY_DIRECT(bool, write)
DECLARE_READ_DIRECT(bool, readLittle)
DECLARE_WRITE_DIRECT(bool, writeLittle)
DECLARE_READ_ARRAY_DIRECT(bool, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(bool, writeLittle)
DECLARE_READ_REVERSE(bool, readBig)
DECLARE_WRITE_REVERSE(bool, writeBig)
DECLARE_READ_ARRAY_REVERSE(bool, readBig)
DECLARE_WRITE_ARRAY_REVERSE(bool, writeBig)

DECLARE_READ_DIRECT(char, read)
DECLARE_WRITE_DIRECT(char, write)
DECLARE_READ_ARRAY_DIRECT(char, read)
DECLARE_WRITE_ARRAY_DIRECT(char, write)
DECLARE_READ_DIRECT(char, readLittle)
DECLARE_WRITE_DIRECT(char, writeLittle)
DECLARE_READ_ARRAY_DIRECT(char, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(char, writeLittle)
DECLARE_READ_REVERSE(char, readBig)
DECLARE_WRITE_REVERSE(char, writeBig)
DECLARE_READ_ARRAY_REVERSE(char, readBig)
DECLARE_WRITE_ARRAY_REVERSE(char, writeBig)

DECLARE_READ_DIRECT(std::int8_t, read)
DECLARE_WRITE_DIRECT(std::int8_t, write)
DECLARE_READ_ARRAY_DIRECT(std::int8_t, read)
DECLARE_WRITE_ARRAY_DIRECT(std::int8_t, write)
DECLARE_READ_DIRECT(std::int8_t, readLittle)
DECLARE_WRITE_DIRECT(std::int8_t, writeLittle)
DECLARE_READ_ARRAY_DIRECT(std::int8_t, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(std::int8_t, writeLittle)
DECLARE_READ_REVERSE(std::int8_t, readBig)
DECLARE_WRITE_REVERSE(std::int8_t, writeBig)
DECLARE_READ_ARRAY_REVERSE(std::int8_t, readBig)
DECLARE_WRITE_ARRAY_REVERSE(std::int8_t, writeBig)

DECLARE_READ_DIRECT(std::uint8_t, read)
DECLARE_WRITE_DIRECT(std::uint8_t, write)
DECLARE_READ_ARRAY_DIRECT(std::uint8_t, read)
DECLARE_WRITE_ARRAY_DIRECT(std::uint8_t, write)
DECLARE_READ_DIRECT(std::uint8_t, readLittle)
DECLARE_WRITE_DIRECT(std::uint8_t, writeLittle)
DECLARE_READ_ARRAY_DIRECT(std::uint8_t, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(std::uint8_t, writeLittle)
DECLARE_READ_REVERSE(std::uint8_t, readBig)
DECLARE_WRITE_REVERSE(std::uint8_t, writeBig)
DECLARE_READ_ARRAY_REVERSE(std::uint8_t, readBig)
DECLARE_WRITE_ARRAY_REVERSE(std::uint8_t, writeBig)

DECLARE_READ_DIRECT(std::int16_t, read)
DECLARE_WRITE_DIRECT(std::int16_t, write)
DECLARE_READ_ARRAY_DIRECT(std::int16_t, read)
DECLARE_WRITE_ARRAY_DIRECT(std::int16_t, write)
DECLARE_READ_DIRECT(std::int16_t, readLittle)
DECLARE_WRITE_DIRECT(std::int16_t, writeLittle)
DECLARE_READ_ARRAY_DIRECT(std::int16_t, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(std::int16_t, writeLittle)
DECLARE_READ_REVERSE(std::int16_t, readBig)
DECLARE_WRITE_REVERSE(std::int16_t, writeBig)
DECLARE_READ_ARRAY_REVERSE(std::int16_t, readBig)
DECLARE_WRITE_ARRAY_REVERSE(std::int16_t, writeBig)

DECLARE_READ_DIRECT(std::uint16_t, read)
DECLARE_WRITE_DIRECT(std::uint16_t, write)
DECLARE_READ_ARRAY_DIRECT(std::uint16_t, read)
DECLARE_WRITE_ARRAY_DIRECT(std::uint16_t, write)
DECLARE_READ_DIRECT(std::uint16_t, readLittle)
DECLARE_WRITE_DIRECT(std::uint16_t, writeLittle)
DECLARE_READ_ARRAY_DIRECT(std::uint16_t, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(std::uint16_t, writeLittle)
DECLARE_READ_REVERSE(std::uint16_t, readBig)
DECLARE_WRITE_REVERSE(std::uint16_t, writeBig)
DECLARE_READ_ARRAY_REVERSE(std::uint16_t, readBig)
DECLARE_WRITE_ARRAY_REVERSE(std::uint16_t, writeBig)

DECLARE_READ_DIRECT(std::int32_t, read)
DECLARE_WRITE_DIRECT(std::int32_t, write)
DECLARE_READ_ARRAY_DIRECT(std::int32_t, read)
DECLARE_WRITE_ARRAY_DIRECT(std::int32_t, write)
DECLARE_READ_DIRECT(std::int32_t, readLittle)
DECLARE_WRITE_DIRECT(std::int32_t, writeLittle)
DECLARE_READ_ARRAY_DIRECT(std::int32_t, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(std::int32_t, writeLittle)
DECLARE_READ_REVERSE(std::int32_t, readBig)
DECLARE_WRITE_REVERSE(std::int32_t, writeBig)
DECLARE_READ_ARRAY_REVERSE(std::int32_t, readBig)
DECLARE_WRITE_ARRAY_REVERSE(std::int32_t, writeBig)

DECLARE_READ_DIRECT(std::uint32_t, read)
DECLARE_WRITE_DIRECT(std::uint32_t, write)
DECLARE_READ_ARRAY_DIRECT(std::uint32_t, read)
DECLARE_WRITE_ARRAY_DIRECT(std::uint32_t, write)
DECLARE_READ_DIRECT(std::uint32_t, readLittle)
DECLARE_WRITE_DIRECT(std::uint32_t, writeLittle)
DECLARE_READ_ARRAY_DIRECT(std::uint32_t, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(std::uint32_t, writeLittle)
DECLARE_READ_REVERSE(std::uint32_t, readBig)
DECLARE_WRITE_REVERSE(std::uint32_t, writeBig)
DECLARE_READ_ARRAY_REVERSE(std::uint32_t, readBig)
DECLARE_WRITE_ARRAY_REVERSE(std::uint32_t, writeBig)

DECLARE_READ_DIRECT(std::int64_t, read)
DECLARE_WRITE_DIRECT(std::int64_t, write)
DECLARE_READ_ARRAY_DIRECT(std::int64_t, read)
DECLARE_WRITE_ARRAY_DIRECT(std::int64_t, write)
DECLARE_READ_DIRECT(std::int64_t, readLittle)
DECLARE_WRITE_DIRECT(std::int64_t, writeLittle)
DECLARE_READ_ARRAY_DIRECT(std::int64_t, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(std::int64_t, writeLittle)
DECLARE_READ_REVERSE(std::int64_t, readBig)
DECLARE_WRITE_REVERSE(std::int64_t, writeBig)
DECLARE_READ_ARRAY_REVERSE(std::int64_t, readBig)
DECLARE_WRITE_ARRAY_REVERSE(std::int64_t, writeBig)

DECLARE_READ_DIRECT(std::uint64_t, read)
DECLARE_WRITE_DIRECT(std::uint64_t, write)
DECLARE_READ_ARRAY_DIRECT(std::uint64_t, read)
DECLARE_WRITE_ARRAY_DIRECT(std::uint64_t, write)
DECLARE_READ_DIRECT(std::uint64_t, readLittle)
DECLARE_WRITE_DIRECT(std::uint64_t, writeLittle)
DECLARE_READ_ARRAY_DIRECT(std::uint64_t, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(std::uint64_t, writeLittle)
DECLARE_READ_REVERSE(std::uint64_t, readBig)
DECLARE_WRITE_REVERSE(std::uint64_t, writeBig)
DECLARE_READ_ARRAY_REVERSE(std::uint64_t, readBig)
DECLARE_WRITE_ARRAY_REVERSE(std::uint64_t, writeBig)

DECLARE_READ_DIRECT(float, read)
DECLARE_WRITE_DIRECT(float, write)
DECLARE_READ_ARRAY_DIRECT(float, read)
DECLARE_WRITE_ARRAY_DIRECT(float, write)
DECLARE_READ_DIRECT(float, readLittle)
DECLARE_WRITE_DIRECT(float, writeLittle)
DECLARE_READ_ARRAY_DIRECT(float, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(float, writeLittle)
DECLARE_READ_REVERSE(float, readBig)
DECLARE_WRITE_REVERSE(float, writeBig)
DECLARE_READ_ARRAY_REVERSE(float, readBig)
DECLARE_WRITE_ARRAY_REVERSE(float, writeBig)

DECLARE_READ_DIRECT(double, read)
DECLARE_WRITE_DIRECT(double, write)
DECLARE_READ_ARRAY_DIRECT(double, read)
DECLARE_WRITE_ARRAY_DIRECT(double, write)
DECLARE_READ_DIRECT(double, readLittle)
DECLARE_WRITE_DIRECT(double, writeLittle)
DECLARE_READ_ARRAY_DIRECT(double, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(double, writeLittle)
DECLARE_READ_REVERSE(double, readBig)
DECLARE_WRITE_REVERSE(double, writeBig)
DECLARE_READ_ARRAY_REVERSE(double, readBig)
DECLARE_WRITE_ARRAY_REVERSE(double, writeBig)

DECLARE_READ_DIRECT(long double, read)
DECLARE_WRITE_DIRECT(long double, write)
DECLARE_READ_ARRAY_DIRECT(long double, read)
DECLARE_WRITE_ARRAY_DIRECT(long double, write)
DECLARE_READ_DIRECT(long double, readLittle)
DECLARE_WRITE_DIRECT(long double, writeLittle)
DECLARE_READ_ARRAY_DIRECT(long double, readLittle)
DECLARE_WRITE_ARRAY_DIRECT(long double, writeLittle)
DECLARE_READ_REVERSE(long double, readBig)
DECLARE_WRITE_REVERSE(long double, writeBig)
DECLARE_READ_ARRAY_REVERSE(long double, readBig)
DECLARE_WRITE_ARRAY_REVERSE(long double, writeBig)

#else

DECLARE_READ_REVERSE(bool, read)
DECLARE_WRITE_REVERSE(bool, write)
DECLARE_READ_ARRAY_REVERSE(bool, read)
DECLARE_WRITE_ARRAY_REVERSE(bool, write)
DECLARE_READ_REVERSE(bool, readLittle)
DECLARE_WRITE_REVERSE(bool, writeLittle)
DECLARE_READ_ARRAY_REVERSE(bool, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(bool, writeLittle)
DECLARE_READ_DIRECT(bool, readBig)
DECLARE_WRITE_DIRECT(bool, writeBig)
DECLARE_READ_ARRAY_DIRECT(bool, readBig)
DECLARE_WRITE_ARRAY_DIRECT(bool, writeBig)

DECLARE_READ_REVERSE(char, read)
DECLARE_WRITE_REVERSE(char, write)
DECLARE_READ_ARRAY_REVERSE(char, read)
DECLARE_WRITE_ARRAY_REVERSE(char, write)
DECLARE_READ_REVERSE(char, readLittle)
DECLARE_WRITE_REVERSE(char, writeLittle)
DECLARE_READ_ARRAY_REVERSE(char, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(char, writeLittle)
DECLARE_READ_DIRECT(char, readBig)
DECLARE_WRITE_DIRECT(char, writeBig)
DECLARE_READ_ARRAY_DIRECT(char, readBig)
DECLARE_WRITE_ARRAY_DIRECT(char, writeBig)

DECLARE_READ_REVERSE(std::int8_t, read)
DECLARE_WRITE_REVERSE(std::int8_t, write)
DECLARE_READ_ARRAY_REVERSE(std::int8_t, read)
DECLARE_WRITE_ARRAY_REVERSE(std::int8_t, write)
DECLARE_READ_REVERSE(std::int8_t, readLittle)
DECLARE_WRITE_REVERSE(std::int8_t, writeLittle)
DECLARE_READ_ARRAY_REVERSE(std::int8_t, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(std::int8_t, writeLittle)
DECLARE_READ_DIRECT(std::int8_t, readBig)
DECLARE_WRITE_DIRECT(std::int8_t, writeBig)
DECLARE_READ_ARRAY_DIRECT(std::int8_t, readBig)
DECLARE_WRITE_ARRAY_DIRECT(std::int8_t, writeBig)

DECLARE_READ_REVERSE(std::uint8_t, read)
DECLARE_WRITE_REVERSE(std::uint8_t, write)
DECLARE_READ_ARRAY_REVERSE(std::uint8_t, read)
DECLARE_WRITE_ARRAY_REVERSE(std::uint8_t, write)
DECLARE_READ_REVERSE(std::uint8_t, readLittle)
DECLARE_WRITE_REVERSE(std::uint8_t, writeLittle)
DECLARE_READ_ARRAY_REVERSE(std::uint8_t, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(std::uint8_t, writeLittle)
DECLARE_READ_DIRECT(std::uint8_t, readBig)
DECLARE_WRITE_DIRECT(std::uint8_t, writeBig)
DECLARE_READ_ARRAY_DIRECT(std::uint8_t, readBig)
DECLARE_WRITE_ARRAY_DIRECT(std::uint8_t, writeBig)

DECLARE_READ_REVERSE(std::int16_t, read)
DECLARE_WRITE_REVERSE(std::int16_t, write)
DECLARE_READ_ARRAY_REVERSE(std::int16_t, read)
DECLARE_WRITE_ARRAY_REVERSE(std::int16_t, write)
DECLARE_READ_REVERSE(std::int16_t, readLittle)
DECLARE_WRITE_REVERSE(std::int16_t, writeLittle)
DECLARE_READ_ARRAY_REVERSE(std::int16_t, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(std::int16_t, writeLittle)
DECLARE_READ_DIRECT(std::int16_t, readBig)
DECLARE_WRITE_DIRECT(std::int16_t, writeBig)
DECLARE_READ_ARRAY_DIRECT(std::int16_t, readBig)
DECLARE_WRITE_ARRAY_DIRECT(std::int16_t, writeBig)

DECLARE_READ_REVERSE(std::uint16_t, read)
DECLARE_WRITE_REVERSE(std::uint16_t, write)
DECLARE_READ_ARRAY_REVERSE(std::uint16_t, read)
DECLARE_WRITE_ARRAY_REVERSE(std::uint16_t, write)
DECLARE_READ_REVERSE(std::uint16_t, readLittle)
DECLARE_WRITE_REVERSE(std::uint16_t, writeLittle)
DECLARE_READ_ARRAY_REVERSE(std::uint16_t, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(std::uint16_t, writeLittle)
DECLARE_READ_DIRECT(std::uint16_t, readBig)
DECLARE_WRITE_DIRECT(std::uint16_t, writeBig)
DECLARE_READ_ARRAY_DIRECT(std::uint16_t, readBig)
DECLARE_WRITE_ARRAY_DIRECT(std::uint16_t, writeBig)

DECLARE_READ_REVERSE(std::int32_t, read)
DECLARE_WRITE_REVERSE(std::int32_t, write)
DECLARE_READ_ARRAY_REVERSE(std::int32_t, read)
DECLARE_WRITE_ARRAY_REVERSE(std::int32_t, write)
DECLARE_READ_REVERSE(std::int32_t, readLittle)
DECLARE_WRITE_REVERSE(std::int32_t, writeLittle)
DECLARE_READ_ARRAY_REVERSE(std::int32_t, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(std::int32_t, writeLittle)
DECLARE_READ_DIRECT(std::int32_t, readBig)
DECLARE_WRITE_DIRECT(std::int32_t, writeBig)
DECLARE_READ_ARRAY_DIRECT(std::int32_t, readBig)
DECLARE_WRITE_ARRAY_DIRECT(std::int32_t, writeBig)

DECLARE_READ_REVERSE(std::uint32_t, read)
DECLARE_WRITE_REVERSE(std::uint32_t, write)
DECLARE_READ_ARRAY_REVERSE(std::uint32_t, read)
DECLARE_WRITE_ARRAY_REVERSE(std::uint32_t, write)
DECLARE_READ_REVERSE(std::uint32_t, readLittle)
DECLARE_WRITE_REVERSE(std::uint32_t, writeLittle)
DECLARE_READ_ARRAY_REVERSE(std::uint32_t, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(std::uint32_t, writeLittle)
DECLARE_READ_DIRECT(std::uint32_t, readBig)
DECLARE_WRITE_DIRECT(std::uint32_t, writeBig)
DECLARE_READ_ARRAY_DIRECT(std::uint32_t, readBig)
DECLARE_WRITE_ARRAY_DIRECT(std::uint32_t, writeBig)

DECLARE_READ_REVERSE(std::int64_t, read)
DECLARE_WRITE_REVERSE(std::int64_t, write)
DECLARE_READ_ARRAY_REVERSE(std::int64_t, read)
DECLARE_WRITE_ARRAY_REVERSE(std::int64_t, write)
DECLARE_READ_REVERSE(std::int64_t, readLittle)
DECLARE_WRITE_REVERSE(std::int64_t, writeLittle)
DECLARE_READ_ARRAY_REVERSE(std::int64_t, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(std::int64_t, writeLittle)
DECLARE_READ_DIRECT(std::int64_t, readBig)
DECLARE_WRITE_DIRECT(std::int64_t, writeBig)
DECLARE_READ_ARRAY_DIRECT(std::int64_t, readBig)
DECLARE_WRITE_ARRAY_DIRECT(std::int64_t, writeBig)

DECLARE_READ_REVERSE(std::uint64_t, read)
DECLARE_WRITE_REVERSE(std::uint64_t, write)
DECLARE_READ_ARRAY_REVERSE(std::uint64_t, read)
DECLARE_WRITE_ARRAY_REVERSE(std::uint64_t, write)
DECLARE_READ_REVERSE(std::uint64_t, readLittle)
DECLARE_WRITE_REVERSE(std::uint64_t, writeLittle)
DECLARE_READ_ARRAY_REVERSE(std::uint64_t, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(std::uint64_t, writeLittle)
DECLARE_READ_DIRECT(std::uint64_t, readBig)
DECLARE_WRITE_DIRECT(std::uint64_t, writeBig)
DECLARE_READ_ARRAY_DIRECT(std::uint64_t, readBig)
DECLARE_WRITE_ARRAY_DIRECT(std::uint64_t, writeBig)

DECLARE_READ_REVERSE(float, read)
DECLARE_WRITE_REVERSE(float, write)
DECLARE_READ_ARRAY_REVERSE(float, read)
DECLARE_WRITE_ARRAY_REVERSE(float, write)
DECLARE_READ_REVERSE(float, readLittle)
DECLARE_WRITE_REVERSE(float, writeLittle)
DECLARE_READ_ARRAY_REVERSE(float, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(float, writeLittle)
DECLARE_READ_DIRECT(float, readBig)
DECLARE_WRITE_DIRECT(float, writeBig)
DECLARE_READ_ARRAY_DIRECT(float, readBig)
DECLARE_WRITE_ARRAY_DIRECT(float, writeBig)

DECLARE_READ_REVERSE(double, read)
DECLARE_WRITE_REVERSE(double, write)
DECLARE_READ_ARRAY_REVERSE(double, read)
DECLARE_WRITE_ARRAY_REVERSE(double, write)
DECLARE_READ_REVERSE(double, readLittle)
DECLARE_WRITE_REVERSE(double, writeLittle)
DECLARE_READ_ARRAY_REVERSE(double, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(double, writeLittle)
DECLARE_READ_DIRECT(double, readBig)
DECLARE_WRITE_DIRECT(double, writeBig)
DECLARE_READ_ARRAY_DIRECT(double, readBig)
DECLARE_WRITE_ARRAY_DIRECT(double, writeBig)

DECLARE_READ_REVERSE(long double, read)
DECLARE_WRITE_REVERSE(long double, write)
DECLARE_READ_ARRAY_REVERSE(long double, read)
DECLARE_WRITE_ARRAY_REVERSE(long double, write)
DECLARE_READ_REVERSE(long double, readLittle)
DECLARE_WRITE_REVERSE(long double, writeLittle)
DECLARE_READ_ARRAY_REVERSE(long double, readLittle)
DECLARE_WRITE_ARRAY_REVERSE(long double, writeLittle)
DECLARE_READ_DIRECT(long double, readBig)
DECLARE_WRITE_DIRECT(long double, writeBig)
DECLARE_READ_ARRAY_DIRECT(long double, readBig)
DECLARE_WRITE_ARRAY_DIRECT(long double, writeBig)

#endif

#undef DECLARE_READ_DIRECT
#undef DECLARE_READ_REVERSE
#undef DECLARE_READ_ARRAY_DIRECT
#undef DECLARE_READ_ARRAY_REVERSE
#undef DECLARE_WRITE_DIRECT
#undef DECLARE_WRITE_REVERSE
#undef DECLARE_WRITE_ARRAY_DIRECT
#undef DECLARE_WRITE_ARRAY_REVERSE
	}
}