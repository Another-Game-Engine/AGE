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
#include <Entities/Entity.hh>
#include <Core/AScene.hh>
#include <MediaFiles/AssetsManager.hpp>


namespace Resources
{
	class Texture;
};

enum Shadow;
class Material;

namespace Component
{
	struct MeshRenderer : public Component::ComponentBase<MeshRenderer>
	{
		MeshRenderer();
		virtual ~MeshRenderer(void);
		MeshRenderer(MeshRenderer const &);
		MeshRenderer	&operator=(MeshRenderer const &);
		void init(std::shared_ptr<AMediaFile> file);
		void init(std::shared_ptr<ObjFile> file);
		virtual void reset();
		inline void setShader(const std::string &_shader) { shader = _shader; }
		void render(std::function<void(OpenGLTools::Shader&)> func = [](OpenGLTools::Shader &s){});
		void renderRaw();
		std::shared_ptr<ObjFile>	const &getMesh() const;

		//////
		////
		// Serialization

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
			// TODO @CESAR
			//mesh = std::static_pointer_cast<ObjFile>(_entity->getScene().lock()->getInstance<AssetsManager>()->loadFromFile(File(meshName)));
		}

		// !Serialization
		////
		//////

		std::shared_ptr<ObjFile>	mesh;
		std::string shader;
	private:
	};

}
