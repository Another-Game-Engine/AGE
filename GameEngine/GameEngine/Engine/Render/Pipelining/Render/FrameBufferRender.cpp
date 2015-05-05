#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>

namespace AGE
{

	FrameBufferRender::FrameBufferRender(std::shared_ptr<PaintingManager> painterManager) :
		ARender(painterManager)
	{
		_nextPass = nullptr;
	}

	FrameBufferRender::FrameBufferRender(FrameBufferRender &&move) :
		ARender(std::move(move)),
		_frame_buffer(std::move(move._frame_buffer)),
		_frame_output(std::move(move._frame_output))
	{
	}

	IRender & FrameBufferRender::render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &infos)
	{
		SCOPE_profile_cpu_i("RenderTimer", "FrameBufferRender pass");

		_frame_buffer.bind();
		if (!_is_update)
		{
			SCOPE_profile_gpu_i("glDrawBuffers");
			SCOPE_profile_cpu_i("RenderTimer", "glDrawBuffers");
			glDrawBuffers(GLint(_drawing_attach.size()), _drawing_attach.data());
			for (auto &storage : _frame_output)
			{
				_frame_buffer.attachment(*storage.second.get(), storage.first);
			}
			_is_update = true;
		}
		renderPass(pipeline, lights, infos);
		_frame_buffer.unbind();
		if (_nextPass != nullptr)
			_nextPass->render(pipeline, lights, infos);
		return (*this);
	}

	size_t FrameBufferRender::nbr_output() const
	{
		return (_frame_output.size());
	}

	std::shared_ptr<IFramebufferStorage> FrameBufferRender::operator[](GLenum attach) const
	{
		auto &element = _frame_output.find(attach);
		if (element == _frame_output.end()) {
			return (nullptr);
		}
		return (element->second);
	}

}