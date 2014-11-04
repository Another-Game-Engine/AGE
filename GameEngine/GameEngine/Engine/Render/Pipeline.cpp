#include <Render/Pipeline.hh>
#include <iostream>
#include <algorithm>
#include <Core/CullableObjects.hh>

namespace gl
{
	Pipeline::Pipeline()
		: _type(DrawType::ALL_OBJECT),
		_toRender(NULL)
	{
		_drawFunc[DrawType::NONE_OBJECT] = &Pipeline::drawNoneObject;
		_drawFunc[DrawType::ALL_OBJECT] = &Pipeline::drawAllObject;
		_drawFunc[DrawType::EACH_FOLLOWING_OBJECT] = &Pipeline::drawEachFollowObject;
	}

	Pipeline::Pipeline(Pipeline const &copy)
		: _type(copy._type),
		_toRender(copy._toRender),
		_render(copy._render),
		_renderPass(copy._renderPass)
	{
		_drawFunc[DrawType::NONE_OBJECT] = &Pipeline::drawNoneObject;
		_drawFunc[DrawType::ALL_OBJECT] = &Pipeline::drawAllObject;
		_drawFunc[DrawType::EACH_FOLLOWING_OBJECT] = &Pipeline::drawEachFollowObject;
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

	Pipeline &Pipeline::pushRender(BaseRender *render)
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
			_renderPass[index]->setPass(*_toRender, 0, _toRender->size());
		for (size_t index = 0; index < _render.size(); ++index)
			_render[index]->render();
	}

	void Pipeline::drawNoneObject()
	{
		for (size_t index = 0; index < _renderPass.size(); ++index)
			_renderPass[index]->setPass();
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
				_renderPass[index]->setPass(*_toRender, r, r + 1);
			for (size_t index = 0; index < _render.size(); ++index)
				_render[index]->render();
		}
	}
}