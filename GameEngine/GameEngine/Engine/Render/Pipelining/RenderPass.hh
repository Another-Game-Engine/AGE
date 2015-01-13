#pragma once

# include <Utils/OpenGL.hh>
# include <Render/Pipelining/Buffer/Framebuffer.hh>
# include <Render/Pipelining/Buffer/IFramebufferStorage.hh>
# include <memory>
# include <vector>
# include <functional>
# include <Render/Key.hh>
# include <unordered_map>
# include <utility>

class Vertices;
class Painter;
class Program;

# define FUNCTION_ARGS std::shared_ptr<Program> const &, std::shared_ptr<Painter> const &, std::vector<Key<Vertices>> const &, std::vector<std::shared_ptr<RenderPass>> const &input

class RenderPass
{
public:
	RenderPass(std::function<RenderPass &(FUNCTION_ARGS)> function);
	RenderPass(RenderPass &&move);

public:
	RenderPass &render(FUNCTION_ARGS);
	template <typename type_t> RenderPass &push_storage_output(GLenum attach, std::shared_ptr<type_t> storage);
	size_t nbr_output() const;
	std::shared_ptr<IFramebufferStorage> const &operator[](GLenum attach) const;
	std::vector<GLenum> const &drawing_attach() const;

private:
	Framebuffer _frame_buffer;
	std::unordered_map<GLenum, std::shared_ptr<IFramebufferStorage>> _frame_output;
	std::function<RenderPass &(FUNCTION_ARGS)> _render_function;
	bool _is_update;
	std::vector<GLenum> _drawing_attach;
};

template <typename type_t>
RenderPass & RenderPass::push_storage_output(GLenum attach, std::shared_ptr<type_t> storage)
{
	_is_update = false;
	_frame_output[attach] = storage;
	for (auto &it = _drawing_attach.begin(); it != _drawing_attach.end(); ++it) {
		if (*it == attach) {
			_drawing_attach.erase(it);
			break;
		}
	}
	_drawing_attach.emplace_back(attach);
	return (*this);
}
