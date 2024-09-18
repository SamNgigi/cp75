We have a wild regex pattern in the `getAvailableMethod()` in game_play.cpp. Just a simple explainer
```C++
std::regex methodRegex(R"(
    ^\s*                                 # Start of line and optional whitespace
    (?:virtual\s+)?                      # Optional virtual keyword
    (?:static\s+)?                       # Optional static keyword
    (?:inline\s+)?                       # Optional inline keyword
    (?:explicit\s+)?                     # Optional explicit keyword
    (?:constexpr\s+)?                    # Optional constexpr keyword
    (?:                                  # Start of return type group
        (?:const\s+)?                    # Optional const for return type
        (?:volatile\s+)?                 # Optional volatile for return type
        (?:\w+::)*\w+                    # Return type (possibly with namespace)
        (?:\s*<[^>]*>)?                  # Optional template parameters
        (?:\s*\*|\s*&)?                  # Optional pointer or reference
        \s+
    )
    (?:\w+::)?                           # Optional class scope
    (\w+)                                # Method name (capture group)
    \s*\([^)]*\)                         # Parameters
    (?:\s*const)?                        # Optional const
    (?:\s*noexcept)?                     # Optional noexcept
    (?:\s*override)?                     # Optional override
    (?:\s*final)?                        # Optional final
    (?:\s*=\s*0)?                        # Optional pure virtual specifier
    \s*\{                                # Opening brace
)", std::regex::extended);
```
