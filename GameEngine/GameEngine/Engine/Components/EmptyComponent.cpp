#include "EmptyComponent.hh"

#include "Core/Engine.hh"

namespace Component
{

	EmptyComponent::EmptyComponent(Engine &engine)
		: ComponentBase<EmptyComponent>(engine, "EmptyComponent")
	{}

	EmptyComponent::~EmptyComponent(void)
	{}
};