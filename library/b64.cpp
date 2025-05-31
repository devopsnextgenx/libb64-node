#include <node.h>
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

// Base64 encoding table
static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

void Sum(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() < 2 || !args[0]->IsNumber() || !args[1]->IsNumber()) {
        args.GetIsolate()->ThrowException(v8::String::NewFromUtf8(args.GetIsolate(), "Invalid arguments").ToLocalChecked());
        return;
    }
    
    double a = args[0]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromJust();
    double b = args[1]->NumberValue(args.GetIsolate()->GetCurrentContext()).FromJust();
    
    args.GetReturnValue().Set(a + b);
}

void HelloWorld(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    
    if (args.Length() < 1 || !args[0]->IsString()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid arguments - expected a string").ToLocalChecked());
        return;
    }

    v8::String::Utf8Value nameStr(isolate, args[0]);
    std::string result = "Hello, World -> " + std::string(*nameStr) + "!!!";
    
    args.GetReturnValue().Set(
        String::NewFromUtf8(isolate, result.c_str()).ToLocalChecked()
    );
}

void Encode(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = args.GetIsolate();
    
    if (args.Length() < 1 || !args[0]->IsString()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid arguments - expected a string").ToLocalChecked());
        return;
    }

    v8::String::Utf8Value input(isolate, args[0]);
    std::string data(*input);
    
    std::string encoded;
    int i = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    int in_len = data.size();
    int pos = 0;

    while (in_len--) {
        char_array_3[i++] = data[pos++];
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; i < 4; i++)
                encoded += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(int j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (int j = 0; j < i + 1; j++)
            encoded += base64_chars[char_array_4[j]];

        while(i++ < 3)
            encoded += '=';
    }

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
    
    std::string decoded;
    int in_len = encoded_string.size();
    int i = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];

    while (in_len-- && (encoded_string[in_] != '=')) {
        if (encoded_string[in_] == '\n' || encoded_string[in_] == '\r') {
            in_++;
            continue;
        }
        
        size_t pos = base64_chars.find(encoded_string[in_]);
        if (pos == std::string::npos) {
            in_++;
            continue;
        }
        
        char_array_4[i++] = pos;
        if (i == 4) {
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; i < 3; i++)
                decoded += char_array_3[i];
            i = 0;
        }
        in_++;
    }

    if (i) {
        int j;
        for (j = 0; j < i; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; j < i - 1; j++)
            decoded += char_array_3[j];
    }

    args.GetReturnValue().Set(String::NewFromUtf8(isolate, decoded.c_str()).ToLocalChecked());
}

void Initialize(Local<Object> exports, Local<Value> module, Local<Context> context, void* priv) {
    NODE_SET_METHOD(exports, "sum", Sum);
    NODE_SET_METHOD(exports, "helloWorld", HelloWorld);
    NODE_SET_METHOD(exports, "encode", Encode);
    NODE_SET_METHOD(exports, "decode", Decode);
}

NODE_MODULE_CONTEXT_AWARE(libb64, Initialize)