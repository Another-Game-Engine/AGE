#pragma once

# include <Utils/OpenGL.hh>
# include <vector>
# include <memory>
# include <Utils/Key.hh>
# include <Utils/SpinLock.hpp>

class StringID;

namespace AGE
{
	class Painter;

	class PaintingManager
	{
	public:
		PaintingManager();
		PaintingManager(PaintingManager &&move);
		PaintingManager(PaintingManager const &copy) = delete;
		PaintingManager &operator=(PaintingManager const &p) = delete;

	public:
		Key<Painter> add_painter(std::vector<std::pair<GLenum, StringID>> &&types);
		std::shared_ptr<Painter> &get_painter(Key<Painter> const &key);
		Key<Painter> get_painter(std::vector<std::pair<GLenum, StringID>> const &types);
		bool has_painter(std::vector<std::pair<GLenum, StringID>> const &types);
	private:
		std::vector<std::shared_ptr<Painter>> _painters;
		AGE::SpinLock _mutex;
	};
}
