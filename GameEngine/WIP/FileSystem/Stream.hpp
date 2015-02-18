#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <string>

namespace AGE
{
	namespace Engine
	{
		class Stream
		{
		public:
			Stream(void) = default;
			Stream(const Stream &other) = default;
			Stream &operator=(const Stream &other) = default;
			virtual ~Stream(void) = default;

			operator bool(void) const;

			virtual bool isOpened(void) const = 0;
			virtual bool endOfFile(void) const = 0;
			virtual int getCharacter(void) const = 0;
			virtual void seekSet(int offset) const = 0;
			virtual void seekEnd(int offset) const = 0;
			virtual void seekCurrent(int offset) const = 0;
			virtual std::size_t tell(void) const = 0;
			virtual bool flush(void) const = 0;
			virtual std::size_t read(void *ptr, std::size_t size, std::size_t nmemb) const = 0;
			virtual std::size_t write(const void *ptr, std::size_t size, std::size_t nmemb) const = 0;

			bool read(bool &dest) const;
			bool readLittle(bool &dest) const;
			bool readBig(bool &dest) const;
			bool read(bool *dest, std::size_t size) const;
			bool readLittle(bool *dest, std::size_t size) const;
			bool readBig(bool *dest, std::size_t size) const;
			bool write(const bool &value) const;
			bool writeLittle(const bool &value) const;
			bool writeBig(const bool &value) const;
			bool write(const bool *value, std::size_t size) const;
			bool writeLittle(const bool *value, std::size_t size) const;
			bool writeBig(const bool *value, std::size_t size) const;

			bool read(char &dest) const;
			bool readLittle(char &dest) const;
			bool readBig(char &dest) const;
			bool read(char *dest, std::size_t size) const;
			bool readLittle(char *dest, std::size_t size) const;
			bool readBig(char *dest, std::size_t size) const;
			bool write(const char &value) const;
			bool writeLittle(const char &value) const;
			bool writeBig(const char &value) const;
			bool write(const char *value, std::size_t size) const;
			bool writeLittle(const char *value, std::size_t size) const;
			bool writeBig(const char *value, std::size_t size) const;

			bool read(std::int8_t &dest) const;
			bool readLittle(std::int8_t &dest) const;
			bool readBig(std::int8_t &dest) const;
			bool read(std::int8_t *dest, std::size_t size) const;
			bool readLittle(std::int8_t *dest, std::size_t size) const;
			bool readBig(std::int8_t *dest, std::size_t size) const;
			bool write(const std::int8_t &value) const;
			bool writeLittle(const std::int8_t &value) const;
			bool writeBig(const std::int8_t &value) const;
			bool write(const std::int8_t *value, std::size_t size) const;
			bool writeLittle(const std::int8_t *value, std::size_t size) const;
			bool writeBig(const std::int8_t *value, std::size_t size) const;

			bool read(std::uint8_t &dest) const;
			bool readLittle(std::uint8_t &dest) const;
			bool readBig(std::uint8_t &dest) const;
			bool read(std::uint8_t *dest, std::size_t size) const;
			bool readLittle(std::uint8_t *dest, std::size_t size) const;
			bool readBig(std::uint8_t *dest, std::size_t size) const;
			bool write(const std::uint8_t &value) const;
			bool writeLittle(const std::uint8_t &value) const;
			bool writeBig(const std::uint8_t &value) const;
			bool write(const std::uint8_t *value, std::size_t size) const;
			bool writeLittle(const std::uint8_t *value, std::size_t size) const;
			bool writeBig(const std::uint8_t *value, std::size_t size) const;

			bool read(std::int16_t &dest) const;
			bool readLittle(std::int16_t &dest) const;
			bool readBig(std::int16_t &dest) const;
			bool read(std::int16_t *dest, std::size_t size) const;
			bool readLittle(std::int16_t *dest, std::size_t size) const;
			bool readBig(std::int16_t *dest, std::size_t size) const;
			bool write(const std::int16_t &value) const;
			bool writeLittle(const std::int16_t &value) const;
			bool writeBig(const std::int16_t &value) const;
			bool write(const std::int16_t *value, std::size_t size) const;
			bool writeLittle(const std::int16_t *value, std::size_t size) const;
			bool writeBig(const std::int16_t *value, std::size_t size) const;

			bool read(std::uint16_t &dest) const;
			bool readLittle(std::uint16_t &dest) const;
			bool readBig(std::uint16_t &dest) const;
			bool read(std::uint16_t *dest, std::size_t size) const;
			bool readLittle(std::uint16_t *dest, std::size_t size) const;
			bool readBig(std::uint16_t *dest, std::size_t size) const;
			bool write(const std::uint16_t &value) const;
			bool writeLittle(const std::uint16_t &value) const;
			bool writeBig(const std::uint16_t &value) const;
			bool write(const std::uint16_t *value, std::size_t size) const;
			bool writeLittle(const std::uint16_t *value, std::size_t size) const;
			bool writeBig(const std::uint16_t *value, std::size_t size) const;

			bool read(std::int32_t &dest) const;
			bool readLittle(std::int32_t &dest) const;
			bool readBig(std::int32_t &dest) const;
			bool read(std::int32_t *dest, std::size_t size) const;
			bool readLittle(std::int32_t *dest, std::size_t size) const;
			bool readBig(std::int32_t *dest, std::size_t size) const;
			bool write(const std::int32_t &value) const;
			bool writeLittle(const std::int32_t &value) const;
			bool writeBig(const std::int32_t &value) const;
			bool write(const std::int32_t *value, std::size_t size) const;
			bool writeLittle(const std::int32_t *value, std::size_t size) const;
			bool writeBig(const std::int32_t *value, std::size_t size) const;

			bool read(std::uint32_t &dest) const;
			bool readLittle(std::uint32_t &dest) const;
			bool readBig(std::uint32_t &dest) const;
			bool read(std::uint32_t *dest, std::size_t size) const;
			bool readLittle(std::uint32_t *dest, std::size_t size) const;
			bool readBig(std::uint32_t *dest, std::size_t size) const;
			bool write(const std::uint32_t &value) const;
			bool writeLittle(const std::uint32_t &value) const;
			bool writeBig(const std::uint32_t &value) const;
			bool write(const std::uint32_t *value, std::size_t size) const;
			bool writeLittle(const std::uint32_t *value, std::size_t size) const;
			bool writeBig(const std::uint32_t *value, std::size_t size) const;

			bool read(std::int64_t &dest) const;
			bool readLittle(std::int64_t &dest) const;
			bool readBig(std::int64_t &dest) const;
			bool read(std::int64_t *dest, std::size_t size) const;
			bool readLittle(std::int64_t *dest, std::size_t size) const;
			bool readBig(std::int64_t *dest, std::size_t size) const;
			bool write(const std::int64_t &value) const;
			bool writeLittle(const std::int64_t &value) const;
			bool writeBig(const std::int64_t &value) const;
			bool write(const std::int64_t *value, std::size_t size) const;
			bool writeLittle(const std::int64_t *value, std::size_t size) const;
			bool writeBig(const std::int64_t *value, std::size_t size) const;

			bool read(std::uint64_t &dest) const;
			bool readLittle(std::uint64_t &dest) const;
			bool readBig(std::uint64_t &dest) const;
			bool read(std::uint64_t *dest, std::size_t size) const;
			bool readLittle(std::uint64_t *dest, std::size_t size) const;
			bool readBig(std::uint64_t *dest, std::size_t size) const;
			bool write(const std::uint64_t &value) const;
			bool writeLittle(const std::uint64_t &value) const;
			bool writeBig(const std::uint64_t &value) const;
			bool write(const std::uint64_t *value, std::size_t size) const;
			bool writeLittle(const std::uint64_t *value, std::size_t size) const;
			bool writeBig(const std::uint64_t *value, std::size_t size) const;

			virtual bool read(float &dest) const;
			bool readLittle(float &dest) const;
			bool readBig(float &dest) const;
			bool read(float *dest, std::size_t size) const;
			bool readLittle(float *dest, std::size_t size) const;
			bool readBig(float *dest, std::size_t size) const;
			bool write(const float &value) const;
			bool writeLittle(const float &value) const;
			bool writeBig(const float &value) const;
			bool write(const float *value, std::size_t size) const;
			bool writeLittle(const float *value, std::size_t size) const;
			bool writeBig(const float *value, std::size_t size) const;

			bool read(double &dest) const;
			bool readLittle(double &dest) const;
			bool readBig(double &dest) const;
			bool read(double *dest, std::size_t size) const;
			bool readLittle(double *dest, std::size_t size) const;
			bool readBig(double *dest, std::size_t size) const;
			bool write(const double &value) const;
			bool writeLittle(const double &value) const;
			bool writeBig(const double &value) const;
			bool write(const double *value, std::size_t size) const;
			bool writeLittle(const double *value, std::size_t size) const;
			bool writeBig(const double *value, std::size_t size) const;

			bool read(long double &dest) const;
			bool readLittle(long double &dest) const;
			bool readBig(long double &dest) const;
			bool read(long double *dest, std::size_t size) const;
			bool readLittle(long double *dest, std::size_t size) const;
			bool readBig(long double *dest, std::size_t size) const;
			bool write(const long double &value) const;
			bool writeLittle(const long double &value) const;
			bool writeBig(const long double &value) const;
			bool write(const long double *value, std::size_t size) const;
			bool writeLittle(const long double *value, std::size_t size) const;
			bool writeBig(const long double *value, std::size_t size) const;

			bool read(std::string &dest) const;
			bool readLittle(std::string &dest) const;
			bool readBig(std::string &dest) const;
			bool read(std::string *dest, std::size_t size) const;
			bool readLittle(std::string *dest, std::size_t size) const;
			bool readBig(std::string *dest, std::size_t size) const;
			bool write(const std::string &value) const;
			bool writeLittle(const std::string &value) const;
			bool writeBig(const std::string &value) const;
			bool write(const std::string *value, std::size_t size) const;
			bool writeLittle(const std::string *value, std::size_t size) const;
			bool writeBig(const std::string *value, std::size_t size) const;

			std::size_t readToken(char *str, std::size_t size) const;
			std::size_t readLine(char *str, std::size_t size) const;
			std::string readToken(void) const;
			std::string readLine(void) const;

			bool puts(const char *str) const;
			bool puts(const std::string &str) const;
			bool vprintf(const char *format, va_list argptr) const;
			bool printf(const char *format, ...) const;

			const Stream &operator<<(bool value) const;
			const Stream &operator<<(char value) const;
			const Stream &operator<<(int8_t value) const;
			const Stream &operator<<(std::uint8_t value) const;
			const Stream &operator<<(int16_t value) const;
			const Stream &operator<<(std::uint16_t value) const;
			const Stream &operator<<(int32_t value) const;
			const Stream &operator<<(std::uint32_t value) const;
			const Stream &operator<<(int64_t value) const;
			const Stream &operator<<(std::uint64_t value) const;
			const Stream &operator<<(float value) const;
			const Stream &operator<<(double value) const;
			const Stream &operator<<(long double value) const;
			const Stream &operator<<(const char *value) const;
			const Stream &operator<<(const std::string &value) const;

			const Stream &operator>>(bool &value) const;
			const Stream &operator>>(char &value) const;
			const Stream &operator>>(int8_t &value) const;
			const Stream &operator>>(std::uint8_t &value) const;
			const Stream &operator>>(int16_t &value) const;
			const Stream &operator>>(std::uint16_t &value) const;
			const Stream &operator>>(int32_t &value) const;
			const Stream &operator>>(std::uint32_t &value) const;
			const Stream &operator>>(int64_t &value) const;
			const Stream &operator>>(std::uint64_t &value) const;
			const Stream &operator>>(float &value) const;
			const Stream &operator>>(double &value) const;
			const Stream &operator>>(long double &value) const;
			const Stream &operator>>(char *value) const;
			const Stream &operator>>(std::string &value) const;

		private:
			static const std::size_t BufferSize = 4096;

			mutable bool error = false;
		};
	}
}
