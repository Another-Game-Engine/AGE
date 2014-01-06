#ifndef  MATERIALHH
# define MATERIALHH

#include <set>

#include "Utils/SmartPointer.hh"
#include <ResourceManager/Texture.hh>
#include <tiny_obj_loader.h>
#include <Utils/File.hpp>
#include <ResourceManager/ResourceManager.hh>
#include <core/AScene.hh>
#include <core/Engine.hh>

namespace Component
{
	class MeshRenderer;
};
class MaterialManager;

class Material
{
private:public:
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 transmittance;
	glm::vec3 emission;
	float shininess;
	SmartPointer<Resources::Texture> ambientTex;
	SmartPointer<Resources::Texture> diffuseTex;
	SmartPointer<Resources::Texture> specularTex;
	SmartPointer<Resources::Texture> normalTex;
	std::map<std::string, glm::vec2> paramVec2;
	std::map<std::string, glm::vec3> paramVec3;
	std::map<std::string, glm::vec4> paramVec4;
	std::map<std::string, glm::mat2> paramMat2;
	std::map<std::string, glm::mat3> paramMat3;
	std::map<std::string, glm::mat4> paramMat4;
	std::map<std::string, int> paramInt;
	std::map<std::string, float> paramFloat;

	Material(const std::string &name) :
		name(name),
		ambient(0),
		diffuse(0),
		specular(0),
		transmittance(0),
		emission(0),
		shininess(0),
		ambientTex(nullptr),
		diffuseTex(nullptr),
		specularTex(nullptr),
		normalTex(nullptr)
	{}

	virtual ~Material()
	{}

	void loadMtl(tinyobj::material_t &m, const File &file, Engine *engine)
	{
		ambient = glm::vec3(m.ambient[0], m.ambient[1], m.ambient[2]);
		diffuse = glm::vec3(m.diffuse[0], m.diffuse[1], m.diffuse[2]);
		specular = glm::vec3(m.specular[0], m.specular[1], m.specular[2]);
		transmittance = glm::vec3(m.transmittance[0], m.transmittance[1], m.transmittance[2]);
		emission = glm::vec3(m.emission[0], m.emission[1], m.emission[2]);
		shininess = m.shininess;
		if (m.ambient_texname.size() > 0)
		{
			auto path = file.getFolder() + m.ambient_texname;
			auto name = "texture:" + File(m.ambient_texname).getFileName();
			engine->getInstance<Resources::ResourceManager>().addResource(
				name,
				new Resources::Texture(),
				path);
			ambientTex = engine->getInstance<Resources::ResourceManager>().getResource(name);
		}
		if (m.diffuse_texname.size() > 0)
		{
			auto path = file.getFolder() + m.diffuse_texname;
			auto name = "texture:" + File(m.diffuse_texname).getFileName();
			engine->getInstance<Resources::ResourceManager>().addResource(
				name,
				new Resources::Texture(),
				path);
			diffuseTex = engine->getInstance<Resources::ResourceManager>().getResource(name);
		}
		if (m.specular_texname.size() > 0)
		{
			auto path = file.getFolder() + m.specular_texname;
			auto name = "texture:" + File(m.specular_texname).getFileName();
			engine->getInstance<Resources::ResourceManager>().addResource(
				name,
				new Resources::Texture(),
				path);
			specularTex = engine->getInstance<Resources::ResourceManager>().getResource(name);
		}
		if (m.normal_texname.size() > 0)
		{
			auto path = file.getFolder() + m.normal_texname;
			auto name = "texture:" + File(m.normal_texname).getFileName();
			engine->getInstance<Resources::ResourceManager>().addResource(
				name,
				new Resources::Texture(),
				path);
			normalTex = engine->getInstance<Resources::ResourceManager>().getResource(name);
		}
	}
	
	void setUniforms(OpenGLTools::UniformBuffer *buffer)
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
			glBindTexture(GL_TEXTURE_2D, ambientTex->getId());
		}
		if (diffuseTex != nullptr)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, diffuseTex->getId());
		}
		if (specularTex != nullptr)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, specularTex->getId());
		}
		if (normalTex != nullptr)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, normalTex->getId());
		}
	}

	Material &operator=(const Material &o)
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

	Material(const Material &o)
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

	const std::string &getName() const
	{
		return name;
	}
private:
};

#endif   //!MATERIALHH