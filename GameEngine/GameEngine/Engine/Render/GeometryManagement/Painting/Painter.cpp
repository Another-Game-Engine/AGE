#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Properties/Properties.hh>

#include <Utils/Debug.hpp>
#include <Threads/ThreadManager.hpp>
#include <Utils/Profiler.hpp>

namespace AGE
{

	Painter::Painter(std::vector<std::pair<GLenum, std::string>>  const &types) :
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
		SCOPE_profile_cpu_function("PainterTimer");

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
		SCOPE_profile_cpu_function("PainterTimer");

		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		if (!key.isValid())
		{
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
		SCOPE_profile_cpu_function("PainterTimer");

		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		if (!key.isValid())
		{
			return (nullptr);
		}
		return (&_vertices[key.getId()]);
	}

	Painter & Painter::draw(GLenum mode, std::shared_ptr<Program> const &program, std::vector<Properties> const &propertiesList, std::vector<Key<Vertices>> const &drawList)
	{
		SCOPE_profile_cpu_function("PainterTimer");

		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());
		program->set_attributes(_buffer);
		_buffer.bind();
		_buffer.update();
		int index = 0;
		for (auto &draw_element : drawList)
		{
			if (draw_element.isValid())
			{
				auto &property = propertiesList[index];
				{
					SCOPE_profile_gpu_i("Update properties");
					SCOPE_profile_cpu_i("RenderTimer", "Update properties");
					property.update_properties(program);
				}
				{
					SCOPE_profile_gpu_i("Update Shader memory");
					SCOPE_profile_cpu_i("RenderTimer", "Update Shader memory");
					program->update();
				}
				{
					SCOPE_profile_gpu_i("Draw");
					SCOPE_profile_cpu_i("RenderTimer", "Draw");
					_vertices[draw_element.getId()].draw(mode);
				}
			}
			++index;
		}
		_buffer.unbind();

		return (*this);
	}

	void Painter::uniqueDraw(GLenum mode, std::shared_ptr<Program> const &program, Properties const &properties, const Key<Vertices> &vertice)
	{
		SCOPE_profile_cpu_function("PainterTimer");

		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());
		program->set_attributes(_buffer);
		_buffer.bind();
		_buffer.update();
		{
			SCOPE_profile_gpu_i("Update properties");
			SCOPE_profile_cpu_i("RenderTimer", "Update properties");
			properties.update_properties(program);
		}
		{
			SCOPE_profile_gpu_i("Update Shader memory");
			SCOPE_profile_cpu_i("RenderTimer", "Update Shader memory");
			program->update();
		}
		{
			SCOPE_profile_gpu_i("Unique draw");
			SCOPE_profile_cpu_i("RenderTimer", "Unique draw");
			_vertices[vertice.getId()].draw(mode);
		}

		_buffer.unbind();
	}

	void Painter::uniqueDraw(GLenum mode, const Key<Vertices> &vertice)
	{
		SCOPE_profile_cpu_function("PainterTimer");

		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());
		_buffer.bind();
		{
			SCOPE_profile_gpu_i("Update Shader memory");
			SCOPE_profile_cpu_i("RenderTimer", "Update Shader memory");
			_buffer.update();
		}
		{
			SCOPE_profile_gpu_i("Unique draw");
			SCOPE_profile_cpu_i("RenderTimer", "Unique draw");
			_vertices[vertice.getId()].draw(mode);
		}

		_buffer.unbind();
	}

	bool Painter::coherent(std::vector<std::pair<GLenum, std::string>> const &types) const
	{
		SCOPE_profile_cpu_function("PainterTimer");

		// to be sure that this function is only called in render thread
		AGE_ASSERT(GetThreadManager()->getCurrentThread() == (AGE::Thread*)GetRenderThread());

		auto &types_buffer = _buffer.get_types();
		if (types.size() != types_buffer.size()) {
			return (false);
		}
		for (auto index = 0ull; index < types_buffer.size(); ++index) {
			if (types[index] != types_buffer[index]) {
				return (false);
			}
		}
		return (true);
	}

	bool Painter::coherent(std::shared_ptr<Program> const &prog) const
	{
		SCOPE_profile_cpu_function("PainterTimer");

		return (prog->coherent_attributes(_buffer.get_types()));
	}
}