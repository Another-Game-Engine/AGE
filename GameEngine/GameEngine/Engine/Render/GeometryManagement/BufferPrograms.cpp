#include <Render/GeometryManagement/BufferPrograms.hh>
#include <Render/Buffer/VertexBuffer.hh>
#include <memory>

BufferPrograms::BufferPrograms(std::vector<GLenum> &&types, std::vector<std::shared_ptr<Program>> const &programs) :
_types(std::move(types)),
_programs(std::move(programs))
{
	for (auto &program : programs) {
		if (program->coherent_attribute(types)) {
			_programs.emplace_back(program);
		}
	}
	_buffers.reserve(_types.size());
	for (auto &type : _types) {
		_buffers.emplace_back(std::make_unique<VertexBuffer>());
	}
}

BufferPrograms::BufferPrograms(BufferPrograms &&move) :
_buffers(std::move(move._buffers))
{

}

Key<Vertices> BufferPrograms::push_back(Vertices const &vertices)
{
}

BufferPrograms & BufferPrograms::pop_bask()
{
	_buffers.pop_back();
}

size_t BufferPrograms::size() const
{
	return (_buffers.size());
}

BufferPrograms & BufferPrograms::bind()
{
	for (auto &buffer : _buffers) {
		buffer.bind();
	}
}

BufferPrograms & BufferPrograms::update()
{
	for (auto &buffer : _buffers) {
		buffer.update();
	}
}

Buffer & BufferPrograms::operator[](size_t index)
{
	return (_buffers[index]);
}

