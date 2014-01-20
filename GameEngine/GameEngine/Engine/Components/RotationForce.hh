#ifndef		ROTATION_FORCE_HH_
#define		ROTATION_FORCE_HH_

#include "Component.hh"
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>

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

		glm::vec3 force;
	private:
		RotationForce(RotationForce const &);
		RotationForce &operator=(RotationForce const &);
	};

}

#endif //!ROTATION_FORCE_HH_