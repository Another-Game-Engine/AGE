#pragma once

#include <Components/ComponentRegistrationManager.hpp>

#include <EngineCoreTest/Components/RotationComponent.hpp>
#include <EngineCoreTest/Components/Lifetime.hpp>

void RegisterComponents()
{
	REGISTER_COMPONENT_TYPE(AGE::Lifetime);
	REGISTER_COMPONENT_TYPE(AGE::RotationComponent);
}