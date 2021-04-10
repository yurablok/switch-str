# switch-str
C++17 O(1) switch for strings.

## Benefits and features:
* O(1) cases matching.
* Faster than `if-elseif-elseif-else`.
* `break`, `continue`, `return`, `[[fallthrough]]` work as expected.
* Compile-time errors for repetitive and unlisted cases.

## Example of usage:
```cpp
#include <switch_str.hpp>

void test(const std::string_view& value) {
    switch_str(value,
        "ERR", "MSH", "OBR", "PID") {
    case_str("ERR"):
        ...
        break;
    case_str("MSH"):
        static_assert(switch_str_meta::cases().size() == 4);
        static_assert(switch_str_meta::cases()[1] == "MSH");
        break;
    case_str("PID"):
        ...
        break;
    case_str("PID"): // Repetition -> compile-time error.
        ...
        break;
    case_str("PV1"): // Unlisted -> compile-time error.
        ...
        break;
    default:
        ...
        break;
    }
}
```
