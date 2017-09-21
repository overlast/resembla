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

#ifndef RESEMBLA_BPED_HPP
#define RESEMBLA_BPED_HPP

#include <string>
#include <vector>
#include <map>

template<typename string_type>
struct bped
{
    using output_type = int;

    bped(string_type const& pattern = string_type())
    {
        init(pattern);
    }

    void init(string_type const& pattern)
    {
        this->pattern = pattern;
        if(pattern.empty()){
            return;
        }
        m = pattern.size();
        block_size = ((m - 1) >> bit_offset<bitvector_type>()) + 1;
        rest_bits = m - (block_size - 1) * bit_width<bitvector_type>();
        sink = bitvector_type{1} << (rest_bits - 1);

        if(block_size == 1){
            preprocess_sp();
        }
        else{
            work.resize(block_size);
            preprocess_lp();
        }
        zeroes.resize(block_size, 0);
    }

    output_type operator()(string_type const &text) const
    {
        if(text.empty()){
            return m;
        }
        else if(m == 0){
            return text.size();
        }

        if(block_size == 1){
            return edit_distance_sp(text);
        }
        else{
            return edit_distance_lp(text);
        }
    }

protected:
    using symbol_type = typename string_type::value_type;
    using bitvector_type = uint64_t;

    string_type pattern;
    size_t m;
    bitvector_type sink;
    size_t block_size;
    size_t rest_bits;

    struct WorkData
    {
        bitvector_type D0;
        bitvector_type HP;
        bitvector_type HN;
        bitvector_type VP;
        bitvector_type VN;

        void reset()
        {
            D0 = HP = HN = VN = 0;
            VP = ~(bitvector_type{0});
        }
    };
    mutable std::vector<WorkData> work;

    std::vector<std::pair<symbol_type, bitvector_type>> PM;
    std::vector<std::pair<symbol_type, std::vector<bitvector_type>>> PMs;
    std::vector<bitvector_type> zeroes;

    template<typename Integer> static constexpr int bit_width()
    {
        return 8 * sizeof(Integer);
    }

    static constexpr int bit_offset(int w)
    {
        return w < 2 ? 0 : (bit_offset(w >> 1) + 1);
    }

    template<typename Integer> static constexpr int bit_offset()
    {
        return bit_offset(bit_width<Integer>());
    }

    template<typename key_type, typename value_type>
    const value_type& find_value(const std::vector<std::pair<key_type, value_type>>& data,
            const key_type c, const value_type& default_value) const
    {
        if(data.size() < 9){
            for(const auto& p: data){
                if(p.first == c){
                    return p.second;
                }
            }
        }
        else{
            size_t l = 0, r = data.size();
            while(l < r){
                auto i = (l + r) / 2;
                const auto& p = data[i];
                if(p.first < c){
                    l = i + 1;
                }
                else if(p.first > c){
                    r = i;
                }
                else{
                    return p.second;
                }
            }
        }
        return default_value;
    }

    void preprocess_sp()
    {
        std::map<symbol_type, bitvector_type> pm;
        for(size_t i = 0; i < pattern.size(); ++i){
            auto j = pm.find(pattern[i]);
            if(j == pm.end()){
                pm[pattern[i]] = bitvector_type{1} << i;
            }
            else{
                j->second |= bitvector_type{1} << i;
            }
        }

        PM.clear();
        for(const auto& p: pm){
            PM.push_back(p);
        }
    }

    void preprocess_lp()
    {
        std::map<symbol_type, std::vector<bitvector_type>> pm;
        for(size_t i = 0; i < block_size - 1; ++i){
            for(size_t j = 0; j < bit_width<bitvector_type>(); ++j){
                if(pm[pattern[i * bit_width<bitvector_type>() + j]].empty()){
                    pm[pattern[i * bit_width<bitvector_type>() + j]].resize(block_size, 0);
                }
                pm[pattern[i * bit_width<bitvector_type>() + j]][i] |= bitvector_type{1} << j;
            }
        }
        for(size_t i = 0; i < rest_bits; ++i){
            if(pm[pattern[(block_size - 1) * bit_width<bitvector_type>() + i]].empty()){
                pm[pattern[(block_size - 1) * bit_width<bitvector_type>() + i]].resize(block_size, 0);
            }
            pm[pattern[(block_size - 1) * bit_width<bitvector_type>() + i]].back() |= bitvector_type{1} << i;
        }

        PMs.clear();
        for(const auto& p: pm){
            PMs.push_back(p);
        }
    }

    output_type edit_distance_sp(string_type const &text) const
    {
        bitvector_type D0, HP, HN, VP = 0, VN = 0;
        for(size_t i = 0; i < m; ++i){
            VP |= bitvector_type{1} << i;
        }

        output_type diff = m;
        for(auto c: text){
            auto X = find_value(PM, c, bitvector_type{0}) | VN;

            D0 = ((VP + (X & VP)) ^ VP) | X;
            HP = VN | ~(VP | D0);
            HN = VP & D0;

            X = (HP << 1) | 1;
            VP = (HN << 1) | ~(X | D0);
            VN = X & D0;

            if(HP & sink){
                ++diff;
            }
            else if(HN & sink){
                --diff;
            }
        }
        return diff;
    }

    output_type edit_distance_lp(string_type const &text) const
    {
        constexpr bitvector_type msb = bitvector_type{1} << (bit_width<bitvector_type>() - 1);
        for(size_t i = 0; i < block_size; ++i){
            work[i].reset();
        }
        for(size_t i = 0; i < rest_bits; ++i){
            work.back().VP |= bitvector_type{1} << i;
        }

        output_type diff = m;
        for(auto c: text){
            const auto& PM = find_value(PMs, c, zeroes);

            for(size_t r = 0; r < block_size; ++r){
                auto& w = work[r];
                auto X = PM[r];
                if(r > 0 && (work[r - 1].HN & msb)){
                    X |= 1;
                }

                w.D0 = ((w.VP + (X & w.VP)) ^ w.VP) | X | w.VN;
                w.HP = w.VN | ~(w.VP | w.D0);
                w.HN = w.VP & w.D0;

                X = w.HP << 1;
                if(r == 0 || work[r - 1].HP & msb){
                    X |= 1;
                }
                w.VP = (w.HN << 1) | ~(X | w.D0);
                if(r > 0 && (work[r - 1].HN & msb)){
                    w.VP |= 1;
                }
                w.VN = X & w.D0;
            }

            if(work.back().HP & sink){
                ++diff;
            }
            else if(work.back().HN & sink){
                --diff;
            }
        }
        return diff;
    }
};
#endif
