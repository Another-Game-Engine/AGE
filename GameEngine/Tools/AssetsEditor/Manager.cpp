#include "Manager.hpp"
#include "Loaders.hh"

bool Manager::loadFile(std::string &&f)
{
	auto file = std::tr2::sys::path(f);
	if (!std::tr2::sys::exists(file))
	{
		std::cerr << "File " << file.basename() << " does not exists" << std::endl;
		return false;
	}

	if (hasFile(std::move(f)))
	{
		std::cout << "File " << f << " already loaded." << std::endl;
		return true;
	}

	auto extension = file.extension();
	if (extension == ".obj")
	{
		 addFutur(_pool.addTask([this, file](){
			return loadObj(file, std::ref(*this));
		}));
	}
	else if (extension == ".mtl")
	{
		addFutur(_pool.addTask([this, file](){
			return loadMtl(file, std::ref(*this));
		}));
	}
	else if (extension == ".tga")
	{
		addFutur(_pool.addTask([this, file](){
			return loadTga(file, std::ref(*this));
		}));
	}
	return false;
}

bool Manager::hasFile(std::string &&file)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_files.find(file) != std::end(_files))
		return true;
	return false;
}

void Manager::addFile(std::shared_ptr<MyMediaFile> &&file)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_files.emplace(std::make_pair(file->getName(), file));
}

void Manager::addFutur(std::future<bool> &&futur)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_futures.emplace_back(std::move(futur));
}

std::size_t Manager::waitForAll(std::chrono::milliseconds &&delay)
{
	auto i = std::begin(_futures);
	while (i != std::end(_futures))
	{
		if (i->wait_for(delay) == std::future_status::ready)
		{
			i->get();
			_futures.erase(i++);
		}
		else
			++i;
	}
	return _futures.size();
}