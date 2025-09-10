#include <iostream>

#include <some/some.h>

int main(int argc, char *argv[])
{
    std::cout << "Base application message" << std::endl;
    std::cout << std::endl;

    std::cout << "Dependency:" << std::endl;
    some::doTheThing();
}
