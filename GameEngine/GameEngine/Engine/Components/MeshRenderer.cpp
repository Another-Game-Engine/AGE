#include "MeshRenderer.hh"

#include "ResourceManager/Texture.hh"

#include "Core/Engine.hh"

namespace Component
{

	MeshRenderer::MeshRenderer(std::string const &name, std::string const &resource) :
		Component::ComponentBase<MeshRenderer>(name),
		_mesh(GameEngine::instance()->resources().getResource(resource))
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}

	SmartPointer<Resources::SharedMesh> const &MeshRenderer::getMesh() const
	{
		return (_mesh);
	}

	void MeshRenderer::addTexture(const std::string &textureName, unsigned int priority)
	{
		SmartPointer<Resources::Texture> texture = GameEngine::instance()->resources().getResource(textureName);

		for (textureMapIt it = _textures.begin(); it != _textures.end(); ++it)
		{
			if (it->first == priority)
				return;
		}
		_textures.insert(std::make_pair(priority, std::make_pair(textureName, texture)));
	}

	void MeshRenderer::removeTexture(unsigned int priority)
	{
		for (textureMapIt it = _textures.begin(); it != _textures.end(); ++it)
		{
			if (it->first == priority)
			{
				_textures.erase(it);
				return;
			}
		}
	}

	void MeshRenderer::bindTextures(OpenGLTools::Shader *shader) const
	{
		unsigned int c = 0;
		unsigned int offset = shader->getLayers().size();

		for (textureMap::const_iterator it = _textures.begin(); it != _textures.end(); ++it)
		{
			glActiveTexture(GL_TEXTURE0 + c + offset);
			glBindTexture(GL_TEXTURE_2D, it->second.second->getId());
//			shader->bindActiveTexture("fTexture" + std::to_string(c), c + offset);
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