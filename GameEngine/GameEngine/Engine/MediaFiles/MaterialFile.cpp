#include "MaterialFile.hpp"
#include "AssetsManager.hpp"
#include "ObjFile.hpp"

MaterialFile::MaterialFile() : MediaFile<MaterialFile>()
{
	_type = MATERIAL;
}

MaterialFile::~MaterialFile()
{
}

MaterialFile::MaterialFile(const MaterialFile &o)
: MediaFile<MaterialFile>(o)
{
	materials = o.materials;
}

MaterialFile &MaterialFile::operator=(const MaterialFile &o)
{
	if (&o != this)
	{
		materials = o.materials;
	}
	return *this;
}


MaterialFile::Material::Material() :
name("noname"),
ambient(0),
diffuse(0),
specular(0),
transmittance(0),
emission(0),
shininess(0),
ambientTex(nullptr),
diffuseTex(nullptr),
specularTex(nullptr),
normalTex(nullptr),
file(nullptr)
{}

void MaterialFile::Material::setUniforms(std::shared_ptr<OpenGLTools::UniformBuffer> buffer)
{
	buffer->setUniform("ambient", ambient);
	buffer->setUniform("diffuse", diffuse);
	buffer->setUniform("specular", specular);
	buffer->setUniform("transmittance", transmittance);
	buffer->setUniform("emission", emission);
	buffer->setUniform("shininess", shininess);
	if (ambientTex != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, ambientTex->getTexture()->getId());
		glBindTexture(GL_TEXTURE_2D, ambientTex->getId());
	}
	if (diffuseTex != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, diffuseTex->getTexture()->getId());
		glBindTexture(GL_TEXTURE_2D, diffuseTex->getId());
	}
	if (specularTex != nullptr)
	{
		glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, specularTex->getTexture()->getId());
		glBindTexture(GL_TEXTURE_2D, specularTex->getId());
	}
	if (normalTex != nullptr)
	{
		glActiveTexture(GL_TEXTURE3);
		//glBindTexture(GL_TEXTURE_2D, normalTex->getTexture()->getId());
		glBindTexture(GL_TEXTURE_2D, normalTex->getId());
	}
}

MaterialFile::Material &MaterialFile::Material::operator=(const MaterialFile::Material &o)
{
	name = o.name;
	ambient = o.ambient;
	diffuse = o.diffuse;
	specular = o.specular;
	transmittance = o.transmittance;
	emission = o.emission;
	shininess = o.shininess;
	ambientTex = o.ambientTex;
	diffuseTex = o.diffuseTex;
	specularTex = o.specularTex;
	normalTex = o.normalTex;
	return *this;
}

MaterialFile::Material::Material(const MaterialFile::Material &o)
{
	name = o.name;
	ambient = o.ambient;
	diffuse = o.diffuse;
	specular = o.specular;
	transmittance = o.transmittance;
	emission = o.emission;
	shininess = o.shininess;
	ambientTex = o.ambientTex;
	diffuseTex = o.diffuseTex;
	specularTex = o.specularTex;
	normalTex = o.normalTex;
}

void MaterialFile::Material::save(cereal::PortableBinaryOutputArchive &ar) const
{
	ar(name, ambient, diffuse, specular, transmittance, emission, shininess, paramVec2, paramVec3, paramVec4, paramMat2, paramMat3, paramMat4, paramInt, paramFloat);
	std::string a, b, c, d;
	a = ambientTex != nullptr ? ambientTex->path.getFullName() : "NULL";
	b = diffuseTex != nullptr ? diffuseTex->path.getFullName() : "NULL";
	c = specularTex != nullptr ? specularTex->path.getFullName() : "NULL";
	d = normalTex != nullptr ? normalTex->path.getFullName() : "NULL";
	ar(a, b, c, d);
}

void MaterialFile::Material::load(cereal::PortableBinaryInputArchive &ar)
{
	ar(name, ambient, diffuse, specular, transmittance, emission, shininess, paramVec2, paramVec3, paramVec4, paramMat2, paramMat3, paramMat4, paramInt, paramFloat);
	std::string a, b, c, d;
	auto manager = _dpyManager.lock()->getInstance<AssetsManager>();
	ar(a, b, c, d);
	if (a != "NULL")
		ambientTex = std::static_pointer_cast<TextureFile>(manager->loadFromFile(File(a)));
	if (b != "NULL")
		diffuseTex = std::static_pointer_cast<TextureFile>(manager->loadFromFile(File(b)));
	if (c != "NULL")
		specularTex = std::static_pointer_cast<TextureFile>(manager->loadFromFile(File(c)));
	if (d != "NULL")
		normalTex = std::static_pointer_cast<TextureFile>(manager->loadFromFile(File(d)));
}



//AMediaFile *MaterialFile::unserialize(cereal::PortableBinaryInputArchive &ar)
//{
//	AMediaFile *res = new ObjFile();
//	res->manager = manager;
//	ar(static_cast<MaterialFile&>(*res));
//	return res;
//}

void MaterialFile::save(cereal::PortableBinaryOutputArchive &ar) const
{
	ar(cereal::make_nvp("material_size", materials.size()));
	ar(materials);
}

void MaterialFile::load(cereal::PortableBinaryInputArchive &ar)
{
	std::size_t size;
	ar(size);
	materials.resize(size);
	for (auto &e : materials)
	{
		e.file = this;
		e._dpyManager = _dpyManager;
	}
	ar(materials);
}