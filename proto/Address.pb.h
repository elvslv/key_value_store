// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Address.proto

#ifndef PROTOBUF_Address_2eproto__INCLUDED
#define PROTOBUF_Address_2eproto__INCLUDED

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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
namespace gen {
class Address;
class AddressDefaultTypeInternal;
extern AddressDefaultTypeInternal _Address_default_instance_;
}  // namespace gen

namespace gen {

namespace protobuf_Address_2eproto {
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
}  // namespace protobuf_Address_2eproto

// ===================================================================

class Address : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:gen.Address) */ {
 public:
  Address();
  virtual ~Address();

  Address(const Address& from);

  inline Address& operator=(const Address& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Address& default_instance();

  static inline const Address* internal_default_instance() {
    return reinterpret_cast<const Address*>(
               &_Address_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(Address* other);

  // implements Message ----------------------------------------------

  inline Address* New() const PROTOBUF_FINAL { return New(NULL); }

  Address* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CopyFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void MergeFrom(const ::google::protobuf::Message& from) PROTOBUF_FINAL;
  void CopyFrom(const Address& from);
  void MergeFrom(const Address& from);
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
  void InternalSwap(Address* other);
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

  // repeated uint32 addr = 1;
  int addr_size() const;
  void clear_addr();
  static const int kAddrFieldNumber = 1;
  ::google::protobuf::uint32 addr(int index) const;
  void set_addr(int index, ::google::protobuf::uint32 value);
  void add_addr(::google::protobuf::uint32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
      addr() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
      mutable_addr();

  // uint32 port = 2;
  void clear_port();
  static const int kPortFieldNumber = 2;
  ::google::protobuf::uint32 port() const;
  void set_port(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:gen.Address)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 > addr_;
  mutable int _addr_cached_byte_size_;
  ::google::protobuf::uint32 port_;
  mutable int _cached_size_;
  friend struct protobuf_Address_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// Address

// repeated uint32 addr = 1;
inline int Address::addr_size() const {
  return addr_.size();
}
inline void Address::clear_addr() {
  addr_.Clear();
}
inline ::google::protobuf::uint32 Address::addr(int index) const {
  // @@protoc_insertion_point(field_get:gen.Address.addr)
  return addr_.Get(index);
}
inline void Address::set_addr(int index, ::google::protobuf::uint32 value) {
  addr_.Set(index, value);
  // @@protoc_insertion_point(field_set:gen.Address.addr)
}
inline void Address::add_addr(::google::protobuf::uint32 value) {
  addr_.Add(value);
  // @@protoc_insertion_point(field_add:gen.Address.addr)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >&
Address::addr() const {
  // @@protoc_insertion_point(field_list:gen.Address.addr)
  return addr_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >*
Address::mutable_addr() {
  // @@protoc_insertion_point(field_mutable_list:gen.Address.addr)
  return &addr_;
}

// uint32 port = 2;
inline void Address::clear_port() {
  port_ = 0u;
}
inline ::google::protobuf::uint32 Address::port() const {
  // @@protoc_insertion_point(field_get:gen.Address.port)
  return port_;
}
inline void Address::set_port(::google::protobuf::uint32 value) {
  
  port_ = value;
  // @@protoc_insertion_point(field_set:gen.Address.port)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)


}  // namespace gen

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Address_2eproto__INCLUDED
