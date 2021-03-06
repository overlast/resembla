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

#include "regex_feature_extractor.hpp"

#include <fstream>
#include <iostream>

namespace resembla {

RegexFeatureExtractor::RegexFeatureExtractor(const std::initializer_list<std::pair<Feature::real_type, std::string>>& patterns)
{
    construct(patterns);
}

RegexFeatureExtractor::RegexFeatureExtractor(const std::string file_path)
{
    construct(load(file_path));
}

Feature::real_type RegexFeatureExtractor::match(const string_type& text) const
{
    for(const auto& i: re_all){
        if(std::regex_match(text, i.second)){
#ifdef DEBUG
            std::cerr << "regex detector: evidence found, text=" << cast_string<std::string>(text) << ", score=" << i.first << std::endl;
#endif
            return i.first;
        }
    }
    return 0.0;
}

std::vector<std::pair<double, std::string>> RegexFeatureExtractor::load(const std::string file_path)
{
    std::ifstream ifs(file_path);
    if(ifs.fail()){
        throw std::runtime_error("input file is not available: " + file_path);
    }

    std::vector<std::pair<double, std::string>> patterns;
    while(ifs.good()){
        std::string line;
        std::getline(ifs, line);
        if(ifs.eof() || line.length() == 0){
            break;
        }
        size_t i = line.find(column_delimiter<>());
        if(i != std::string::npos){
            patterns.push_back(std::make_pair(std::stod(line.substr(0, i)), line.substr(i + 1)));
        }
    }
    return patterns;
}

Feature::text_type RegexFeatureExtractor::operator()(const string_type& text) const
{
    return Feature::toText(match(text));
}

}
