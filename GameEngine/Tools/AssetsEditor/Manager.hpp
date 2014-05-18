//#pragma once
//
//#include <filesystem>
//#include <map>
//#include <iostream>
//#include <thread>
//#include <future>
//#include <tiny_obj_loader.h>
//#include <set>
//#include <Threads/Pool.hpp>
//#include <list>
//
//#include "MediaFiles/MediaFile.hpp"
//
//class Manager
//{
//public:
//	Manager()
//		: _pool(4)
//	{}
//
//	~Manager()
//	{}
//
//	bool loadFile(std::string &&file);
//	bool hasFile(std::string &&file);
// 	void addFile(std::shared_ptr<MyMediaFile> &&file);
//	void addFutur(std::future<bool> &&futur);
//	std::size_t waitForAll(std::chrono::milliseconds &&delay);
//private:
//	Manager(const Manager &o);
//	std::mutex _mutex;
//	std::map<std::string, std::shared_ptr<MyMediaFile>> _files;
//	Pool _pool;
//	std::list<std::future<bool>> _futures;
//};