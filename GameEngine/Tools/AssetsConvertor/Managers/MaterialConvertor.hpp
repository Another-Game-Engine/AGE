#ifndef  __MATERIAL_CONVERTOR_HH__
# define __MATERIAL_CONVERTOR_HH__

#include <Managers/AssetsConvertorManager.hh>
#include <TinyObjLoader/tiny_obj_loader.h>
#include <MediaFiles/MaterialFile.hpp>
#include <Managers/AConvertor.hh>
#include <MediaFiles/TextureFile.hpp>

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

	virtual std::string setName(const File &file) const
	{
		auto obj = file.getShortFileName();
		obj = "material__" + obj;
		return obj;
	}

	virtual std::shared_ptr<AMediaFile> convert(const File &file)
	{
		auto obj = file.getFullName().substr(0, file.getFullName().find_last_of("."));
		obj += ".obj";
		File fileMaterial(obj);
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
			loadMtl(m, file, material->materials[i]);
		}
		return material;
	}

	void loadMtl(tinyobj::material_t &m,
		const File &file,
		MaterialFile::Material &material)
	{
		material.name = m.name;
		material.ambient = glm::vec3(m.ambient[0], m.ambient[1], m.ambient[2]);
		material.diffuse = glm::vec3(m.diffuse[0], m.diffuse[1], m.diffuse[2]);
		material.specular = glm::vec3(m.specular[0], m.specular[1], m.specular[2]);
		material.transmittance = glm::vec3(m.transmittance[0], m.transmittance[1], m.transmittance[2]);
		material.emission = glm::vec3(m.emission[0], m.emission[1], m.emission[2]);
		material.shininess = m.shininess;
		if (m.ambient_texname.size() > 0)
		{
			auto path = file.getFolder() + m.ambient_texname;
			material.ambientTex = std::static_pointer_cast<TextureFile>(_manager->load(path));
		}
		if (m.diffuse_texname.size() > 0)
		{
			auto path = file.getFolder() + m.diffuse_texname;
			material.diffuseTex = std::static_pointer_cast<TextureFile>(_manager->load(path));
		}
		if (m.specular_texname.size() > 0)
		{
			auto path = file.getFolder() + m.specular_texname;
			material.specularTex = std::static_pointer_cast<TextureFile>(_manager->load(path));
		}
		if (m.normal_texname.size() > 0)
		{
			auto path = file.getFolder() + m.normal_texname;
			material.normalTex = std::static_pointer_cast<TextureFile>(_manager->load(path));
		}
	}

};

#endif  //__MATERIAL_CONVERTOR_HH__