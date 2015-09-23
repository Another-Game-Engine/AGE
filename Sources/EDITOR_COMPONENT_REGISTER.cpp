#pragma once

#include <Components/ComponentRegistrationManager.hpp>

#include <Reader/Components/RotationComponent.hpp>
#include <Reader/Components/Lifetime.hpp>

void RegisterComponents()
{
	REGISTER_COMPONENT_TYPE(AGE::Lifetime);
	REGISTER_COMPONENT_TYPE(AGE::RotationComponent);
}