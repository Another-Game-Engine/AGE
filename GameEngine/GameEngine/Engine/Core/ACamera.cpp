
#include "ACamera.hh"
#include "Core/Engine.hh"
#include "ResourceManager/CubeMap.hh"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

ACamera::ACamera(Engine &engine) :
_engine(engine),
_hasMoved(true),
_skybox(NULL)
{
}

glm::mat4         &ACamera::setTransform()
{
	_hasMoved = true;
	return (_transform);
}

glm::mat4 		  &ACamera::getTransform()
{
	return (_transform);
}

glm::mat4         &ACamera::setProjection()
{
	_hasMoved = true;
	return (_projection);
}

glm::mat4 		  &ACamera::getProjection()
{
	return (_projection);
}

bool              ACamera::hasMoved() const
{
	return (_hasMoved);
}

void              ACamera::resetMoved()
{
	_hasMoved = false;
}


void ACamera::attachSkybox(const std::string &name, const std::string &cubeMapShader)
{
	_skybox = _engine.getInstance<Resources::ResourceManager>().getResource(name);
	_cubeMapShader = cubeMapShader;
}

void ACamera::dettachSkybox()
{
	_skybox = NULL;
}

void ACamera::update()
{
	if (_skybox.get())
	{
		OpenGLTools::Shader *s = _engine.getInstance<Renderer>().getShader(_cubeMapShader);
		assert(s != NULL && "Skybox does not have a shader associated");

		_engine.getInstance<Renderer>().getUniform("cameraUniform")->setUniform("projection", getProjection());

		glm::mat4 t = getTransform();
		t[3][0] = 0;
		t[3][1] = 0;
		t[3][2] = 0;
		t[3][3] = 1;

		_engine.getInstance<Renderer>().getUniform("cameraUniform")->setUniform("view", t);
		_engine.getInstance<Renderer>().getUniform("cameraUniform")->flushChanges();

		_engine.getInstance<Renderer>().getFbo().bindDrawTargets(s->getTargets(), s->getTargetsNumber());

		s->use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox->getId());
		glDepthMask(0);
		_skybox->draw();
		glDepthMask(1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	customUpdate();
}
