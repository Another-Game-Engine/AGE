#pragma once

# include <Render/Pipelining/Render/IRender.hh>

#include <Render/Pipelining/Render/RenderModes.hh>
# include <functional>
#include <vector>
#include <bitset>

namespace AGE
{
	class ARender : public IRender
	{
	public:
		virtual ~ARender() {}

	public:
		bool recompilePrograms();
		void setNextPass(std::shared_ptr<ARender> nextPass);

		bool renderModeCompatible(RenderModeSet const &renderMode);

	protected:
		ARender(std::shared_ptr<PaintingManager> painterManager);
		ARender(ARender &&move);
		virtual void renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &) = 0;

	protected:
		// Bitsets to test the objects against
		RenderModeSet _mandatory;
		RenderModeSet _forbidden;
		// Render pass utils
		std::shared_ptr<PaintingManager> _painterManager;
		std::vector<std::shared_ptr<Program>> _programs;
		// Next render pass
		std::shared_ptr<ARender> _nextPass;
	};

}