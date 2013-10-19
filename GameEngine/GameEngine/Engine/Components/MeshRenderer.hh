#ifndef		MESHRENDERER_HH_
#define		MESHRENDERER_HH_

#include "AComponent.hh"

class MeshRenderer : AComponent
{
private:
	Smartpointer<SharedMesh>	_mesh;
	std::string					_shader;

public:
	MeshRenderer(void);
	virtual ~MeshRenderer(void);

	virtual void	start();
	virtual void	update();
	virtual void	stop();

	bool			setShader(std::string);
};

#endif