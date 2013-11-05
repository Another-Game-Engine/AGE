#include "MeshRenderer.hh"

#include "ResourceManager/Texture.hh"

#include "Core/Engine.hh"

namespace Components
{

	MeshRenderer::MeshRenderer(std::string const &name, std::string const &resource) :
		AComponent(name),
		_mesh(GameEngine::instance()->resources().getResource(resource)),
		_next(NULL)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}

	void	MeshRenderer::start()
	{
	}

	void	MeshRenderer::update()
	{
		GameEngine::instance()->renderer().addToRenderQueue(this);
	}

	void	MeshRenderer::stop()
	{
	}

	bool	MeshRenderer::setShader(std::string const &name)
	{
		_shader = name;
		return (true);
	}

	std::string const		&MeshRenderer::getShader() const
	{
		return (_shader);
	}

	SmartPointer<Resources::SharedMesh> const &MeshRenderer::getMesh() const
	{
		return (_mesh);
	}

	void MeshRenderer::addTexture(const std::string &textureName, const std::string &name, unsigned int priority)
	{
		SmartPointer<Resources::Texture> texture = GameEngine::instance()->resources().getResource(textureName);

		for (textureMapIt it = _textures.begin(); it != _textures.end(); ++it)
		{
			if (it->second.first == name)
				return;
		}
		_textures.insert(std::make_pair(priority, std::make_pair(name, texture)));
	}

	void MeshRenderer::removeTexture(const std::string &name)
	{
		for (textureMapIt it = _textures.begin(); it != _textures.end(); ++it)
		{
			if (it->second.first == name)
			{
				_textures.erase(it);
				return;
			}
		}
	}

	void MeshRenderer::bindTextures() const
	{
		unsigned int c = 0;
		for (textureMap::const_iterator it = _textures.begin(); it != _textures.end(); ++it)
		{
			glActiveTexture(GL_TEXTURE0 + c);
			glBindTexture(GL_TEXTURE_2D, it->second.second->getId());
			++c;
		}
	}

	void MeshRenderer::unbindTextures() const
	{
		unsigned int c = 0;
		for (textureMap::const_iterator it = _textures.begin(); it != _textures.end(); ++it)
		{
			glActiveTexture(GL_TEXTURE0 + c);
			glBindTexture(GL_TEXTURE_2D, 0);
			++c;
		}
		glActiveTexture(GL_TEXTURE0);
	}

	void			MeshRenderer::setNext(MeshRenderer *next)
	{
		_next = next;
	}

	MeshRenderer	*MeshRenderer::getNext() const
	{
		return (_next);
	}

	void MeshRenderer::addMaterial(SmartPointer<Material> material)
	{
		material->addObject(this);
		_materials.insert(material->getName());
	}

	void MeshRenderer::removeMaterial(SmartPointer<Material> material)
	{
		// be carefull when iterating on it
		material->removeObject(this);
		_materials.erase(material->getName());
	}

	void MeshRenderer::addMaterial(const std::string &material)
	{
		SmartPointer<Material> m = GameEngine::instance()->renderer().getMaterialManager().getMaterial(material);
		if (!m.get())
			return;
		m->addObject(this);
		_materials.insert(m->getName());
	}

	void MeshRenderer::removeMaterial(const std::string &material)
	{
		SmartPointer<Material> m = GameEngine::instance()->renderer().getMaterialManager().getMaterial(material);
		if (!m.get())
			return;
		m->removeObject(this);
		_materials.erase(m->getName());
	}
}