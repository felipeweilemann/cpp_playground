#include <iostream>

template<class T>
void sum(T const& a, T const& b, T& c);

int main() {
    auto a = 2.1;
    auto b = 3.2;
    double c{};
    sum(a, b, c);
    std::cout << "Sum: " << c << std::endl;
    return 0;
}

template<class T>
void sum(const T &a, const T &b, T &c) {
    c = a + b;
}