#ifndef		ROTATION_FORCE_HH_
#define		ROTATION_FORCE_HH_

#include "AComponent.hh"
#include "Utils/SmartPointer.hh"
#include <glm/glm.hpp>

namespace Components
{
	class RotationForce : public AComponent
	{
	public:
		RotationForce(const glm::vec3 &force = glm::vec3(0,0,0));
		virtual ~RotationForce(void);

		virtual void	start() final;
		virtual void	update() final;
		virtual void	stop() final;

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