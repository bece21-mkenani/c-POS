#include "sales.h"
#include "db_interface.h"
#include <iostream>
#include <iomanip>
#include <fstream>

const double VAT_RATE = 0.165; // 16.5% VAT

double calculateVAT(double amount) {
    return amount * VAT_RATE;
}

void viewSalesSummary(const std::string &username) {
    std::string data = getSalesData(username);
    if (data == "ERROR") {
        std::cerr << "Failed to fetch sales data!\n";
        return;
    }

    std::cout << "\n=== Sales Summary ===\n";
    std::cout << "Username: " << username << "\n";
    std::cout << "Raw Data:\n" << data << "\n";

    // TODO: Add proper parsing and formatting of the data
    // This is a placeholder - implement your actual summary logic here
}

void recordSale(const std::string &username) {
    std::string product_id, client_name, client_phone;
    double unit_price;
    int quantity;

    std::cout << "\n=== New Sale ===\n";
    std::cout << "Product ID: ";
    std::cin >> product_id;

    std::cout << "Unit Price: ";
    std::cin >> unit_price;

    std::cout << "Quantity: ";
    std::cin >> quantity;

    std::cin.ignore(); // Clear buffer
    std::cout << "Client Name: ";
    std::getline(std::cin, client_name);

    std::cout << "Client Phone: ";
    std::getline(std::cin, client_phone);

    double vat = calculateVAT(unit_price * quantity);

    if (storeSale(username, product_id, unit_price, vat, quantity, client_name, client_phone)) {
        std::cout << "✓ Sale recorded successfully!\n";
    } else {
        std::cerr << "✗ Failed to record sale!\n";
    }
}

void exportSales(const std::string &username) {
    std::string data = getSalesData(username);
    if (data == "ERROR") {
        std::cerr << "No sales data available!\n";
        return;
    }

    std::ofstream file("sales_export.csv");
    if (file) {
        file << "Product ID,Unit Price,Quantity,VAT,Client Name,Client Phone\n";
        file << data;
        std::cout << "✓ Sales exported to sales_export.csv\n";
    } else {
        std::cerr << "✗ Failed to create export file!\n";
    }
}
