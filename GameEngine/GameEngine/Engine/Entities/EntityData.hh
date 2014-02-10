
#ifndef Entity_HH_
#define Entity_HH_

#include <map>

#include "Core/Input.hh"
#include "Core/Timer.hh"
#include <memory>
#include "OpenGL/Shader.hh"
#include "Utils/Barcode.h"
#include "Utils/PubSub.hpp"
#include "glm/glm.hpp"
#include <Components/Component.hh>
#include <Utils/GlmSerialization.hpp>
#include <Core/Tags.hh>

class AScene;
class EntityManager;
class Entity;

class EntityData : public PubSub
{
private:
	static size_t 					_currentId;
	Entity                          _handle;
public:
	enum	GetFlags
	{
		ONLY_THIS_FLAGS,
		THIS_FLAGS,
		NOT_THIS_FLAGS
	};

	enum	EntityFlags
	{
		HAS_MOVED = 1,
		ACTIVE = 2,

	};

	typedef std::vector<std::shared_ptr<Component::Base> >	t_ComponentsList;

	Entity &getHandle();
	void setHandle(Entity &handle);

private:
	friend class cereal::access;
	AScene              *_scene;
	size_t 				_flags;

	glm::mat4 			_localTransform;
	glm::mat4 			_globalTransform;

	glm::vec3           _localRotation;
	glm::vec3           _localScale;
	glm::vec3           _localTranslation;

	glm::vec3           _globalRotation;
	glm::vec3           _globalScale;
	glm::vec3           _globalTranslation;

	t_ComponentsList	_components;

	Barcode             _code;
public:
	EntityData(AScene *scene);
	virtual ~EntityData();

	AScene                  *getScene() const;
	void                    translate(const glm::vec3 &v);
	void                    setTranslation(const glm::vec3 &v);
	glm::vec3 const         &getTranslation() const;
	void                    rotate(const glm::vec3 &v);
	void                    setRotation(const glm::vec3 &v);
	glm::vec3 const         &getRotation() const;
	void                    scale(const glm::vec3 &v);
	void                    setScale(const glm::vec3 &v);
	glm::vec3 const         &getScale() const;


	glm::mat4 const  		&getLocalTransform();
	// TO DELETE
	glm::mat4   			&setLocalTransform();  // <-- //

	glm::mat4 const			&getGlobalTransform() const;
	// TO DELETE
	void 					computeGlobalTransform(glm::mat4 const &fatherTransform);  // <-- //
	void                    computeGlobalTransform(const Entity &parent);

	void                    computeTransformAndUpdateGraphnode();

	size_t 					getFlags() const;
	void 					setFlags(size_t flags);
	void 					addFlags(size_t flags);
	void 					removeFlags(size_t flags);

	void                    addTag(unsigned int tags);
	void                    removeTag(unsigned int tags);
	bool                    isTagged(unsigned int tags) const;
	bool                    isTagged(Barcode &code);

	Barcode                 &getCode();
	void                    reset();

	//////////////
	//
	// Graphnode

	const Entity	    	&getParent() const;
	void 					removeChild(Entity &child, bool notify = true);
	void 					setParent(Entity &parent, bool notify = true);
	void 					addChild(Entity &child, bool notify = true);
	void                    removeParent(bool notify = true);
	std::set<Entity>::iterator getChildsBegin();
	std::set<Entity>::iterator getChildsEnd();

	Entity                  _parent;
	std::set<Entity>        _childs;

	//
	//
	//////////////

	////////////////////////////
	//
	//
	// COMPONENTS OPERATIONS
	//
	//
	//

	template <typename T>
	bool hasComponent() const
	{
		return _code.isSet(T::getTypeId() + MAX_TAG_NUMBER);
	}

	template <typename T, typename... Args>
	std::shared_ptr<T> addComponent(Args &&...args)
	{
		// get the component type ID
		unsigned int id = T::getTypeId();

		// if entity already have component, return it
		if (_code.isSet(id + MAX_TAG_NUMBER))
		{
			return  std::static_pointer_cast<T>(_components[id]);
		}
		// else if entity components array is to small, resize it
		else if (_components.size() <= id)
		{
			_components.resize(id + 1);
		}
		// if component has never been created, create one
		if (!_components[id].get())
		{
			_components[id] = std::shared_ptr<T>(new T());
			assert(_components[id].get() != nullptr && "Memory error : Component creation failed.");
			_components[id]->setEntity(getHandle());
		}
		//init component
		std::static_pointer_cast<T>(_components[id])->init(std::forward<Args>(args)...);
		_code.add(id + MAX_TAG_NUMBER);
		broadCast(std::string("componentAdded" + std::to_string(id + MAX_TAG_NUMBER)), _handle);
		return std::static_pointer_cast<T>(_components[id]);
	}

	template <typename T>
	std::shared_ptr<T> getComponent() const
	{
		unsigned int id = T::getTypeId();
		if (!hasComponent<T>())
			return nullptr;
		return std::static_pointer_cast<T>(_components[id]);
	}

	template <typename T>
	void removeComponent()
	{
		unsigned int id = T::getTypeId();
		if (!hasComponent<T>())
			return;
		_code.remove(id + MAX_TAG_NUMBER);
		_components[id].get()->reset();
		broadCast(std::string("componentRemoved" + std::to_string(id + MAX_TAG_NUMBER)), _handle);
		// component remove -> signal to system
	}


	//
	//
	/////////////

	//////////////////
	//
	// Serialization

	template <class Archive>
	void save(Archive &ar) const
	{
		// Save Entity informations
		ar(cereal::make_nvp("entityID", _scene->registrarSerializedEntity(_handle.getId())));
		ar(cereal::make_nvp("flags", _flags));
		ar(cereal::make_nvp("localTransform", _localTransform));
		ar(cereal::make_nvp("globalTransform", _globalTransform));

		// Save Entity Components
		std::size_t cptNumber = 0;

		for (auto &e : _components)
		{
			if (!e.get())
				continue;
			++cptNumber;
		}
		ar(cereal::make_nvp("component_number", cptNumber));
		for (auto &e : _components)
		{
			if (!e.get())
				continue;
			e->serializeBase(ar);
		}
	}

	template <class Archive>
	void load(Archive &ar)
	{
		// load Entity informations
		std::size_t entityID;
		ar(entityID);
		_scene->registrarUnserializedEntity(_handle, entityID);
		ar(_flags);
		ar(_localTransform);
		ar(_globalTransform);
		std::size_t cptNumber = 0;
		ar(cptNumber);
		for (unsigned int i = 0; i < cptNumber; ++i)
		{
			std::size_t type = 0;
			unsigned int typeId;
			ar(type);
			unsigned int position;
			Component::Base *cpt = _scene->createFromType(type, ar, _handle, typeId);
			cpt->setEntity(_handle);
			if (_components.size() <= typeId)
				_components.resize(typeId + 1);
			_components[typeId] = std::shared_ptr<Component::Base>(cpt);
			_code.add(typeId + MAX_TAG_NUMBER);
			broadCast(std::string("componentAdded" + std::to_string(typeId + MAX_TAG_NUMBER)), _handle);
		}
	}

	//
	//
	////////////////
};

#endif