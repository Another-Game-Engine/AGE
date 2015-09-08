#include <Components/Component.hh>

namespace AGE
{

	ComponentBase::ComponentBase()
		: _typeId(-1)
	{
	}

	ComponentBase::~ComponentBase()
	{
	}

	ComponentType ComponentBase::_typeCounter = 0;

	void ComponentBase::copyFrom(const ComponentBase *model)
	{
		_typeId = model->_typeId;
		_copyFrom(model);
	}

	bool ComponentBase::doSerialize()
	{
		return true;
	}

#ifdef EDITOR_ENABLED
	bool ComponentBase::editorUpdate()
	{
		return false;
	}
	bool ComponentBase::isExposedInEditor()
	{
		return true;
	}
	bool ComponentBase::serializeInExport()
	{
		return true;
	}
#endif
}