#include <Render/Pipeline.hh>
#include <iostream>
#include <algorithm>

namespace gl
{
	Pipeline::Pipeline()
		: _times(NULL),
		_rendering(NULL),
		_nbrRendering(0)
	{
	}

	Pipeline::~Pipeline()
	{
		if (_rendering != NULL)
			delete[] _rendering;
		if (_times != NULL)
			delete[] _times;
	}

	Pipeline &Pipeline::setToRender(AGE::Vector<AGE::Drawable> const &toRender)
	{
		_toRender = &toRender;
		return (*this);
	}

	Pipeline &Pipeline::setRendering(uint8_t time, Render *rendering)
	{
		_min = std::min(time, _min);
		_max = std::max(time, _max);
		uint8_t tmp_nbrRendering = _nbrRendering + 1;
		Render **tmp_rendering = new Render*[tmp_nbrRendering];
		memcpy(tmp_rendering, _rendering, sizeof(Render *)* _nbrRendering);
		tmp_rendering[tmp_nbrRendering - 1] = rendering;
		uint8_t *tmp_times = new uint8_t[tmp_nbrRendering];
		memcpy(tmp_times, _times, sizeof(uint8_t)* _nbrRendering);
		tmp_times[tmp_nbrRendering - 1] = time;
		if (_rendering != NULL)
			delete[] _rendering;
		if (_times != NULL)
			delete[] _times;
		_rendering = tmp_rendering;
		_times = tmp_times;
		_nbrRendering = tmp_nbrRendering;
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

	Pipeline &Pipeline::draw(uint8_t time)
	{
		for (uint8_t index = 0; index < _nbrRendering; ++index)
		{
			if (time == _times[index])
			{
				if (_rendering[index]->getType() == RENDER_PASS)
					((RenderPass *)_rendering[index])->setObjectsToRender(*_toRender);
				_rendering[index]->render();
			}
		}
		return (*this);
	}
}