#include <Render/GeometryManagement/Painting/Painter.hh>

namespace AGE
{

	Painter::Painter(std::vector<GLenum>  const &types) :
		_buffer(types)
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
		auto offset = 0ull;
		for (auto &vertices : _vertices) {
			offset += vertices.nbr_vertex();
		}
		_vertices.emplace_back(_buffer.get_types(), nbrVertex, nbrIndices, offset);
		_buffer.insert(_vertices.back());
		return (Key<Vertices>::createKey(_vertices.size() - 1));
	}

	Painter & Painter::remove_vertices(Key<Vertices> &key)
	{
		if (!key) {
			return (*this);
		}
		auto iterator = _vertices.begin() + key.getId();
		iterator->remove();
		key.destroy();
		_vertices.erase(iterator);
		auto offset = 0ull;
		for (auto &vertices : _vertices) {
			vertices.reset(offset);
			offset = vertices.nbr_vertex();
		}
		return (*this);
	}

	Vertices * Painter::get_vertices(Key<Vertices> const &key)
	{
		if (!key) {
			return (nullptr);
		}
		return (&_vertices[key.getId()]);
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
				_vertices[draw_element.getId()].update(_programs[program.getId()]);
				_vertices[draw_element.getId()].draw(mode);
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

}