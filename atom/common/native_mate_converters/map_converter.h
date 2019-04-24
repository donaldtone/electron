#ifndef ATOM_COMMON_NATIVE_MATE_CONVERTERS_MAP_CONVERTER_H_
#define ATOM_COMMON_NATIVE_MATE_CONVERTERS_MAP_CONVERTER_H_

#include "gin/converter.h"

namespace gin {

template <typename T>
struct Converter<std::map<std::string, T>> {
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     std::map<std::string, T>* out) {
    if (!val->IsObject())
      return false;

    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object> dict = val->ToObject(context).ToLocalChecked();
    v8::Local<v8::Array> keys =
        dict->GetOwnPropertyNames(context).ToLocalChecked();
    for (uint32_t i = 0; i < keys->Length(); ++i) {
      v8::Local<v8::Value> key = keys->Get(i);
      T value;
      if (Converter<T>::FromV8(isolate, dict->Get(key), &value))
        (*out)[gin::V8ToString(isolate, key)] = std::move(value);
    }
    return true;
  }
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                   const std::map<std::string, T>& val) {
    v8::Local<v8::Object> result = v8::Object::New(isolate);
    for (auto i = val.begin(); i != val.end(); i++) {
      result->Set(Converter<T>::ToV8(isolate, i->first),
                  Converter<T>::ToV8(isolate, i->second));
    }
    return result;
  }
};

}  // namespace gin

namespace mate {

template <typename T>
struct Converter<std::map<std::string, T>> {
  static bool FromV8(v8::Isolate* isolate,
                     v8::Local<v8::Value> val,
                     std::map<std::string, T>* out) {
    return gin::Converter<std::map<std::string, T>>::FromV8(isolate, val, out);
  }
  static v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                   const std::map<std::string, T>& val) {
    return gin::Converter<std::map<std::string, T>>::ToV8(isolate, val);
  }
};

}  // namespace mate

#endif  // ATOM_COMMON_NATIVE_MATE_CONVERTERS_MAP_CONVERTER_H_