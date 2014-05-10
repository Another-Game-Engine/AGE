#include <Systems/TransformationRegisterSystem.hpp>
#include <Context/SdlContext.hh>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <Utils/GlmSerialization.hpp>


TRSFilter::TRSFilter(std::weak_ptr<AScene> &&scene)
: EntityFilter(std::move(scene))
, _system(nullptr)
{}

TRSFilter::~TRSFilter(){}

void TRSFilter::componentAdded(EntityData &&e, COMPONENT_ID typeId)
{
	// @CESAR TODO
	if (e.getBarcode().match(_barcode))
	{
		_collection.insert(e.getEntity());
		assert(_system != nullptr && "You forgot to link system.");
		_system->loadEntity(const_cast<Entity&>(e.getEntity()));
	}
}

TransformationRegisterSystem::TransformationRegisterSystem(std::weak_ptr<AScene> &&scene)
: System(std::move(scene))
, _filter(std::move(scene))
, _file("")
{
	_name = "transformation_register_system";
}

void TransformationRegisterSystem::setFile(const File &file)
{
	_file = file;
	if (!_file.exists())
		return;
	std::ifstream s(_file.getFullName());
	cereal::JSONInputArchive ar(s);
	ar(_loaded);
}

TransformationRegisterSystem::~TransformationRegisterSystem()
{}

void TransformationRegisterSystem::loadEntity(Entity &e)
{
	auto compo = _scene.lock()->getComponent<Component::TransformationRegister>(e);
	auto it = _loaded.find(compo->name);
	if (it != std::end(_loaded))
	{
		_scene.lock()->setLocalTransform(e, it->second);
	}
}

void TransformationRegisterSystem::saveToFile()
{
	Component::TransformationRegister *compo = nullptr;
	for (auto e : _filter.getCollection())
	{
		compo = _scene.lock()->getComponent<Component::TransformationRegister>(e);
		_loaded[compo->name] = _scene.lock()->getLocalTransform(e);
	}
	assert(_file.getFullName().size() != 0 && "File path is not defined.");
	std::ofstream s(_file.getFullName());
	cereal::JSONOutputArchive ar(s);
	ar(_loaded);
}

void TransformationRegisterSystem::updateBegin(double time)
{}

void TransformationRegisterSystem::updateEnd(double time)
{}

void TransformationRegisterSystem::mainUpdate(double time)
{
	std::shared_ptr<Component::TransformationRegister> compo;
	for (auto e : _filter.getCollection())
	{
	}

	if (_scene.lock()->getInstance<Input>()->getInput(SDLK_p))
	{
		saveToFile();
	}
}

bool TransformationRegisterSystem::initialize()
{
	_filter.requireComponent<Component::TransformationRegister>();
	_filter._system = this;
	return true;
}
