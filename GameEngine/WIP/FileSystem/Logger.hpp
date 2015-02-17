#pragma once

#include <mutex>
#include <iomanip>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>
#include <cstdarg>
#include <cstddef>
#include <cstdint>

#include "Singleton.hpp"

namespace Age
{
	namespace Engine
	{
		class Logger final : public Singleton<Logger>
		{
			friend Access;

		private:
			std::mutex mutex;
			std::ostringstream stream;

			Logger(std::streamsize precision = 3);
			Logger(const Logger &) = default;
			Logger &operator=(const Logger &) = default;
			
			std::string getTime(void);
			std::string formatStream(const char *format, ...);
			std::string formatStream(const char *format, va_list argptr);

			template <typename FirstType, typename SecondType, typename... Args>
			void internalLog(FirstType &&first, SecondType &&second, Args &&...args)
			{
				internalLog(std::forward<FirstType>(first));
				internalLog(std::forward<SecondType>(second), std::forward<Args>(args)...);
			}

			template <typename Type>
			void internalLog(const Type &arg)
			{
				const std::streamsize precision = stream.precision();
				stream << arg;
				stream.precision(precision);
			}

			template <typename Type>
			void internalLog(Type &arg)
			{
				const std::streamsize precision = stream.precision();
				stream << arg;
				stream.precision(precision);
			}

			void logNormal(void) = delete;

			template <typename... Args>
			void logNormal(Args &&...args)
			{
				std::lock_guard<std::mutex> lock(mutex);
				internalLog(std::forward<Args>(args)...);
#if defined(DEBUG)
				fputs(formatStream("%sLog: %s\n", getTime().c_str(), stream.str().c_str()).c_str(), stdout);
#else
				fputs(formatStream("%s\n", stream.str().c_str()).c_str(), stdout);
#endif
				fflush(stdout);
				stream.str("");
				stream.clear();
			}

			void formatLogNormal(const char *format, va_list argptr);

			void logWarning(void) = delete;

			template <typename... Args>
			void logWarning(Args &&...args)
			{
				std::lock_guard<std::mutex> lock(mutex);
				internalLog(std::forward<Args>(args)...);
#if defined(DEBUG)
				fputs(formatStream("%sWarning: %s\n", getTime().c_str(), stream.str().c_str()).c_str(), stderr);
#else
				fputs(formatStream("%s\n", stream.str().c_str()).c_str(), stderr);
#endif
				fflush(stderr);
				stream.str("");
				stream.clear();
			}

			void formatLogWarning(const char *format, va_list argptr);

			void logError(void) = delete;

			template <typename... Args>
			void logError(Args &&...args)
			{
				std::lock_guard<std::mutex> lock(mutex);
				internalLog(std::forward<Args>(args)...);
#if defined(DEBUG)
				fputs(formatStream("%sError: %s\n", getTime().c_str(), stream.str().c_str()).c_str(), stderr);
#else
				fputs(formatStream("%s\n", stream.str().c_str()).c_str(), stderr);
#endif
				fflush(stderr);
				stream.str("");
				stream.clear();
			}

			void formatLogError(const char *format, va_list argptr);

			void logFatal(void) = delete;

			template <typename... Args>
			void logFatal(Args &&...args)
			{
				std::lock_guard<std::mutex> lock(mutex);
				internalLog(std::forward<Args>(args)...);
#if defined(DEBUG)
				fputs(formatStream("%sFatal: %s", getTime().c_str(), stream.str().c_str()).c_str(), stderr);
#else
				fputs(formatStream("%s", stream.str().c_str()).c_str(), stderr);
#endif
				fflush(stderr);
#if defined(PLATFORM_WINDOWS)
				MessageBox(nullptr, stream.str().c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
#endif
				stream.str("");
				stream.clear();
				mutex.unlock();
			}

			void formatLogFatal(const char *format, va_list argptr);

			void logDebug(void) = delete;

			template <typename... Args>
			void logDebug(Args &&...args)
			{
#if defined(DEBUG)
				std::lock_guard<std::mutex> lock(mutex);
				internalLog(std::forward<Args>(args)...);
				fputs(formatStream("%sDebug: %s\n", getTime().c_str(), stream.str().c_str()).c_str(), stdout);
				fflush(stdout);
				stream.str("");
				stream.clear();
#endif
			}

			void formatLogDebug(const char *format, va_list argptr);

		public:
			// Log level
			enum Level : std::uint8_t
			{
				Normal,
				Warning,
				Error,
				Fatal,
				Debug
			};

			// Destructor
			~Logger(void) = default;

			// Methods
			void log(Level level) = delete;

			template <typename... Args>
			void log(Level level, Args &&...args)
			{
				switch (level)
				{
				case Normal:
					logNormal(std::forward<Args>(args)...);
					break;
				case Warning:
					logWarning(std::forward<Args>(args)...);
					break;
				case Error:
					logError(std::forward<Args>(args)...);
					break;
				case Fatal:
					logFatal(std::forward<Args>(args)...);
					break;
				case Debug:
					logDebug(std::forward<Args>(args)...);
					break;
				default:
					log(Level::Error, "Unknown log level");
					break;
				}
			}

			void formatLog(Level level, const char *format, ...);

			void setLogPrecision(std::streamsize precision);
		};
	}
}