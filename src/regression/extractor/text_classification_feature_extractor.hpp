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

#ifndef RESEMBLA_TEXT_CLASSIFICATION_FEATURE_EXTRACTOR_HPP
#define RESEMBLA_TEXT_CLASSIFICATION_FEATURE_EXTRACTOR_HPP

#include <string>
#include <unordered_map>
#include <mutex>

#include <libsvm/svm.h>

#include "feature_extractor.hpp"

namespace resembla {

struct TextClassificationFeatureExtractor: public FeatureExtractor::Function
{
    TextClassificationFeatureExtractor(const std::string& file_path);

    Feature::text_type operator()(const string_type& text) const;

protected:
    using std::unordered_map<string_type, id_type> = word_id_map;
    word_id_map word_ids;

    svm_model *model;
    mutable std::mutex mutex_model;

    std::vector<svm_node> toNodes(const input_type& x) const;
};

}
#endif
