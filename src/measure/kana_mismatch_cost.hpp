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

#ifndef RESEMBLA_KANA_MISMATCH_COST_HPP
#define RESEMBLA_KANA_MISMATCH_COST_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "../string_util.hpp"

namespace resembla {

template<typename string_type>
struct KanaMismatchCost
{
    using value_type = typename string_type::value_type;

    KanaMismatchCost(const std::string& letter_similarity_file_path)
    {
        if(letter_similarity_file_path.empty()){
            return;
        }

        std::basic_ifstream<value_type> ifs(letter_similarity_file_path);
        if(ifs.fail()){
            throw std::runtime_error("input file is not available: " + letter_similarity_file_path);
        }

        while(ifs.good()){
            string_type line;
            std::getline(ifs, line);
            if(ifs.eof() || line.length() == 0){
                break;
            }

            auto columns = split(line, column_delimiter<value_type>());
            if(columns.size() < 2){
                throw std::runtime_error("invalid line in " + letter_similarity_file_path + ": " + cast_string<std::string>(line));
            }
            auto letters= columns[0];
            auto cost = std::stod(columns[1]);

            std::sort(std::begin(letters), std::end(letters));
            for(size_t i = 0; i < letters.size() - 1; ++i){
                string_type s(1, letters[i]);
                for(size_t j = i + 1; j < letters.size(); ++j){
                    string_type p = s + letters[j];
                    letter_similarities[p] = cost;
                }
            }
        }
    }

    double operator()(const value_type a, const value_type b) const
    {
        if(a == b){
            return 0.0;
        }

        auto p = letter_similarities.find(a < b ? string_type({a, b}) : string_type({b, a}));
        if(p != std::end(letter_similarities)){
            return p->second;
        }

        return 1.0;
    }

protected:
    std::unordered_map<string_type, double> letter_similarities;
};

}
#endif
