#include "array.hxx"

#include <iostream>
#include <variant>

using namespace ptl;

int main()
{
    size_t const size = 4;
    Array<int> a(size);
    for (int i = 0; i < size; i++) {
        a.set(i, i);
    }
    static int const expected[][size] = {
        { 0, 0, 0, 0, },
        { 0, 0, 0, 0, },
        { 0, 1, 0, 0, },
        { 0, 1, 2, 0, },
        { 0, 1, 2, 3, },
    };
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < size; j++) {
            if (a.get(j, i) != expected[i][j]) {
                return 1;
            }
        }
    }
    return 0;
}
