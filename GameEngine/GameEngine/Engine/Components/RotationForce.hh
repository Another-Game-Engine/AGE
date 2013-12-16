#ifndef		ROTATION_FORCE_HH_
#define		ROTATION_FORCE_HH_

#include "Component.hh"
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>

namespace Component
{
	class RotationForce : public Component::ComponentBase<RotationForce>
	{
	public:
		RotationForce(Engine &engine, const glm::vec3 &force = glm::vec3(0,0,0));
		virtual ~RotationForce(void);

		void setForce(const glm::vec3 &force);
		const glm::vec3 &getForce() const;
	private:
		glm::vec3 _force;
	private:
		RotationForce(RotationForce const &);
		RotationForce &operator=(RotationForce const &);
	};

}

#endif //!ROTATION_FORCE_HH_