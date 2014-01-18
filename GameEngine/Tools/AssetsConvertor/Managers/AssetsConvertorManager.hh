#ifndef   __ASSETS_CONVERTOR_MANAGER_HPP__
# define  __ASSETS_CONVERTOR_MANAGER_HPP__

#include <Utils/File.hpp>
#include <map>
#include <memory>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>

#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/ObjFile.hpp>

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
		_convertors.emplace(key, std::make_unique<T>(this));
	}

	bool load(const std::string filename, const std::string name);

	bool serializeData();
private:
	File _outputDirectory;
	std::map < std::string, std::shared_ptr<Basebase> > _files;
	std::map < std::size_t, std::unique_ptr<AConvertor> > _convertors;
};

#endif    //__ASSETS_CONVERTOR_MANAGER_HPP__