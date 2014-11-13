#include <Render/RenderManager.hh>
#include <Render/Storage.hh>
#include <algorithm>
#include <iostream>
#include <string>
#include <Render/Pool.hh>
#include <Render/SimpleFormGeometry.hh>
#include <Render/Data.hh>


RenderManager::RenderManager()
{
	static size_t id = 0;
	_layout = id++;
}

RenderManager::~RenderManager()
{
}