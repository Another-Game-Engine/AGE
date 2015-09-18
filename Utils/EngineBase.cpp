#include "EngineBase.hpp"

std::size_t EngineBase::getNumberOfArguments(void) const
{
	return numberOfArguments;
}

const std::string &EngineBase::getArgument(std::size_t num) const
{
	assert(num < arguments.size() && "Invalid argument");
	return arguments[num];
}

const std::string &EngineBase::getProjectName(void) const
{
	return projectName;
}

const std::string &EngineBase::getPassword(void) const
{
	return password;
}

const std::string &EngineBase::getApplicationPath(void) const
{
	return applicationPath;
}

const std::string &EngineBase::getDataPath(void) const
{
	return dataPath;
}

const std::string &EngineBase::getHomePath(void) const
{
	return homePath;
}

const std::string &EngineBase::getSavePath(void) const
{
	return savePath;
}

const std::string &EngineBase::getCachePath(void) const
{
	return cachePath;
}

std::size_t EngineBase::getFrameNumber(void) const
{
	return frame;
}

EngineBase::EngineBase(void)
	: EngineBase(0, nullptr)
	//, _timer(nullptr)
{

}

EngineBase::EngineBase(int argc, char *argv[])
	: EngineBase(argc, argv, "AgeGameEngine", "")
{
	return;
}

EngineBase::EngineBase(const std::string &projectName, const std::string &password)
	: EngineBase(0, nullptr, projectName, password)
{
	return;
}

EngineBase::EngineBase(int argc, char *argv[], const std::string &projectName, const std::string &password)
	: numberOfArguments(argc), projectName(projectName), password(password)
{
	arguments.reserve(argc);
	for (int index = 0; index < argc; ++index)
	{
		arguments.push_back(argv[index]);
	}
}

EngineBase::~EngineBase()
{
}