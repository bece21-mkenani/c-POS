#include "auth.h"
#include "db_interface.h"
#include <iostream>
#include <limits>

/**
 * Official credential validation against fixed database records
 * Uses secure HTTP authentication via libcurl
 */
bool authenticateUser(std::string &username, std::string &password) {
    const int MAX_ATTEMPTS = 3;
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS) {
        std::cout << "\n=== POS SYSTEM LOGIN ===\n";

        // Secure input handling
        std::cout << "Username: ";
        if (!std::getline(std::cin, username)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cout << "Password: ";
        if (!std::getline(std::cin, password)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Official HTTP validation
        if (login(username, password)) {
            std::cout << "\n Access granted. Loading dashboard...\n";
            return true;
        }

        attempts++;
        if (attempts < MAX_ATTEMPTS) {
            std::cerr << "\n Invalid credentials. Attempts left: "
                     << (MAX_ATTEMPTS - attempts) << "\n";
        }
    }

    std::cerr << "\n⚠ Account locked. Contact administrator.\n";
    return false;
}
