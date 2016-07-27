/*
 * accssnode (C) 2016 by Gandalf Sievers
 * Created by Gandalf Sievers on 26.05.16.
 *
 * MIT-License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <node.h>
extern "C"
{
    #include "../accss/src/argsparser.h"
    #include "../accss/src/ast.h"
    #include "../accss/src/helper.h"
    #include "../accss/src/gettokens.h"
    #include "../accss/src/getpast.h"
    #include "../accss/src/error.h"
    #include "../accss/src/compressor.h"
    #include "../accss/src/translator.h"
    #include "../accss/src/debug.h"
}

namespace accss {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Maybe;
using v8::MaybeLocal;
using v8::Context;
using v8::Boolean;

bool getBoolFromOptions(Isolate* isolate, Local<Object> options, const char* key, bool defaultValue)
{
    Local<Context> context(isolate->GetCurrentContext());

    Maybe<bool> optionSet = options->Has(context, String::NewFromUtf8(isolate, key));

    if(!optionSet.IsNothing() && optionSet.FromJust())
    {
        MaybeLocal<Value> optionMayBeValue = options->Get(context, String::NewFromUtf8(isolate, key));
        if(!optionMayBeValue.IsEmpty())
        {
            Local<Value> optionValue = optionMayBeValue.ToLocalChecked();
            if(!optionValue->IsBoolean())
            {
                isolate->ThrowException(Exception::TypeError(
                    String::Concat( String::NewFromUtf8(isolate, key), String::NewFromUtf8(isolate, " must be boolean"))
                    )
                );
                return defaultValue;
            }
            MaybeLocal<Boolean> optionMayBe = optionValue->ToBoolean(context);
            if(!optionMayBe.IsEmpty())
            {
                Local<Boolean> optionBoolean = optionMayBe.ToLocalChecked ();
                return optionBoolean->Value();
            }
        }
    }
    return defaultValue;
}

unsigned char getCompatFromOptions(Isolate* isolate, Local<Object> options, const char* key, unsigned char defaultValue)
{
    Local<Context> context(isolate->GetCurrentContext());

    Maybe<bool> optionSet = options->Has(context, String::NewFromUtf8(isolate, key));

    if(!optionSet.IsNothing() && optionSet.FromJust())
    {
        MaybeLocal<Value> optionMayBeValue = options->Get(context, String::NewFromUtf8(isolate, key));
        if(!optionMayBeValue.IsEmpty())
        {
            Local<Value> optionValue = optionMayBeValue.ToLocalChecked();
            if(!optionValue->IsString())
            {
                isolate->ThrowException(Exception::TypeError(
                    String::Concat( String::NewFromUtf8(isolate, key), String::NewFromUtf8(isolate, " must be string"))
                    )
                );
                return defaultValue;
            }
            MaybeLocal<String> optionMayBe = optionValue->ToString(context);
            if(!optionMayBe.IsEmpty())
            {
                Local<String> optionString = optionMayBe.ToLocalChecked();

                const char* str = *(v8::String::Utf8Value(optionString));
                if(strcmp(str, "all") == 0)
                {
                    return ACCSSOPTION_ALL;
                }
                else if(strcmp(str, "ie7") == 0)
                {
                    return ACCSSOPTION_ALL;
                }
                else if(strcmp(str, "ie8") == 0)
                {
                    return ACCSSOPTION_GE_IE8;
                }
                else if(strcmp(str, "ie9") == 0)
                {
                    return ACCSSOPTION_GE_IE9;
                }
                else if(strcmp(str, "ie10") == 0)
                {
                    return ACCSSOPTION_GE_IE10;
                }
                else if(strcmp(str, "ie11") == 0)
                {
                    return ACCSSOPTION_GE_IE11;
                }
                else if(strcmp(str, "chrome") == 0)
                {
                    return ACCSSOPTION_GE_CHROME;
                }
                else if(strcmp(str, "future") == 0)
                {
                    return ACCSSOPTION_FUTURE;
                }
                else if(strcmp(str, "none") == 0)
                {
                    return ACCSSOPTION_NONE;
                }
                else
                {
                    isolate->ThrowException(Exception::TypeError(
                        String::Concat( String::NewFromUtf8(isolate, key), String::NewFromUtf8(isolate, ": unrecognized option"))
                        )
                    );
                    return defaultValue;
                }
            }
        }
    }
    return defaultValue;
}

// This is the implementation of the "main" method in accss
// Input arguments are passed using the
// const FunctionCallbackInfo<Value>& args struct
void minify(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    bool restructure = true;
    bool preserve = false;
    unsigned char compat = 255;

  // Check the number of arguments passed.
    if (args.Length() < 1 || args.Length() > 2) {
    // Throw an Error that is passed back to JavaScript
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong number of arguments"))
        );
        return;
    }

  // Check the argument types
    if (!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong arguments"))
        );
        return;
    }


      // Check the argument types
    if (args.Length() == 2)
    {
        if (!args[1]->IsObject()) {
            isolate->ThrowException(Exception::TypeError(
                String::NewFromUtf8(isolate, "Wrong arguments"))
            );
            return;
        }
        Local<Object> options = args[1]->ToObject();

        restructure = getBoolFromOptions(isolate, options, "restructure", restructure);
        preserve = getBoolFromOptions(isolate, options, "preserve-splitted", preserve);
        compat = getCompatFromOptions(isolate, options, "compat", compat);
    }

    Local<String> inStr = args[0]->ToString();

    size_t outlen = 0;
    char*  outstr = NULL;
    size_t inlen = inStr->Utf8Length();

    char* string = (char*)malloc(sizeof(char)*(inlen+1));
    memset(string, 0, inlen);

    inStr->WriteUtf8(string);

    if(inlen > 0)
    {
        char error = 0;
        struct astnode* stylesheet = NULL;
        struct token_info tokens = getTokens(string, &error);

        free(string);

        if(error)
        {
            deleteTokens(&tokens);
            isolate->ThrowException(Exception::SyntaxError(
                String::NewFromUtf8(isolate, "Error Tokenizing CSS\n"))
            );
            return;
        }

        stylesheet = getAST(&tokens, 0, &error);

        deleteTokens(&tokens);
        if(error)
        {
            isolate->ThrowException(Exception::SyntaxError(
                String::NewFromUtf8(isolate, "Error creating AST tree\n"))
            );
            return;
        }

        stylesheet = compress(stylesheet, restructure, !preserve, compat);

        outstr = translate(stylesheet);

        deleteASTTree(stylesheet);

        outlen = strlen(outstr);
    }
    else
    {
        outlen = inlen;
        outstr = string;
    }

    v8::MaybeLocal<String> mayBeStr = String::NewFromUtf8(isolate, outstr, v8::NewStringType::kNormal, outlen);
    if(!mayBeStr.IsEmpty())
    {
        Local<String> str = mayBeStr.ToLocalChecked();
        args.GetReturnValue().Set(str);
    }

    free(outstr);

  // Set the return value (using the passed in
  // FunctionCallbackInfo<Value>&)

}

void Init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "minify", minify);
}

NODE_MODULE(accssnode, Init)

}