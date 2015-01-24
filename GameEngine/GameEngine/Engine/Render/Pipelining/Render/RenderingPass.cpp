#include <Render/Pipelining/Render/RenderingPass.hh>
#include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>

RenderingPass::RenderingPass(std::function<IRendering &(FUNCTION_ARGS)> const &function) :
ARendering(function)
{

}

RenderingPass::RenderingPass(RenderingPass &&move) :
ARendering(std::move(move)),
_frame_buffer(std::move(move._frame_buffer)),
_frame_output(std::move(move._frame_output))
{

}

IRendering & RenderingPass::render(std::vector<AGE::Drawable> const &drawables, IRenderingPipeline &_, PaintingManager &paintingManager, std::vector<std::shared_ptr<IRenderingPipeline>> const &other)
{
	_frame_buffer.bind();
	if (!_is_update) {
		glDrawBuffers(_drawing_attach.size(), _drawing_attach.data());
		for (auto &storage : _frame_output) {
			_frame_buffer.attachment(*storage.second.get(), storage.first);
		}
		_is_update = true;
	}
	_render_function(drawables, _, paintingManager, other);
	_frame_buffer.unbind();
	return (*this);
}

size_t RenderingPass::nbr_output() const
{
	return (_frame_output.size());
}

std::shared_ptr<IFramebufferStorage> RenderingPass::operator[](GLenum attach) const
{
	auto &element = _frame_output.find(attach);
	if (element == _frame_output.end()) {
		return (nullptr);
	}
	return (element->second);
}

