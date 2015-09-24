#pragma once

#include <Components/ComponentRegistrationManager.hpp>

#include <Reader/Components/RotationComponent.cpp>
#include <Reader/Components/Lifetime.cpp>

void RegisterComponents()
{
	REGISTER_COMPONENT_TYPE(AGE::Lifetime);
	REGISTER_COMPONENT_TYPE(AGE::RotationComponent);
}