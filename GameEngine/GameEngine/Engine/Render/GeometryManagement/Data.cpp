#include <Render/GeometryManagement/Data.hh>

/**
* Method:    operator Attribute
* FullName:  Data::operator Attribute
* Access:    public 
* Returns:   
* Qualifier: const
* Goal:		 cast the instance in attribute with the its own attribute
*/
Data::operator Attribute() const
{
	return (_type);
}

/**
* Method:    operator size_t
* FullName:  Data::operator size_t
* Access:    public 
* Returns:   
* Qualifier: const
* Goal:		 cast the instance into a size_t with its attribute
*/
Data::operator size_t() const
{
	return (size_t(_type));
}

/**
* Method:    size
* FullName:  Data::size
* Access:    public 
* Returns:   size_t
* Qualifier: const
* Goal:		 number of byte into the instance
*/
size_t Data::size() const
{
	return (_data.size());
}

/**
* Method:    data
* FullName:  Data::data
* Access:    public 
* Returns:   void const *
* Qualifier: const
* Goal:		 the data contain into the instance
*/
void const *Data::data() const
{
	return (_data.data());
}

/**
* Method:    range
* FullName:  Data::range
* Access:    public 
* Returns:   glm::vec2 const &
* Qualifier: const
* Goal:		 give the range where the data is placed into a buffer
*/
glm::vec2 const &Data::range() const
{
	return (_range);
}


/**
* Method:    range
* FullName:  Data::range
* Access:    public 
* Returns:   Data &
* Qualifier:
* Parameter: glm::vec2 const & r
* Goal:		 set the range where the data must be placed into a buffer
*/
Data &Data::range(glm::vec2 const &r)
{
	_range = r;
	return (*this);
}

