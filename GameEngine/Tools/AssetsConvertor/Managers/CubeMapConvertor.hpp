#ifndef   __CUBEMAP_CONVERTOR_HPP__
# define  __CUBEMAP_CONVERTOR_HPP__

#include <Managers/AssetsConvertorManager.hh>
#include <TinyObjLoader/tiny_obj_loader.h>
#include <MediaFiles/CubeMapFile.hpp>
#include <Managers/AConvertor.hh>
#include <Parsers/Parsers.hh>

class CubeMapConvertor : public AConvertor
{
public:
	CubeMapConvertor(AssetsConvertorManager *manager)
		: AConvertor(manager, std::set<std::string>({ "skybox" }))
	{
	}

	virtual ~CubeMapConvertor()
	{}

	virtual std::string setName(const File &file) const
	{
		auto obj = file.getShortFileName();
		obj = "skybox__" + obj;
		return obj;
	}

	virtual std::shared_ptr<AMediaFile> convert(const File &file)
	{
		//if (!file.exists())
		//	return std::shared_ptr<AMediaFile>(nullptr);
		std::string inputfile = file.getFullName();
		std::shared_ptr<CubeMapFile> res{ new CubeMapFile };
		res->px = std::static_pointer_cast<TextureFile>(_manager->load(file.getFullName() + "/posx.tga"));
		res->py = std::static_pointer_cast<TextureFile>(_manager->load(file.getFullName() + "/posy.tga"));
		res->pz = std::static_pointer_cast<TextureFile>(_manager->load(file.getFullName() + "/posz.tga"));
		res->nx = std::static_pointer_cast<TextureFile>(_manager->load(file.getFullName() + "/negx.tga"));
		res->ny = std::static_pointer_cast<TextureFile>(_manager->load(file.getFullName() + "/negy.tga"));
		res->nz = std::static_pointer_cast<TextureFile>(_manager->load(file.getFullName() + "/negz.tga"));
		return res;
	}
};

#endif // __CUBEMAP_CONVERTOR_HPP__