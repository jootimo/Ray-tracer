#include "../Math.h"

template <class T>
void expect(T value, T expected)
{
    if(value != expected) {
        std::cerr << "Test failed with value " << value << ", when " << expected <<
                " was expected." << std::endl;
        exit(-1); 
    }
}

int main(int argc, char **argv)
{
    float x = 0;
    float y = 0;
    expect(solve_quadratic(2,0,-8,x,y), true);
    expect(solve_quadratic(1,7,0,x,y), true);
    expect(solve_quadratic(0,0,0,x,y), true);
    expect(solve_quadratic(0.02,0.001,8.001,x,y), false);

    std::cout << "All tests passed." << std::endl;

    return 0;   
}
