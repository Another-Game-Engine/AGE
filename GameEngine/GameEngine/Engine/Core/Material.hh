#ifndef  __MATERIAL_HH__
# define __MATERIAL_HH__

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
private:
	std::string _name;
	Engine &_engine;
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;
	glm::vec3 _transmittance;
	glm::vec3 _emission;
	float _shininess;
	SmartPointer<Resources::Texture> _ambientTex;
	SmartPointer<Resources::Texture> _diffuseTex;
	SmartPointer<Resources::Texture> _specularTex;
	SmartPointer<Resources::Texture> _normalTex;
	std::map<std::string, glm::vec2> _paramVec2;
	std::map<std::string, glm::vec3> _paramVec3;
	std::map<std::string, glm::vec4> _paramVec4;
	std::map<std::string, glm::mat2> _paramMat2;
	std::map<std::string, glm::mat3> _paramMat3;
	std::map<std::string, glm::mat4> _paramMat4;
	std::map<std::string, int> _paramInt;
	std::map<std::string, float> _paramFloat;
public:
	Material(Engine &engine, const std::string &name) :
		_name(name),
		_engine(engine),
		_ambient(0),
		_diffuse(0),
		_specular(0),
		_transmittance(0),
		_emission(0),
		_shininess(0),
		_ambientTex(nullptr),
		_diffuseTex(nullptr),
		_specularTex(nullptr),
		_normalTex(nullptr)
	{}

	virtual ~Material()
	{}

	void loadMtl(tinyobj::material_t &m, const File &file)
	{
		_ambient = glm::vec3(m.ambient[0], m.ambient[1], m.ambient[2]);
		_diffuse = glm::vec3(m.diffuse[0], m.diffuse[1], m.diffuse[2]);
		_specular = glm::vec3(m.specular[0], m.specular[1], m.specular[2]);
		_transmittance = glm::vec3(m.transmittance[0], m.transmittance[1], m.transmittance[2]);
		_emission = glm::vec3(m.emission[0], m.emission[1], m.emission[2]);
		_shininess = m.shininess;
		if (m.ambient_texname.size() > 0)
		{
			auto path = file.getFolder() + m.ambient_texname;
			auto name = "texture:" + File(m.ambient_texname).getFileName();
			_engine.getInstance<Resources::ResourceManager>().addResource(
				name,
				new Resources::Texture(),
				path);
			_ambientTex = _engine.getInstance<Resources::ResourceManager>().getResource(name);
		}
		if (m.diffuse_texname.size() > 0)
		{
			auto path = file.getFolder() + m.diffuse_texname;
			auto name = "texture:" + File(m.diffuse_texname).getFileName();
			_engine.getInstance<Resources::ResourceManager>().addResource(
				name,
				new Resources::Texture(),
				path);
			_diffuseTex = _engine.getInstance<Resources::ResourceManager>().getResource(name);
		}
		if (m.specular_texname.size() > 0)
		{
			auto path = file.getFolder() + m.specular_texname;
			auto name = "texture:" + File(m.specular_texname).getFileName();
			_engine.getInstance<Resources::ResourceManager>().addResource(
				name,
				new Resources::Texture(),
				path);
			_specularTex = _engine.getInstance<Resources::ResourceManager>().getResource(name);
		}
		if (m.normal_texname.size() > 0)
		{
			auto path = file.getFolder() + m.normal_texname;
			auto name = "texture:" + File(m.normal_texname).getFileName();
			_engine.getInstance<Resources::ResourceManager>().addResource(
				name,
				new Resources::Texture(),
				path);
			_normalTex = _engine.getInstance<Resources::ResourceManager>().getResource(name);
		}
	}
	
	void setUniforms(OpenGLTools::UniformBuffer *buffer)
	{
		buffer->setUniform("ambient", _ambient);
		buffer->setUniform("diffuse", _diffuse);
		buffer->setUniform("specular", _specular);
		buffer->setUniform("transmittance", _transmittance);
		buffer->setUniform("emission", _emission);
		buffer->setUniform("shininess", _shininess);
		if (_ambientTex != nullptr)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _ambientTex->getId());
		}
		if (_diffuseTex != nullptr)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _diffuseTex->getId());
		}
		if (_specularTex != nullptr)
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, _specularTex->getId());
		}
		if (_normalTex != nullptr)
		{
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, _normalTex->getId());
		}
	}

private:
	Material(const Material &o);
	Material &operator=(const Material &o);
};

#endif   //!__MATERIAL_HH__