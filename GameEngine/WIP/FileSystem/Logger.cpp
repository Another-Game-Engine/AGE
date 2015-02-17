#include "Debug.hpp"
#include "Logger.hpp"

namespace Age
{
	namespace Engine
	{
		Logger::Logger(std::streamsize precision)
		{
			setLogPrecision(precision);
		}

		std::string Logger::getTime(void)
		{
			const time_t t = time(nullptr);
			struct tm *timeval = localtime(&t);
			return formatStream("[%02d:%02d:%02d] ", timeval->tm_hour, timeval->tm_min, timeval->tm_sec);
		}

		std::string Logger::formatStream(const char *format, ...)
		{
			va_list argptr;
			va_start(argptr, format);
			std::string text = formatStream(format, argptr);
			va_end(argptr);
			return text;
		}

		std::string Logger::formatStream(const char *format, va_list argptr)
		{
			std::string text;
			int size = 1024;
			text.resize(static_cast<std::size_t>(size));
			while (true)
			{
				const int needed = vsnprintf(&text[0], size - 1, format, argptr);
				if (needed >= 0 && needed < size)
				{
					text[needed] = '\0';
					break;
				}
				else
				{
					size = needed > 0 ? needed + 1 : 2 * size;
					text.resize(static_cast<std::size_t>(size));
				}
			}
			return text;
		}

		void Logger::formatLogNormal(const char *format, va_list argptr)
		{
			const std::string formattedText = formatStream(format, argptr);
#if defined(AGE_DEBUG)
			fputs(formatStream("%sLog: %s\n", getTime().c_str(), formattedText.c_str()).c_str(), stdout);
#else
			fputs(formatStream("%s\n", formattedText.c_str()).c_str(), stdout);
#endif
			fflush(stdout);
		}

		void Logger::formatLogWarning(const char *format, va_list argptr)
		{
			const std::string formattedText = formatStream(format, argptr);
#if defined(AGE_DEBUG)
			fputs(formatStream("%sWarning: %s\n", getTime().c_str(), formattedText.c_str()).c_str(), stderr);
#else
			fputs(formatStream("%s\n", formattedText.c_str()).c_str(), stderr);
#endif
			fflush(stderr);
		}

		void Logger::formatLogError(const char *format, va_list argptr)
		{
			const std::string formattedText = formatStream(format, argptr);
#if defined(AGE_DEBUG)
			fputs(formatStream("%sError: %s\n", getTime().c_str(), formattedText.c_str()).c_str(), stderr);
#else
			fputs(formatStream("%s\n", formattedText.c_str()).c_str(), stderr);
#endif
			fflush(stderr);
		}

		void Logger::formatLogFatal(const char *format, va_list argptr)
		{
			const std::string text = formatStream(format, argptr);
#if defined(AGE_DEBUG)
			const std::string formattedText = formatStream("%sFatal: %s", getTime().c_str(), text.c_str());
#else
			const std::string formattedText = formatStream("%s", text.c_str());
#endif
			fputs(formattedText.c_str(), stderr);
			fflush(stderr);
#if defined(PLATFORM_WINDOWS)
			MessageBox(nullptr, text.c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
#endif
			mutex.unlock();
		}

		void Logger::formatLogDebug(const char *format, va_list argptr)
		{
#if defined(AGE_DEBUG)
			const std::string formattedText = formatStream(format, argptr);
			fputs(formatStream("%sAGE_DEBUG: %s\n", getTime().c_str(), formattedText.c_str()).c_str(), stderr);
			fflush(stderr);
#endif
		}

		void Logger::formatLog(Level level, const char *format, ...)
		{
			va_list argptr;
			va_start(argptr, format);
			switch (level)
			{
			case Normal:
				formatLogNormal(format, argptr);
				break;
			case Warning:
				formatLogWarning(format, argptr);
				break;
			case Error:
				formatLogError(format, argptr);
				break;
			case Fatal:
				formatLogFatal(format, argptr);
				break;
			case Debug:
				formatLogDebug(format, argptr);
				break;
			default:
				log(Level::Error, "Unknown log level");
				break;
			}
			va_end(argptr);
		}

		void Logger::setLogPrecision(std::streamsize precision)
		{
			stream.precision(precision);
			stream << std::fixed << std::setprecision(static_cast<int>(precision));
		}
	}
}