#ifndef   __TAG_HPP__
# define  __TAG_HPP__

#include <unordered_map>
#include <string>

class Tag
{
public:
	Tag(const std::string &tag = "")
		: str_(tag),
		id_(1)
	{
		if (tag.length())
		{
			if (list_.find(tag) != std::end(list_))
			{
				id_ = list_[tag];
			}
			else
			{
				id_ = list_.size() + 1;
				list_.insert(std::pair<std::string, unsigned int>(tag, id_));
			}
		}
	}

	Tag(const Tag &other)
	{
		str_ = other.str_;
		id_ = other.id_;
	}

	Tag &operator=(const Tag &other)
	{
		str_ = other.str_;
		id_ = other.id_;
		return *this;
	}

	~Tag()
	{}

	const std::string &getString() const
	{
		return str_;
	}

	const unsigned int getId() const
	{
		return id_;
	}

	bool operator==(const Tag &other)
	{
		return (id_ == other.id_);
	}

	bool operator!=(const Tag &other)
	{
		return (id_ != other.id_);
	}

private:
	std::string str_;
	unsigned int id_;
	static std::unordered_map<std::string, unsigned int> list_;
	static std::unordered_map<std::string, unsigned int> init()
	{
		std::unordered_map<std::string, unsigned int> map;
		map[""] = 0;
		return map;
	}
};

#endif    //__TAG_HPP__