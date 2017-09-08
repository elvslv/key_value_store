// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Message.proto

#ifndef PROTOBUF_Message_2eproto__INCLUDED
#define PROTOBUF_Message_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "Address.pb.h"
// @@protoc_insertion_point(includes)
namespace gen {
class Address;
class AddressDefaultTypeInternal;
extern AddressDefaultTypeInternal _Address_default_instance_;
class Event;
class EventDefaultTypeInternal;
extern EventDefaultTypeInternal _Event_default_instance_;
class Message;
class MessageDefaultTypeInternal;
extern MessageDefaultTypeInternal _Message_default_instance_;
class Node;
class NodeDefaultTypeInternal;
extern NodeDefaultTypeInternal _Node_default_instance_;
}  // namespace gen

namespace gen {

namespace protobuf_Message_2eproto {
// Internal implementation detail -- do not call these.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[];
  static const ::google::protobuf::uint32 offsets[];
  static void InitDefaultsImpl();
  static void Shutdown();
};
void AddDescriptors();
void InitDefaults();
}  // namespace protobuf_Message_2eproto

enum MessageType {
  PING = 0,
  ACK = 1,
  PING_REQ = 2,
  MessageType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  MessageType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool MessageType_IsValid(int value);
const MessageType MessageType_MIN = PING;
const MessageType MessageType_MAX = PING_REQ;
const int MessageType_ARRAYSIZE = MessageType_MAX + 1;

const ::google::protobuf::EnumDescriptor* MessageType_descriptor();
inline const ::std::string& MessageType_Name(MessageType value) {
  return ::google::protobuf::internal::NameOfEnum(
    MessageType_descriptor(), value);
}
inline bool MessageType_Parse(
    const ::std::string& name, MessageType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<MessageType>(
    MessageType_descriptor(), name, value);
}
enum EventTypes {
  JOINED = 0,
  FAILED = 1,
  EventTypes_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  EventTypes_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool EventTypes_IsValid(int value);
const EventTypes EventTypes_MIN = JOINED;
const EventTypes EventTypes_MAX = FAILED;
const int EventTypes_ARRAYSIZE = EventTypes_MAX + 1;

const ::google::protobuf::EnumDescriptor* EventTypes_descriptor();
inline const ::std::string& EventTypes_Name(EventTypes value) {
  return ::google::protobuf::internal::NameOfEnum(
    EventTypes_descriptor(), value);
}
inline bool EventTypes_Parse(
    const ::std::string& name, EventTypes* value) {
  return ::google::protobuf::internal::ParseNamedEnum<EventTypes>(
    EventTypes_descriptor(), name, value);
}
// ===================================================================

class Node : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:gen.Node) */ {
 public:
  Node();
  virtual ~Node();

  Node(const Node& from);

  inline Node& operator=(const Node& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Node& default_instance();

  static inline const Node* internal_default_instance() {
    return reinterpret_cast<const Node*>(
               &_Node_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(Node* other);

  // implements Message ----------------------------------------------

  inline Node* New() const PROTOBUF_FINAL { return New(NULL); }

  Node* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Node& from);
  void MergeFrom(const Node& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Node* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // .gen.Address address = 1;
  bool has_address() const;
  void clear_address();
  static const int kAddressFieldNumber = 1;
  const ::gen::Address& address() const;
  ::gen::Address* mutable_address();
  ::gen::Address* release_address();
  void set_allocated_address(::gen::Address* address);

  // @@protoc_insertion_point(class_scope:gen.Node)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::gen::Address* address_;
  mutable int _cached_size_;
  friend struct protobuf_Message_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Event : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:gen.Event) */ {
 public:
  Event();
  virtual ~Event();

  Event(const Event& from);

  inline Event& operator=(const Event& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Event& default_instance();

  static inline const Event* internal_default_instance() {
    return reinterpret_cast<const Event*>(
               &_Event_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(Event* other);

  // implements Message ----------------------------------------------

  inline Event* New() const PROTOBUF_FINAL { return New(NULL); }

  Event* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Event& from);
  void MergeFrom(const Event& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Event* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // .gen.Address address = 2;
  bool has_address() const;
  void clear_address();
  static const int kAddressFieldNumber = 2;
  const ::gen::Address& address() const;
  ::gen::Address* mutable_address();
  ::gen::Address* release_address();
  void set_allocated_address(::gen::Address* address);

  // .gen.EventTypes event = 1;
  void clear_event();
  static const int kEventFieldNumber = 1;
  ::gen::EventTypes event() const;
  void set_event(::gen::EventTypes value);

  // @@protoc_insertion_point(class_scope:gen.Event)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::gen::Address* address_;
  int event_;
  mutable int _cached_size_;
  friend struct protobuf_Message_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Message : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:gen.Message) */ {
 public:
  Message();
  virtual ~Message();

  Message(const Message& from);

  inline Message& operator=(const Message& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Message& default_instance();

  static inline const Message* internal_default_instance() {
    return reinterpret_cast<const Message*>(
               &_Message_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    2;

  void Swap(Message* other);

  // implements Message ----------------------------------------------

  inline Message* New() const PROTOBUF_FINAL { return New(NULL); }

  Message* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Message& from);
  void MergeFrom(const Message& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const PROTOBUF_FINAL;
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const PROTOBUF_FINAL;
  void InternalSwap(Message* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // .gen.Address sourceAddress = 2;
  bool has_sourceaddress() const;
  void clear_sourceaddress();
  static const int kSourceAddressFieldNumber = 2;
  const ::gen::Address& sourceaddress() const;
  ::gen::Address* mutable_sourceaddress();
  ::gen::Address* release_sourceaddress();
  void set_allocated_sourceaddress(::gen::Address* sourceaddress);

  // .gen.Address destinationAddress = 3;
  bool has_destinationaddress() const;
  void clear_destinationaddress();
  static const int kDestinationAddressFieldNumber = 3;
  const ::gen::Address& destinationaddress() const;
  ::gen::Address* mutable_destinationaddress();
  ::gen::Address* release_destinationaddress();
  void set_allocated_destinationaddress(::gen::Address* destinationaddress);

  // .gen.Event event = 4;
  bool has_event() const;
  void clear_event();
  static const int kEventFieldNumber = 4;
  const ::gen::Event& event() const;
  ::gen::Event* mutable_event();
  ::gen::Event* release_event();
  void set_allocated_event(::gen::Event* event);

  // .gen.Address targetAddress = 5;
  bool has_targetaddress() const;
  void clear_targetaddress();
  static const int kTargetAddressFieldNumber = 5;
  const ::gen::Address& targetaddress() const;
  ::gen::Address* mutable_targetaddress();
  ::gen::Address* release_targetaddress();
  void set_allocated_targetaddress(::gen::Address* targetaddress);

  // .gen.MessageType messageType = 1;
  void clear_messagetype();
  static const int kMessageTypeFieldNumber = 1;
  ::gen::MessageType messagetype() const;
  void set_messagetype(::gen::MessageType value);

  // @@protoc_insertion_point(class_scope:gen.Message)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::gen::Address* sourceaddress_;
  ::gen::Address* destinationaddress_;
  ::gen::Event* event_;
  ::gen::Address* targetaddress_;
  int messagetype_;
  mutable int _cached_size_;
  friend struct protobuf_Message_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Node

// .gen.Address address = 1;
inline bool Node::has_address() const {
  return this != internal_default_instance() && address_ != NULL;
}
inline void Node::clear_address() {
  if (GetArenaNoVirtual() == NULL && address_ != NULL) delete address_;
  address_ = NULL;
}
inline const ::gen::Address& Node::address() const {
  // @@protoc_insertion_point(field_get:gen.Node.address)
  return address_ != NULL ? *address_
                         : *::gen::Address::internal_default_instance();
}
inline ::gen::Address* Node::mutable_address() {
  
  if (address_ == NULL) {
    address_ = new ::gen::Address;
  }
  // @@protoc_insertion_point(field_mutable:gen.Node.address)
  return address_;
}
inline ::gen::Address* Node::release_address() {
  // @@protoc_insertion_point(field_release:gen.Node.address)
  
  ::gen::Address* temp = address_;
  address_ = NULL;
  return temp;
}
inline void Node::set_allocated_address(::gen::Address* address) {
  delete address_;
  address_ = address;
  if (address) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Node.address)
}

// -------------------------------------------------------------------

// Event

// .gen.EventTypes event = 1;
inline void Event::clear_event() {
  event_ = 0;
}
inline ::gen::EventTypes Event::event() const {
  // @@protoc_insertion_point(field_get:gen.Event.event)
  return static_cast< ::gen::EventTypes >(event_);
}
inline void Event::set_event(::gen::EventTypes value) {
  
  event_ = value;
  // @@protoc_insertion_point(field_set:gen.Event.event)
}

// .gen.Address address = 2;
inline bool Event::has_address() const {
  return this != internal_default_instance() && address_ != NULL;
}
inline void Event::clear_address() {
  if (GetArenaNoVirtual() == NULL && address_ != NULL) delete address_;
  address_ = NULL;
}
inline const ::gen::Address& Event::address() const {
  // @@protoc_insertion_point(field_get:gen.Event.address)
  return address_ != NULL ? *address_
                         : *::gen::Address::internal_default_instance();
}
inline ::gen::Address* Event::mutable_address() {
  
  if (address_ == NULL) {
    address_ = new ::gen::Address;
  }
  // @@protoc_insertion_point(field_mutable:gen.Event.address)
  return address_;
}
inline ::gen::Address* Event::release_address() {
  // @@protoc_insertion_point(field_release:gen.Event.address)
  
  ::gen::Address* temp = address_;
  address_ = NULL;
  return temp;
}
inline void Event::set_allocated_address(::gen::Address* address) {
  delete address_;
  address_ = address;
  if (address) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Event.address)
}

// -------------------------------------------------------------------

// Message

// .gen.MessageType messageType = 1;
inline void Message::clear_messagetype() {
  messagetype_ = 0;
}
inline ::gen::MessageType Message::messagetype() const {
  // @@protoc_insertion_point(field_get:gen.Message.messageType)
  return static_cast< ::gen::MessageType >(messagetype_);
}
inline void Message::set_messagetype(::gen::MessageType value) {
  
  messagetype_ = value;
  // @@protoc_insertion_point(field_set:gen.Message.messageType)
}

// .gen.Address sourceAddress = 2;
inline bool Message::has_sourceaddress() const {
  return this != internal_default_instance() && sourceaddress_ != NULL;
}
inline void Message::clear_sourceaddress() {
  if (GetArenaNoVirtual() == NULL && sourceaddress_ != NULL) delete sourceaddress_;
  sourceaddress_ = NULL;
}
inline const ::gen::Address& Message::sourceaddress() const {
  // @@protoc_insertion_point(field_get:gen.Message.sourceAddress)
  return sourceaddress_ != NULL ? *sourceaddress_
                         : *::gen::Address::internal_default_instance();
}
inline ::gen::Address* Message::mutable_sourceaddress() {
  
  if (sourceaddress_ == NULL) {
    sourceaddress_ = new ::gen::Address;
  }
  // @@protoc_insertion_point(field_mutable:gen.Message.sourceAddress)
  return sourceaddress_;
}
inline ::gen::Address* Message::release_sourceaddress() {
  // @@protoc_insertion_point(field_release:gen.Message.sourceAddress)
  
  ::gen::Address* temp = sourceaddress_;
  sourceaddress_ = NULL;
  return temp;
}
inline void Message::set_allocated_sourceaddress(::gen::Address* sourceaddress) {
  delete sourceaddress_;
  sourceaddress_ = sourceaddress;
  if (sourceaddress) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Message.sourceAddress)
}

// .gen.Address destinationAddress = 3;
inline bool Message::has_destinationaddress() const {
  return this != internal_default_instance() && destinationaddress_ != NULL;
}
inline void Message::clear_destinationaddress() {
  if (GetArenaNoVirtual() == NULL && destinationaddress_ != NULL) delete destinationaddress_;
  destinationaddress_ = NULL;
}
inline const ::gen::Address& Message::destinationaddress() const {
  // @@protoc_insertion_point(field_get:gen.Message.destinationAddress)
  return destinationaddress_ != NULL ? *destinationaddress_
                         : *::gen::Address::internal_default_instance();
}
inline ::gen::Address* Message::mutable_destinationaddress() {
  
  if (destinationaddress_ == NULL) {
    destinationaddress_ = new ::gen::Address;
  }
  // @@protoc_insertion_point(field_mutable:gen.Message.destinationAddress)
  return destinationaddress_;
}
inline ::gen::Address* Message::release_destinationaddress() {
  // @@protoc_insertion_point(field_release:gen.Message.destinationAddress)
  
  ::gen::Address* temp = destinationaddress_;
  destinationaddress_ = NULL;
  return temp;
}
inline void Message::set_allocated_destinationaddress(::gen::Address* destinationaddress) {
  delete destinationaddress_;
  destinationaddress_ = destinationaddress;
  if (destinationaddress) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Message.destinationAddress)
}

// .gen.Event event = 4;
inline bool Message::has_event() const {
  return this != internal_default_instance() && event_ != NULL;
}
inline void Message::clear_event() {
  if (GetArenaNoVirtual() == NULL && event_ != NULL) delete event_;
  event_ = NULL;
}
inline const ::gen::Event& Message::event() const {
  // @@protoc_insertion_point(field_get:gen.Message.event)
  return event_ != NULL ? *event_
                         : *::gen::Event::internal_default_instance();
}
inline ::gen::Event* Message::mutable_event() {
  
  if (event_ == NULL) {
    event_ = new ::gen::Event;
  }
  // @@protoc_insertion_point(field_mutable:gen.Message.event)
  return event_;
}
inline ::gen::Event* Message::release_event() {
  // @@protoc_insertion_point(field_release:gen.Message.event)
  
  ::gen::Event* temp = event_;
  event_ = NULL;
  return temp;
}
inline void Message::set_allocated_event(::gen::Event* event) {
  delete event_;
  event_ = event;
  if (event) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Message.event)
}

// .gen.Address targetAddress = 5;
inline bool Message::has_targetaddress() const {
  return this != internal_default_instance() && targetaddress_ != NULL;
}
inline void Message::clear_targetaddress() {
  if (GetArenaNoVirtual() == NULL && targetaddress_ != NULL) delete targetaddress_;
  targetaddress_ = NULL;
}
inline const ::gen::Address& Message::targetaddress() const {
  // @@protoc_insertion_point(field_get:gen.Message.targetAddress)
  return targetaddress_ != NULL ? *targetaddress_
                         : *::gen::Address::internal_default_instance();
}
inline ::gen::Address* Message::mutable_targetaddress() {
  
  if (targetaddress_ == NULL) {
    targetaddress_ = new ::gen::Address;
  }
  // @@protoc_insertion_point(field_mutable:gen.Message.targetAddress)
  return targetaddress_;
}
inline ::gen::Address* Message::release_targetaddress() {
  // @@protoc_insertion_point(field_release:gen.Message.targetAddress)
  
  ::gen::Address* temp = targetaddress_;
  targetaddress_ = NULL;
  return temp;
}
inline void Message::set_allocated_targetaddress(::gen::Address* targetaddress) {
  delete targetaddress_;
  targetaddress_ = targetaddress;
  if (targetaddress) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Message.targetAddress)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


}  // namespace gen

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::gen::MessageType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::gen::MessageType>() {
  return ::gen::MessageType_descriptor();
}
template <> struct is_proto_enum< ::gen::EventTypes> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::gen::EventTypes>() {
  return ::gen::EventTypes_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Message_2eproto__INCLUDED
