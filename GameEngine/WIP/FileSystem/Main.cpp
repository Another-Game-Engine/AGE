#include "Buffer.hpp"
#include "Engine.hpp"
#include "Plugin.hpp"
#include "FileInterface.hpp"
#include "Logger.hpp"

struct MyManager : public Age::Engine::PluginManagerBase < Age::Engine::FileInterface, MyManager >
{
protected:
	virtual bool onPluginLoaded(const std::string &name) override final
	{
		Singleton<AGE::Logger>::getInstance()->log(Age::Engine::Logger::Level::Normal, "Plugin ", name, " loaded correctly!");
		return true;
	}
};

int main(int argc, char *argv[])
{
	std::shared_ptr<Age::Engine::Engine> engine = Age::Engine::Engine::GetInstance(argc, argv);
	engine->initialize();
	return engine->run();
}
