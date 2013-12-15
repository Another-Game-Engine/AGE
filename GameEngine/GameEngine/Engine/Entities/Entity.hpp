	template <typename T>
	bool hasComponent() const
	{
		return code_.isSet<T>();
	}

	template <typename T, typename... Args>
	SmartPointer<T> addComponent(Args ...args)
	{
		unsigned int id = T::getTypeId();
		if (hasComponent(id))
		{
			return static_cast<SmartPointer<T> >(_components[id]);
		}
		else if (_components.size() <= id)
		{
			_components.resize(id + 10);
		}
		SmartPointer<T> tmp(new T(_engine, args...));
		// todo assert if new T fail
		_code.add(id);
		_components[id] = tmp;
		tmp->setFather(this);
		pub(std::string("componentAdded" + std::to_string(id)), _handle);
		return tmp;
	}

	template <typename T>
	SmartPointer<T> getComponent() const
	{
		unsigned int id = T::getTypeId();
		if (!hasComponent(id))
			return nullptr;
		return static_cast<SmartPointer<T> >(_components[id]);
	}

	template <typename T>
	void removeComponent()
	{
		unsigned int id = T::getTypeId();
		if (!hasComponent(id))
			return;
		code_.remove(id);
		delete _components[id];
		_components[id]	= nullptr;
		pub(std::string("componentRemoved" + std::to_string(id)), _handle);
		// component remove -> signal to system
	}