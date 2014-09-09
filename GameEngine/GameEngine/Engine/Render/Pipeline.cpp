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
		_drawFunc[DrawType::NONE] = &Pipeline::drawNone;
		_drawFunc[DrawType::GLOBAL] = &Pipeline::drawGlobal;
		_drawFunc[DrawType::SEPARATE] = &Pipeline::drawSeparate;
	}

	Pipeline::~Pipeline()
	{
	}

	Pipeline &Pipeline::update(AGE::Vector<AGE::Drawable> const &toRender)
	{
		_toRender = &toRender;
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

	Pipeline &Pipeline::draw()
	{
		(this->*_drawFunc[_type])();
		return (*this);
	}

	void Pipeline::drawAllObject()
	{
		if (_toRender == NULL)
			return;
		for (size_t index = 0; index < _renderPass.size(); ++index)
			_renderPass[index]->setDraw(*_toRender, 0, _toRender->size());
		for (size_t index = 0; index < _render.size(); ++index)
			_render[index]->render();
	}

	void Pipeline::drawNoneObject()
	{
		for (size_t index = 0; index < _renderPass.size(); ++index)
			_renderPass[index]->setDraw();
		for (size_t index = 0; index < _render.size(); ++index)
			_render[index]->render();
	}

	void Pipeline::drawEachFollowObject()
	{
		if (_toRender == NULL)
			return ;
		for (size_t r = 0; r < _toRender->size(); ++r)
		{
			for (size_t index = 0; index < _renderPass.size(); ++index)
				_renderPass[index]->setDraw(*_toRender, r, r + 1);
			for (size_t index = 0; index < _render.size(); ++index)
				_render[index]->render();
		}
	}
}