#ifndef  __VELOCITY_COMPONENT_HPP__
# define __VELOCITY_COMPONENT_HPP__

#include <Components/Component.hh>
#include <functional>
#include <Core/Engine.hh>
#include <Entities/Entity.hh>

namespace Component
{
	struct Velocity : public ComponentBase<Velocity>
	{
	private:
		Velocity(const Velocity &o);
		Velocity &operator=(const Velocity &o);
	public:
		Velocity() :
			direction(0,0,0),
			function([&](double time, double totalTime, const glm::vec3 direction){return direction * glm::vec3(time);}),
			ComponentBase<Velocity>()
		{}

		virtual ~Velocity()
		{}

		void init(const glm::vec3 &_direction, std::function<glm::vec3(double time, double totalTime, const glm::vec3 &direction)> _function = [](double time, double totalTime, const glm::vec3 &direction){
			return direction * static_cast<float>(time);
		})
		{
			direction = _direction;
			function = _function;
		}

		const glm::vec3 compute(double time, double totalTime) const
		{
			auto r = function(time, totalTime, this->direction);
			return r;
		}

		virtual void reset()
		{}

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar, Entity e)
		{
			auto res = new Velocity();
			res->setEntity(e);
			ar(*res);
			return res;
		}

		template <typename Archive>
		void save(Archive &ar) const
		{
		}

		template <typename Archive>
		void load(Archive &ar)
		{
		}

		// !Serialization
		////
		//////

		glm::vec3 direction;
		std::function<glm::vec3(double time, double totalTime, const glm::vec3 &direction)> function;
	};

}
#endif  //__VELOCITY_COMPONENT_HPP__