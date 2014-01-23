#ifndef		MESHRENDERER_HH_
#define		MESHRENDERER_HH_

#include <map>
#include <set>

#include "Components/Component.hh"
#include "Utils/SmartPointer.hh"
#include "ResourceManager/SharedMesh.hh"
#include "OpenGL/Shader.hh"
#include <core/Renderer.hh>
#include <cereal/types/string.hpp>

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
		void init(std::string const &resource);
		virtual void reset();
		inline void setShader(const std::string &_shader) { shader = _shader; }
		void render();
		SmartPointer<Resources::SharedMesh>	const &getMesh() const;
		std::vector<SmartPointer<Material>> &getMaterials();
		Material *getMaterial(const std::string &name);
		Material *getMaterial(unsigned int index);

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
			std::string meshName = mesh->getName();
			ar(cereal::make_nvp("meshName", meshName));
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			ar(shader);
			std::string meshName;
			ar(meshName);
			mesh = _entity->getScene()->getEngine().getInstance<Resources::ResourceManager>().getResource(meshName);
		}

		// !Serialization
		////
		//////

		SmartPointer<Resources::SharedMesh>	mesh;
		std::string shader;
		std::vector<Material> materials;
	private:
		MeshRenderer(MeshRenderer const &);
		MeshRenderer	&operator=(MeshRenderer const &);
	};

}

#endif