#include "FPSSystem.hh"
#include <Text/FontManager.hh>
#include <Core/Renderer.hh>

FPSSystem::FPSSystem(std::weak_ptr<AScene> &&scene)
: System(std::move(scene))
{}

FPSSystem::~FPSSystem(void)
{
	std::cout << "dtor" << std::endl;
}

void FPSSystem::updateBegin(double time)
{}

void FPSSystem::updateEnd(double time)
{}

bool FPSSystem::initialize()
{
	if (!_scene.lock()->getInstance<FontManager>()->loadFont(File("../../Assets/Serialized/myFont.cpdFont")))
		return false;

	if (_scene.lock()->getInstance<Renderer>()->addShader("2DText",
		"../../Shaders/2DText.vp",
		"../../Shaders/2DText.fp") == nullptr)
		return false;
	return true;
}


void FPSSystem::mainUpdate(double time)
{
	float ftime = static_cast<float>(time);

	_timeCounter += ftime;
	_frameCounter += 1;

    _scene.lock()->getInstance<FontManager>()->draw2DString("FPS :\n" + std::to_string(_lastFrame), "myFont", 40, glm::ivec2(10, 10), glm::vec4(1), "2DText");

	if (_timeCounter >= 1.0f)
	{
		_lastFrame = _frameCounter;
		_timeCounter = 0.0f;
		_frameCounter = 0;
	}
}