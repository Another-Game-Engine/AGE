#include <Render/GeometryManagement/Painting/PaintingManager.hh>
# include <Render/GeometryManagement/Painting/Painter.hh>
#include <mutex>

namespace AGE
{
	PaintingManager::PaintingManager()
	{

	}

	PaintingManager::PaintingManager(PaintingManager &&move) :
		_painters(std::move(move._painters))
	{

	}


	Key<Painter> PaintingManager::add_painter(std::vector<std::pair<GLenum, std::string>> &&types)
	{
		int sizeOfArray;
		{
			std::lock_guard<AGE::SpinLock> lock(_mutex);
			_painters.emplace_back(std::make_shared<Painter>(types));
			sizeOfArray = int(_painters.size()) - 1;
		}
		return (Key<Painter>::createKey(sizeOfArray));
	}

	std::shared_ptr<Painter> &PaintingManager::get_painter(Key<Painter> const &key)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_painters[key.getId()]);
	}

	Key<Painter> PaintingManager::get_painter(std::vector<std::pair<GLenum, std::string>> const &types)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		for (auto index = 0; index < int(_painters.size()); ++index) {
			if (_painters[index]->coherent(types)) {
				return (Key<Painter>::createKey(index));
			}
		}
		return (Key<Painter>::createKey(-1));
	}

	bool PaintingManager::has_painter(std::vector<std::pair<GLenum, std::string>> const &types)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		for (auto &painter : _painters) {
			if (painter->coherent(types)) {
				return (true);
			}
		}
		return (false);
	}
}

