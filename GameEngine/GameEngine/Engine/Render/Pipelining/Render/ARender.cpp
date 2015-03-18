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
				return false;
			}
		}
		return (true);
	}

}