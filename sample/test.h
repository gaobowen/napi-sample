#ifndef SAMPLE_TEST_H
#define SAMPLE_TEST_H
#include <napi.h>

Napi::Value plus(const Napi::CallbackInfo& info);

Napi::Value call_dll(const Napi::CallbackInfo& info);

Napi::Value cpp_call_js(const Napi::CallbackInfo& info);

Napi::Value cpp_thread_call_js(const Napi::CallbackInfo& info);
#endif