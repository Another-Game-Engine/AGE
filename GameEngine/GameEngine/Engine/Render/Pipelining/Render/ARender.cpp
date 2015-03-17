#include <Render/Pipelining/Render/ARender.hh>
#include <Render/Program.hh>

namespace AGE
{

	ARender::ARender(std::shared_ptr<PaintingManager> painterManager) :
		_painterManager(painterManager)
	{

	}

	ARender::ARender(ARender &&move) :
		_render_function(std::move(move._render_function))
	{

	}

	void ARender::setRenderFunction(std::function<void(RenderPipeline const &, RenderLightList const &, CameraInfos const &)> const &function)
	{
		_render_function = function;
	}

	bool ARender::recompilePrograms()
	{
		for (auto &prog : _programs)
		{
			bool success = prog->compile();
			if (!success)
			{
				return false;
			}
		}
		return (true);
	}

}