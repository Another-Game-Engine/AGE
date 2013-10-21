#include "MeshRenderer.hh"

#include "Core/Engine.hh"

namespace Components
{

MeshRenderer::MeshRenderer(std::string const &name) :
	AComponent(name),
	_next(NULL)
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

void								MeshRenderer::setNext(SmartPointer<MeshRenderer> const &n)
{
	_next = n;
}

SmartPointer<MeshRenderer> const	&MeshRenderer::getNext() const
{
	return (_next);
}

bool	MeshRenderer::setShader(std::string const &name)
{
	_shader = name;
	return (true);
}

SmartPointer<Resources::SharedMesh> const &MeshRenderer::getMesh() const
{
	return (_mesh);
}

}