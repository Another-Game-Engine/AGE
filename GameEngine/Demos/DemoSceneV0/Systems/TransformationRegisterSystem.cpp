#include <Systems/TransformationRegisterSystem.hpp>
#include <Context/SdlContext.hh>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <Utils/GlmSerialization.hpp>


TRSFilter::TRSFilter(std::weak_ptr<AScene> scene)
: EntityFilter(scene)
, _system(nullptr)
{}

TRSFilter::~TRSFilter(){}

void TRSFilter::_componentAdded(Entity &e, std::size_t typeId)
{
	if (_code.match(e->getCode()))
	{
		_collection.insert(e);
		assert(_system != nullptr && "You forgot to link system.");
		_system->loadEntity(e);
	}
}


TransformationRegisterSystem::TransformationRegisterSystem(std::weak_ptr<AScene> scene)
: System(scene)
, _filter(scene)
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
	auto compo = e->getComponent<Component::TransformationRegister>();
	auto it = _loaded.find(compo->name);
	if (it != std::end(_loaded))
	{
		e->setLocalTransform(it->second, true);
	}
}

void TransformationRegisterSystem::saveToFile()
{
	std::shared_ptr<Component::TransformationRegister> compo;
	for (auto e : _filter.getCollection())
	{
		compo = e->getComponent<Component::TransformationRegister>();
		_loaded[compo->name] = e->getLocalTransform();
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
