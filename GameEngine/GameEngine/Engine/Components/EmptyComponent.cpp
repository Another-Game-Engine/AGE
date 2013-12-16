#include "EmptyComponent.hh"

#include "Core/Engine.hh"

namespace Component
{

	EmptyComponent::EmptyComponent(Engine &engine, Handle &entity)
		: ComponentBase<EmptyComponent>(engine, entity, "EmptyComponent")
	{}

	EmptyComponent::~EmptyComponent(void)
	{}
};