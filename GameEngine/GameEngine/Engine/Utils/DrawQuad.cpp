#include "DrawQuad.hh"
#include <OpenGL/VertexManager.hh>
#include <OpenGL/Shader.hh>
#include <Core/Renderer.hh>
#include <glm/gtc/matrix_transform.hpp>

DrawQuad::DrawQuad() :
#if !TEST_NEW_VERTEXMANAGER
	_vertexManager(NULL),
#endif
	_fboToScreen(NULL)
{
}


DrawQuad::~DrawQuad()
{
#if !TEST_NEW_VERTEXMANAGER
	if (_vertexManager != NULL)
		delete _vertexManager;
#endif
}

void	DrawQuad::initShaders()
{
	_fboToScreen = _renderer->getShader("fboToScreen");
	_fboToScreenMultisampled = _renderer->getShader("fboToScreenMultisampled");

	assert(_fboToScreen != NULL &&
		   _fboToScreenMultisampled != NULL &&
		   "Cannot find the fboToScreen shaders.");

	_fboToScreen->use();

	GLint		view = glGetUniformLocation(_fboToScreen->getId(), "view");
	GLint		proj = glGetUniformLocation(_fboToScreen->getId(), "projection");

	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	glUniformMatrix4fv(proj, 1, GL_FALSE, glm::value_ptr(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)));

	_fboToScreenMultisampled->use();

	view = glGetUniformLocation(_fboToScreenMultisampled->getId(), "view");
	proj = glGetUniformLocation(_fboToScreenMultisampled->getId(), "projection");

	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	glUniformMatrix4fv(proj, 1, GL_FALSE, glm::value_ptr(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f)));
}

void	DrawQuad::draw(GLuint texture, int sampleNbr, glm::uvec2 const &textureSize)
{
	if (_fboToScreen == NULL)
		initShaders();

	GLenum textureType;

	if (sampleNbr > 1)
	{
		_fboToScreenMultisampled->use();
		GLint		sampleNbrLocation = glGetUniformLocation(_fboToScreenMultisampled->getId(), "sampleNbr");
		GLint		ctxSizeLocation = glGetUniformLocation(_fboToScreenMultisampled->getId(), "ctxSize");

		glUniform1i(sampleNbrLocation, sampleNbr);
		glUniform2ui(ctxSizeLocation, textureSize.x, textureSize.y);
		textureType = GL_TEXTURE_2D_MULTISAMPLE;
	}
	else
	{
		_fboToScreen->use();
		textureType = GL_TEXTURE_2D;
	}

	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(textureType, texture);
#if !TEST_NEW_VERTEXMANAGER
	_quad.draw();
#endif
	glEnable(GL_DEPTH_TEST);
}

void DrawQuad::init(std::weak_ptr<DependenciesInjector> &&engine)
{
	_renderer = engine.lock()->getInstance<Renderer>();
#if !TEST_NEW_VERTEXMANAGER
	std::array<Attribute, 2> param =
	{
		Attribute(GL_FLOAT, sizeof(float), 2),
		Attribute(GL_FLOAT, sizeof(float), 2)
	};

	_vertexManager = new VertexManager<2>(param);
	_vertexManager->init();
#endif

	// Init the Quad:
	// ------------------------------------
	// x,y vertex positions
	float quadPos[] = {
		-1.0, -1.0,
		1.0, -1.0,
		1.0, 1.0,
		1.0, 1.0,
		-1.0, 1.0,
		-1.0, -1.0
	};

	// per-vertex texture coordinates
	float quadUvs[] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0
	};

	unsigned int indice[] = { 0, 1, 2, 3, 4, 5 };
#if !TEST_NEW_VERTEXMANAGER
	std::array<Data, 2> data =
	{
		Data(6 * 2 * sizeof(float), quadPos),
		Data(6 * 2 * sizeof(float), quadUvs)
	};
	Data indicesData(6 * sizeof(unsigned int), indice);
	_quad = Vertice<2>(6, data, &indicesData);
	_vertexManager->addVertice(_quad);
#endif
	// ------------------------------------
}
