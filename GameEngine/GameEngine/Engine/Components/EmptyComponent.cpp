#include "EmptyComponent.hh"

#include "Core/Engine.hh"

namespace Component
{

	EmptyComponent::EmptyComponent()
		: ComponentBase("EmptyComponent")
	{}

	EmptyComponent::~EmptyComponent(void)
	{}
};