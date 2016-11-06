/*
Copyright (c) 2006, Garrett Franklin Boyer
 All rights reserved.
 
 
 Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
Neither the name of the Georgia Institute of Technology nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "config.h"

#if SIZEOF_LONG==4
#define INT32_T long
#elif SIZEOF_INT==4
#define INT32_T int
#elif SIZEOF_SHORT==4
#define INT32_T short
#else
"Could not find a suitable 4-byte type.";
#endif

#if SIZEOF_LONG==2
#define INT16_T long
#elif SIZEOF_INT==2
#define INT16_T int
#elif SIZEOF_SHORT==2
#define INT16_T short
#else
"Could not find a suitable 4-byte type.";
#endif

typedef INT16_T i16;
typedef unsigned INT16_T ui16;
typedef INT32_T i32;
typedef unsigned INT32_T ui32;
typedef signed char i8;
typedef unsigned char ui8;

#ifndef unlikely
#define unlikely(x) (x)
#define likely(x) (x)
#endif

#ifndef integer_t
#define integer_t int /* needed for some reason on macs... */
#endif

#ifdef reg
#undef reg /* SunOS headers define this to be register */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define Warning(x) int warn_about_##x

#define ALLOC(x) (((x)=malloc(sizeof(*(x))))!=NULL)
#define ALLOC_ARRAY(x,n) (((x)=malloc((n)*sizeof(*(x))))!=NULL)

#ifdef __cplusplus
#ifdef EXCEPTIONS
#include <exception>
#endif

#include <new>

#define CPP_ADAPT(cl, ctor, dtor, copy) \
    cl::cl() { Exception::when(ctor(this) < 0); } \
    cl::~cl() { dtor(this); } \
    cl::cl(const cl& o) { Exception::when(copy(this, &other)<0)); } \
    cl& operator = (const cl& o) { dtor(this); Exception::when(!copy(this, &other)); }

#define MAKE_ASSIGNOP(cl) \
	const cl&operator=(const cl&o) \
	{if(this!=&o){this->~cl();new(this)cl(o);return *this;}}

#define DISALLOW_COPY(cl) \
        private: cl (const cl&); void operator=(const cl&)

#define FILL_INEQUALITY_COMPARATORS(cl1) \
        friend bool operator > (const cl& a, const cl& b) { return b < a; } \
        friend bool operator >= (const cl& a, const cl& b) { return !(a < b); } \
        friend bool operator <= (const cl& a, const cl& b) { return !(b < a); }

#define FILL_ALL_COMPARATORS(cl) \
        FILL_INEQUALITY_COMPARATORS(cl) \
        friend bool operator != (const cl& a, const cl& b) { return !(a == b); }

#define FILL_COMPARATORS_FROM_EXP(p1, p2, compexp) \
        friend bool operator == (p1, p2) \
         { return (compexp) == 0; } \
        friend bool operator != (p1, p2) \
         { return (compexp) != 0; } \
        friend bool operator >= (p1, p2) \
         { return (compexp) >= 0; } \
        friend bool operator <= (p1, p2) \
         { return (compexp) <= 0; } \
        friend bool operator > (p1, p2) \
         { return (compexp) > 0; } \
        friend bool operator < (p1, p2) \
         { return (compexp) < 0; } \

#ifndef DEBUG_ONLY
#ifdef DEBUG
#define DEBUG_ONLY(x) x
#else
#define DEBUG_ONLY(x) (void)0
#endif
#endif
#ifdef EXCEPTIONS
#define NOTHROW throw()
#define THROWS(e) throw e
#else
#define NOTHROW
#define THROWS(e)
#endif

/* Okay, exceptions?
 * Well, exceptions are nice.  They free you up from a lot of silly
 * error checking.
 *
 * HOWEVER...
 *
 * Lots of C++ libraries disable exception handling.
 */

#ifdef __GNUC__
#define UNUSED(x) /* nothing */
#define FORMAT_PRINTF(fnum,ell) __attribute__((format(__printf__,fnum,ell)))
#else
#define UNUSED(x) x
#define FORMAT_PRINTF(formatnum,dotdotdotnum)
#endif

class Exception
#ifdef EXCEPTIONS
 : public std::exception
#endif
{
 public:
#ifdef EXCEPTIONS
    static void when(bool cond)
     { if (cond) throw Exception(); }
    static void when(bool cond, const char *message)
     { if (cond) throw Exception(message); }
#else
    static void when(bool UNUSED(cond)) { }
    static void when(bool UNUSED(cond), const char *UNUSED(message)) { }
#endif

 public:
    Exception() NOTHROW : message(0) { }
    Exception(const char *inMessage) NOTHROW : message(inMessage) { }
    
    const char *what() const NOTHROW { return message; }
    
 private:
    const char *message;
};

class OpenFile
{
 public:
    OpenFile(const char *fname, const char *options)
     : f(fopen(fname, options)) { }
    OpenFile(FILE *inF) NOTHROW : f(inF) { }
    OpenFile(const OpenFile &o) NOTHROW : f(o.f) { }
    ~OpenFile() NOTHROW { if (f) (void) fclose(f); }
    void close() { if (f) (void) fclose(f); f = NULL; }
    operator FILE * () NOTHROW { return f; }
    OpenFile& operator = (FILE *inF) NOTHROW { f = inF; return *this; }
    OpenFile& operator = (const OpenFile &o) NOTHROW { f = o.f; return *this; }
 private:
    FILE *f;
};

namespace Memory
{
    template<typename T>
    inline T * allocBytes(size_t bytes) THROWS((Exception))
     { T *p = reinterpret_cast<T*>(malloc(bytes));
       Exception::when(!p);
       return p; }
    template<typename T>
    inline T * alloc(size_t elems = 1) THROWS((Exception))
     { return allocBytes<T>(elems * sizeof(T)); }
    template<typename T>
    inline T * allocZeroed(size_t elems = 1) THROWS((Exception))
     { return reinterpret_cast<T*>(calloc(elems, sizeof(T))); }
    
    template<typename T>
    inline T * reallocBytes(T* ptr, size_t bytes) THROWS((Exception))
     { T *p = reinterpret_cast<T*>(realloc(ptr, bytes));
       Exception::when(!p);
       return p; }
    template<typename T>
    inline T * resize(T* ptr, size_t elems = 1) THROWS((Exception))
     { return reallocBytes<T>(ptr, elems * sizeof(T)); }
    
    template<typename T>
    inline T * copyBytes(T* dest, const T* src, size_t b = sizeof(T)) NOTHROW
     { memcpy(dest, src, b); return dest; }
    template<typename T>
    inline T * copy(T* dest, const T* src, size_t elems = 1) NOTHROW
     { return copyBytes(dest, src, elems * sizeof(T)); }
    
    template<typename T>
    inline T * dupBytes(const T* src, size_t size = sizeof(T))
     { T* p = allocBytes<T>(size); return copyBytes(p, src, size); }
    template<typename T>
    inline T * dup(const T* src, size_t elems = 1)
     { return dupBytes(src, elems * sizeof(T)); }
    
    template<typename T>
    inline void zero(T* start, size_t count = 1) NOTHROW
     { zeroBytes(start, count * sizeof(T)); }
    template<typename T>
    inline void zeroBytes(T* start, size_t bytes = sizeof(T)) NOTHROW
     { ::memset(start, 0, bytes); }
    
    template<typename T>
    inline void free(T* ptr) NOTHROW
     { ::free(ptr); }
    
    template<typename T>
    inline T* construct(T* p)
     { new(p)T(); return p; }
    template<typename T>
    inline T* constructAll(T* m, size_t elems)
     { for (size_t i = 0; i < elems; i++) new(m+i)T(); return m; }
    template<>
    inline char* constructAll<char>(char* m, size_t elems)
     { return m; }
    
    template<typename T, typename U>
    inline T* construct(T* p, U u)
     { new(p)T(u); return p; }
    template<typename T, typename U>
    inline T* constructAll(T* m, U u, size_t elems)
     { for (size_t i = 0; i < elems; i++) new(m+i)T(u); return m; }
    
    template<typename T, typename U, typename V>
    inline T* construct(T* p, U u, V v)
     { new(p)T(u, v); return p; }
    template<typename T, typename U, typename V>
    inline T* constructAll(T* m, U u, V v, size_t elems)
     { for (size_t i = 0; i < elems; i++) new(m+i)T(u, v); return m; }
    
    template<typename T, typename U, typename V, typename W>
    inline T* construct(T* p, U u, V v, W w)
     { new(p)T(u, v, w); return p; }
    template<typename T, typename U, typename V, typename W>
    inline T* constructAll(T* m, U u, V v, W w, size_t elems)
     { for (size_t i = 0; i < elems; i++) new(m+i)T(u, v, w); return m; }
    
    template<typename T>
    void destruct(T* m)
     { m->~T(); }
    template<typename T>
    void destructAll(T* m, size_t elems)
     { for (size_t i = 0; i < elems; i++) i[m].~T(); }
    
    template<typename T>
    inline T* copyConstruct(T* dest, const T* src, size_t elems = 1)
     { for (size_t i = 0; i < elems; i++) new(dest+i)T(src[i]); return dest; }
    template<>
    inline char* copyConstruct<char>(char* dest, const char* src, size_t elems)
     { memcpy(dest, src, elems); return dest; }
    template<typename T>
    inline T* dupConstruct(const T* src, size_t elems = 1)
     { return copyConstruct(Memory::alloc<T>(elems), src, elems); }
    
    /**
     * Use this in place of the "=" operator when you would like a "smarter"
     * assignment operator.  This is especially useful if you have:
     *
     *  ExpensiveToCopyClass var;
     *
     *  while (...) {
     *    var = someFunction();
     *  }
     *
     * Compilers like G++ would create "someFunction" so that it takes in
     * a reference to unconstructed memory, rather than returning the
     * structure.  This function destructs the destination first so that
     * an unnecessary copy is completely avoided.
     */
    template<typename T>
    void assign(T& dest, const T& src)
     { if (&dest != &src) { dest.~T(); new(&dest)T(src); } }
    
    /**
     * This is the most conservative move method.
     * This assumes nothing about the underlying structure.  However, this
     * is inefficient for almost all data types, assuming one never keeps
     * foreign references to the data.
     */
    template<class T>
    struct ConstructorMove
    {
        static void move(T* dest, T* src)
         { new(dest)T(*src); src->~T(); }
        static void move(T* dest, T* src, size_t len)
         { while (len--) { move(dest, src); dest++; src++; } }
        static T* resize(T* src, size_t newlen, size_t preservelen)
         { T* dest = Memory::alloc<T>(newlen);
           move(dest, src, preservelen);
           Memory::free(src);
           return dest; }
    };
    
    template<typename T>
    struct Chunk
    {
        static Chunk* cast(T* obj)
         { return reinterpret_cast<Chunk*>(obj); }
        char a[sizeof(T)];
    };
    
    /**
     * This move method will work fine for any class that does not store
     * any pointers that point within its own structure.
     */
    template<class T>
    struct RelocatableMove
    {
        static void move(T* dest, T* src)
         { *Chunk<T>::cast(dest) = *Chunk<T>::cast(src); }
        static void move(T* dest, T* src, size_t len)
         { Memory::copy(dest, src, len); }
        static T* resize(T* src, size_t newlen, size_t preservelen)
         { return Memory::resize(src, newlen); }
    };
    
    /**
     * This move assumes a swap method.
     * This is good for STL-like things that provide a swap but no nice
     * relocate.  This is conservative in that it will work as long as it
     * compiles.
     */
    template<class T>
    struct SwapMove
    {
        static void move(T* dest, T* src)
         { new(dest)T(); dest->swap(*src); src->~T(); }
        static void move(T* dest, T* src, size_t len)
         { while (len--) { move(dest, src); dest++; src++; } }
        static T* resize(T* src, size_t newlen, size_t preservelen)
         { T* dest = Memory::alloc<T>(newlen);
           move(dest, src, preservelen);
           Memory::free(src);
           return dest; }
    };
    
    /**
     * A move method.
     */
    template<typename T>
    struct Move
    {
        /**
         * Reallocate source to newlen length, keeping preservelen elements.
         * May or may not move the pointer.
         * This does not manipulate any elements of the new or old array
         * except for the first preservelen elements, so you must call any
         * necessary destructors or default constructors.
         * @param src - the beginning of a malloc-allocated source array
         * @param newlen - the new length
         * @param preservelen - the number of elements to copy over
         * @return the new pointer, which may or may not be the same.
         */
        static T* resize(T* src, size_t newlen, size_t preservelen)
         { return ConstructorMove<T>::resize(src, newlen, preservelen); }
        /**
         * Move one element of src to dest.
         * At the end, this will construct dest (which is fresh memory) and
         * destruct dest.
         */
        static void move(T* dest, T* src)
         { ConstructorMove<T>::move(dest, src); }
        /**
         * Move len elements of src to dest.
         * At the end, all the len elements of src will be cleared, and
         * dest elements will be initialized.
         */
        static void move(T* dest, T* src, size_t len)
         { ConstructorMove<T>::move(dest, src, len); }
    };
    
    #ifndef DOXYGEN
    #define DEFINE_MOVE_STRATEGY_MEMORY_NS(t, s) \
        template<> struct Move< t > : public s< t > { };
    #else
    #define DEFINE_MOVE_STRATEGY_MEMORY_NS(t, s) /* avoid documentation */
    #endif
    
    #define RELOCATABLE_TYPE_MEMORY_NS(t) \
        DEFINE_MOVE_STRATEGY_MEMORY_NS(t, RelocatableMove)
    #define DEFINE_MOVE_STRATEGY(t, s) \
        namespace Memory { DEFINE_MOVE_STRATEGY_MEMORY_NS(t, s) };
    #define RELOCATABLE_TYPE(t) DEFINE_MOVE_STRATEGY(t, RelocatableMove)
    #define COPY_TYPE(t) DEFINE_MOVE_STRATEGY(t, ConstructorMove)
    
    RELOCATABLE_TYPE_MEMORY_NS(char);
    RELOCATABLE_TYPE_MEMORY_NS(unsigned char);
    RELOCATABLE_TYPE_MEMORY_NS(signed char);
    RELOCATABLE_TYPE_MEMORY_NS(int);
    RELOCATABLE_TYPE_MEMORY_NS(unsigned int);
    RELOCATABLE_TYPE_MEMORY_NS(short);
    RELOCATABLE_TYPE_MEMORY_NS(unsigned short);
    RELOCATABLE_TYPE_MEMORY_NS(long);
    RELOCATABLE_TYPE_MEMORY_NS(unsigned long);
    
    template<typename T>
    struct Move<T*> : public RelocatableMove<T*> { };
    
    /**
     * An auto pointer for C pointers.
     */
    template <typename T>
    class Ptr
    {
     public:
        Ptr() NOTHROW : ptr(0) { }
        Ptr(T *&inPtr) NOTHROW : ptr(inPtr) { inPtr = 0; }
        Ptr(T *inPtr) NOTHROW : ptr(inPtr) { }
        Ptr(Ptr &o) NOTHROW : ptr(o.ptr) { o.ptr = 0; }
        Ptr(size_t elems) : ptr(alloc<T>(elems)) { }
        Ptr(const T *orig, size_t elems) : ptr(dup(orig, elems)) { }
        ~Ptr() NOTHROW { if (ptr) ::free(ptr); }
        
        operator T* () const NOTHROW { return ptr; }
        T* operator -> () const { return ptr; }
        
        /* Note: DO NOT SELF ASSIGN! (I may fix this later) */
        Ptr& operator = (T *o) NOTHROW
         { ::free(ptr); ptr = o; return *this; }
        Ptr& operator = (Ptr& o) NOTHROW
         { ::free(ptr); ptr = o.ptr; o.ptr = 0; return *this; }
        
        void resize(size_t elems)
         { ptr = Memory::resize(ptr, elems); }
        void reallocBytes(size_t b)
         { ptr = Memory::reallocBytes(ptr, b); }
        
        T* get() const NOTHROW { return ptr; }
        T* release() NOTHROW { T *t = ptr; ptr = 0; return t; }
        void reset(T* p = 0) NOTHROW { free(ptr); ptr = p; }
        
        void swap(Ptr &o) NOTHROW
         { T *t = ptr; ptr = o.ptr; o.ptr = t; }
        
     private:
        Ptr(const Ptr& o) NOTHROW { }
        Ptr& operator = (const Ptr& o) { }
        
        T *ptr;
    };
    
    /**
     * Custom auto_ptr-like implementation that does destructors et al.
     */
    template <typename T>
    class Ref
    {
     public:
        explicit Ref(T *inRef = 0) NOTHROW : ptr(inRef) { }
        Ref(Ref &o) NOTHROW : ptr(o.ptr) { o.ptr = 0; }
        ~Ref() NOTHROW { delete ptr; }
        
        operator T* () const NOTHROW { return ptr; }
        T* operator -> () const { return ptr; }
        T& operator * () const { return *ptr; }
        
        /* DO NOT self assign! (I may fix this later) */
        Ref& operator = (T *o) NOTHROW
         { delete ptr; ptr = o; return *this; }
        Ref& operator = (Ref& o) NOTHROW
         { delete ptr; ptr = o.ptr; o.ptr = 0; return *this; }
        
        T* get() const NOTHROW { return ptr; }
        T* release() NOTHROW { T *t = ptr; ptr = 0; return t; }
        void reset(T* p = 0) NOTHROW { delete ptr; ptr = p; }
        
        void swap(Ref<T> &o) NOTHROW
         { T *t = ptr; ptr = o.ptr; o.ptr = t; }
        
     private:
        Ref(const Ref& o) NOTHROW { }
        Ref& operator = (const Ref& o) { }
        
        T *ptr;
    };
    
    template <typename T>
    struct Move<Ref<T> > : public RelocatableMove<Ref<T> > { }; 
    
    template <typename T>
    struct Move<Ptr<T> > : public RelocatableMove<Ptr<T> > { };
    
    template <typename T>
    void swap(T& a, T& b)
     { T t = a; a = b; b = t; }
    
    /**
     * Like a std::vector, except you can specify relocation methods, and
     * thus you could put something such as a Ref in it.
     *
     * It also provides a way to gain unconstructed memory for manual
     * construction, in case you have a particularly expensive copy
     * constructor, or even yet, a class such that copy construction makes
     * no sense (such as something like a file).
     */
    template <typename T>
    class Array
    {
     public:
        typedef T value_type;
        typedef T& reference_type;
        typedef const T& const_reference_type;
        typedef const T* const_iterator;
        typedef T* iterator;
        typedef Move<T> MoveMethod;
        
        struct SuggestedCapacity
        {
            SuggestedCapacity(int inVal)
             : val(inVal)
             { }
            
            int val;
        };
        
     public:
        Array()
         : ptr(0), len(0), cap(0)
         { }
        Array(size_t len)
         : len(len), cap(goodCapacity(len))
         { ptr = Memory::constructAll(Memory::alloc<T>(cap), len); }
        Array(size_t len, const SuggestedCapacity& suggestedCapacity)
         : len(len), cap(suggestedCapacity.val)
         { ptr = Memory::constructAll(Memory::alloc<T>(cap), len); }
        template<class U>
        Array(size_t len, U u)
         : len(len), cap(goodCapacity(len))
         { ptr = constructAll(Memory::alloc<T>(cap), u, cap); }
        template<class U, class V>
        Array(size_t len, U u, V v)
         : len(len), cap(goodCapacity(len))
         { ptr = constructAll(Memory::alloc<T>(cap), u, v, cap); }
        Array(const T* start, size_t len)
         : len(len), cap(goodCapacity(len))
         { ptr = Memory::alloc<T>(cap);
           Memory::copyConstruct(ptr, start, len); }
        Array(const Array& o)
         : len(o.len), cap(goodCapacity(o.len))
         { ptr = Memory::alloc<T>(cap);
           Memory::copyConstruct(ptr, o.ptr, len); }
        ~Array()
         { Memory::destructAll(ptr, len); Memory::free(ptr); }
        
        const Array& operator = (const Array& o)
         { if (this != &o) { Memory::destruct(this); new(this)Array(o); }
           return *this; }
        
        const_iterator begin() const
         { return ptr; }
        const_iterator end() const
         { return ptr + len; }
        iterator begin()
         { return ptr; }
        iterator end()
         { return ptr + len; }
        
        size_t size() const
         { return len; }
        size_t capacity() const
         { return cap; }
        
        template<class U>
        void resize(size_t newlen, U u)
         { setLen(newlen, u); }
        void resize(size_t newlen)
         { setLen(newlen); }
        
        void clear()
         { decLen(0); }
        
        template<class U>
        void push_back(U u)
         { growToAtLeast(len + 1); new(end()-1)T(u); }
        
        /**
         * Increases size by 1 and returns the new last element but does
         * not construct anything.
         *
         * You should call this in the following way:
         *  new(array.make_back())SomeClass(parameters);
         *
         * You should use this if you want to avoid any unnecessary copy
         * constructors.
         *
         * @return a pointer to an <b>unconstructed</b> element appended to
         * the end of the array
         */
        void* make_back()
         { growToAtLeast(len + 1); return end() - 1; }
        
        void pop_back()
         { Memory::destruct(ptr + (--len)); }
        void shrink(int inc)
         { Memory::destructAll(ptr + len - inc, inc);
           len -= inc; }
        
        const_reference_type back() const
         { return ptr[len-1]; }
        reference_type back()
         { return ptr[len-1]; }
        
        const_reference_type front() const
         { return ptr[0]; }
        reference_type front()
         { return ptr[0]; }
        
        void ensureCapacity(size_t newcap)
         { if (newcap > capacity()) incCapacity(newcap); }
        
        void trim()
         { decCapacity(len); }
        
        void swap(Array& o)
        {
            Memory::swap(ptr, o.ptr);
            Memory::swap(len, o.len);
            Memory::swap(cap, o.cap);
        }
        
        const_reference_type operator [] (ptrdiff_t i) const
         { return ptr[i]; }
        reference_type operator [] (ptrdiff_t i)
         { return ptr[i]; }
        
     private:
        void incLen(size_t newlen)
        {
            if (newlen > cap) incCapacity(newlen + len);
            Memory::constructAll(ptr + len, newlen - len);
            len = newlen;
        }
        template<typename U>
        void incLen(size_t newlen, U u)
        {
            if (newlen > cap) incCapacity(newlen + len);
            Memory::constructAll(ptr + len, u, newlen - len);
            len = newlen;
        }
        
        void decLen(size_t newlen)
        {
            Memory::destructAll(ptr + newlen, len - newlen);
            len = newlen;
        }
        
        void setLen(size_t newlen)
        {
            if (newlen > len) incLen(newlen);
            else decLen(newlen);
        }
        template<typename U>
        void setLen(size_t newlen, U u)
        {
            if (newlen > len) incLen(newlen, u);
            else decLen(newlen);
        }
        
        /* like incLen but does not construct anything */
        void growToAtLeast(size_t newlen)
        {
            if (newlen > cap) incCapacity(len + newlen);
            len = newlen;
        }
        
        void incCapacity(size_t newcap)
        {
            ptr = MoveMethod::resize(ptr, newcap, cap);
            cap = newcap;
        }
        
        void decCapacity(size_t newcap)
        {
            ptr = MoveMethod::resize(ptr, newcap, newcap);
            cap = newcap;
        }
        
        void setCapacity(size_t newcap)
        {
            ptr = MoveMethod::resize(ptr, newcap,
                cap < newcap ? cap : newcap);
            cap = newcap;
        }
        
        static size_t goodCapacity(size_t newlen)
        {
            if (sizeof(T) < sizeof(long))
                return (newlen + sizeof(long) - 1) & ~(sizeof(long) - 1);
            else
                return newlen;
        }
        
     private:
        T *ptr;
        size_t len;
        size_t cap;
    };
    
    template <typename T>
    struct Move<Array<T> > : public RelocatableMove<Array<T> > { };
    
    template<typename A>
    void swap(const Array<A>& a, const Array<A>& b)
     { a.swap(b); }
}

using Memory::Ptr;
using Memory::Ref;
using Memory::Array;

/* Although this is one of the things I don't really like (a class that is
 * both a string and a string buffer) I'm doing this anyway because it's
 * convenient.
 */
class String
{
 public:
    String()
     : mArray(1)
     { *mArray.begin() = 0; }
    String(int finalLength)
     : mArray(finalLength + 1)
     { }
    String(int finalLength, char c)
     : mArray(finalLength + 1, c)
     { terminate(); }
    String(const String& other)
     : mArray(other.mArray)
     { }
    String(const char *begin, size_t thestrlen)
     : mArray(begin, thestrlen + 1)
     { terminate(); }
    String(const char *begin, const char *onepastend)
     : mArray(begin, onepastend - begin + 1)
     { terminate(); }
    String(const char *begin)
     : mArray(begin, strlen(begin) + 1)
     { terminate(); }
    String(const char *a, size_t alen, const char *b, size_t blen)
     : mArray(alen + blen + 1)
    {
        memcpy(mArray.begin(), a, alen);
        memcpy(mArray.begin() + alen, b, blen);
        terminate();
    }
    
    operator const char * () const
     { return mArray.begin(); }
    
    const char* c_str() const
     { return mArray.begin(); }
    
    size_t length() const
     { return mArray.size() - 1; }
    void setLength(size_t newlen)
     { mArray.resize(newlen + 1); terminate(); }
    void fixLength()
     { mArray.resize(strlen(mArray.begin()) + 1); }
    
    void ensureCapacity(size_t capacity)
     { mArray.ensureCapacity(capacity); }
    void trimCapacity()
     { mArray.trim(); }
    
    char* begin()
     { return mArray.begin(); }
    const char* begin() const   
     { return mArray.begin(); }
    
    bool startsWith(const char *s) const;
    bool startsWith(const char *s, size_t l) const;
    bool endsWith(const char *s) const;
    bool endsWith(const char *s, size_t l) const;
    
    /* TODO: Evil casting... */
    char* find(char c) const
     { return strchr(mArray.begin(), c); }
    char* findr(char c) const
     { return strrchr(mArray.begin(), c); }
    char* find(const char* s) const 
     { return strstr(mArray.begin(), s); }
    
    String padLeft(char padChar, size_t len) const;
    String padCenter(char padChar, size_t len) const;
    String padRight(char padChar, size_t len) const;
    
    String trimLeft() const;
    String trimRight() const;
    String trim() const;
    
    void replace(char findchar, char replacechar);
    
    void backspace(int i = 1)
     { setLength(length() - i); }
    
    void append(const char *addStr, size_t addLength)
    {
        size_t mysize = mArray.size();
        mArray.resize(mysize + addLength);
        strcpy(mArray.begin() + mysize - 1, addStr);
    }
    
    void shrink(int inc)
    {
        mArray.shrink(inc);
    }
    
    void assign(const char *ptr, int newLength)
    {
        mArray.resize(newLength + 1);
        Memory::copy(mArray.begin(), ptr, newLength);
        mArray.begin()[newLength] = '\0';
    }

    void assignTerminated(const char *ptr, int newSize)
    {
        mArray.resize(newSize);
        Memory::copy(mArray.begin(), ptr, newSize);
    }
    
    bool empty() const
     { return mArray.size() == 1; }
    
    const String& operator += (const String& o)
    {
        size_t mylen = length();
        size_t addsize = o.mArray.size();
        mArray.resize(mylen + addsize);
        strcpy(mArray.begin() + mylen, o.mArray.begin());
        return *this;
    }
    
    const String& operator += (char c)
    {
        size_t myoldsize = mArray.size();
        mArray.resize(myoldsize + 1);
        mArray[myoldsize - 1] = c;
        mArray[myoldsize] = '\0';
        return *this;
    }
    
    const String& operator += (const char *s)
    {
        append(s, strlen(s));
        return *this;
    }
    
    const String& operator = (const char *s)
    {
        assignTerminated(s, strlen(s) + 1);
        return *this;
    }
    
    const String& operator = (const String& other)
    {
        assignTerminated(other.mArray.begin(), other.mArray.size());
        return *this;
    }
    
    String substring(int start, int len) const
     { return String(mArray.begin() + start, len); }
    String substring(int start) const
     { return String(mArray.begin() + start, length() - start); }
    String left(int count) const
     { return String(mArray.begin(), count); }
    String right(int count) const
     { return String(mArray.begin() + length() - count, count); }
    
    /**
     * Inclusive substring operator, like in many scripting languages.
     */
    String operator () (int startinc, int endinc) const
     { return String(mArray.begin() + wrapIndex(startinc),
                     mArray.begin() + wrapIndex(endinc) + 1); }
    String operator () (const char *startinc, int endinc) const
     { return String(startinc, mArray.begin() + wrapIndex(endinc) + 1); }
    String operator () (int startinc, const char *endinc) const
     { return String(mArray.begin() + wrapIndex(startinc), endinc + 1); }
    char operator () (int wrappedIndex) const
     { return mArray[wrapIndex(wrappedIndex)]; }
    char operator [] (int index) const
     { return mArray.begin()[index]; }
    char& operator [] (int index)
     { return mArray.begin()[index]; }
    
    int wrapIndex(int i) const
     { return i < 0 ? length() + i : i; }
    
    String toLower() const;
    String toUpper() const;
    
    int compareTo(const String& other) const
     { return strcmp(mArray.begin(), other.mArray.begin()); }
    int compareTo(const char* s) const
     { return strcmp(mArray.begin(), s); }
    
    FILL_COMPARATORS_FROM_EXP(
        const String& a, const String& b, a.compareTo(b));
    FILL_COMPARATORS_FROM_EXP(
        const String& a, const char* b, a.compareTo(b));
    FILL_COMPARATORS_FROM_EXP(
        const char* a, const String& b, -b.compareTo(a));
    
    friend String operator + (const String& a, const String& b)
     { return String(a.mArray.begin(), a.length(), b.mArray.begin(), b.length()); }
    friend String operator + (const char* a, const String& b)
     { return String(a, strlen(a), b.mArray.begin(), b.length()); }
    friend String operator + (const String& a, const char* b)
     { return String(a.mArray.begin(), a.length(), b, strlen(b)); }
    
    void terminate()
     { mArray.end()[-1] = 0; }
    
 protected:
    Array<char> mArray;
};


template<typename T>
class TMatrix
{
 public:
    TMatrix(size_t nr, size_t nc)
     : m(Memory::constructAll(Memory::alloc<T>(nr * nc), nr * nc)),
       nr(nr), nc(nc)
    { }
    template<typename U>
    TMatrix(size_t nr, size_t nc, const U& val)
     : m(Memory::constructAll(Memory::alloc<T>(nr * nc), val, nr * nc)),
       nr(nr), nc(nc)
    { }
    TMatrix(size_t nr, size_t nc, T* handoff)
     : m(handoff), nr(nr), nc(nc)
    { }
    TMatrix(const TMatrix& o)
     : m(Memory::dupConstruct(o.m, o.nr * o.nc)), nr(o.nr), nc(o.nc)
    { }
    ~TMatrix()
    {
        Memory::destructAll(m, nr * nc);
        Memory::free(m);
    }
    
    size_t nRow() const { return nr; }
    size_t nCol() const { return nc; }
    size_t rowLen() const { return nc * sizeof(T); }
    
    const TMatrix& operator = (const TMatrix& o)
    {
        if (o.nr == nr && o.nc == nc)
        {
            for (size_t i = nr * nc; i--;) m[i] = o.m[i];
        }
        else
        {
            Memory::assign(*this, o);
        }
    }
    
    const TMatrix& operator = (const T& o)
    {
        for (size_t i = nr * nc; i--;) m[i] = o;
    }
    
    /*
    This is, in general, a bad idea.
    It's nice to know that your values aren't going to be copied around
    once they hit the matrix.
    void resize(size_t nr2, size_t nc2)
    {
        if (nr2 != nr || nc2 != nc)
        {
            size_t ncmin = nc < nc2 ? nc : nc2;
            T* m2 = Memory::alloc<T>(nr2 * nc2);
            for (size_t r = 0; r < nr2; r++)
            {
                T* p = m + r * nc;
                T* p2 = m2 + r * nc2;
                size_t c = 0;
                
                if (r < nr) for (c = 0; c < ncmin; c++) new(p2+c)T(p[c]);
                for (; c < nc; c++) new(p2+c)T();
            }
            Memory::destruct(this);
            m = m2;
            nr = nr2;
            nc = nc2;
        }
    }
    */
    
    const T& operator () (int i, int j)
     { return m[nc*i + j]; }
    
    const T* operator[] (ptrdiff_t i) const { return m + (nc * i); }
    T* operator[] (ptrdiff_t i) { return m + (nc * i); }
    
 protected:
    T* m;
    size_t nr;
    size_t nc;
};


namespace Bits
{
    template<typename TWord>
    inline TWord mask(int bits)
     { return (TWord(2) << (bits - 1)) - 1; }
    template<typename TWord>
    inline TWord truncate(TWord value, int bits)
     { return value & mask<TWord>(bits); }
    template<typename TWord>
    inline TWord extract(TWord value, int shift, int bits = 1)
     { return truncate(value >> shift, bits); }
    template<typename TWord>
    inline TWord signMask(int bits)
     { return TWord(1) << (bits - 1); }
    template<typename TWord>
    inline TWord extend(TWord value, int oldWidth)
     { return (~((value & (1 << (oldWidth - 1))) - 1)) | value; }
    template<typename TWord>
    inline TWord extend(TWord value, int oldWidth, int newWidth)
     { return truncate(extend(value, oldWidth), newWidth); }
    template<typename TWord>
    inline TWord extractSigned(TWord value, int shift, int bits)
     { return extend(extract(value, shift, bits), bits); }
    template<typename TWord>
    inline TWord extractSigned(TWord value, int shift, int bits, int totalWidth)
     { return extend(extract(value, shift, bits), bits, totalWidth); }
    template<typename TWord>
    /* these signed overflow detectors are somewhat forgiving... */
    inline TWord hasSignedOverflow(TWord value, int srcBits, int destBits)
     { TWord mask = (~Bits::mask<TWord>(srcBits - 1));
       TWord extended = Bits::mask<TWord>(destBits) & mask;
       TWord masked = value & mask;
       return (masked != 0 && masked != mask && masked != extended); }
    template<typename TWord>
    inline TWord hasSignedOverflow(TWord value, int srcBits)
     { TWord unsignedMask = ~Bits::mask<TWord>(srcBits);
       TWord signedMask = ~Bits::mask<TWord>(srcBits - 1);
       return (value & unsignedMask) != 0
           && (value & signedMask) != signedMask; }
    template<typename TWord>
    inline TWord hasUnsignedOverflow(TWord value, int bits)
     { return (value & ~Bits::mask<TWord>(bits)) != 0; }

};

/*
class UTime
{
 public:
    UTime() { gettimeofday(&tv, NULL); }
    UTime(long sec, long usec) { tv.tv_sec = sec; tv.tv_usec = usec; }
    
    friend long operator - (const UTime& a, const UTime& b)
    {
        return (a.tv.tv_sec - b.tv.tv_sec) * 1000000
             + (a.tv.tv_usec - b.tv.tv_usec);
    }
    
    friend UTime operator + (const UTime& a, long msec)
    {
        long usec = a.tv.tv_usec + msec;
        long sec = a.tv.tv_sec + (usec % 1000000);
        return UTime(sec, usec);
    }
    
    long getSec() const { return tv.tv_sec; }
    long getMicros() const { return tv.tv_usec; }
    long getMillis() const { return tv.tv_usec / 1000; }
    
 private:
    struct timeval tv;
};
*/

template<typename T>
class AbstractList
{
 public:
    virtual const T& at(int i) const = 0;
    virtual size_t size() const = 0;
    const T& operator [] (int i) const
     { return at(i); }
};

template<typename T, typename TPtrToIndexable>
class ListImpl : public AbstractList<T>
{
 public:
    ListImpl(TPtrToIndexable backing)
     : backing(backing)
     { }
    
    const T& at(int i) const
     { return (*backing)[i]; }
    size_t size() const
     { return (*backing).size(); }
    
 private:
    TPtrToIndexable backing;
};

template <typename T>
T max(T a, T b)
 { return a > b ? a : b; }
template <typename T>
T min(T a, T b)
{ return a < b ? a : b; }

#endif /*__cplusplus*/

typedef int SVal;
typedef unsigned int GVal;
typedef unsigned int GByte;

typedef GVal GWord;
typedef GVal GInstr;
typedef GVal GReg;
typedef GVal GAddr;

typedef enum
{
    SuccessPass = 3, SuccessWarn = 2, SuccessFail = 1
} SuccessT;

#define Failed(suc) ((suc) == SuccessFail)
#define Passed(suc) ((suc) == SuccessPass)

#ifndef ASSERT
#ifdef DEBUG
#define ASSERT(x) \
    ((x) || (fprintf(stderr, "Assertion %s failed (%s:%d/%s)\n", #x, \
                   __FILE__, __LINE__, __func__), \
             fflush(stderr), \
             abort(), 0))
#else
#define ASSERT(x) (void)0
#endif
#endif

#define HERE (printf("%d\n", __LINE__), fflush(stdout));

#endif/*COMMON_H_INCLUDED*/

