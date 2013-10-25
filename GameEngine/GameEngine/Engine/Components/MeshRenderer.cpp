#include "MeshRenderer.hh"

#include "ResourceManager/Texture.hh"

#include "Core/Engine.hh"

namespace Components
{

	MeshRenderer::MeshRenderer(std::string const &name, std::string const &resource) :
		AComponent(name),
		_mesh(GameEngine::instance()->resources().getResource(resource))
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

	void MeshRenderer::addTexture(const std::string &textureName)
	{
		_textures.push_back(GameEngine::instance()->resources().getResource(textureName));
	}

	void MeshRenderer::bindTextures() const
	{
		unsigned int c = 0;
		for (auto &i : _textures)
		{
			glActiveTexture(GL_TEXTURE0 + c++);
			glBindTexture(GL_TEXTURE_2D, i->getId());
		}
	}

	void MeshRenderer::unbindTextures() const
	{
		unsigned int c = 0;
		for (auto &i : _textures)
		{
			glActiveTexture(GL_TEXTURE0 + c++);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

}