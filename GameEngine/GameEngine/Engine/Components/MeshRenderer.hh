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
		std::vector<Material> &getMaterials();
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
		void serialize(Archive &ar)
		{
			ar(CEREAL_NVP(shader));
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