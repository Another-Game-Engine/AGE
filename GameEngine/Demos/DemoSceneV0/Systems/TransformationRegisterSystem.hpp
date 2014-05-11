#pragma once

#include <memory>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
//#include <Entities/EntityData.hh>
#include <Utils/File.hpp>

class TransformationRegisterSystem;
class AScene;
class Entity;

class TRSFilter : public EntityFilter
{
public:
	TRSFilter(std::weak_ptr<AScene> &&scene);
	virtual ~TRSFilter();
	TransformationRegisterSystem *_system;
	virtual void componentAdded(Entity &&e, unsigned short typeId);
};

class TransformationRegisterSystem : public System
{
public:
	TransformationRegisterSystem(std::weak_ptr<AScene> &&scene);
	void setFile(const File &file);
	virtual ~TransformationRegisterSystem();
	void loadEntity(Entity &e);
	void saveToFile();
private:
	TRSFilter _filter;
	std::map<std::string, glm::mat4> _loaded;
	File _file;

	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual bool initialize();
};