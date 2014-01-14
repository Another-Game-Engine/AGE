#ifndef   __ASSETS_CONVERTOR_MANAGER_HPP__
# define  __ASSETS_CONVERTOR_MANAGER_HPP__

#include <Utils/File.hpp>
#include <map>
#include <memory>

struct AMediaFile;
class AConvertor;

class AssetsConvertorManager
{
public:
	AssetsConvertorManager();
	~AssetsConvertorManager();

	void setOutputDirectory(const std::string directory);

	template <typename T>
	void registerConvertor()
	{
		std::size_t key = typeid(T).hash_code();
		if (_convertors.find(key) != std::end(_convertors))
			return;
		_convertors.insert(std::make_pair(key, new T(std::make_shared(this))));
	}

	bool load(const std::string filename, const std::string name);
private:
	File _outputDirectory;
	std::map < std::string, std::auto_ptr<AMediaFile> > _files;
	std::map < std::size_t, std::unique_ptr<AConvertor> > _convertors;
};

#endif    //__ASSETS_CONVERTOR_MANAGER_HPP__