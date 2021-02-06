#include <iostream>

bool quaternion();
bool vector();
bool matrix();
bool base_change();

int main(int argc, char* argv[])
{
    std::cout << "base_change : \t\t" << (base_change() ? "Ok" : "Error") << std::endl;
    std::cout << "Press Enter to Continue";
    std::cin.ignore();
	return 0;
}
