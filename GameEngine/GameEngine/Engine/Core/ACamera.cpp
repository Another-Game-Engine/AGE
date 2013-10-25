
#include "ACamera.hh"
#include "Core/Engine.hh"
#include "ResourceManager/CubeMap.hh"

ACamera::ACamera() :
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
	_skybox = GameEngine::instance()->resources().getResource(name);
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
		OpenGLTools::Shader *s = GameEngine::instance()->renderer().getShader(_cubeMapShader);
		assert(s != NULL && "Skybox does not have a shader associated");
		s->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox->getId());
		glDepthMask(0);
		glDepthMask(1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	customUpdate();
}
