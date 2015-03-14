#pragma once

# include <Utils/OpenGL.hh>
# include <Render/Pipelining/Buffer/Framebuffer.hh>
# include <Render/Pipelining/Buffer/IFramebufferStorage.hh>
# include <Render/Pipelining/Render/ARendering.hh>
# include <unordered_map>
# include <utility>

namespace AGE
{

	class Vertices;
	class Painter;
	class Program;
	class IRenderContext;

	class RenderingPass : public ARendering
	{
	public:
		RenderingPass(std::function<void(std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter)> const &function);
		RenderingPass(RenderingPass &&move);
		virtual ~RenderingPass(){}

	public:
		virtual IRendering &render(std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter) override final;

	public:
		template <typename type_t> RenderingPass &push_storage_output(GLenum attach, std::shared_ptr<type_t> storage);
		size_t nbr_output() const;
		std::shared_ptr<IFramebufferStorage> operator[](GLenum attach) const;
		std::vector<GLenum> const &drawing_attach() const;

	private:
		Framebuffer _frame_buffer;
		std::unordered_map<GLenum, std::shared_ptr<IFramebufferStorage>> _frame_output;
		bool _is_update;
		std::vector<GLenum> _drawing_attach;
	};

	template <typename type_t>
	RenderingPass & RenderingPass::push_storage_output(GLenum attach, std::shared_ptr<type_t> storage)
	{
		_is_update = false;
		_frame_output[attach] = storage;
		if (attach != GL_DEPTH_ATTACHMENT) {
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