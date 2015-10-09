#include <System/System.h>

namespace AGE
{
	class AnimatedSklSystem : public AGE::System < AnimatedSklSystem >
	{
	public:
		AnimatedSklSystem(AScene *scene);
		virtual ~AnimatedSklSystem();
	private:
		EntityFilter _filter;
		virtual void updateBegin(float time);
		virtual void updateEnd(float time);
		virtual void mainUpdate(float time);
		virtual bool initialize();
	};
}