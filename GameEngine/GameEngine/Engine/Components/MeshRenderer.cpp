#include "MeshRenderer.hh"

#include "Core/Engine.hh"

namespace Component
{
	MeshRenderer::MeshRenderer() :
		Component::ComponentBase<MeshRenderer>(),
		mesh(nullptr)
	{
	}

	MeshRenderer::~MeshRenderer(void)
	{
	}

	void MeshRenderer::init(std::shared_ptr<AMediaFile> r)
	{
		mesh = std::static_pointer_cast<ObjFile>(r);
	}

	void MeshRenderer::init(std::shared_ptr<ObjFile> r)
	{
		mesh = r;
	}

	std::shared_ptr<ObjFile> const &MeshRenderer::getMesh() const
	{
		return (mesh);
	}

	void MeshRenderer::reset()
	{
		mesh = nullptr;
	}

	void MeshRenderer::render(GLuint shadowTex)
	{
		OpenGLTools::UniformBuffer *perModelUniform = _entity->getScene()->getEngine().getInstance<Renderer>().getUniform("PerModel");
		OpenGLTools::UniformBuffer *materialUniform = _entity->getScene()->getEngine().getInstance<Renderer>().getUniform("MaterialBasic");
		auto s = _entity->getScene()->getEngine().getInstance<Renderer>().getShader(shader);
		if (s)
			s->use();
		perModelUniform->setUniform("model", _entity->getGlobalTransform());
		perModelUniform->flushChanges();
		for (unsigned int i = 0; i < mesh->material->materials.size(); ++i)
		{
			mesh->material->materials[i].setUniforms(materialUniform);
			if (shadowTex != 0)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, shadowTex);
			}
			materialUniform->flushChanges();
			mesh->geometries[i].buffer.draw(GL_TRIANGLES);
		}
	}
}
