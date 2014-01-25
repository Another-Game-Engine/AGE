#ifndef   __AMEDIA_FILE_HPP__
# define  __AMEDIA_FILE_HPP__

#include <functional>
#include <Utils/File.hpp>

#include <cereal/cereal.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/portable_binary.hpp>

class AssetsManager;
struct ObjFile;
struct TextureFile;
struct MaterialFile;


struct AMediaFile
{
public:
	File path;
	std::string name;
	std::size_t type;
	static AssetsManager *manager;
	std::size_t childs;
public:
	AMediaFile() :
		childs(0)
	{
		manager = nullptr;
	}
	virtual ~AMediaFile(){}

	template <typename Archive>
	void serialize(std::ofstream &s)
	{
		Archive ar(s);
		ar(type);
		_serialize(ar);
	}

	template <typename Archive>
	static std::shared_ptr<AMediaFile> loadFromFile(const File &file)
	{
		assert(file.exists() == true && "File does not exist.");
		static constexpr std::size_t objType = typeid(ObjFile).hash_code();
		static constexpr std::size_t materialType = typeid(MaterialFile).hash_code();
		static constexpr std::size_t textureType = typeid(TextureFile).hash_code();
		std::size_t serializedFileType = 0;
		std::shared_ptr<AMediaFile> res{ nullptr };

		std::ifstream ifs(file.getFullName());
		Archive ar(ifs);
		ar(serializedFileType);
		switch (serializedFileType)
		{
		case objType:
			res = std::make_shared<ObjFile>();
			ar(static_cast<ObjFile&>(*res.get()));
			break;
		case materialType:
			res = std::make_shared<MaterialFile>();
			ar(static_cast<MaterialFile&>(*res.get()));
			break;
		case textureType:
			res = std::make_shared<TextureFile>();
			ar(static_cast<TextureFile&>(*res.get()));
			break;
		default:
			break;
		}
		assert(res != nullptr && "Unknown MediaFile type.");
		assert(manager != nullptr && "Media Manager is not set.");
		return res;
	}

	static void setManager(AssetsManager *_manager)
	{
		manager = _manager;
	}

	virtual void _serialize(cereal::JSONOutputArchive &ar) = 0;
	virtual void _serialize(cereal::BinaryOutputArchive &ar) = 0;
	virtual void _serialize(cereal::XMLOutputArchive &ar) = 0;
	virtual void _serialize(cereal::PortableBinaryOutputArchive &ar) = 0;
};

#endif    //__AMEDIA_FILE_HPP__