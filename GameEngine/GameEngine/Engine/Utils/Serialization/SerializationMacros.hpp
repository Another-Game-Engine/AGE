#pragma once

#define SERIALIZATION_ARCHIVE_FORWARD_DECLARATION() \
namespace cereal \
{ \
class PortableBinaryOutputArchive; \
class JSONOutputArchive; \
class XMLOutputArchive; \
class BinaryOutputArchive; \
class PortableBinaryInputArchive; \
class JSONInputArchive; \
class XMLInputArchive; \
class BinaryInputArchive; \
}

#define SERIALIZATION_SAVE_DECLARATION(Type) \
	namespace cereal { \
	void save(PortableBinaryOutputArchive &ar, const Type &v); \
	void save(JSONOutputArchive &ar, const Type &v); \
	void save(XMLOutputArchive &ar, const Type &v); \
	void save(BinaryOutputArchive &ar, const Type &v); \
	}

#define SERIALIZATION_LOAD_DECLARATION(Type) \
	namespace cereal { \
    void load(PortableBinaryInputArchive &ar, Type &v); \
    void load(JSONInputArchive &ar, Type &v); \
    void load(XMLInputArchive &ar, Type &v); \
    void load(BinaryInputArchive &ar, Type &v); \
    }


#define SERIALIZATION_SAVE_DEFINITION(Type, Code) \
	namespace cereal { \
	void save(PortableBinaryOutputArchive &ar, const Type &v) \
    { \
	/Code/; \
	} \
	void save(JSONOutputArchive &ar, const Type &v) \
    { \
	/Code/; \
	} \
	void save(XMLOutputArchive &ar, const Type &v) \
    { \
	/Code/; \
	} \
	void save(BinaryOutputArchive &ar, const Type &v) \
    { \
	/Code/; \
	} \
    }

#define SERIALIZATION_LOAD_DEFINITION(Type, Code) \
	namespace cereal { \
	void load(PortableBinaryInputArchive &ar, Type &v) \
    { \
	/Code/; \
	} \
	void load(JSONInputArchive &ar, Type &v) \
    { \
	/Code/; \
	} \
	void load(XMLInputArchive &ar, Type &v) \
    { \
	/Code/; \
	} \
	void load(BinaryInputArchive &ar, Type &v) \
    { \
	/Code/; \
	} \
	}

#define SERIALIZATION_SERIALIZE_DECLARATION(Type) \
	namespace cereal { \
    void serialize(PortableBinaryInputArchive &ar, Type &v); \
    void serialize(JSONInputArchive &ar, Type &v); \
    void serialize(XMLInputArchive &ar, Type &v); \
    void serialize(BinaryInputArchive &ar, Type &v); \
	void serialize(PortableBinaryOutputArchive &ar, Type &v); \
	void serialize(JSONOutputArchive &ar, Type &v); \
	void serialize(XMLOutputArchive &ar, Type &v); \
	void serialize(BinaryOutputArchive &ar, Type &v); \
	}

#define SERIALIZATION_SERIALIZE_DEFINITION(Type, Code) \
	namespace cereal { \
    void serialize(PortableBinaryInputArchive &ar, Type &v) \
		        { \
	/Code/; \
		        } \
    void serialize(JSONInputArchive &ar, Type &v) \
			    { \
	/Code/; \
			    } \
    void serialize(XMLInputArchive &ar, Type &v) \
			    { \
	/Code/; \
			    } \
    void serialize(BinaryInputArchive &ar, Type &v) \
			    { \
	/Code/; \
			    } \
	void serialize(PortableBinaryOutputArchive &ar, Type &v) \
			    { \
	/Code/; \
			    } \
	void serialize(JSONOutputArchive &ar, Type &v) \
			    { \
	/Code/; \
			    } \
	void serialize(XMLOutputArchive &ar, Type &v) \
			    { \
	/Code/; \
			    } \
	void serialize(BinaryOutputArchive &ar, Type &v) \
			    { \
	/Code/; \
			    } \
				}

#define SERIALIZATION_SERIALIZE_METHOD_DECLARATION() \
    void serialize(cereal::PortableBinaryInputArchive &ar); \
    void serialize(cereal::JSONInputArchive &ar); \
    void serialize(cereal::XMLInputArchive &ar); \
    void serialize(cereal::BinaryInputArchive &ar); \
	void serialize(cereal::PortableBinaryOutputArchive &ar); \
	void serialize(cereal::JSONOutputArchive &ar); \
	void serialize(cereal::XMLOutputArchive &ar); \
	void serialize(cereal::BinaryOutputArchive &ar);

#define SERIALIZATION_SERIALIZE_METHOD_DEFINITION(Type, Code) \
    void Type::serialize(cereal::PortableBinaryInputArchive &ar) \
		        { \
	/Code/; \
		        } \
    void Type::serialize(cereal::JSONInputArchive &ar) \
			    { \
	/Code/; \
			    } \
    void Type::serialize(cereal::XMLInputArchive &ar) \
			    { \
	/Code/; \
			    } \
    void Type::serialize(cereal::BinaryInputArchive &ar) \
			    { \
	/Code/; \
			    } \
	void Type::serialize(cereal::PortableBinaryOutputArchive &ar) \
			    { \
	/Code/; \
			    } \
	void Type::serialize(cereal::JSONOutputArchive &ar) \
			    { \
	/Code/; \
			    } \
	void Type::serialize(cereal::XMLOutputArchive &ar) \
			    { \
	/Code/; \
			    } \
	void Type::serialize(cereal::BinaryOutputArchive &ar) \
			    { \
	/Code/; \
			    }

#define MACRO_STR(str)(#str)