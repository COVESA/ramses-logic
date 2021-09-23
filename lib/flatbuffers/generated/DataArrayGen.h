// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_DATAARRAY_RLOGIC_SERIALIZATION_H_
#define FLATBUFFERS_GENERATED_DATAARRAY_RLOGIC_SERIALIZATION_H_

#include "flatbuffers/flatbuffers.h"

namespace rlogic_serialization {

struct floatArr;
struct floatArrBuilder;

struct intArr;
struct intArrBuilder;

struct DataArray;
struct DataArrayBuilder;

enum class EDataArrayType : uint8_t {
  Float = 0,
  Vec2f = 1,
  Vec3f = 2,
  Vec4f = 3,
  Int32 = 4,
  Vec2i = 5,
  Vec3i = 6,
  Vec4i = 7,
  MIN = Float,
  MAX = Vec4i
};

inline const EDataArrayType (&EnumValuesEDataArrayType())[8] {
  static const EDataArrayType values[] = {
    EDataArrayType::Float,
    EDataArrayType::Vec2f,
    EDataArrayType::Vec3f,
    EDataArrayType::Vec4f,
    EDataArrayType::Int32,
    EDataArrayType::Vec2i,
    EDataArrayType::Vec3i,
    EDataArrayType::Vec4i
  };
  return values;
}

inline const char * const *EnumNamesEDataArrayType() {
  static const char * const names[9] = {
    "Float",
    "Vec2f",
    "Vec3f",
    "Vec4f",
    "Int32",
    "Vec2i",
    "Vec3i",
    "Vec4i",
    nullptr
  };
  return names;
}

inline const char *EnumNameEDataArrayType(EDataArrayType e) {
  if (flatbuffers::IsOutRange(e, EDataArrayType::Float, EDataArrayType::Vec4i)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesEDataArrayType()[index];
}

enum class ArrayUnion : uint8_t {
  NONE = 0,
  floatArr = 1,
  intArr = 2,
  MIN = NONE,
  MAX = intArr
};

inline const ArrayUnion (&EnumValuesArrayUnion())[3] {
  static const ArrayUnion values[] = {
    ArrayUnion::NONE,
    ArrayUnion::floatArr,
    ArrayUnion::intArr
  };
  return values;
}

inline const char * const *EnumNamesArrayUnion() {
  static const char * const names[4] = {
    "NONE",
    "floatArr",
    "intArr",
    nullptr
  };
  return names;
}

inline const char *EnumNameArrayUnion(ArrayUnion e) {
  if (flatbuffers::IsOutRange(e, ArrayUnion::NONE, ArrayUnion::intArr)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesArrayUnion()[index];
}

template<typename T> struct ArrayUnionTraits {
  static const ArrayUnion enum_value = ArrayUnion::NONE;
};

template<> struct ArrayUnionTraits<rlogic_serialization::floatArr> {
  static const ArrayUnion enum_value = ArrayUnion::floatArr;
};

template<> struct ArrayUnionTraits<rlogic_serialization::intArr> {
  static const ArrayUnion enum_value = ArrayUnion::intArr;
};

bool VerifyArrayUnion(flatbuffers::Verifier &verifier, const void *obj, ArrayUnion type);
bool VerifyArrayUnionVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

struct floatArr FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef floatArrBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DATA = 4
  };
  const flatbuffers::Vector<float> *data() const {
    return GetPointer<const flatbuffers::Vector<float> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct floatArrBuilder {
  typedef floatArr Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_data(flatbuffers::Offset<flatbuffers::Vector<float>> data) {
    fbb_.AddOffset(floatArr::VT_DATA, data);
  }
  explicit floatArrBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  floatArrBuilder &operator=(const floatArrBuilder &);
  flatbuffers::Offset<floatArr> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<floatArr>(end);
    return o;
  }
};

inline flatbuffers::Offset<floatArr> CreatefloatArr(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<float>> data = 0) {
  floatArrBuilder builder_(_fbb);
  builder_.add_data(data);
  return builder_.Finish();
}

struct floatArr::Traits {
  using type = floatArr;
  static auto constexpr Create = CreatefloatArr;
};

inline flatbuffers::Offset<floatArr> CreatefloatArrDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<float> *data = nullptr) {
  auto data__ = data ? _fbb.CreateVector<float>(*data) : 0;
  return rlogic_serialization::CreatefloatArr(
      _fbb,
      data__);
}

struct intArr FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef intArrBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DATA = 4
  };
  const flatbuffers::Vector<int32_t> *data() const {
    return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct intArrBuilder {
  typedef intArr Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_data(flatbuffers::Offset<flatbuffers::Vector<int32_t>> data) {
    fbb_.AddOffset(intArr::VT_DATA, data);
  }
  explicit intArrBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  intArrBuilder &operator=(const intArrBuilder &);
  flatbuffers::Offset<intArr> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<intArr>(end);
    return o;
  }
};

inline flatbuffers::Offset<intArr> CreateintArr(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<int32_t>> data = 0) {
  intArrBuilder builder_(_fbb);
  builder_.add_data(data);
  return builder_.Finish();
}

struct intArr::Traits {
  using type = intArr;
  static auto constexpr Create = CreateintArr;
};

inline flatbuffers::Offset<intArr> CreateintArrDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<int32_t> *data = nullptr) {
  auto data__ = data ? _fbb.CreateVector<int32_t>(*data) : 0;
  return rlogic_serialization::CreateintArr(
      _fbb,
      data__);
}

struct DataArray FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef DataArrayBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_TYPE = 6,
    VT_DATA_TYPE = 8,
    VT_DATA = 10
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  rlogic_serialization::EDataArrayType type() const {
    return static_cast<rlogic_serialization::EDataArrayType>(GetField<uint8_t>(VT_TYPE, 0));
  }
  rlogic_serialization::ArrayUnion data_type() const {
    return static_cast<rlogic_serialization::ArrayUnion>(GetField<uint8_t>(VT_DATA_TYPE, 0));
  }
  const void *data() const {
    return GetPointer<const void *>(VT_DATA);
  }
  template<typename T> const T *data_as() const;
  const rlogic_serialization::floatArr *data_as_floatArr() const {
    return data_type() == rlogic_serialization::ArrayUnion::floatArr ? static_cast<const rlogic_serialization::floatArr *>(data()) : nullptr;
  }
  const rlogic_serialization::intArr *data_as_intArr() const {
    return data_type() == rlogic_serialization::ArrayUnion::intArr ? static_cast<const rlogic_serialization::intArr *>(data()) : nullptr;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<uint8_t>(verifier, VT_TYPE) &&
           VerifyField<uint8_t>(verifier, VT_DATA_TYPE) &&
           VerifyOffset(verifier, VT_DATA) &&
           VerifyArrayUnion(verifier, data(), data_type()) &&
           verifier.EndTable();
  }
};

template<> inline const rlogic_serialization::floatArr *DataArray::data_as<rlogic_serialization::floatArr>() const {
  return data_as_floatArr();
}

template<> inline const rlogic_serialization::intArr *DataArray::data_as<rlogic_serialization::intArr>() const {
  return data_as_intArr();
}

struct DataArrayBuilder {
  typedef DataArray Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(DataArray::VT_NAME, name);
  }
  void add_type(rlogic_serialization::EDataArrayType type) {
    fbb_.AddElement<uint8_t>(DataArray::VT_TYPE, static_cast<uint8_t>(type), 0);
  }
  void add_data_type(rlogic_serialization::ArrayUnion data_type) {
    fbb_.AddElement<uint8_t>(DataArray::VT_DATA_TYPE, static_cast<uint8_t>(data_type), 0);
  }
  void add_data(flatbuffers::Offset<void> data) {
    fbb_.AddOffset(DataArray::VT_DATA, data);
  }
  explicit DataArrayBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  DataArrayBuilder &operator=(const DataArrayBuilder &);
  flatbuffers::Offset<DataArray> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<DataArray>(end);
    return o;
  }
};

inline flatbuffers::Offset<DataArray> CreateDataArray(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    rlogic_serialization::EDataArrayType type = rlogic_serialization::EDataArrayType::Float,
    rlogic_serialization::ArrayUnion data_type = rlogic_serialization::ArrayUnion::NONE,
    flatbuffers::Offset<void> data = 0) {
  DataArrayBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_name(name);
  builder_.add_data_type(data_type);
  builder_.add_type(type);
  return builder_.Finish();
}

struct DataArray::Traits {
  using type = DataArray;
  static auto constexpr Create = CreateDataArray;
};

inline flatbuffers::Offset<DataArray> CreateDataArrayDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    rlogic_serialization::EDataArrayType type = rlogic_serialization::EDataArrayType::Float,
    rlogic_serialization::ArrayUnion data_type = rlogic_serialization::ArrayUnion::NONE,
    flatbuffers::Offset<void> data = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return rlogic_serialization::CreateDataArray(
      _fbb,
      name__,
      type,
      data_type,
      data);
}

inline bool VerifyArrayUnion(flatbuffers::Verifier &verifier, const void *obj, ArrayUnion type) {
  switch (type) {
    case ArrayUnion::NONE: {
      return true;
    }
    case ArrayUnion::floatArr: {
      auto ptr = reinterpret_cast<const rlogic_serialization::floatArr *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case ArrayUnion::intArr: {
      auto ptr = reinterpret_cast<const rlogic_serialization::intArr *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyArrayUnionVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyArrayUnion(
        verifier,  values->Get(i), types->GetEnum<ArrayUnion>(i))) {
      return false;
    }
  }
  return true;
}

}  // namespace rlogic_serialization

#endif  // FLATBUFFERS_GENERATED_DATAARRAY_RLOGIC_SERIALIZATION_H_
