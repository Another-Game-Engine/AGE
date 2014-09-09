#include <Render/Pipeline.hh>
#include <iostream>
#include <algorithm>
#include <Core/Drawable.hh>

namespace gl
{
	Pipeline::Pipeline()
	{
	}

	Pipeline::~Pipeline()
	{
	}

	Pipeline &Pipeline::setDraw(AGE::Vector<AGE::Drawable> const &r)
	{
		toRender = &r;
		return (*this);
	}

	Pipeline &Pipeline::addRendering(uint8_t time, Render *rendering)
	{
		_rendering[time] = rendering;
		return (*this);
	}

	uint8_t Pipeline::getMaxTime() const
	{
		return (_max);
	}

	uint8_t Pipeline::getMinTime() const
	{
		return (_min);
	}

	Pipeline &Pipeline::draw(uint8_t time, size_t index)
	{
		draw(time, index, index + 1);
		return (*this);
	}

	Pipeline &Pipeline::draw(uint8_t time)
	{
		draw(time, 0, toRender->size());
		return (*this);
	}

	Pipeline &Pipeline::draw(uint8_t time, size_t start, size_t end)
	{
		auto &element = _rendering.find(time);
		if (element == _rendering.end())
			return (*this);
		if (element->second->getType() == RenderType::RENDER_PASS)
			((RenderPass *)element->second)->setDraw(*toRender, start, end);
		element->second->render();
		return (*this);
	}

	Pipeline &Pipeline::draw()
	{

	}
}