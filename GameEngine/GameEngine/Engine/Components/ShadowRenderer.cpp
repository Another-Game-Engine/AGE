#include "ShadowRenderer.hpp"

namespace Component
{
	ShadowRenderer::ShadowRenderer()
		: _mesh(nullptr),
		_shader("")
	{
	}

	ShadowRenderer::~ShadowRenderer()
	{
	}

	void ShadowRenderer::init(std::shared_ptr<AMediaFile> const &media)
	{
		_mesh = std::static_pointer_cast<ObjFile>(media);
	}

	void ShadowRenderer::init(std::shared_ptr<ObjFile> const &obj)
	{
		_mesh = obj;
	}

	void ShadowRenderer::setShader(std::string const &s)
	{
		_shader = s;
	}

	void ShadowRenderer::reset()
	{
		_mesh = nullptr;
		_shader = "";
	}

	void ShadowRenderer::render(glm::mat4 const &VPLight)
	{
		glm::mat4 depthMVP = VPLight * _entity->getGlobalTransform();
		OpenGLTools::UniformBuffer *lightMVP = _entity->getScene()->getEngine().getInstance<Renderer>().getUniform("Light");
		auto shader = _entity->getScene()->getEngine().getInstance<Renderer>().getShader(_shader);
		if (shader)
			shader->use();
		lightMVP->setUniform("lightMVP", glm::mat4(0)/*depthMVP*/);
		lightMVP->flushChanges();
		for (unsigned int i = 0; i < _mesh->material->materials.size(); ++i)
		{
			_mesh->geometries[i].buffer.draw(GL_TRIANGLES);
		}
	}

	std::shared_ptr<ObjFile> const &ShadowRenderer::getMesh() const
	{
		return (_mesh);
	}
}