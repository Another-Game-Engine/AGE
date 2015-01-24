#include <Render/GeometryManagement/Painting/Painter.hh>

Painter::Painter(std::vector<GLenum>  const &types) :
_buffer(types),
_vertices(256)
{
}

Painter::Painter(Painter &&move) :
_buffer(std::move(move._buffer)),
_programs(std::move(move._programs)),
_vertices(std::move(move._vertices))
{

}

Painter &Painter::set_programs(std::vector<std::shared_ptr<Program>> const &programs)
{
	for (auto &program : programs) {
		if (_buffer.coherent_program(program)) {
			_programs.emplace_back(program);
		}
	}
	return (*this);
}

Key<Vertices> Painter::add_vertices(size_t nbrVertex, size_t nbrIndices)
{
	size_t offset = 0;
	if (!_keys_positions.empty())
	{
		auto &lastAdded = _vertices.get(_keys_positions.back());
		offset = lastAdded.offset() + lastAdded.nbr_vertex();
	}
	auto index = _vertices.alloc(_buffer.get_types(), nbrVertex, nbrIndices, offset);
	_keys_positions.push_back(index);
	_buffer.insert(_vertices.get(index));
	return (Key<Vertices>::createKey(index));
}

Painter & Painter::remove_vertices(Key<Vertices> &key)
{
	if (!key) {
		return (*this);
	}
	auto &vertices = _vertices.get(key.getId());
	auto nbr_vertex = vertices.nbr_vertex();
	vertices.remove();
	_vertices.dealloc(key.getId());
	auto key_to_rm = std::find(_keys_positions.begin(), _keys_positions.end(), key);
	for (auto key_it = key_to_rm + 1; key_it != _keys_positions.end(); ++key_it) {
		auto &currentVertice = _vertices.get(*key_it);
		currentVertice.reset(currentVertice.offset() - nbr_vertex);
	}
	_keys_positions.erase(key_to_rm);
	key.destroy();
	return (*this);
}

Vertices * Painter::get_vertices(Key<Vertices> const &key)
{
	if (!key) {
		return (nullptr);
	}
	return (&_vertices.get(key.getId()));
}

Painter & Painter::update()
{
	_buffer.update();
	return (*this);
}

Key<Program> Painter::get_key_program(std::string const &name)
{
	auto index = 0;
	for (auto &program : _programs) {
		if (name == program->name()) {
			return (Key<Program>::createKey(index));
		}
		++index;
	}
	return (Key<Program>::createKey(-1));
}

Key<Program> Painter::get_key_program(std::shared_ptr<Program> const &p)
{
	auto index = 0;
	for (auto &program : _programs) {
		if (*p == *program) {
			return (Key<Program>::createKey(index));
		}
		++index;
	}
	return (Key<Program>::createKey(-1));
}

Program * Painter::get_program(Key<Program> const &program)
{
	if (!program) {
		return (nullptr);
	}
	return (_programs[program.getId()].get());
}

Painter & Painter::draw(GLenum mode, Key<Program> const &program, std::vector<Key<Vertices>> const &drawList)
{
	if (!program) {
		return (*this);
	}
	_buffer.bind();
	for (auto &draw_element : drawList) {
		if (draw_element) {
			_vertices.get(draw_element.getId()).update(_programs[program.getId()]);
			_vertices.get(draw_element.getId()).draw(mode);
		}
	}
	_buffer.unbind();
	return (*this);
}

Painter & Painter::draw(GLenum mode, std::shared_ptr<Program> const &p, std::vector<Key<Vertices>> const &drawList)
{
	draw(mode, get_key_program(p), drawList);
	return (*this);
}

bool Painter::coherent(std::vector<GLenum> const &types) const
{
	return (_buffer.coherent_program(types));
}

void Painter::print() const
{
	std::cout << "Painter :"<< std::endl;
	std::cout << "{" << std::endl;
	_buffer.print();
	std::cout << "vector<Vertices> size [" << _keys_positions.size() << "]:" << std::endl;
	for (auto &key_position : _keys_positions) {
		std::cout << "	";
		_vertices.get(key_position).print();
	}
	std::cout << "}" << std::endl;
}