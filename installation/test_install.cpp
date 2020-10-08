#include <iostream>
#include <libcellml>

int main()
{
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "    Welcome to libCellML!" << std::endl;
    std::cout << "    This version number is " << libcellml::versionString() << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}
