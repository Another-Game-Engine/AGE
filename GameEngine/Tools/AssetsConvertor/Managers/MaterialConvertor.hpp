#ifndef  __MATERIAL_CONVERTOR_HH__
# define __MATERIAL_CONVERTOR_HH__

#include <Managers/AssetsConvertorManager.hh>
#include <TinyObjLoader/tiny_obj_loader.h>
#include <MediaFiles/MaterialFile.hpp>
#include <Managers/AConvertor.hh>

class AssetsConvertorManager;

class MaterialConvertor : public AConvertor
{
public:
	MaterialConvertor(AssetsConvertorManager *manager)
		: AConvertor(manager, std::set<std::string>({ "mtl" }))
	{
	}

	virtual ~MaterialConvertor()
	{}

	virtual std::shared_ptr<AMediaFile> convert(const File file)
	{
		File fileMaterial(file.getFullName());
		if (!fileMaterial.exists())
			return std::shared_ptr<AMediaFile>(nullptr);

		std::string inputfile = fileMaterial.getFullName();
		std::vector<tinyobj::shape_t> shapes;
		std::string err = tinyobj::LoadObj(shapes, inputfile.c_str(), fileMaterial.getFolder().c_str());
		assert(err.empty() && "Tiny Obj error loading file.");

		std::shared_ptr<MaterialFile> material{ new MaterialFile };
		material->materials.resize(shapes.size());

		for (auto i = 0; i < shapes.size(); ++i)
		{
			auto &mesh = shapes[i];
			auto &m = mesh.material;
			material->materials[i].loadMtl(m, fileMaterial, _manager);
		}
		return material;
	}
};

#endif  //__MATERIAL_CONVERTOR_HH__