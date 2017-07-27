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

#ifndef __KEYWORD_MATCHER_HPP__
#define __KEYWORD_MATCHER_HPP__

#include "keyword_match_preprocessor.hpp"

#include <iostream>

namespace resembla {

template<typename string_type>
struct KeywordMatcher
{
public:
    const std::string name;

    KeywordMatcher(const std::string name): name(name){}

    double operator()(const typename KeywordMatchPreprocessor<string_type>::output_type& target,
            const typename KeywordMatchPreprocessor<string_type>::output_type& reference) const
    {
        // TODO: normalize target text
        // TODO: use synonyms
        if(reference.keywords.empty()){
            return 0.0;
        }
        double score = 0.0;
        for(const auto& keyword: reference.keywords){
            // TODO: approximate match
            if(target.text.find(keyword) != string_type::npos){
#ifdef DEBUG
                std::cerr << "matced: keyword=" << cast_string<std::string>(keyword) << ", target=" << cast_string<std::string>(target.text) << ", reference=" << cast_string<std::string>(reference.text) << std::endl;
#endif
                score += 1.0;
            }
            else{
                std::cerr << "not matced: keyword=" << cast_string<std::string>(keyword) << ", target=" << cast_string<std::string>(target.text) << ", reference=" << cast_string<std::string>(reference.text) << std::endl;
                score -= 1.0;
            }
        }
#ifdef DEBUG
        std::cerr << "keyword match score=" << score / reference.keywords.size() << std::endl;
#endif
        return score / reference.keywords.size();
    }
};

}
#endif