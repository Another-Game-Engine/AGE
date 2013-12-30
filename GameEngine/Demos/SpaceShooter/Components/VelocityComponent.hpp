#ifndef  __VELOCITY_COMPONENT_HPP__
# define __VELOCITY_COMPONENT_HPP__

#include <Components/Component.hh>
#include <functional>
#include <Core/Engine.hh>
#include <Entities/Handle.hh>

namespace Component
{
	class Velocity : public ComponentBase<Velocity>
	{
	private:
		Velocity(const Velocity &o);
		Velocity &operator=(const Velocity &o);
	public:
		Velocity(Engine &engine, Handle &entity) :
			_direction(0,0,0),
			_function([&](double time, double totalTime, const glm::vec3 direction){return direction * glm::vec3(time);}),
			ComponentBase<Velocity>(engine, entity, "VelocityComponent")
		{}

		virtual ~Velocity()
		{}

		void init(const glm::vec3 &direction, std::function<glm::vec3(double time, double totalTime, const glm::vec3 &direction)> function = [](double time, double totalTime, const glm::vec3 &direction){
			return direction * static_cast<float>(time);
		})
		{
			_direction = direction;
			_function = function;
		}

		const glm::vec3 compute(double time, double totalTime) const
		{
			auto r = _function(time, totalTime, this->_direction);
			return r;
		}

		virtual void reset()
		{}
	private:
		std::function<glm::vec3(double time, double totalTime, const glm::vec3 &direction)> _function;
		glm::vec3 _direction;
	};

}
#endif  //__VELOCITY_COMPONENT_HPP__