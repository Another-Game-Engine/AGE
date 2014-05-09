#include "MeshRenderer.hh"
#include "Core/Engine.hh"
#include <Core/AScene.hh>	

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

	MeshRenderer::MeshRenderer(MeshRenderer const &o)
	{
		mesh = o.mesh;
		shader = o.shader;
	}

	MeshRenderer &MeshRenderer::operator=(MeshRenderer const &o)
	{
		mesh = o.mesh;
		shader = o.shader;
		return *this;
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

	void MeshRenderer::render(std::shared_ptr<Renderer> renderer, const glm::mat4 &globalTrans, std::function<void(OpenGLTools::Shader&)> func)
	{
		auto perModelUniform = renderer->getUniform("PerModel");
		auto materialUniform = renderer->getUniform("MaterialBasic");

		auto s = renderer->getShader(shader);
		if (s)
			s->use();
		func(*s);
		perModelUniform->setUniform("model", globalTrans);
		perModelUniform->flushChanges();
		for (std::size_t i = 0; i < mesh->material->materials.size(); ++i)
		{
			mesh->material->materials[i].setUniforms(materialUniform);
			materialUniform->flushChanges();
			mesh->geometries[i].buffer.draw(GL_TRIANGLES);
		}
	}

	void MeshRenderer::renderRaw(std::shared_ptr<Renderer> renderer, const glm::mat4 &trans)
	{
		std::shared_ptr<OpenGLTools::UniformBuffer> perModelUniform = renderer->getUniform("PerModel");

		perModelUniform->setUniform("model", trans);
		perModelUniform->flushChanges();
		for (unsigned int i = 0; i < mesh->material->materials.size(); ++i)
		{
			mesh->geometries[i].buffer.draw(GL_TRIANGLES);
		}
	}

}
