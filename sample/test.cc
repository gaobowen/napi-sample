#include "test.h"
#include <Windows.h>
#include <functional>
#include <sstream>
#include <thread>

Napi::Value plus(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);
    if (info.Length() < 1)
        return Napi::Boolean::New(env, false);
    if (!info[0].IsNumber())
        return Napi::Boolean::New(env, false);
    if (!info[1].IsNumber())
        return Napi::Boolean::New(env, false);
    auto a = info[0].As<Napi::Number>().DoubleValue();
    auto b = info[1].As<Napi::Number>().DoubleValue();

    return Napi::Number::New(env, a + b);
}


Napi::Value call_dll(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);

    auto m = LoadLibraryExA("user32.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    typedef int(*MyMessageBoxA)(int, char*, char*, int);
    auto box = (MyMessageBoxA)GetProcAddress(m, "MessageBoxA");
    box(0, "js call dll function", "abc", 0);

    return Napi::Boolean::New(env, true);
}

Napi::Value cpp_call_js(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);
    if (info.Length() < 0)
        return Napi::Boolean::New(env, false);
    if (!info[0].IsFunction())
        return Napi::Boolean::New(env, false);
    auto jscb = info[0].As<Napi::Function>();

    //回调函数入参
    napi_value a, b;
    napi_create_double(env, 1, &a);
    napi_create_double(env, 2, &b);

    napi_value argv[] = { a, b };

    //回调函数返回值
    napi_value result;
    napi_call_function(env, env.Global(), jscb, 2, argv, &result);

    return Napi::Value(env, result);
}


Napi::ThreadSafeFunction tsfunc;
void thread_function() {
    auto threadid = std::this_thread::get_id();
    std::stringstream ss;
    ss << threadid;
    auto threadidstr = new std::string(ss.str());

    auto callback = [threadid](Napi::Env env, Napi::Function jscb, std::string* p_str) {
        jscb.Call({ Napi::String::New(env, *p_str) });
        delete p_str;
    };

    tsfunc.Acquire();
    napi_status status = tsfunc.BlockingCall(threadidstr, callback);
    if (status != napi_ok) {
        printf("tsfunc.BlockingCall error.\n");
    }
    tsfunc.Release();

}

Napi::Value cpp_thread_call_js(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::EscapableHandleScope scope(env);
    if (info.Length() < 0)
        return Napi::Boolean::New(env, false);
    if (!info[0].IsFunction())
        return Napi::Boolean::New(env, false);
    auto jscb = info[0].As<Napi::Function>();
    tsfunc = Napi::ThreadSafeFunction::New(env, jscb, "my-tsfunc", 2, 2);
    std::thread(thread_function).join();
    std::thread(thread_function).join();
    return Napi::Boolean::New(env, true);
}