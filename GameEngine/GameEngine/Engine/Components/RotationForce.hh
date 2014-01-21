#ifndef		ROTATION_FORCE_HH_
#define		ROTATION_FORCE_HH_

#include "Component.hh"
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>

namespace Component
{
	struct RotationForce : public Component::ComponentBase<RotationForce>
	{
		RotationForce();
		virtual ~RotationForce(void);

		void init(const glm::vec3 &force = glm::vec3(0, 0, 0));
		virtual void reset();

		void setForce(const glm::vec3 &force);
		const glm::vec3 &getForce() const;

		//////
		////
		// Serialization

		template <typename Archive>
		Base *unserialize(Archive &ar)
		{
			auto res = new RotationForce();
			ar(*res);
			return res;
		}

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(CEREAL_NVP(force));
		}

		// !Serialization
		////
		//////

		glm::vec3 force;
	private:
		RotationForce(RotationForce const &);
		RotationForce &operator=(RotationForce const &);
	};

}

#endif //!ROTATION_FORCE_HH_