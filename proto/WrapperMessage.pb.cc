// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: WrapperMessage.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "WrapperMessage.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace gen {
class MessageDefaultTypeInternal : public ::google::protobuf::internal::ExplicitlyConstructed<Message> {
  public:
  const ::gen::key_value_store::Message* keyvaluestoremessage_;
  const ::gen::membership_protocol::Message* membershipprotocolmessage_;
} _Message_default_instance_;

namespace protobuf_WrapperMessage_2eproto {


namespace {

::google::protobuf::Metadata file_level_metadata[1];

}  // namespace

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTableField
    const TableStruct::entries[] = {
  {0, 0, 0, ::google::protobuf::internal::kInvalidMask, 0, 0},
};

PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::AuxillaryParseTableField
    const TableStruct::aux[] = {
  ::google::protobuf::internal::AuxillaryParseTableField(),
};
PROTOBUF_CONSTEXPR_VAR ::google::protobuf::internal::ParseTable const
    TableStruct::schema[] = {
  { NULL, NULL, 0, -1, -1, false },
};

const ::google::protobuf::uint32 TableStruct::offsets[] = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, _internal_metadata_),
  ~0u,  // no _extensions_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, _oneof_case_[0]),
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, sourceaddress_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, destinationaddress_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, id_),
  GOOGLE_PROTOBUF_GENERATED_DEFAULT_ONEOF_FIELD_OFFSET((&_Message_default_instance_), keyvaluestoremessage_),
  GOOGLE_PROTOBUF_GENERATED_DEFAULT_ONEOF_FIELD_OFFSET((&_Message_default_instance_), membershipprotocolmessage_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(Message, messageInternal_),
};

static const ::google::protobuf::internal::MigrationSchema schemas[] = {
  { 0, -1, sizeof(Message)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&_Message_default_instance_),
};

namespace {

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "WrapperMessage.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

}  // namespace

void TableStruct::Shutdown() {
  _Message_default_instance_.Shutdown();
  delete file_level_metadata[0].reflection;
}

void TableStruct::InitDefaultsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::InitProtobufDefaults();
  ::gen::protobuf_Address_2eproto::InitDefaults();
  ::gen::key_value_store::protobuf_KeyValueStoreMessage_2eproto::InitDefaults();
  ::gen::membership_protocol::protobuf_MembershipProtocolMessage_2eproto::InitDefaults();
  _Message_default_instance_.DefaultConstruct();
  _Message_default_instance_.get_mutable()->sourceaddress_ = const_cast< ::gen::Address*>(
      ::gen::Address::internal_default_instance());
  _Message_default_instance_.get_mutable()->destinationaddress_ = const_cast< ::gen::Address*>(
      ::gen::Address::internal_default_instance());
  _Message_default_instance_.keyvaluestoremessage_ = const_cast< ::gen::key_value_store::Message*>(
      ::gen::key_value_store::Message::internal_default_instance());
  _Message_default_instance_.membershipprotocolmessage_ = const_cast< ::gen::membership_protocol::Message*>(
      ::gen::membership_protocol::Message::internal_default_instance());
}

void InitDefaults() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &TableStruct::InitDefaultsImpl);
}
void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] = {
      "\n\024WrapperMessage.proto\022\003gen\032\rAddress.pro"
      "to\032\032KeyValueStoreMessage.proto\032\037Membersh"
      "ipProtocolMessage.proto\"\374\001\n\007Message\022#\n\rs"
      "ourceAddress\030\001 \001(\0132\014.gen.Address\022(\n\022dest"
      "inationAddress\030\002 \001(\0132\014.gen.Address\022\n\n\002id"
      "\030\003 \001(\t\022<\n\024keyValueStoreMessage\030\004 \001(\0132\034.g"
      "en.key_value_store.MessageH\000\022E\n\031membersh"
      "ipProtocolMessage\030\005 \001(\0132 .gen.membership"
      "_protocol.MessageH\000B\021\n\017messageInternalb\006"
      "proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 366);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "WrapperMessage.proto", &protobuf_RegisterTypes);
  ::gen::protobuf_Address_2eproto::AddDescriptors();
  ::gen::key_value_store::protobuf_KeyValueStoreMessage_2eproto::AddDescriptors();
  ::gen::membership_protocol::protobuf_MembershipProtocolMessage_2eproto::AddDescriptors();
  ::google::protobuf::internal::OnShutdown(&TableStruct::Shutdown);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;

}  // namespace protobuf_WrapperMessage_2eproto


// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Message::kSourceAddressFieldNumber;
const int Message::kDestinationAddressFieldNumber;
const int Message::kIdFieldNumber;
const int Message::kKeyValueStoreMessageFieldNumber;
const int Message::kMembershipProtocolMessageFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Message::Message()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    protobuf_WrapperMessage_2eproto::InitDefaults();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:gen.Message)
}
Message::Message(const Message& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  id_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.id().size() > 0) {
    id_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.id_);
  }
  if (from.has_sourceaddress()) {
    sourceaddress_ = new ::gen::Address(*from.sourceaddress_);
  } else {
    sourceaddress_ = NULL;
  }
  if (from.has_destinationaddress()) {
    destinationaddress_ = new ::gen::Address(*from.destinationaddress_);
  } else {
    destinationaddress_ = NULL;
  }
  clear_has_messageInternal();
  switch (from.messageInternal_case()) {
    case kKeyValueStoreMessage: {
      mutable_keyvaluestoremessage()->::gen::key_value_store::Message::MergeFrom(from.keyvaluestoremessage());
      break;
    }
    case kMembershipProtocolMessage: {
      mutable_membershipprotocolmessage()->::gen::membership_protocol::Message::MergeFrom(from.membershipprotocolmessage());
      break;
    }
    case MESSAGEINTERNAL_NOT_SET: {
      break;
    }
  }
  // @@protoc_insertion_point(copy_constructor:gen.Message)
}

void Message::SharedCtor() {
  id_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&sourceaddress_, 0, reinterpret_cast<char*>(&destinationaddress_) -
    reinterpret_cast<char*>(&sourceaddress_) + sizeof(destinationaddress_));
  clear_has_messageInternal();
  _cached_size_ = 0;
}

Message::~Message() {
  // @@protoc_insertion_point(destructor:gen.Message)
  SharedDtor();
}

void Message::SharedDtor() {
  id_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (this != internal_default_instance()) {
    delete sourceaddress_;
  }
  if (this != internal_default_instance()) {
    delete destinationaddress_;
  }
  if (has_messageInternal()) {
    clear_messageInternal();
  }
}

void Message::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Message::descriptor() {
  protobuf_WrapperMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_WrapperMessage_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Message& Message::default_instance() {
  protobuf_WrapperMessage_2eproto::InitDefaults();
  return *internal_default_instance();
}

Message* Message::New(::google::protobuf::Arena* arena) const {
  Message* n = new Message;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Message::clear_messageInternal() {
// @@protoc_insertion_point(one_of_clear_start:gen.Message)
  switch (messageInternal_case()) {
    case kKeyValueStoreMessage: {
      delete messageInternal_.keyvaluestoremessage_;
      break;
    }
    case kMembershipProtocolMessage: {
      delete messageInternal_.membershipprotocolmessage_;
      break;
    }
    case MESSAGEINTERNAL_NOT_SET: {
      break;
    }
  }
  _oneof_case_[0] = MESSAGEINTERNAL_NOT_SET;
}


void Message::Clear() {
// @@protoc_insertion_point(message_clear_start:gen.Message)
  id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (GetArenaNoVirtual() == NULL && sourceaddress_ != NULL) {
    delete sourceaddress_;
  }
  sourceaddress_ = NULL;
  if (GetArenaNoVirtual() == NULL && destinationaddress_ != NULL) {
    delete destinationaddress_;
  }
  destinationaddress_ = NULL;
  clear_messageInternal();
}

bool Message::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:gen.Message)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // .gen.Address sourceAddress = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_sourceaddress()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .gen.Address destinationAddress = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_destinationaddress()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // string id = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(26u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_id()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->id().data(), this->id().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "gen.Message.id"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .gen.key_value_store.Message keyValueStoreMessage = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_keyvaluestoremessage()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // .gen.membership_protocol.Message membershipProtocolMessage = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(42u)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_membershipprotocolmessage()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:gen.Message)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:gen.Message)
  return false;
#undef DO_
}

void Message::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:gen.Message)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .gen.Address sourceAddress = 1;
  if (this->has_sourceaddress()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, *this->sourceaddress_, output);
  }

  // .gen.Address destinationAddress = 2;
  if (this->has_destinationaddress()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2, *this->destinationaddress_, output);
  }

  // string id = 3;
  if (this->id().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->id().data(), this->id().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "gen.Message.id");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      3, this->id(), output);
  }

  // .gen.key_value_store.Message keyValueStoreMessage = 4;
  if (has_keyvaluestoremessage()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      4, *messageInternal_.keyvaluestoremessage_, output);
  }

  // .gen.membership_protocol.Message membershipProtocolMessage = 5;
  if (has_membershipprotocolmessage()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      5, *messageInternal_.membershipprotocolmessage_, output);
  }

  // @@protoc_insertion_point(serialize_end:gen.Message)
}

::google::protobuf::uint8* Message::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:gen.Message)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .gen.Address sourceAddress = 1;
  if (this->has_sourceaddress()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        1, *this->sourceaddress_, deterministic, target);
  }

  // .gen.Address destinationAddress = 2;
  if (this->has_destinationaddress()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        2, *this->destinationaddress_, deterministic, target);
  }

  // string id = 3;
  if (this->id().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->id().data(), this->id().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "gen.Message.id");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        3, this->id(), target);
  }

  // .gen.key_value_store.Message keyValueStoreMessage = 4;
  if (has_keyvaluestoremessage()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        4, *messageInternal_.keyvaluestoremessage_, deterministic, target);
  }

  // .gen.membership_protocol.Message membershipProtocolMessage = 5;
  if (has_membershipprotocolmessage()) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageNoVirtualToArray(
        5, *messageInternal_.membershipprotocolmessage_, deterministic, target);
  }

  // @@protoc_insertion_point(serialize_to_array_end:gen.Message)
  return target;
}

size_t Message::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:gen.Message)
  size_t total_size = 0;

  // string id = 3;
  if (this->id().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->id());
  }

  // .gen.Address sourceAddress = 1;
  if (this->has_sourceaddress()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        *this->sourceaddress_);
  }

  // .gen.Address destinationAddress = 2;
  if (this->has_destinationaddress()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        *this->destinationaddress_);
  }

  switch (messageInternal_case()) {
    // .gen.key_value_store.Message keyValueStoreMessage = 4;
    case kKeyValueStoreMessage: {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *messageInternal_.keyvaluestoremessage_);
      break;
    }
    // .gen.membership_protocol.Message membershipProtocolMessage = 5;
    case kMembershipProtocolMessage: {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          *messageInternal_.membershipprotocolmessage_);
      break;
    }
    case MESSAGEINTERNAL_NOT_SET: {
      break;
    }
  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Message::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:gen.Message)
  GOOGLE_DCHECK_NE(&from, this);
  const Message* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Message>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:gen.Message)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:gen.Message)
    MergeFrom(*source);
  }
}

void Message::MergeFrom(const Message& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:gen.Message)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.id().size() > 0) {

    id_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.id_);
  }
  if (from.has_sourceaddress()) {
    mutable_sourceaddress()->::gen::Address::MergeFrom(from.sourceaddress());
  }
  if (from.has_destinationaddress()) {
    mutable_destinationaddress()->::gen::Address::MergeFrom(from.destinationaddress());
  }
  switch (from.messageInternal_case()) {
    case kKeyValueStoreMessage: {
      mutable_keyvaluestoremessage()->::gen::key_value_store::Message::MergeFrom(from.keyvaluestoremessage());
      break;
    }
    case kMembershipProtocolMessage: {
      mutable_membershipprotocolmessage()->::gen::membership_protocol::Message::MergeFrom(from.membershipprotocolmessage());
      break;
    }
    case MESSAGEINTERNAL_NOT_SET: {
      break;
    }
  }
}

void Message::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:gen.Message)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Message::CopyFrom(const Message& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:gen.Message)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Message::IsInitialized() const {
  return true;
}

void Message::Swap(Message* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Message::InternalSwap(Message* other) {
  id_.Swap(&other->id_);
  std::swap(sourceaddress_, other->sourceaddress_);
  std::swap(destinationaddress_, other->destinationaddress_);
  std::swap(messageInternal_, other->messageInternal_);
  std::swap(_oneof_case_[0], other->_oneof_case_[0]);
  std::swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Message::GetMetadata() const {
  protobuf_WrapperMessage_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_WrapperMessage_2eproto::file_level_metadata[kIndexInFileMessages];
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// Message

// .gen.Address sourceAddress = 1;
bool Message::has_sourceaddress() const {
  return this != internal_default_instance() && sourceaddress_ != NULL;
}
void Message::clear_sourceaddress() {
  if (GetArenaNoVirtual() == NULL && sourceaddress_ != NULL) delete sourceaddress_;
  sourceaddress_ = NULL;
}
const ::gen::Address& Message::sourceaddress() const {
  // @@protoc_insertion_point(field_get:gen.Message.sourceAddress)
  return sourceaddress_ != NULL ? *sourceaddress_
                         : *::gen::Address::internal_default_instance();
}
::gen::Address* Message::mutable_sourceaddress() {
  
  if (sourceaddress_ == NULL) {
    sourceaddress_ = new ::gen::Address;
  }
  // @@protoc_insertion_point(field_mutable:gen.Message.sourceAddress)
  return sourceaddress_;
}
::gen::Address* Message::release_sourceaddress() {
  // @@protoc_insertion_point(field_release:gen.Message.sourceAddress)
  
  ::gen::Address* temp = sourceaddress_;
  sourceaddress_ = NULL;
  return temp;
}
void Message::set_allocated_sourceaddress(::gen::Address* sourceaddress) {
  delete sourceaddress_;
  sourceaddress_ = sourceaddress;
  if (sourceaddress) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Message.sourceAddress)
}

// .gen.Address destinationAddress = 2;
bool Message::has_destinationaddress() const {
  return this != internal_default_instance() && destinationaddress_ != NULL;
}
void Message::clear_destinationaddress() {
  if (GetArenaNoVirtual() == NULL && destinationaddress_ != NULL) delete destinationaddress_;
  destinationaddress_ = NULL;
}
const ::gen::Address& Message::destinationaddress() const {
  // @@protoc_insertion_point(field_get:gen.Message.destinationAddress)
  return destinationaddress_ != NULL ? *destinationaddress_
                         : *::gen::Address::internal_default_instance();
}
::gen::Address* Message::mutable_destinationaddress() {
  
  if (destinationaddress_ == NULL) {
    destinationaddress_ = new ::gen::Address;
  }
  // @@protoc_insertion_point(field_mutable:gen.Message.destinationAddress)
  return destinationaddress_;
}
::gen::Address* Message::release_destinationaddress() {
  // @@protoc_insertion_point(field_release:gen.Message.destinationAddress)
  
  ::gen::Address* temp = destinationaddress_;
  destinationaddress_ = NULL;
  return temp;
}
void Message::set_allocated_destinationaddress(::gen::Address* destinationaddress) {
  delete destinationaddress_;
  destinationaddress_ = destinationaddress;
  if (destinationaddress) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Message.destinationAddress)
}

// string id = 3;
void Message::clear_id() {
  id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
const ::std::string& Message::id() const {
  // @@protoc_insertion_point(field_get:gen.Message.id)
  return id_.GetNoArena();
}
void Message::set_id(const ::std::string& value) {
  
  id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:gen.Message.id)
}
#if LANG_CXX11
void Message::set_id(::std::string&& value) {
  
  id_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:gen.Message.id)
}
#endif
void Message::set_id(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:gen.Message.id)
}
void Message::set_id(const char* value, size_t size) {
  
  id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:gen.Message.id)
}
::std::string* Message::mutable_id() {
  
  // @@protoc_insertion_point(field_mutable:gen.Message.id)
  return id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
::std::string* Message::release_id() {
  // @@protoc_insertion_point(field_release:gen.Message.id)
  
  return id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
void Message::set_allocated_id(::std::string* id) {
  if (id != NULL) {
    
  } else {
    
  }
  id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), id);
  // @@protoc_insertion_point(field_set_allocated:gen.Message.id)
}

// .gen.key_value_store.Message keyValueStoreMessage = 4;
bool Message::has_keyvaluestoremessage() const {
  return messageInternal_case() == kKeyValueStoreMessage;
}
void Message::set_has_keyvaluestoremessage() {
  _oneof_case_[0] = kKeyValueStoreMessage;
}
void Message::clear_keyvaluestoremessage() {
  if (has_keyvaluestoremessage()) {
    delete messageInternal_.keyvaluestoremessage_;
    clear_has_messageInternal();
  }
}
 const ::gen::key_value_store::Message& Message::keyvaluestoremessage() const {
  // @@protoc_insertion_point(field_get:gen.Message.keyValueStoreMessage)
  return has_keyvaluestoremessage()
      ? *messageInternal_.keyvaluestoremessage_
      : ::gen::key_value_store::Message::default_instance();
}
::gen::key_value_store::Message* Message::mutable_keyvaluestoremessage() {
  if (!has_keyvaluestoremessage()) {
    clear_messageInternal();
    set_has_keyvaluestoremessage();
    messageInternal_.keyvaluestoremessage_ = new ::gen::key_value_store::Message;
  }
  // @@protoc_insertion_point(field_mutable:gen.Message.keyValueStoreMessage)
  return messageInternal_.keyvaluestoremessage_;
}
::gen::key_value_store::Message* Message::release_keyvaluestoremessage() {
  // @@protoc_insertion_point(field_release:gen.Message.keyValueStoreMessage)
  if (has_keyvaluestoremessage()) {
    clear_has_messageInternal();
    ::gen::key_value_store::Message* temp = messageInternal_.keyvaluestoremessage_;
    messageInternal_.keyvaluestoremessage_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
void Message::set_allocated_keyvaluestoremessage(::gen::key_value_store::Message* keyvaluestoremessage) {
  clear_messageInternal();
  if (keyvaluestoremessage) {
    set_has_keyvaluestoremessage();
    messageInternal_.keyvaluestoremessage_ = keyvaluestoremessage;
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Message.keyValueStoreMessage)
}

// .gen.membership_protocol.Message membershipProtocolMessage = 5;
bool Message::has_membershipprotocolmessage() const {
  return messageInternal_case() == kMembershipProtocolMessage;
}
void Message::set_has_membershipprotocolmessage() {
  _oneof_case_[0] = kMembershipProtocolMessage;
}
void Message::clear_membershipprotocolmessage() {
  if (has_membershipprotocolmessage()) {
    delete messageInternal_.membershipprotocolmessage_;
    clear_has_messageInternal();
  }
}
 const ::gen::membership_protocol::Message& Message::membershipprotocolmessage() const {
  // @@protoc_insertion_point(field_get:gen.Message.membershipProtocolMessage)
  return has_membershipprotocolmessage()
      ? *messageInternal_.membershipprotocolmessage_
      : ::gen::membership_protocol::Message::default_instance();
}
::gen::membership_protocol::Message* Message::mutable_membershipprotocolmessage() {
  if (!has_membershipprotocolmessage()) {
    clear_messageInternal();
    set_has_membershipprotocolmessage();
    messageInternal_.membershipprotocolmessage_ = new ::gen::membership_protocol::Message;
  }
  // @@protoc_insertion_point(field_mutable:gen.Message.membershipProtocolMessage)
  return messageInternal_.membershipprotocolmessage_;
}
::gen::membership_protocol::Message* Message::release_membershipprotocolmessage() {
  // @@protoc_insertion_point(field_release:gen.Message.membershipProtocolMessage)
  if (has_membershipprotocolmessage()) {
    clear_has_messageInternal();
    ::gen::membership_protocol::Message* temp = messageInternal_.membershipprotocolmessage_;
    messageInternal_.membershipprotocolmessage_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
void Message::set_allocated_membershipprotocolmessage(::gen::membership_protocol::Message* membershipprotocolmessage) {
  clear_messageInternal();
  if (membershipprotocolmessage) {
    set_has_membershipprotocolmessage();
    messageInternal_.membershipprotocolmessage_ = membershipprotocolmessage;
  }
  // @@protoc_insertion_point(field_set_allocated:gen.Message.membershipProtocolMessage)
}

bool Message::has_messageInternal() const {
  return messageInternal_case() != MESSAGEINTERNAL_NOT_SET;
}
void Message::clear_has_messageInternal() {
  _oneof_case_[0] = MESSAGEINTERNAL_NOT_SET;
}
Message::MessageInternalCase Message::messageInternal_case() const {
  return Message::MessageInternalCase(_oneof_case_[0]);
}
#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace gen

// @@protoc_insertion_point(global_scope)
