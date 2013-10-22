#include "EmptyComponent.hh"

#include "Core/Engine.hh"

namespace Components
{

	EmptyComponent::EmptyComponent()
		: AComponent("EmptyComponent")
	{}

	EmptyComponent::~EmptyComponent(void)
	{}

	void EmptyComponent::start()
	{}

	void EmptyComponent::update()
	{}

	void EmptyComponent::stop()
	{}
};