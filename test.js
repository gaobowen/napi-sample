let { plus, call_dll, cpp_call_js, cpp_thread_call_js } = require('./build/Release/napi_sample.node');

(async function () {

    console.log('test export=>', plus);
    console.log('test call=>', plus(1.2, 1.3));

    console.log('test call_dll=>', call_dll());

    let c = 3;
    let js_callback = function(a, b){
        return a + b + c;
    }
    console.log('test cpp_call_js=>', cpp_call_js(js_callback));


    let js_callback_async = function(id){
        console.log('thread_id',id);
    }
    console.log('test cpp_thread_call_js=>', cpp_thread_call_js(js_callback_async));

})();

