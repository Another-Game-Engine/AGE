#ifndef   __AMEDIA_FILE_HPP__
# define  __AMEDIA_FILE_HPP__

#include <functional>
#include <Utils/File.hpp>

#include <Utils/DependenciesInjector.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>


class AssetsManager;

struct AMediaFile
{
public:
	File path;
	std::string name;
	enum MEDIA_TYPE
	{
		UNKNOWN = 0
		, OBJ = 1
		, TEXTURE = 2
		, MATERIAL = 3
		, CUBEMAP = 4
		, COLLISION_SHAPE_STATIC = 5
		, COLLISION_SHAPE_DYNAMIC = 6
		, COLLISION_BOX = 7
		, COLLISION_SPHERE = 8
		, COLLISION_MULTI_SPHERE = 9
	};
protected:
	std::size_t _childs;
	MEDIA_TYPE _type;
	std::weak_ptr<DependenciesInjector> _dependencyManager;
public:
	AMediaFile();
	virtual ~AMediaFile();
	AMediaFile(const AMediaFile &o);
	AMediaFile &operator=(const AMediaFile &o);
	void serialize(std::ofstream &s);

	void serializeAsBulletFile(std::ofstream &s);

	void saveToFile();

	inline std::size_t getChilds() const
	{
		return _childs;
	}

	inline void incrementChilds()
	{
		++_childs;
	}

	inline MEDIA_TYPE getType() const
	{
		return _type;
	}

protected:
	friend class AssetsManager;
	virtual void _serialize(cereal::PortableBinaryOutputArchive &ar) = 0;
	virtual void _serialize(cereal::PortableBinaryInputArchive &ar) = 0;
};

#endif    //__AMEDIA_FILE_HPP__
