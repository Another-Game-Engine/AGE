#include <Render/GeometryManagement/Buffer/BufferPrograms.hh>
#include <Render/Buffer/VertexBuffer.hh>
#include <Render/Buffer/IndexBuffer.hh>
#include <Render/ProgramResources/Types/ProgramResourcesType.hh>
#include <Render/Program.hh>
#include <memory>

BufferPrograms::BufferPrograms(std::vector<GLenum> &&types) :
_types(std::move(types)),
_indices_buffer(std::make_unique<IndexBuffer>())
{
	_buffers.reserve(_types.size());
	_vertex_array.bind();
	auto index = 0ull;
	for (auto &type : _types) {
		auto &iterator = available_types.find(type);
		if (iterator != available_types.end()) {
			auto &a = iterator->second;
			_buffers.emplace_back(std::make_unique<VertexBuffer>());
			_buffers.back().bind();
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index++, a.nbr_component, a.type_component, GL_FALSE, 0, 0);
		}
	}
	_indices_buffer.bind();
	_vertex_array.unbind();
}

BufferPrograms::BufferPrograms(BufferPrograms &&move) :
_types(std::move(move._types)),
_buffers(std::move(move._buffers)),
_indices_buffer(std::move(move._indices_buffer)),
_vertex_array(std::move(move._vertex_array))
{
}

bool BufferPrograms::insert(Vertices &vertices)
{
	if (vertices.nbr_buffer() != _types.size()) {
		return (false);
	}
	for (auto index = 0ull; index < vertices.nbr_buffer(); ++index) {
		if (_types[index] != vertices.get_type(index)) {
			return (false);
		}
	}
	for (auto index = 0ull; index < vertices.nbr_buffer(); ++index) {
		vertices.set_block_memory(_buffers[index].push_back(vertices.transfer_data(index)), index);
	}
	vertices.set_indices_block_memory(_indices_buffer.push_back(vertices.transfer_indices_data()));
	return (true);
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
	_indices_buffer.update();
	return (*this);
}

BufferPrograms & BufferPrograms::clear()
{
	for (auto &buffer : _buffers) {
		buffer.clear();
	}
	_indices_buffer.clear();
	return (*this);
}

bool BufferPrograms::coherent_program(std::shared_ptr<Program> const &p)
{
	return (p->coherent_attribute(_types));
}

std::vector<GLenum> const & BufferPrograms::get_types() const
{
	return (_types);
}