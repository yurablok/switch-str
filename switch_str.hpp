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
//         "ERR", "MSH", "OBR", "PID") {
//     case_str("ERR"):
//         ...
//         break;
//     case_str("MSH"):
//         static_assert(switch_str_meta::cases().size() == 4);
//         static_assert(switch_str_meta::cases()[1] == "MSH");
//         break;
//     case_str("PID"):
//         ...
//         break;
//     case_str("PID"): // Repetition -> compile-time error.
//         ...
//         break;
//     case_str("PV1"): // Unlisted -> compile-time error.
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
// v1.1 11-Apr-21   Check for unlisted is now implemented on `static_assert` instead of `case 0`.
//                  Added `switch_str_meta`.
// v1.0 05-Apr-21   Initial stable.

#pragma once
#include <array>
#include <string_view>
#include <unordered_map>

#ifndef switch_str

#define switch_str(STR, ...) \
    switch ( \
        struct switch_str_meta { \
            static constexpr std::array<std::string_view, \
                    std::initializer_list<const char*>({ __VA_ARGS__ }).size()> cases() { \
                return {{ __VA_ARGS__ }}; \
            } \
        }; \
        [](const std::string_view& str) -> uint32_t { \
            static const auto m = []() { \
                std::unordered_map<std::string_view, uint32_t> mm; \
                mm.reserve(switch_str_meta::cases().size()); \
                for (uint32_t i = 0; i < static_cast<uint32_t>(switch_str_meta::cases().size()); ++i) { \
                    mm[switch_str_meta::cases()[i]] = i; \
                } \
                return mm; \
            }(); \
            const auto it = m.find(str); \
            if (it == m.end()) { \
                return static_cast<uint32_t>(switch_str_meta::cases().size()); \
            } \
            return it->second; \
        }(STR) \
    )

#define case_str(STR) \
    case []() constexpr -> uint32_t { \
        constexpr uint32_t idx = []() constexpr -> uint32_t { \
            for (uint32_t i = 0; i < static_cast<uint32_t>(switch_str_meta::cases().size()); ++i) { \
                if (switch_str_meta::cases()[i] == STR) { \
                    return i; \
                } \
            } \
            return static_cast<uint32_t>(switch_str_meta::cases().size()); \
        }(); \
        static_assert(idx != switch_str_meta::cases().size(), "unlisted case"); \
        return idx; \
    }()

#endif // switch_str
