#ifndef				ARCHIVE_HPP_
# define			ARCHIVE_HPP_

# include			<fstream>

namespace			Archive
{
  inline bool			open(std::ofstream &file, std::string const & path)
  {
    file.open(path.c_str(), std::ios::out | std::ios::binary);
    return (!file.fail());
  }

  inline bool			open(std::ifstream &file, std::string const & path)
  {
    file.open(path.c_str(), std::ios::out | std::ios::binary);
    return (!file.fail());
  }

  /*
  ** Serialize T
  */
  template <typename T>
  inline void			serialize(std::ofstream &file, T const & a)
  {
    file.write(reinterpret_cast<const char *>(&a), sizeof(a));
  }

  /*
  ** Serialize std::string
  */
  inline void			serialize(std::ofstream &file, std::string const & a)
  {
    serialize(file, (int)(a.size()));
    file.write(a.c_str(), a.size());
  }

  /*
  ** Unserialize T
  */
  template <typename T>
  inline void			unserialize(std::ifstream &file, T & a)
  {
    file.read(reinterpret_cast<char *>(&a), sizeof(a));
  }

  /*
  ** Unserialize std::string
  */
  inline void			unserialize(std::ifstream &file, std::string & a)
  {
    int				    strSize;
    unserialize(file, strSize);
	char                *str = new char[strSize + 1]();
    file.read(str, strSize);
	str[strSize] = '\0';
	a.assign(str);
	delete[] str;
  }
}

#endif				/* !ARCHIVE_HPP_ */
