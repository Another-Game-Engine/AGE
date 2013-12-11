#ifndef   __ANY_HPP__
# define  __ANY_HPP__

#include <memory>
#include <functional>

class Any
{
public:
	/////////////////////////
	// IData struct
	/////////////////////////

	struct IData
	{
		IData(){}
		virtual ~IData(){}
	};

	/////////////////////////
	// Data struct
	/////////////////////////

	template <typename T>
	struct Data : public IData
	{
		T data;

		Data(const T &d)
			: data(d)
		{
		}

		virtual ~Data()
		{}

		T &get()
		{
			return data;
		}

		T& operator=(T &d)
		{
			data = d;
			return d;
		}
	};


	/////////////////////////////
	// Any class implementation
	/////////////////////////////

public:
	std::shared_ptr<IData> _data;
public:
	template<typename T>
	Any(T d)
		: _data(new Data<T>(d))
	{
	}

	~Any(){}

	Any(const Any &o)
	{
		_data = o._data;
	}

	template<typename T>
	T get()
	{
		return (static_cast<Data<T>*>(_data.get()))->get();
	}

	template<typename T>
	operator T() const
	{
		return (static_cast<Data<T>*>(_data.get()))->get();
	}
};

#endif    //!__ANY_HPP__