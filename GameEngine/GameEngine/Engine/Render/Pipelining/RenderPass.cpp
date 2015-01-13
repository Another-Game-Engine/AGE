#include <Render/Pipelining/RenderPass.hh>

RenderPass::RenderPass(std::function<RenderPass &(FUNCTION_ARGS)> function) :
_render_function(function)
{

}

RenderPass::RenderPass(RenderPass &&move) :
_frame_buffer(std::move(move._frame_buffer)),
_frame_output(std::move(move._frame_output)),
_render_function(std::move(move._render_function))
{

}

RenderPass & RenderPass::render(std::shared_ptr<Program> const &program, std::shared_ptr<Painter> const &painter, std::vector<Key<Vertices>> const &vertices, std::vector<std::shared_ptr<RenderPass>> const &input)
{
	_frame_buffer.bind();
	if (!_is_update) {
		glDrawBuffers(_drawing_attach.size(), _drawing_attach.data());
		for (auto &storage : _frame_output) {
			_frame_buffer.attachment(*storage.second.get(), storage.first);
		}
		_is_update = true;
	}
	_render_function(program, painter, vertices, input);
	_frame_buffer.unbind();
	return (*this);
}

size_t RenderPass::nbr_output() const
{
	return (_frame_output.size());
}

std::shared_ptr<IFramebufferStorage> const & RenderPass::operator[](GLenum attach) const
{
	auto &element = _frame_output.find(attach);
	if (element == _frame_output.end()) {
		return (nullptr);
	}
	return (element->second);
}

