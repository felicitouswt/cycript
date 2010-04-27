/* Cycript - Inlining/Optimizing JavaScript Compiler
 * Copyright (C) 2009  Jay Freeman (saurik)
*/

/* Modified BSD License {{{ */
/*
 *        Redistribution and use in source and binary
 * forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the
 *    above copyright notice, this list of conditions
 *    and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the
 *    above copyright notice, this list of conditions
 *    and the following disclaimer in the documentation
 *    and/or other materials provided with the
 *    distribution.
 * 3. The name of the author may not be used to endorse
 *    or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/* }}} */

#ifndef CYCRIPT_JAVASCRIPT_HPP
#define CYCRIPT_JAVASCRIPT_HPP

#include <JavaScriptCore/JSBase.h>
#include <JavaScriptCore/JSContextRef.h>
#include <JavaScriptCore/JSStringRef.h>
#include <JavaScriptCore/JSObjectRef.h>
#include <JavaScriptCore/JSValueRef.h>

#ifdef HAVE_FFI_FFI_H
#include <ffi/ffi.h>
#else
#include <ffi.h>
#endif

extern JSStringRef Array_s;
extern JSStringRef cy_s;
extern JSStringRef length_s;
extern JSStringRef message_s;
extern JSStringRef name_s;
extern JSStringRef pop_s;
extern JSStringRef prototype_s;
extern JSStringRef push_s;
extern JSStringRef splice_s;
extern JSStringRef toCYON_s;
extern JSStringRef toJSON_s;
extern JSStringRef toPointer_s;
extern JSStringRef toString_s;

void CYInitializeDynamic();
JSGlobalContextRef CYGetJSContext();
JSObjectRef CYGetGlobalObject(JSContextRef context);

extern "C" void CYSetupContext(JSGlobalContextRef context);
const char *CYExecute(apr_pool_t *pool, CYUTF8String code);

void CYSetArgs(int argc, const char *argv[]);

bool CYCastBool(JSContextRef context, JSValueRef value);
double CYCastDouble(JSContextRef context, JSValueRef value);

CYUTF8String CYPoolUTF8String(apr_pool_t *pool, JSContextRef context, JSStringRef value);
const char *CYPoolCString(apr_pool_t *pool, JSContextRef context, JSStringRef value);

JSValueRef CYGetProperty(JSContextRef context, JSObjectRef object, size_t index);
JSValueRef CYGetProperty(JSContextRef context, JSObjectRef object, JSStringRef name);

void CYSetProperty(JSContextRef context, JSObjectRef object, size_t index, JSValueRef value);
void CYSetProperty(JSContextRef context, JSObjectRef object, JSStringRef name, JSValueRef value, JSPropertyAttributes attributes = kJSPropertyAttributeNone);
void CYSetProperty(JSContextRef context, JSObjectRef object, JSStringRef name, JSValueRef (*callback)(JSContextRef, JSObjectRef, JSObjectRef, size_t, const JSValueRef[], JSValueRef *), JSPropertyAttributes attributes = kJSPropertyAttributeNone);

JSObjectRef CYGetCachedObject(JSContextRef context, JSStringRef name);

JSValueRef CYCastJSValue(JSContextRef context, bool value);
JSValueRef CYCastJSValue(JSContextRef context, double value);
JSValueRef CYCastJSValue(JSContextRef context, int value);
JSValueRef CYCastJSValue(JSContextRef context, unsigned int value);
JSValueRef CYCastJSValue(JSContextRef context, long int value);
JSValueRef CYCastJSValue(JSContextRef context, long unsigned int value);
JSValueRef CYCastJSValue(JSContextRef context, long long int value);
JSValueRef CYCastJSValue(JSContextRef context, long long unsigned int value);

JSValueRef CYCastJSValue(JSContextRef context, JSStringRef value);
JSValueRef CYCastJSValue(JSContextRef context, const char *value);

JSObjectRef CYCastJSObject(JSContextRef context, JSValueRef value);
JSValueRef CYJSUndefined(JSContextRef context);
JSValueRef CYJSNull(JSContextRef context);

void *CYCastPointer_(JSContextRef context, JSValueRef value);

template <typename Type_>
_finline Type_ CYCastPointer(JSContextRef context, JSValueRef value) {
    return reinterpret_cast<Type_>(CYCastPointer_(context, value));
}

void CYPoolFFI(apr_pool_t *pool, JSContextRef context, sig::Type *type, ffi_type *ffi, void *data, JSValueRef value);
JSValueRef CYFromFFI(JSContextRef context, sig::Type *type, ffi_type *ffi, void *data, bool initialize = false, JSObjectRef owner = NULL);

JSValueRef CYCallFunction(apr_pool_t *pool, JSContextRef context, size_t setups, void *setup[], size_t count, const JSValueRef arguments[], bool initialize, JSValueRef *exception, sig::Signature *signature, ffi_cif *cif, void (*function)());

bool CYIsCallable(JSContextRef context, JSValueRef value);
JSValueRef CYCallAsFunction(JSContextRef context, JSObjectRef function, JSObjectRef _this, size_t count, const JSValueRef arguments[]);

const char *CYPoolCCYON(apr_pool_t *pool, JSContextRef context, JSObjectRef object);

struct CYHooks {
    void *(*ExecuteStart)(JSContextRef);
    void (*ExecuteEnd)(JSContextRef, void *);

    JSValueRef (*RuntimeProperty)(JSContextRef, CYUTF8String);
    void (*CallFunction)(JSContextRef, ffi_cif *, void (*)(), uint8_t *, void **);

    void (*Initialize)();
    void (*SetupContext)(JSContextRef);

    bool (*PoolFFI)(apr_pool_t *, JSContextRef, sig::Type *, ffi_type *, void *, JSValueRef);
    JSValueRef (*FromFFI)(JSContextRef, sig::Type *, ffi_type *, void *, bool, JSObjectRef);
};

extern struct CYHooks *hooks_;

JSObjectRef CYMakePointer(JSContextRef context, void *pointer, size_t length, sig::Type *type, ffi_type *ffi, JSObjectRef owner);

void CYFinalize(JSObjectRef object);

const char *CYPoolCString(apr_pool_t *pool, JSContextRef context, JSValueRef value);

JSStringRef CYCopyJSString(const char *value);
JSStringRef CYCopyJSString(JSStringRef value);
JSStringRef CYCopyJSString(CYUTF8String value);
JSStringRef CYCopyJSString(JSContextRef context, JSValueRef value);

class CYJSString {
  private:
    JSStringRef string_;

    void Clear_() {
        if (string_ != NULL)
            JSStringRelease(string_);
    }

  public:
    CYJSString(const CYJSString &rhs) :
        string_(CYCopyJSString(rhs.string_))
    {
    }

    template <typename Arg0_>
    CYJSString(Arg0_ arg0) :
        string_(CYCopyJSString(arg0))
    {
    }

    template <typename Arg0_, typename Arg1_>
    CYJSString(Arg0_ arg0, Arg1_ arg1) :
        string_(CYCopyJSString(arg0, arg1))
    {
    }

    CYJSString &operator =(const CYJSString &rhs) {
        Clear_();
        string_ = CYCopyJSString(rhs.string_);
        return *this;
    }

    ~CYJSString() {
        Clear_();
    }

    void Clear() {
        Clear_();
        string_ = NULL;
    }

    operator JSStringRef() const {
        return string_;
    }
};

#endif/*CYCRIPT_JAVASCRIPT_HPP*/
