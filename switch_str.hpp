// switch_str
// C++17 O(1) switch for strings.
//
// Benefits and features:
// * O(1) cases matching.
// * Faster than `if-elseif-elseif-else`.
// * `break`, `continue`, `return`, `[[fallthrough]]` work as expected.
// * Compile-time errors for repetitive and unlisted cases.
//
// Example of usage:
// ```cpp
// #include <switch_str.hpp>
//
// void test(const std::string_view& value) {
//     switch_str(value,
//         "ERR", "MSH", "OBR", "PID") // No `{` !
//     case_str("ERR"):
//         ...
//         break;
//     case_str("MSH"):
//         ...
//         break;
//     case_str("PID"):
//         ...
//         break;
//     case_str("PID"): // Duplication -> compile-time error.
//         ...
//         break;
//     case_str("PV1"): // Not listed -> compile-time error.
//         ...
//         break;
//     default:
//         ...
//         break;
//     }
// }
// ```
//
// Author: Yurii Blok
// License: BSL-1.0
// https://github.com/yurablok/switch-str
// History:
// v1.0 05-Apr-21   Initial stable.

#pragma once
#include <string_view>
#include <unordered_map>

#ifndef switch_str

#define switch_str(STR, ...) \
    switch(constexpr std::string_view switch_cases[] = { __VA_ARGS__ }; \
        [](const std::string_view& str, const std::string_view cases[], const uint32_t cases_size) -> uint32_t { \
            static const auto m = [](const std::string_view cases[], const uint32_t cases_size) { \
                std::unordered_map<std::string_view, uint32_t> mm; \
                mm.reserve(cases_size); \
                for (uint32_t i = 0; i < cases_size; ++i) { \
                    mm[cases[i]] = i + 1; \
                } \
                return std::move(mm); \
            }(cases, cases_size); \
            const auto it = m.find(str); \
            if (it == m.end()) { \
                return cases_size + 1; \
            } \
            return it->second; \
        }(STR, switch_cases, sizeof(switch_cases) / sizeof(switch_cases[0]))) { \
    case 0: break;

#define case_str(STR) \
    case [](const std::string_view& str, const std::string_view cases[], \
            const uint32_t cases_size) -> uint32_t { \
        for (uint32_t i = 0; i < cases_size; ++i) { \
            if (cases[i] == str) { \
                return i + 1; \
            } \
        } \
        return 0; \
    }(STR, switch_cases, sizeof(switch_cases) / sizeof(switch_cases[0]))

#endif // switch_str
