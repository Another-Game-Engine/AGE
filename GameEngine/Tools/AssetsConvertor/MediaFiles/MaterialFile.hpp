#ifndef   __MATERIAL_FILE_HPP__
# define  __MATERIAL_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <vector>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/GlmSerialization.hpp>
#include <OpenGL/UniformBuffer.hh>

struct MaterialFile : public MediaFile<MaterialFile>
{
	MaterialFile() : MediaFile<MaterialFile>()
	{
	}

	virtual ~MaterialFile()
	{
	}

	struct Material
	{
		std::string name;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 transmittance;
		glm::vec3 emission;
		float shininess;
		std::string ambientTex;
		std::string diffuseTex;
		std::string specularTex;
		std::string normalTex;
		std::map<std::string, glm::vec2> paramVec2;
		std::map<std::string, glm::vec3> paramVec3;
		std::map<std::string, glm::vec4> paramVec4;
		std::map<std::string, glm::mat2> paramMat2;
		std::map<std::string, glm::mat3> paramMat3;
		std::map<std::string, glm::mat4> paramMat4;
		std::map<std::string, int> paramInt;
		std::map<std::string, float> paramFloat;

		Material() :
			name("noname"),
			ambient(0),
			diffuse(0),
			specular(0),
			transmittance(0),
			emission(0),
			shininess(0),
			ambientTex("NULL"),
			diffuseTex("NULL"),
			specularTex("NULL"),
			normalTex("NULL")
		{}

		void loadMtl(tinyobj::material_t &m, const File &file, AssetsConvertorManager *manager)
		{
			name = m.name;
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
				// GET TEXTURE
				//engine->getInstance<Resources::ResourceManager>().addResource(
				//	name,
				//	new Resources::Texture(),
				//	path);
				//ambientTex = engine->getInstance<Resources::ResourceManager>().getResource(name);
			}
			if (m.diffuse_texname.size() > 0)
			{
				auto path = file.getFolder() + m.diffuse_texname;
				auto name = "texture:" + File(m.diffuse_texname).getFileName();
				// GET TEXTURE
				//engine->getInstance<Resources::ResourceManager>().addResource(
				//	name,
				//	new Resources::Texture(),
				//	path);
				//diffuseTex = engine->getInstance<Resources::ResourceManager>().getResource(name);
			}
			if (m.specular_texname.size() > 0)
			{
				auto path = file.getFolder() + m.specular_texname;
				auto name = "texture:" + File(m.specular_texname).getFileName();
				// GET TEXTURE
				//engine->getInstance<Resources::ResourceManager>().addResource(
				//	name,
				//	new Resources::Texture(),
				//	path);
				//specularTex = engine->getInstance<Resources::ResourceManager>().getResource(name);
			}
			if (m.normal_texname.size() > 0)
			{
				auto path = file.getFolder() + m.normal_texname;
				auto name = "texture:" + File(m.normal_texname).getFileName();
				// GET TEXTURE
				//engine->getInstance<Resources::ResourceManager>().addResource(
				//	name,
				//	new Resources::Texture(),
				//	path);
				//normalTex = engine->getInstance<Resources::ResourceManager>().getResource(name);
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
			//if (ambientTex != nullptr)
			//{
			//	glActiveTexture(GL_TEXTURE0);
			//	glBindTexture(GL_TEXTURE_2D, ambientTex->getId());
			//}
			//if (diffuseTex != nullptr)
			//{
			//	glActiveTexture(GL_TEXTURE1);
			//	glBindTexture(GL_TEXTURE_2D, diffuseTex->getId());
			//}
			//if (specularTex != nullptr)
			//{
			//	glActiveTexture(GL_TEXTURE2);
			//	glBindTexture(GL_TEXTURE_2D, specularTex->getId());
			//}
			//if (normalTex != nullptr)
			//{
			//	glActiveTexture(GL_TEXTURE3);
			//	glBindTexture(GL_TEXTURE_2D, normalTex->getId());
			//}
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

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(name, ambient, diffuse, specular, transmittance, emission, shininess, ambientTex, diffuseTex, specularTex, normalTex, paramVec2, paramVec3, paramVec4, paramMat2, paramMat3, paramMat4, paramInt, paramFloat);
		}
	};

	std::vector<Material> materials;

	template <typename Archive>
	AMediaFile *unserialize(Archive &ar)
	{
		AMediaFile *res = new ObjFile();
		ar(static_cast<MaterialFile&>(*res));
		return res;
	}

	template <typename Archive>
	void serialize(Archive &ar)
	{
		ar(materials);
	}
};


#endif    //__MATERIAL_FILE_HPP__