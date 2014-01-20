#ifndef		MESHRENDERER_HH_
#define		MESHRENDERER_HH_

#include <map>
#include <set>

#include "Components/Component.hh"
#include "Utils/SmartPointer.hh"
#include "ResourceManager/SharedMesh.hh"
#include "OpenGL/Shader.hh"
#include <core/Renderer.hh>

namespace Resources
{
	class Texture;
};

class Material;

namespace Component
{

	class MeshRenderer : public Component::ComponentBase<MeshRenderer>
	{
	private:
		SmartPointer<Resources::SharedMesh>	_mesh;
		std::string _shader;
		std::vector<Material> _materials;

		MeshRenderer();
		MeshRenderer(MeshRenderer const &);
		MeshRenderer	&operator=(MeshRenderer const &);
	public:
		MeshRenderer(AScene *scene, Entity &entity);
		virtual ~MeshRenderer(void);
		void init(std::string const &resource);
		virtual void reset();
		void setShader(const std::string &shader) { _shader = shader; }
		void render();
		SmartPointer<Resources::SharedMesh>	const &getMesh() const;
		std::vector<Material> &getMaterials();
		Material *getMaterial(const std::string &name);
		Material *getMaterial(unsigned int index);
	};

}

#endif