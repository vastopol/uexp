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
#include "common.h"
#include <ctype.h>

bool String::startsWith(const char *s) const
 { return startsWith(s, strlen(s)); }
bool String::startsWith(const char *s, size_t l) const
 { return memcmp(mArray.begin(), s, l) == 0; }
bool String::endsWith(const char *s) const
 { return endsWith(s, strlen(s)); }
bool String::endsWith(const char *s, size_t l) const
 { return mArray.size() > l && memcmp(mArray.begin() + length() - l, s, l) == 0; }

String String::toLower() const
{
    String newstring(*this);
    
    for (char *s = newstring.mArray.begin(); *s; s++)
    {
        if (*s >= 'A' && *s <= 'Z')
            *s |= 0x20;
    }
    
    return newstring;
}

String String::toUpper() const
{
    String newstring(*this);
    
    for (char *s = newstring.mArray.begin(); *s; s++)
    {
        if (*s >= 'a' && *s <= 'z')
            *s &= ~0x20;
    }
    
    return newstring;
}

String String::padLeft(char padChar, size_t len) const
{
    size_t mylen = length();
    
    if (len < mylen)
    {
        return *this;
    }
    else
    {
        String result(len);
        memset(result.begin(), padChar, len - mylen);
        memcpy(result.begin() + len - mylen, this->begin(), mylen + 1);
        return result;
    }
}

String String::padRight(char padChar, size_t len) const
{
    size_t mylen = length();
    
    if (len < mylen)
    {
        return *this;
    }
    else
    {
        String result(len);
        memcpy(result.begin(), this->begin(), mylen);
        memset(result.begin() + mylen, padChar, len - mylen);
        result.terminate();
        return result;
    }
}

String String::padCenter(char padChar, size_t len) const
{
    size_t mylen = length();
    
    if (len < mylen)
    {
        return *this;
    }
    else
    {
        String result(len);
        int startlen = (len - mylen) / 2;
        int endlen = (len - mylen + 1) / 2;
        char *pos = result.begin();
        memset(pos, ' ', startlen);
        pos += startlen;
        memcpy(pos, this->begin(), mylen);
        pos += mylen;
        memset(pos, ' ', endlen);
        result.terminate();
        return result;
    }
}

String String::trimLeft() const
{
    const char *s = begin();
    while (isspace(*s)) s++;
    return String(s, mArray.end());
}

String String::trimRight() const
{
    const char *s = mArray.end() - 2;
    const char *b = begin();
    while (s >= b && isspace(*s)) s--;
    return String(begin(), s);
}

String String::trim() const
{
    const char *e = mArray.end() - 2;
    const char *b = begin();
    while (isspace(*b)) b++;
    while (e >= b && isspace(*e)) e--;
    return String(b, e + 1);
}

void String::replace(char findchar, char replacechar)
{
    char* pos = mArray.begin();
    
    while (*pos)
    {
        if (*pos == findchar) *pos = replacechar;
    }
}

Array<char> common_cpp_useless()
{
    Array<char> someArray;
    printf("end of construction\n");
    someArray.resize(50);
    printf("end of resize(50)\n");
    someArray.resize(90);
    printf("end of resize(90)\n");
    someArray.resize(20);
    printf("end of resize(20)\n");
    return someArray;
}

char* common_cpp_useless2()
{
    static char array[87];
    Memory::constructAll(array, 85);
    return array;
}

