#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Properties/Properties.hh>

#include <Utils/Debug.hpp>
#include <Threads/ThreadManager.hpp>

namespace AGE
{

	Painter::Painter(std::vector<GLenum>  const &types) :
		_buffer(types)
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());
	}

	Painter::Painter(Painter &&move) :
		_buffer(std::move(move._buffer)),
		_vertices(std::move(move._vertices))
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());
	}

	Key<Vertices> Painter::add_vertices(size_t nbrVertex, size_t nbrIndices)
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		auto offset = 0ull;
		for (auto &vertices : _vertices) {
			offset += vertices.nbr_vertex();
		}
		_vertices.emplace_back(_buffer.get_types(), nbrVertex, nbrIndices, offset);
		_buffer.insert(_vertices.back());
		return (Key<Vertices>::createKey(int(_vertices.size()) - 1));
	}

	Painter & Painter::remove_vertices(Key<Vertices> &key)
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

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

	Painter &Painter::remove_properties(Key<Properties> &key)
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		_propertiesToRemove.push_back(key);
		return (*this);
	}

	Vertices * Painter::get_vertices(Key<Vertices> const &key)
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		if (!key) {
			return (nullptr);
		}
		return (&_vertices[key.getId()]);
	}

	Painter & Painter::draw(GLenum mode, std::shared_ptr<Program> const &program, std::vector<Properties> const &propertiesList, std::vector<Key<Vertices>> const &drawList)
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		AGE_ASSERT(program->coherent_attribute(_buffer.get_types()));

		_buffer.bind();
		_buffer.update();
		int index = 0;
		for (auto &draw_element : drawList)
		{
			if (draw_element)
			{
				auto &property = propertiesList[index];
				property.update_properties(program);
				program->update();
				_vertices[draw_element.getId()].draw(mode);
			}
			++index;
		}
		_buffer.unbind();

		return (*this);
	}

	bool Painter::coherent(std::vector<GLenum> const &types) const
	{
		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		auto &types_buffer = _buffer.get_types();
		if (types.size() != types_buffer.size()) {
			return (true);
		}
		for (auto index = 0ull; index < types_buffer.size(); ++index) {
			if (types[index] != types_buffer[index]) {
				return (false);
			}
		}
		return (true);
	}

}