#ifndef		MESHRENDERER_HH_
#define		MESHRENDERER_HH_

#include <map>

#include "AComponent.hh"
#include "Utils/SmartPointer.hh"
#include "ResourceManager/SharedMesh.hh"

namespace Resources
{
	class Texture;
};

namespace Components
{

class MeshRenderer : public AComponent
{
private:
	typedef std::multimap<unsigned int, std::pair<std::string, SmartPointer<Resources::Texture> > > textureMap;
	typedef textureMap::iterator textureMapIt;

	SmartPointer<Resources::SharedMesh>	_mesh;

	std::string							_shader;
	textureMap                          _textures;

	MeshRenderer();
	MeshRenderer(MeshRenderer const &);
	MeshRenderer	&operator=(MeshRenderer const &);

public:
	MeshRenderer(std::string const &name, std::string const &resource);
	virtual ~MeshRenderer(void);

	virtual void	start();
	virtual void	update();
	virtual void	stop();

	bool				setShader(std::string const &name);
	std::string const	&getShader() const;

	void addTexture(const std::string &textureName, const std::string &name, unsigned int priority = 0);
	void removeTexture(const std::string &name);
	void bindTextures() const;
	void unbindTextures() const;

	SmartPointer<Resources::SharedMesh>	const &getMesh() const;
};

}

#endif