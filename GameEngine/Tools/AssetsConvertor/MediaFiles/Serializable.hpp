#ifndef  __SERIALIZABLE_HPP__
# define __SERIALIZABLE_HPP__

#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

struct AAA
{

	int a;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & a;
    }
};

struct BBB : public AAA
{
	int b;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & b;
    }
};


////#include <typeinfo>
////
////class Serializable
////{
////public:
////	virtual ~Serializable(){}
////	unsigned int getSerializableId(){ return _serializableId; }
////	virtual void serialize() = 0;
////	virtual Serializable *unserialize() = 0;
////protected:
////	Serializable(){}
////	unsigned int _serializableId;
////};
////
////template <std::size_t typeId, typename T>
////class SerializableTpt : public Serializable
////{
////	enum 
////	{
////		_ID = typeId;
////	};
////public:
////	static Serializable *create() { return new T(); }
////	static const std::size_t ID;
////	static void enable() { volatile std::size_t s = ID; }
////protected:
////	SerializableTpt(){ _serializableId = ID; }
////};
////
////class SerializableFactory
////{
////public:
////	typedef Serializable * (*_serFactory)();
////
////	static std::size_t registerType(std::size_t id, _serFactory method)
////	{
////		_map()[id] = method;
////		return id;
////	}
////
////	static Serializable *unserialise(std::size_t id)
////	{
////		return _map()[id]();
////	}
////
////	static std::map<std::size_t, _serFactory> &_map()
////	{
////		static std::map<std::size_t, _serFactory> map;
////		return map;
////	}
////
////private:
////	SerializableFactory(){}
////	~SerializableFactory(){}
////	SerializableFactory(const SerializableFactory &){}
////	SerializableFactory& operator=(const SerializableFactory&);
////};
////
////template <int id, typename type>
////const std::size_t SerializableTpt<id, type>::ID = SerializableFactory::registerType(SerializableTpt<id, type>::_ID, &SerializableTpt<id, type>::create);
////
////
////#include <stdio.h>
////#include <stdlib.h>
////#include <string.h>
////#include <inttypes.h>
////#include <map>
////
////class Message
////{
////   protected:
////      Message() {};
////
////   public:
////      virtual ~Message() { }
////      int getMessageType() const { return m_type; }
////      virtual void say() = 0;
////
////   protected:
////      uint16_t m_type;
////};
////
////template<int TYPE, typename IMPL>
////class MessageTmpl: public Message
////{
////   enum { _MESSAGE_ID = TYPE };
////   public:
////     static Message* Create() { return new IMPL(); }
////     static const uint16_t MESSAGE_ID; // for registration
////     static void Enable() { volatile uint16_t x = MESSAGE_ID; }
////   protected:
////      MessageTmpl() { m_type = MESSAGE_ID; } //use parameter to instanciate template
////};
////
////class MessageFactory 
////{
////   public:
////     typedef Message* (*t_pfFactory)();
////
////     static MessageFactory *getInstance()
////     {
////       static MessageFactory fact;
////       return &fact;
////     }
////
////     uint16_t Register(uint16_t msgid, t_pfFactory factoryMethod)
////     {
////       printf("Registering constructor for msg id %d\n", msgid);
////       map[msgid] = factoryMethod;
////       return msgid;
////     }
////
////     Message *Create(uint16_t msgid)
////     {
////       return map[msgid]();
////     }
////
////	 std::map<uint16_t, t_pfFactory> map;
////  private:
////     MessageFactory() {};
////     MessageFactory(MessageFactory const&) {};
////     MessageFactory& operator=(MessageFactory const&);
////     ~MessageFactory() {};
////};
////
////std::map<uint16_t, t_pfFactory> MessageFactory::m_List;
////
////template <int TYPE, typename IMPL>
////const uint16_t MessageTmpl<TYPE, IMPL>::MESSAGE_ID = MessageFactory::getInstance()->Register(
////     MessageTmpl<TYPE, IMPL >::_MESSAGE_ID, &MessageTmpl<TYPE, IMPL >::Create);
////
////
////class PingMessage: public MessageTmpl < 10, PingMessage >
////{ 
////  public:
////  PingMessage() {}
////  virtual void say() { printf("Ping\n"); }
////};
////
////class PongMessage: public MessageTmpl < 11, PongMessage >
////{ 
////  public:
////  PongMessage() {}
////  virtual void say() { printf("Pong\n"); }
////};

#endif   //__SERIALIZABLE_HPP__