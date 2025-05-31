#include <node.h>
#include "b64/cencode.h"
#include "b64/cdecode.h"
#include <string>
#include <vector>
#include <sstream>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::String;
using v8::Number;
using v8::Local;
using v8::Value;
using v8::Object;
using v8::Context;
using v8::Primitive;

void Encode(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    
    if (args.Length() < 1 || !args[0]->IsString()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid arguments - expected a string").ToLocalChecked());
        return;
    }

    v8::String::Utf8Value input(isolate, args[0]);
    std::string data(*input);
    
    // Initialize encoding state
    base64_encodestate state;
    base64_init_encodestate(&state);
    
    // Calculate required buffer size
    size_t input_length = data.size();
    size_t output_length = base64_encode_length(input_length, &state);
    
    // Create output buffer
    std::vector<char> encoded_buffer(output_length);
    
    // Encode the data
    size_t count = base64_encode_block(data.c_str(), input_length, encoded_buffer.data(), &state);
    count += base64_encode_blockend(encoded_buffer.data() + count, &state);
    
    // Convert to string and return
    std::string encoded(encoded_buffer.data(), count);
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, encoded.c_str()).ToLocalChecked());
}

void Decode(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    
    if (args.Length() < 1 || !args[0]->IsString()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid arguments - expected a string").ToLocalChecked());
        return;
    }

    v8::String::Utf8Value input(isolate, args[0]);
    std::string encoded_string(*input);
    
    // Initialize decoding state
    base64_decodestate state;
    base64_init_decodestate(&state);
    
    // Calculate maximum output size
    size_t max_length = base64_decode_maxlength(encoded_string.length());
    std::vector<char> decoded_buffer(max_length);
    
    // Decode the data
    size_t count = base64_decode_block(encoded_string.c_str(), 
                                     encoded_string.length(),
                                     decoded_buffer.data(),
                                     &state);
    
    // Convert to string and return
    std::string decoded(decoded_buffer.data(), count);
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, decoded.c_str()).ToLocalChecked());
}

void Initialize(Local<Object> exports, Local<Value> module, Local<Context> context, void* priv) {
    NODE_SET_METHOD(exports, "encode", Encode);
    NODE_SET_METHOD(exports, "decode", Decode);
}

NODE_MODULE_CONTEXT_AWARE(libb64, Initialize)