#pragma once

#include <map>
#include <set>

#include <Components/Component.hh>
#include <MediaFiles/TextureFile.hpp>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/CubeMapFile.hpp>
#include <Render/Shader.hh>
#include <cereal/types/string.hpp>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>
#include <MediaFiles/AssetsManager.hpp>
#include "Behaviors/Cullable.hpp"

namespace Resources
{
	class Texture;
};

enum Shadow;
class Material;
class Renderer;

namespace Component
{

	struct MeshRenderer : public Component::ComponentBase<MeshRenderer>, public AGE::ComponentBehavior::Cullable
	{
		MeshRenderer();
		virtual ~MeshRenderer(void);
		MeshRenderer(MeshRenderer &&o);
		MeshRenderer &operator=(MeshRenderer &&o);
		void init(AScene *, std::shared_ptr<AGE::MeshInstance> file);
		virtual void reset(AScene *);

		//////
		////
		// Serialization

		template <typename Archive>
		void save(Archive &ar) const
		{
			//ar(CEREAL_NVP(shader));
			//std::string meshName = mesh->path.getFullName();
			//ar(cereal::make_nvp("meshName", meshName));
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			//ar(shader);
			//std::string meshName;
			//ar(meshName);
			// TODO @CESAR
			//mesh = std::static_pointer_cast<ObjFile>(_entity->getScene().lock()->getInstance<AssetsManager>()->loadFromFile(File(meshName)));
		}

		// !Serialization
		////
		//////

	private:
		MeshRenderer(MeshRenderer const &);
		MeshRenderer	&operator=(MeshRenderer const &);
	};

}
