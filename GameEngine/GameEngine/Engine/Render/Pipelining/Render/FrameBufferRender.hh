#pragma once

# include <Utils/OpenGL.hh>
# include <Render/Pipelining/Buffer/Framebuffer.hh>
# include <Render/Pipelining/Buffer/IFramebufferStorage.hh>
# include <Render/Pipelining/Render/ARender.hh>
# include <unordered_map>
# include <utility>

namespace AGE
{

	class Vertices;
	class Painter;
	class Program;
	class IRenderContext;
	class ARenderingPipeline;

	class FrameBufferRender : public ARender
	{
	public:
		FrameBufferRender(FrameBufferRender &&move);
		virtual ~FrameBufferRender(){}

	public:
		virtual IRender &render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &infos) override final;

	public:
		template <typename type_t> FrameBufferRender &push_storage_output(GLenum attach, std::shared_ptr<type_t> storage);
		size_t nbr_output() const;
		std::shared_ptr<IFramebufferStorage> operator[](GLenum attach) const;
		std::vector<GLenum> const &drawing_attach() const;

	protected:
		FrameBufferRender(std::shared_ptr<PaintingManager> painterManager);
		virtual void renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &) = 0;
		Framebuffer _frame_buffer;
		std::unordered_map<GLenum, std::shared_ptr<IFramebufferStorage>> _frame_output;
		std::vector<GLenum> _drawing_attach;

	private:
		bool _is_update;
	};

	template <typename type_t>
	FrameBufferRender & FrameBufferRender::push_storage_output(GLenum attach, std::shared_ptr<type_t> storage)
	{
		_is_update = false;
		_frame_output[attach] = storage;
		if (attach != GL_DEPTH_ATTACHMENT &&
			attach != GL_STENCIL_ATTACHMENT && 
			attach != GL_DEPTH_STENCIL_ATTACHMENT) {
			for (auto &it = _drawing_attach.begin(); it != _drawing_attach.end(); ++it) {
				if (*it == attach) {
					_drawing_attach.erase(it);
					break;
				}
			}
			_drawing_attach.emplace_back(attach);
		}
		return (*this);
	}

}