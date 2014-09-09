#include <Render/Pipeline.hh>
#include <iostream>
#include <algorithm>
#include <Core/Drawable.hh>

namespace gl
{
	Pipeline::Pipeline(uint8_t priority)
		: _priority(priority),
		_type(DrawType::GLOBAL),
		_toRender(NULL)
	{
	}

	Pipeline::~Pipeline()
	{
	}

	Pipeline &Pipeline::update(AGE::Vector<AGE::Drawable> const &toRender)
	{
		_toRender = &toRender;
		for (size_t index = 0; index < _renderPass.size(); ++index)
			_renderPass[index]->setDraw(*_toRender);
		return (*this);
	}

	Pipeline &Pipeline::config(DrawType type)
	{
		_type = type;
		return (*this);
	}

	Pipeline &Pipeline::pushRender(Render *render)
	{
		_render.push_back(render);
		return (*this);
	}
	
	Pipeline &Pipeline::pushRenderPass(RenderPass *renderPass)
	{
		_render.push_back(renderPass);
		_renderPass.push_back(renderPass);
		return (*this);
	}

	bool Pipeline::operator<(Pipeline const &p)
	{
		return (_priority < p._priority);
	}

	bool Pipeline::operator>(Pipeline const &p)
	{
		return (_priority > p._priority);
	}

	bool Pipeline::operator<=(Pipeline const &p)
	{
		return (_priority <= p._priority);
	}

	bool Pipeline::operator>=(Pipeline const &p)
	{
		return (_priority >= p._priority);
	}
	bool Pipeline::operator==(Pipeline const &p)
	{
		return (_priority == p._priority);
	}

	bool Pipeline::operator!=(Pipeline const &p)
	{
		return (_priority != p._priority);
	}

}