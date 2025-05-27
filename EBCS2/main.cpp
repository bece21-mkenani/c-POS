#include <iostream>
#include "auth.h"
#include "sales.h"
#include "menu.h"
#include "utils.h"

int main() {
    std::string username, password;

    if(!authenticateUser(username, password)) {
        return 1;
    }

    int choice;
    do {
        displayMainMenu();
        if(!(std::cin >> choice)) {
            clearInputBuffer();
            std::cerr << "Invalid input!" << std::endl;
            continue;
        }

        switch(choice) {
            case 1:
                recordSale(username);
                break;
            case 2:
                viewSalesSummary(username);
                break;
            case 3:
                exportSales(username);
                break;
            case 4:
                std::cout << "Exiting system..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice! Please enter 1-4." << std::endl;
        }
    } while(choice != 4);

    return 0;
}
