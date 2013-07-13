//
// SqratUtil: Squirrel Utilities
//

//
// Copyright (c) 2009 Brandon Jones
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

#if !defined(_SCRAT_UTIL_H_)
#define _SCRAT_UTIL_H_

#include <squirrel.h>
#include <string.h>


namespace Sqrat {

typedef std::basic_string<SQChar> string;

class DefaultVM {
private:
    static HSQUIRRELVM& staticVm() {
        static HSQUIRRELVM vm;
        return vm;
    }
public:
    static HSQUIRRELVM Get() {
        return staticVm();
    }
    static void Set(HSQUIRRELVM vm) {
        staticVm() = vm;
    }
};

class ErrorHandling {
private:
    static bool& errorHandling() {
        static bool eh = true;
        return eh;
    }
public:
    static bool IsEnabled() {
        return errorHandling();
    }
    static void Enable(bool enable) {
        errorHandling() = enable;
    }
};

class Exception {
public:
    Exception(const string& msg) : message(msg) {}
    Exception(const Exception& ex) : message(ex.message) {}

    const string Message() const {
        return message;
    }

private:
    string message;
};

inline string LastErrorString( HSQUIRRELVM vm ) {
    const SQChar* sqErr;
    sq_getlasterror(vm);
    if(sq_gettype(vm, -1) == OT_NULL) {
        return string();
    }
    sq_tostring(vm, -1);
    sq_getstring(vm, -1, &sqErr);
    return string(sqErr);
}
class Error {
public:
    static Error& Instance() {
        static Error instance;
        return instance;
    }
    static string FormatTypeError(HSQUIRRELVM vm, SQInteger idx, const string& expectedType) {
        string err = _SC("wrong type (") + expectedType + _SC(" expected");
        if (SQ_SUCCEEDED(sq_typeof(vm, idx))) {
            const SQChar* actualType;
            sq_tostring(vm, -1);
            sq_getstring(vm, -1, &actualType);
            sq_pop(vm, 1);
            err = err + _SC(", got ") + actualType;
        }
        sq_pop(vm, 1);
        err = err + _SC(")");
        return err;
    }
    void Clear(HSQUIRRELVM vm) {
        //TODO: use mutex to lock errMap in multithreaded environment
        errMap.erase(vm);
    }
    string Message(HSQUIRRELVM vm) {
        //TODO: use mutex to lock errMap in multithreaded environment
        string err = errMap[vm];
        errMap.erase(vm);
        return err;
    }
    bool Occurred(HSQUIRRELVM vm) {
        //TODO: use mutex to lock errMap in multithreaded environment
        return errMap.find(vm) != errMap.end();
    }
    void Throw(HSQUIRRELVM vm, const string& err) {
        //TODO: use mutex to lock errMap in multithreaded environment
        if (errMap.find(vm) == errMap.end()) {
            errMap[vm] = err;
        }
    }

private:
    Error() {}

    std::map< HSQUIRRELVM, string > errMap;
};

}

#endif
