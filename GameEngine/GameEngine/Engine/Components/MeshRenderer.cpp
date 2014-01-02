#include "MeshRenderer.hh"

#include "ResourceManager/Texture.hh"

#include "Core/Engine.hh"
#include <ResourceManager/Texture.hh>
#include <ResourceManager/ResourceManager.hh>

namespace Component
{
	MeshRenderer::MeshRenderer(AScene *scene, Handle &entity) :
		Component::ComponentBase<MeshRenderer>(scene, entity, "MeshComponent"),
		_mesh(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}

	void MeshRenderer::init(std::string const &resource)
	{
		_mesh = _scene->getEngine().getInstance<Resources::ResourceManager>().getResource(resource);
	}

	SmartPointer<Resources::SharedMesh> const &MeshRenderer::getMesh() const
	{
		return (_mesh);
	}

	void MeshRenderer::reset()
	{
		_mesh = nullptr;
	}

	void MeshRenderer::render()
	{
		OpenGLTools::UniformBuffer *perModelUniform = _scene->getEngine().getInstance<Renderer>().getUniform("PerModel");
		auto shader = _scene->getEngine().getInstance<Renderer>().getShader(_shader);
		if (shader)
			shader->use();
		perModelUniform->setUniform("model", _entity->getGlobalTransform());
		perModelUniform->flushChanges();
		if (_mesh != nullptr)
			_mesh->draw();
	}

	//void MeshRenderer::addTexture(const std::string &textureName, unsigned int priority)
	//{
	//	SmartPointer<Resources::Texture> texture = _scene->getEngine().getInstance<Resources::ResourceManager>().getResource(textureName);

	//	for (textureMapIt it = _textures.begin(); it != _textures.end(); ++it)
	//	{
	//		if (it->first == priority)
	//			return;
	//	}
	//	_textures.insert(std::make_pair(priority, std::make_pair(textureName, texture)));
	//}

	//void MeshRenderer::removeTexture(unsigned int priority)
	//{
	//	for (textureMapIt it = _textures.begin(); it != _textures.end(); ++it)
	//	{
	//		if (it->first == priority)
	//		{
	//			_textures.erase(it);
	//			return;
	//		}
	//	}
	//}

	//void MeshRenderer::bindTextures(OpenGLTools::Shader *shader) const
	//{
	//	unsigned int c = 0;
	//	unsigned int offset = shader->getLayers().size();

	//	for (textureMap::const_iterator it = _textures.begin(); it != _textures.end(); ++it)
	//	{
	//		glActiveTexture(GL_TEXTURE0 + c + offset);
	//		glBindTexture(GL_TEXTURE_2D, it->second.second->getId());
	//		//			shader->bindActiveTexture("fTexture" + std::to_string(c), c + offset);
	//		++c;
	//	}
	//}

	//void MeshRenderer::unbindTextures() const
	//{
	//	unsigned int c = 0;
	//	for (textureMap::const_iterator it = _textures.begin(); it != _textures.end(); ++it)
	//	{
	//		glActiveTexture(GL_TEXTURE0 + c);
	//		glBindTexture(GL_TEXTURE_2D, 0);
	//		++c;
	//	}
	//	glActiveTexture(GL_TEXTURE0);
	//}
}
