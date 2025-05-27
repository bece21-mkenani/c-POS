#include "menu.h"
#include <iostream>

void displayMainMenu() {
    std::cout << "\n=== Main Menu ===" << std::endl
              << "1. Record New Sale" << std::endl
              << "2. View Sales Summary" << std::endl
              << "3. Export Sales Data" << std::endl
              << "4. Exit System" << std::endl
              << "Enter choice (1-4): ";
}

void displaySalesMenu() {
    std::cout << "\n=== Sales Reports ===" << std::endl
              << "1. Daily Summary" << std::endl
              << "2. High/Low Analysis" << std::endl
              << "3. Detailed Transactions" << std::endl
              << "4. Return to Main Menu" << std::endl
              << "Enter choice (1-4): ";
}
