#ifndef   __META_DATA_HPP__
#define   __META_DATA_HPP__

#include <string>
#include <map>

namespace Meta
{
	class Data
	{
	public:
		Data(const std::string name, std::size_t size) :
			_name(name)
			, _size(size)
		{}

		~Data()
		{}

		inline const std::string getName() { return _name; } const
		inline void setName(const std::string name) { _name = name; }
		inline const std::size_t getSize() { return _size; }
		inline void setSize(std::size_t size) { _size = size; }
	private:
		std::string _name;
		std::size_t _size;
	};


	template <typename T>
	class Creator
	{
	public:
		Creator(const std::string name, std::size_t size)
		{
			initialize(name, size);
		}

		static void initialize(const std::string name, std::size_t size)
		{
			getData()->initialize(name, size);
		}

		static Data *getData()
		{
			static Data instance;
			return &instance;
		}
	private:
	};

	class Manager
	{
	public:
		static inline void add(Data *data)
		{
			auto name = data->getName();
			auto &it = getMap().find(name);
			if (it == std::end(getMap()))
				getMap().insert(std::make_pair(name, data));
		}

		static inline const Data *get(const std::string name)
		{
			auto &it = getMap().find(name);
			if (it == std::end(getMap()))
				return nullptr;
			return it->second;
		}
	private:
		static inline std::map<std::string, const Data*> &getMap()
		{
			static std::map<std::string, const Data*> map;
			return map;
		}
	};

}

#endif    //__META_DATA_HPP__