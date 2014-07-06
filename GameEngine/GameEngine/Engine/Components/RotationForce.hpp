#ifndef		ROTATION_FORCE_HH_
#define		ROTATION_FORCE_HH_

#include "Component.hh"
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>

namespace Component
{
	struct RotationForce : public Component::ComponentBase<RotationForce>
	{
		RotationForce();
		virtual ~RotationForce(void);

		void init(AScene *, const glm::vec3 &force = glm::vec3(0, 0, 0));
		virtual void reset(AScene *);

		void setForce(const glm::vec3 &force);
		const glm::vec3 &getForce() const;
		
		RotationForce(RotationForce const &o)
		{
			force = o.force;
		}
		RotationForce &operator=(RotationForce const &o)
		{
			force = o.force;
			return *this;
		}

		//////
		////
		// Serialization

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
	};

}

#endif //!ROTATION_FORCE_HH_
