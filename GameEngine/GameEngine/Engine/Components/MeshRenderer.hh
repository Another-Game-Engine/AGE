#ifndef		MESHRENDERER_HH_
#define		MESHRENDERER_HH_

#include <map>
#include <set>

#include "Components/Component.hh"
#include "Utils/SmartPointer.hh"
#include "ResourceManager/SharedMesh.hh"
#include "OpenGL/Shader.hh"

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
		typedef std::map<unsigned int, std::pair<std::string, SmartPointer<Resources::Texture> > > textureMap;
		typedef textureMap::iterator textureMapIt;

		SmartPointer<Resources::SharedMesh>	_mesh;

		textureMap                          _textures;

		MeshRenderer();
		MeshRenderer(MeshRenderer const &);
		MeshRenderer	&operator=(MeshRenderer const &);
	public:
		MeshRenderer(AScene *scene, Handle &entity);
		virtual ~MeshRenderer(void);
		void init(std::string const &resource);
		virtual void reset();

		void addTexture(const std::string &textureName, unsigned int priority = 0);
		void removeTexture(unsigned int priority);
		void bindTextures(OpenGLTools::Shader *shader) const;
		void unbindTextures() const;

		void			setNext(MeshRenderer *next);
		MeshRenderer	*getNext() const;

		SmartPointer<Resources::SharedMesh>	const &getMesh() const;
	};

}

#endif