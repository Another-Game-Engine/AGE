#pragma once

#include <cstdint>
#include <vector>

#include "BFCItemID.hpp"

namespace AGE
{
	struct BFCCullableObject;
	class BFCBlockManagerFactory;

	class BFCCullableHandle
	{
	public:
		bool invalid() const;
		bool operator==(const BFCCullableHandle &o) const;
		inline const BFCCullableObject *getPtr() const { return _elementPtr; }
		inline BFCCullableObject *getPtr() { return _elementPtr; }
		template<typename T> // be carefull that's static cast, be sure to call the good type
		inline T *getPtr() { return (T*)(_elementPtr); }
		template<typename T> // be carefull that's static cast, be sure to call the good type
		inline const T *getPtr() const { return (T*)(_elementPtr); }
		inline const BFCItemID getItemId() const { return _itemID; }

	private:
		BFCItemID              _itemID;
		BFCCullableObject*     _elementPtr = nullptr;

		friend class BFCBlockManagerFactory;
	};

	class BFCCullableHandleGroup
	{
	public:
		bool invalid() const;
		inline const std::vector<BFCCullableHandle> &getHandles() const { return _handles; }
		inline std::vector<BFCCullableHandle> &getHandles() { return _handles; }
	private:
		std::vector<BFCCullableHandle>     _handles;
		friend class BFCBlockManagerFactory;
	};
}