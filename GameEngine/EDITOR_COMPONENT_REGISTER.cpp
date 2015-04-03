#pragma once

#include <Components/ComponentRegistrationManager.hpp>

#include <RotationComponent.hpp>
#include <Lifetime.hpp>

void RegisterComponents()
{
	REGISTER_COMPONENT_TYPE(AGE::Lifetime);
	REGISTER_COMPONENT_TYPE(AGE::RotationComponent);
}