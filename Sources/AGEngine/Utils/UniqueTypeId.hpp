#pragma once

namespace AGE
{
	typedef unsigned int UniqueTypeId;
	static const UniqueTypeId UndefinedTypeId;

	struct TypeID
	{
	private:
		struct InternalBase
		{
			static UniqueTypeId counter;
		};

		template <typename T>
		struct Internal : public InternalBase
		{
			static UniqueTypeId Get()
			{
				static UniqueTypeId t = counter++;
				return t;
			}
		};
	public:
		template <typename T>
		static UniqueTypeId Get()
		{
			return Internal<T>::Get();
		}
	};
}