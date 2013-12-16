#include "MeshRenderer.hh"

#include "ResourceManager/Texture.hh"

#include "Core/Engine.hh"
#include <ResourceManager/Texture.hh>
#include <ResourceManager/ResourceManager.hh>

namespace Component
{
	MeshRenderer::MeshRenderer(Engine &engine, Handle &entity, std::string const &resource) :
		Component::ComponentBase<MeshRenderer>(engine, entity, "MeshComponent"),
		_mesh(_engine.getInstance<Resources::ResourceManager>().getResource(resource))
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
		SmartPointer<Resources::Texture> texture = _engine.getInstance<Resources::ResourceManager>().getResource(textureName);

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
}
