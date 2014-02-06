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

	void ShadowRenderer::render()
	{
		auto &renderer = _entity->getScene()->getEngine().getInstance<Renderer>();
		std::shared_ptr<OpenGLTools::UniformBuffer> perModelUniform(renderer.getUniform("PerModel"));
		auto shader = renderer.getShader(_shader);
		if (shader)
			shader->use();
		perModelUniform->setUniform("model", _entity->getGlobalTransform());
		perModelUniform->flushChanges();
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