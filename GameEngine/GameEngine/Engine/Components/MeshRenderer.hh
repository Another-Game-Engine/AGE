#pragma once

#include <map>
#include <set>

#include <Components/Component.hh>
#include <MediaFiles/TextureFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/CubeMapFile.hpp>
#include "OpenGL/Shader.hh"
#include <core/Renderer.hh>
#include <cereal/types/string.hpp>
#include <Entities/EntityData.hh>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>
#include <MediaFiles/AssetsManager.hpp>

namespace Resources
{
	class Texture;
};

class Material;

namespace Component
{
	struct MeshRenderer : public Component::ComponentBase<MeshRenderer>
	{
		MeshRenderer();
		virtual ~MeshRenderer(void);
		void init(std::shared_ptr<AMediaFile> file);
		void init(std::shared_ptr<ObjFile> file);
		virtual void reset();
		inline void setShader(const std::string &_shader) { shader = _shader; }
		void render(bool shadow, GLuint shadowTex, glm::mat4 const &lightVP);
		void render_shadowmap(GLuint shadowTex, glm::mat4 const &lightVP);
		void setShaderShadowMap(std::string const &shaderShadow);
		std::string const &getShaderShadowMap() const;
		std::shared_ptr<ObjFile>	const &getMesh() const;

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new MeshRenderer();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void save(Archive &ar) const
		{
			ar(CEREAL_NVP(shader));
			std::string meshName = mesh->path.getFullName();
			ar(cereal::make_nvp("meshName", meshName));
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			ar(shader);
			std::string meshName;
			ar(meshName);
			mesh = std::static_pointer_cast<ObjFile>(_entity->getScene()->getInstance<AssetsManager>()->loadFromFile<cereal::BinaryInputArchive>(File(meshName)));
		}

		// !Serialization
		////
		//////

		std::shared_ptr<ObjFile>	mesh;
		std::string shader;
	private:
		std::string _shaderShadow;
		MeshRenderer(MeshRenderer const &);
		MeshRenderer	&operator=(MeshRenderer const &);
	};

}
