#ifndef SP_H
#define SP_H

#include <cstdlib>

#ifndef SP_H_QUIET
    #define SP_H_QUIET 0
#endif

#ifndef CRASH_ON_NULL_POINTER
    #define CRASH_ON_NULL_POINTER 1
#endif

#if !SP_H_QUIET
#include <iostream>
    #define SP_H_USING_NAMESPACE_STD
#endif

#if CRASH_ON_NULL_POINTER
#include <cstdlib>
    #define SP_H_USING_NAMESPACE_STD

    #define SP_H_QUOTE(X) (#X)
#endif

#ifdef SP_H_USING_NAMESPACE_STD
using namespace std;
#endif



//====================================================================
#define MANAGED(CLASSNAME) \
class CLASSNAME; /* forward declaration */ \
typedef SP<CLASSNAME> CLASSNAME##_ref; \
template class SP<CLASSNAME>;

typedef unsigned reason_t;
#define SP_INCOMPATIBLE 0x10
#define SP_NULL 0x20
#define SP_OK 0x0

typedef void(*sp_error_t)(reason_t);
static sp_error_t sp_error =0;

template<class T>
class SP
{
    T* p;
    unsigned long* c;

    template<typename>
    friend class SP;

public:
    SP(T* _ptr) : p(_ptr), c(new unsigned long(1)) {}
    SP() : p(NULL), c(NULL) {}

    SP<T>(const SP<T>& o)
    {
        _Assign(o.p, o.c);
    }

    template<class Y>
    SP(Y* _ptr)
    {
        _Assign(dynamic_cast<T*>(_ptr));
        if(_ptr && !p) {
            _Error(SP_INCOMPATIBLE);
        }
    }

    template<class Y>
    SP(const SP<Y>& o)
    {
        _Assign(dynamic_cast<T*>(o.p), o.c);
    }

    SP<T>& operator=(const SP<T>& o)
    {
        _Release();
        _Assign(o.p, o.c);
        
        return *this;
    }

    SP<T>& operator=(T* _p)
    {
        _Release();
        _Assign(_p);

        return *this;
    }

    template<class Y>
    SP<T>& operator=(Y* _p)
    {
        _Release();
        _Assign(dynamic_cast<T*>(_p));
        if(!p && _p) {
            _Error(SP_INCOMPATIBLE);
        }
        return *this;
    }

    template<class Y>
    SP<T>& operator=(const SP<Y>& o)
    {
        _Release();
        _Assign(dynamic_cast<T*>(o.p), o.c);
        return *this;
    }

    void Nullify()
    {
        _Release();
        p = NULL;
        c = NULL;
    }

    T& operator*() const
    {
        if(not_null()) {
            return *ptr();
        } else {
            _Error(SP_NULL);
            return *ptr();
        }
    }

    T* ptr() const
    {
        return p;
    }

    T* operator->() const
    {
        if(is_null()) {
            _Error(SP_NULL);
        }
        return ptr();
    }

    bool is_null() const { return ptr() == NULL; }
    bool not_null() const { return ptr() != NULL; }

private:
    void _Release()
    {
        if(not_null() && ((--(*c)) == 0)) {
            delete p;
            delete c;
            p = NULL;
            c = NULL;
        }
    }

    void _Assign(T* const& _p, unsigned long* const& _c)
    {
        p = _p;
        if(p) {
            c = _c;
            if(c) {
                (*c)++;
            }
        } else if(_c) {
            c = NULL;
            _Error(SP_INCOMPATIBLE);
        } else {
            c = NULL;
        }
    }

    void _Assign(T* _p)
    {
        p = _p;
        if(p) {
            c = new unsigned long(1);
        } else {
            c = NULL;
        }
    }

    static void _Error(reason_t reason)
    {
        if(!sp_error) {
#if !SP_H_QUIET
            switch(reason) {
            case SP_INCOMPATIBLE:
                cerr << "incompatible pointer types" << endl;
                break;
            case SP_NULL:
                cerr << "dereferenced NULL pointer" << endl;
                break;
            }
#endif
#if CRASH_ON_NULL_POINTER
            abort();
#endif
        } else {
            sp_error(reason);
        }
    }
};

#endif
