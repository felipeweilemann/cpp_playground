#include <iostream>
#include <type_traits>

// Helper to test if a type T is streamable with std::cout
template<typename T>
class IsStreamable
{
private:
    // Test expression for stream insertion
    template<typename U, typename = decltype(std::cout << std::declval<U>())>
    static std::true_type test(int);

    // Fallback if test expression fails
    template<typename U>
    static std::false_type test(...);

public:
    // Result is based on which overload of test() is chosen
    static constexpr bool value = decltype(test<T>(0))::value;
};
