#include "utils.h"
#include <iostream>
#include <limits>

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

double getValidDouble(const std::string& prompt) {
    double value;
    while(true) {
        std::cout << prompt;
        if(std::cin >> value) break;
        std::cin.clear();
        clearInputBuffer();
        std::cerr << "Invalid input. Please enter a numeric value." << std::endl;
    }
    return value;
}

int getValidInt(const std::string& prompt) {
    int value;
    while(true) {
        std::cout << prompt;
        if(std::cin >> value) break;
        std::cin.clear();
        clearInputBuffer();
        std::cerr << "Invalid input. Please enter a whole number." << std::endl;
    }
    return value;
}
