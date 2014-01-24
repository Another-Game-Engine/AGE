#ifndef  __TGA_CONVERTOR_HPP__
#define  __TGA_CONVERTOR_HPP__

#include <Managers/AssetsConvertorManager.hh>
#include <TinyObjLoader/tiny_obj_loader.h>
#include <MediaFiles/TextureFile.hpp>
#include <Managers/AConvertor.hh>
#include <Parsers/Parsers.hh>

class TgaConvertor : public AConvertor
{
public:
	TgaConvertor(AssetsConvertorManager *manager)
		: AConvertor(manager, std::set<std::string>({ "tga" }))
	{
	}

	virtual ~TgaConvertor()
	{}

	virtual std::shared_ptr<AMediaFile> convert(const File file)
	{
		if (!file.exists())
			return std::shared_ptr<AMediaFile>(nullptr);
		std::string inputfile = file.getFullName();
		std::shared_ptr<TextureFile> res{ new TextureFile };
		res->datas = std::make_unique<GLbyte>(loadTGA(inputfile.c_str(), &res->width, &res->height, &res->components, &res->format));
		assert(res->datas != nullptr && "Error while loading TGA file.");
		return res;
	}
};

#endif   //__TGA_CONVERTOR_HPP__
