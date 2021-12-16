#include <napi.h>
#include "test.h"

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "plus"), Napi::Function::New(env, plus));    
    exports.Set(Napi::String::New(env, "call_dll"), Napi::Function::New(env, call_dll));
    exports.Set(Napi::String::New(env, "cpp_call_js"), Napi::Function::New(env, cpp_call_js));
    exports.Set(Napi::String::New(env, "cpp_thread_call_js"), Napi::Function::New(env, cpp_thread_call_js));  
    
    return exports;
}

NODE_API_MODULE(napi_sample, Init)