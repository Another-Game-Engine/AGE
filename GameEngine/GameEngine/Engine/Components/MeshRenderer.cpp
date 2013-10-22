#include "MeshRenderer.hh"

#include "Core/Engine.hh"

namespace Components
{

MeshRenderer::MeshRenderer(std::string const &name, std::string const &resource) :
	AComponent(name),
	_mesh(AssetsManager::instance()->getResource(resource)),
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

std::string const		&MeshRenderer::getShader() const
{
	return (_shader);
}

SmartPointer<Resources::SharedMesh> const &MeshRenderer::getMesh() const
{
	return (_mesh);
}

}