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

#ifndef RESEMBLA_UNIFORM_COST_HPP
#define RESEMBLA_UNIFORM_COST_HPP

namespace resembla {

struct UniformCost
{
    template<typename value_type>
    double operator()(const value_type a, const value_type b) const
    {
        return a == b ? 0.0 : 1.0;
    }
};

}
#endif
