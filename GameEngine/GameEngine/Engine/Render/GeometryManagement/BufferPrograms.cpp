#include <Render/GeometryManagement/BufferPrograms.hh>
#include <Render/Buffer/VertexBuffer.hh>
#include <Render/ProgramResources/Types/ProgramResourcesType.hh>
#include <memory>

BufferPrograms::BufferPrograms(std::vector<GLenum> &&types, std::vector<std::shared_ptr<Program>> const &programs) :
_types(std::move(types)),
_programs(std::move(programs))
{
	for (auto &program : programs) {
		if (program->coherent_attribute(_types)) {
			_programs.emplace_back(program);
		}
	}
	_buffers.reserve(_types.size());
	_vertex_array.bind();
	auto index = 0ull;
	for (auto &type : _types) {
		auto &iterator = available_types.find(type);
		if (iterator != available_types.end()) {
			auto &a = iterator->second;
			_buffers.emplace_back(std::make_unique<VertexBuffer>());
			_buffers.back().bind();
			glVertexAttribPointer(index, a.nbr_component, a.type_component, GL_FALSE, 0, 0);
		}
	}
	_vertex_array.unbind();
}

BufferPrograms::BufferPrograms(BufferPrograms &&move) :
_types(std::move(move._types)),
_buffers(std::move(move._buffers)),
_programs(std::move(move._programs)),
_vertices(std::move(move._vertices))
{
}

Key<Vertices> BufferPrograms::push_back(Vertices &vertices)
{
	if (vertices.nbr_buffer() != _types.size()) {
		return (Key<Vertices>::createKey(-1));
	}
	for (auto index = 0ull; index < vertices.nbr_buffer(); ++index) {
		if (_types[index] != vertices.get_type(index)) {
			return (Key<Vertices>::createKey(-1));
		}
	}
	for (auto index = 0ull; index < vertices.nbr_buffer(); ++index) {
		vertices.set_block_memory(_buffers[index].push_back(vertices.transfer_data(index)), index);
	}
	_vertices.emplace_back(vertices);
	return (Key<Vertices>::createKey(_vertices.size() - 1));
}

size_t BufferPrograms::size() const
{
	return (_buffers.size());
}

BufferPrograms & BufferPrograms::bind()
{
	_vertex_array.bind();
	return (*this);
}

BufferPrograms & BufferPrograms::unbind()
{
	_vertex_array.unbind();
	return (*this);
}

BufferPrograms & BufferPrograms::update()
{
	for (auto &buffer : _buffers) {
		buffer.update();
	}
	return (*this);
}

Buffer & BufferPrograms::operator[](Key<Vertices> const &key)
{
	return (_buffers[key.getId()]);
}

