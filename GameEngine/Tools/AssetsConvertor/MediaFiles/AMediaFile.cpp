#include <MediaFiles/AMediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/TextureFile.hpp>

AssetsManager *AMediaFile::_manager = nullptr;

template <typename Archive>
static std::shared_ptr<AMediaFile> AMediaFile::loadFromFile(const File &file)
{
	assert(file.exists() == true && "File does not exist.");
	MEDIA_TYPE serializedFileType = UNKNOWN;
	std::shared_ptr<AMediaFile> res{ nullptr };

	std::ifstream ifs(file.getFullName());
	Archive ar(ifs);
	ar(serializedFileType);
	switch (serializedFileType)
	{
	case OBJ:
		res = std::make_shared<ObjFile>();
		ar(static_cast<ObjFile&>(*res.get()));
		break;
	case MATERIAL:
		res = std::make_shared<MaterialFile>();
		ar(static_cast<MaterialFile&>(*res.get()));
		break;
	case TEXTURE:
		res = std::make_shared<TextureFile>();
		ar(static_cast<TextureFile&>(*res.get()));
		break;
	default:
		break;
	}
	assert(res != nullptr && "Unknown MediaFile type.");
	assert(_manager != nullptr && "Media Manager is not set.");
	_manager->add(res);
	return res;
}