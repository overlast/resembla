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

#include "flag_feature_aggregator.hpp"

#include <algorithm>
#include <iostream>

namespace resembla {

Feature::real_type FlagFeatureAggregatorImpl::operator()(Feature::real_type a, Feature::real_type b) const
{
    auto c = (std::abs(a) + std::abs(b) - 3.0 * std::abs(a - b)) / 2.0;
#ifdef DEBUG
    std::cerr << "flag-type feature: a=" << a << ", b=" << b << ", c=" << c << std::endl;
#endif
    return std::min(std::max(c, -1.0), 1.0);
}

}
