#include <System/System.h>

namespace AGE
{
	class RotationSystem : public AGE::System<RotationSystem>
	{
	public:
		RotationSystem(AScene *scene);
		virtual ~RotationSystem();
	private:
		EntityFilter _filter;
		virtual void updateBegin(float time);
		virtual void updateEnd(float time);
		virtual void mainUpdate(float time);
		virtual bool initialize();
	};
}