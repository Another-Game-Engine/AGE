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
#include <tiny_obj_loader.h>
#include <MediaFiles/TextureFile.hpp>

struct MaterialFile : public MediaFile<MaterialFile>
{
	MaterialFile() : MediaFile<MaterialFile>()
	{
		_type = MATERIAL;
	}

	virtual ~MaterialFile()
	{
	}

	MaterialFile(const MaterialFile &o)
		: MediaFile<MaterialFile>(o)
	{
		materials = o.materials;
	}

	MaterialFile &operator=(const MaterialFile &o)
	{
		if (&o != this)
		{
			materials = o.materials;
		}
		return *this;
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
		std::shared_ptr<TextureFile> ambientTex;
		std::shared_ptr<TextureFile> diffuseTex;
		std::shared_ptr<TextureFile> specularTex;
		std::shared_ptr<TextureFile> normalTex;
		std::map<std::string, glm::vec2> paramVec2;
		std::map<std::string, glm::vec3> paramVec3;
		std::map<std::string, glm::vec4> paramVec4;
		std::map<std::string, glm::mat2> paramMat2;
		std::map<std::string, glm::mat3> paramMat3;
		std::map<std::string, glm::mat4> paramMat4;
		std::map<std::string, int> paramInt;
		std::map<std::string, float> paramFloat;
		MaterialFile *file;
	public:

		Material() :
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

		template <typename Archive>
		void save(Archive &ar) const
		{
			ar(name, ambient, diffuse, specular, transmittance, emission, shininess, paramVec2, paramVec3, paramVec4, paramMat2, paramMat3, paramMat4, paramInt, paramFloat);
			std::string a, b, c, d;
			a = ambientTex != nullptr ? ambientTex->path.getFullName() : "NULL";
			b = diffuseTex != nullptr ? diffuseTex->path.getFullName() : "NULL";
			c = specularTex != nullptr ? specularTex->path.getFullName() : "NULL";
			d = normalTex != nullptr ? normalTex->path.getFullName() : "NULL";
			ar(a, b, c, d);
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			ar(name, ambient, diffuse, specular, transmittance, emission, shininess, paramVec2, paramVec3, paramVec4, paramMat2, paramMat3, paramMat4, paramInt, paramFloat);
			std::string a, b, c, d;
			ar(a, b, c, d);
			if (a != "NULL")
				ambientTex = std::static_pointer_cast<TextureFile>(_manager->loadFromFile<Archive>(File(a)));
			if (b != "NULL")
				diffuseTex = std::static_pointer_cast<TextureFile>(_manager->loadFromFile<Archive>(File(b)));
			if (c != "NULL")
				specularTex = std::static_pointer_cast<TextureFile>(_manager->loadFromFile<Archive>(File(c)));
			if (d != "NULL")
				normalTex = std::static_pointer_cast<TextureFile>(_manager->loadFromFile<Archive>(File(d)));
		}

	};

	std::vector<Material> materials;

	template <typename Archive>
	AMediaFile *unserialize(Archive &ar)
	{
		AMediaFile *res = new ObjFile();
		res->manager = manager;
		ar(static_cast<MaterialFile&>(*res));
		return res;
	}

	template <typename Archive>
	void save(Archive &ar) const
	{
		ar(cereal::make_nvp("material_size", materials.size()));
		ar(materials);
	}

	template <typename Archive>
	void load(Archive &ar)
	{
		std::size_t size;
		ar(size);
		materials.resize(size);
		for (auto &e : materials)
			e.file = this;
		ar(materials);
	}

};


#endif    //__MATERIAL_FILE_HPP__