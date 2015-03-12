#include <Systems/FreeFlyCamera.hh>
#include <Components/CameraComponent.hpp>
#include <Core/Inputs/Input.hh>

namespace AGE
{
	FreeFlyCamera::FreeFlyCamera(AScene *scene) :
		System(std::move(scene)),
		_cameras(std::move(scene))
	{
		_name = "Free fly camera";
	}

	bool FreeFlyCamera::initialize()
	{
		_cameras.requireComponent<CameraComponent>();
		return (true);
	}

	void FreeFlyCamera::updateBegin(double time)
	{

	}

	void FreeFlyCamera::mainUpdate(double time)
	{
		const float verticalAngleLimit = glm::pi<float>() / 2.0f;
		size_t camIndex = 0;
		float camTranslationSpeed = 5.0f;
		float camRotationSpeed = 2.0f;

		if (_cameras.getCollection().size() < _cameraAngles.size())
			_cameraAngles.resize(_cameras.getCollection().size());
		for (auto cam : _cameras.getCollection())
		{
			auto &camLink = cam.getLink();
			Input *inputs = _scene->getInstance<Input>();

			if (inputs->getPhysicalKeyPressed(AGE_w))
				camLink.setForward(glm::vec3(0, 0, -camTranslationSpeed * time));
			if (inputs->getPhysicalKeyPressed(AGE_s))
				camLink.setForward(glm::vec3(0, 0, camTranslationSpeed * time));
			if (inputs->getPhysicalKeyPressed(AGE_a))
				camLink.setForward(glm::vec3(-camTranslationSpeed * time, 0, 0));
			if (inputs->getPhysicalKeyPressed(AGE_d))
				camLink.setForward(glm::vec3(camTranslationSpeed * time, 0, 0));

			if (camIndex == _cameraAngles.size())
			{
				glm::quat camRotation = camLink.getOrientation();
				_cameraAngles.emplace_back(glm::vec2(glm::eulerAngles(camRotation)));
			}

			if (inputs->getPhysicalKeyPressed(AGE_UP))
				_cameraAngles[camIndex].x += camRotationSpeed * time;
			if (inputs->getPhysicalKeyPressed(AGE_DOWN))
				_cameraAngles[camIndex].x -= camRotationSpeed * time;
			if (inputs->getPhysicalKeyPressed(AGE_RIGHT))
				_cameraAngles[camIndex].y -= camRotationSpeed * time;
			if (inputs->getPhysicalKeyPressed(AGE_LEFT))
				_cameraAngles[camIndex].y += camRotationSpeed * time;

			_cameraAngles[camIndex].x = glm::clamp(_cameraAngles[camIndex].x, -verticalAngleLimit, verticalAngleLimit);

			camLink.setOrientation(glm::quat(glm::vec3(_cameraAngles[camIndex], 0)));
			++camIndex;
		}
	}

	void FreeFlyCamera::updateEnd(double time)
	{

	}
}