#include <Render/Pipelining/Render/ARender.hh>
#include <Render/Program.hh>

namespace AGE
{

	ARender::ARender(std::shared_ptr<PaintingManager> painterManager) :
		_painterManager(painterManager)
	{

	}

	ARender::ARender(ARender &&move)
	{

	}

	bool ARender::recompilePrograms()
	{
		for (auto &prog : _programs)
		{
			bool success = prog->compile();
			if (!success)
			{
				return (false);
			}
		}
		if (_nextPass)
			return (_nextPass->recompilePrograms());
		return (true);
	}

	void ARender::setNextPass(std::shared_ptr<ARender> nextPass)
	{
		_nextPass = nextPass;
	}

	bool ARender::renderModeCompatible(RenderModeSet const &renderMode)
	{
		return ((_mandatory & renderMode) == _mandatory && (_forbidden & renderMode).to_ulong() == 0);
	}

}