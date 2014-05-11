
#ifndef Entity_HH_
#define Entity_HH_

#include <map>

#include "Core/Input.hh"
#include "Core/Timer.hh"
#include <memory>
//#include "OpenGL/Shader.hh"
#include "Utils/Barcode.h"
#include "glm/glm.hpp"
#include <Components/Component.hh>
#include <Utils/GlmSerialization.hpp>
#include <Core/Tags.hh>
#include <cereal/types/set.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/array.hpp>

class AScene;
class EntityManager;
class Entity;
class Shader;

class EntityData
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

	typedef std::vector<std::shared_ptr<Component::Base>>	t_ComponentsList;

	Entity &getHandle();
	void setHandle(Entity &handle);

private:
	friend class cereal::access;
	friend class std::vector<EntityData>;
	friend class EntityIdRegistrar;
	friend class AScene;

	std::weak_ptr<AScene> _scene;
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

	EntityData(std::weak_ptr<AScene> &&scene);
	EntityData();
	EntityData(const EntityData &o);
	const EntityData &operator=(const EntityData &o);
public:

	virtual ~EntityData();
	EntityData(EntityData &&o);

	std::weak_ptr<AScene>   getScene();
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
	void        			setLocalTransform(const glm::mat4 &t, bool forceMovedFlag = false);

	glm::mat4 const			&getGlobalTransform() const;
	// TO DELETE
	void 					computeGlobalTransform(glm::mat4 const &fatherTransform);  // <-- //
	void                    computeGlobalTransform(const Entity &parent);

	void                    computeTransformAndUpdateGraphnode();

	size_t 					getFlags() const;
	void 					setFlags(size_t flags);
	void 					addFlags(size_t flags);
	void 					removeFlags(size_t flags);

	void                    addTag(unsigned short tags);
	void                    removeTag(unsigned short tags);
	bool                    isTagged(unsigned short tags) const;
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
		unsigned short id = T::getTypeId();

		// if entity already have component, return it
		if (_code.isSet(id + MAX_TAG_NUMBER))
		{
			return  std::static_pointer_cast<T>(_components[id]);
		}
		// else if entity components array is to small, resize it
		else if (_components.size() <= id)
		{
			_components.resize(id + 4);
		}
		// if component has never been created, create one
		if (!_components[id].get())
		{
			_components[id] = std::make_shared<T>();
			assert(_components[id].get() != nullptr && "Memory error : Component creation failed.");
			_components[id]->setEntity(getHandle());
		}
		//init component
		std::static_pointer_cast<T>(_components[id])->init(std::forward<Args>(args)...);
		_code.add(id + MAX_TAG_NUMBER);
		_scene.lock()->informFilters(true, unsigned short(id + MAX_TAG_NUMBER), std::move(_handle));
		return std::static_pointer_cast<T>(_components[id]);
	}

	template <typename T>
	std::shared_ptr<T> getComponent() const
	{
		std::size_t id = T::getTypeId();
		// No more verification and static cast
		// better performance but more dangerous
		return std::static_pointer_cast<T>(_components[id]);
	}

	template <typename T>
	void removeComponent()
	{
		unsigned short id = T::getTypeId();
		if (!hasComponent<T>())
			return;
		_code.remove(id + MAX_TAG_NUMBER);
		_components[id].get()->reset();
		_scene.lock()->informFilters(false, id + MAX_TAG_NUMBER, std::move(_handle));
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
		ar(cereal::make_nvp("entityID", _scene.lock()->registrarSerializedEntity(_handle.getId())));
		ar(cereal::make_nvp("flags", _flags));
		ar(cereal::make_nvp("localTransform", _localTransform));

		// serialize tags
		std::array<bool, MAX_TAG_NUMBER> tags{false};
		for (unsigned int i = 0; i < MAX_TAG_NUMBER; ++i)
			tags[i] = _code.isSet(i);
		ar(cereal::make_nvp("tags", tags));

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

		// serialize graphnode
		std::set<std::size_t> childIds;
		for (auto e : _childs)
		{
			childIds.insert(_scene.lock()->registrarSerializedEntity(e.getId()));
		}
		ar(CEREAL_NVP(childIds));
		ar(cereal::make_nvp("haveParent", _parent.get() != nullptr));
		if (_parent.get() != nullptr)
			ar(cereal::make_nvp("parentID", _scene.lock()->registrarSerializedEntity(_parent.getId())));
		else
			ar(cereal::make_nvp("parentID", _scene.lock()->registrarSerializedEntity(-1)));
	}

	template <class Archive>
	void load(Archive &ar)
	{
		// load Entity informations
		auto scene = _scene.lock();
		std::size_t entityID;
		ar(entityID);
		scene->registrarUnserializedEntity(_handle, entityID);
		ar(_flags);
		ar(_localTransform);

		// load tags
		std::array<bool, MAX_TAG_NUMBER> tags{false};
		ar(tags);
		for (unsigned int i = 0; i < MAX_TAG_NUMBER; ++i)
		{
			if (tags[i])
				_code.add(i);
		}

		std::size_t cptNumber = 0;
		ar(cptNumber);
		for (std::size_t i = 0; i < cptNumber; ++i)
		{
			std::size_t type = 0;
			std::size_t typeId;
			ar(type);
			auto cpt = scene->createFromType(type, ar, _handle, typeId);
			if (_components.size() <= typeId)
				_components.resize(typeId + 1);
			_components[typeId] = cpt;
			_code.add(typeId + MAX_TAG_NUMBER);
			_scene.lock()->informFilters(true, typeId + MAX_TAG_NUMBER, std::move(_handle));
		}
		// unserialize graphnode
		EntityIdRegistrar::GraphNodeUnserialize graphUnser;
		ar(graphUnser.childs);
		ar(graphUnser.haveParent);
		ar(graphUnser.parent);
		scene->registrarGraphNode(entityID, graphUnser);
	}

	//
	//
	////////////////
};

#endif
