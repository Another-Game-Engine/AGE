#ifndef   __META_DATA_HPP__
#define   __META_DATA_HPP__

#include <string>
#include <map>
#include <unordered_set>

#include <Utils/StripQualifiers.hpp>

# define PASTE_TOKENS_2( _, __ ) _##__
# define PASTE_TOKENS( _, __ ) PASTE_TOKENS_2( _, __ )
# define NAME_GENERATOR_INTERNAL( _ ) PASTE_TOKENS( GENERATED_TOKEN_, _ )
# define NAME_GENERATOR( ) NAME_GENERATOR_INTERNAL( __COUNTER__ )

// For registering a Type, following by META_DATA to register members
# define META_REG(T) \
	Meta::Creator<StripQual<T>::type >NAME_GENERATOR()(#T, sizeof(T)); \
	void Meta::Creator<StripQual<T>::type>::registerMetaData()

# define META_REG_POD(T) \
    Meta::Creator<StripQual<T>::type> NAME_GENERATOR( )( #T, sizeof( T ) ); \
    void Meta::Creator<StripQual<T>::type>::registerMetaData() \
  { \
  }

// MetaData getters
// -> By type
# define META_TYPE(T) Meta::Creator<StripQual<T>::type>::getData()
// -> By Object
# define META_OBJECT(O)(Meta::Creator<StripQual<decltype(O)>::type>::getData())
// -> By string name
# define META_NAME(S)(Meta::Manager::getData(S))

// To insert inside public class declaration
# define META_DATA(T) \
	static void addMember(const std::string name, std::size_t offset, Meta::Data *data); \
	static StripQual<T>::type *nullCast(){ T::registerMetaData(); } \
	static void registerMetaData();

// To add class members to MetaData
# define META_MEMBER(M) \
	addMember(#M, (std::size_t)(&(nullCast()->M)), META_OBJECT(nullCast()->M))

namespace Meta
{
	class Data;

	class Member
	{
	public:
		Member(const std::string name, std::size_t offset, Data *data) :
			_name(name)
			, _offset(offset)
			, _data(data)
		{}
		inline const std::string getName() { return _name; } const
			inline const std::size_t getOffset() { return _offset; } const
			inline const Data *getData() { return _data; } const
			inline void setName(const std::string name) { _name = name; }
		inline void setOffset(const std::size_t offset) { _offset = offset; }
	private:
		std::string _name;
		std::size_t _offset;
		const Data *_data;
	};

	class Data
	{
	public:
		Data(const std::string name = "", std::size_t size = 0) :
			_name(name)
			, _size(size)
		{}

		~Data()
		{}

		inline void initialize(const std::string name, std::size_t size)
		{
			_name = name;
			_size = size;
		}

		inline void addMember(const Member *member)
		{
			_members.insert(member);
		}

		inline const std::string getName() { return _name; } const
			inline void setName(const std::string name) { _name = name; }
		inline const std::size_t getSize() { return _size; }
		inline void setSize(std::size_t size) { _size = size; }
	private:
		std::string _name;
		std::size_t _size;
		std::unordered_set<const Member*> _members;
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
			registerMetaData();
		}

		static Data *getData()
		{
			static Data instance;
			return &instance;
		}

		static T *nullCast()
		{
			return reinterpret_cast<T*>(nullptr);
		}

		static void addMember(const std::string name, std::size_t offset, Meta::Data *data)
		{
			getData()->addMember(new Member(name, offset, data));
		}

		static void registerMetaData();
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

		static inline const Data *getData(const std::string name)
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