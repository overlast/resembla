/*
Resembla: Word-based Japanese similar sentence search library
https://github.com/tuem/resembla

Copyright 2017 Takashi Uemura

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef RESEMBLA_STRING_UTIL_HPP
#define RESEMBLA_STRING_UTIL_HPP

#include <string>
#include <vector>

namespace resembla {

using string_type = std::wstring;

// common initialization procedures for using wchar_t
void init_locale();

template<typename src_type, typename dest_type>
void cast_string(const src_type& src, dest_type& dest);

template<typename src_type>
void cast_string(const src_type& src, src_type& dest)
{
    dest = src;
}

template<typename dest_type, typename src_type>
dest_type cast_string(const src_type& src)
{
    dest_type desc;
    cast_string(src, desc);
    return desc;
}

// TODO: use constexpr
template<typename char_type = char>
constexpr char_type column_delimiter();
// TODO: implement by a generic template function like this:
//    return cast_string<string_type>(std::string(1, '\t'))[0];

template<>
constexpr char column_delimiter()
{
    return '\t';
}

template<>
constexpr wchar_t column_delimiter()
{
    return L'\t';
}

template<typename char_type = char>
constexpr char_type feature_delimiter();

template<>
constexpr char feature_delimiter()
{
    return '&';
}

template<>
constexpr wchar_t feature_delimiter()
{
    return L'&';
}

template<typename char_type = char>
constexpr char_type keyvalue_delimiter();

template<>
constexpr char keyvalue_delimiter()
{
    return '=';
}

template<>
constexpr wchar_t keyvalue_delimiter()
{
    return L'=';
}

template<typename char_type = char>
constexpr char_type value_delimiter();

template<>
constexpr char value_delimiter()
{
    return ',';
}

template<>
constexpr wchar_t value_delimiter()
{
    return L',';
}

// split text by delimiter
template<typename string_type>
std::vector<string_type> split(const string_type& text, const typename string_type::value_type delimiter)
{
    std::vector<string_type> result;
    bool finished = false;
    for(size_t start = 0, end; start < text.length();){
        end = text.find(delimiter, start);
        if(end == string_type::npos){
            end = text.length();
            finished = true;
        }
        result.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    if(!finished){
        result.push_back(string_type());
    }
    return result;
}

}
#endif
