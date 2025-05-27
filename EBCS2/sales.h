#ifndef SALES_H
#define SALES_H

#include <string>

void viewSalesSummary(const std::string &username);
void recordSale(const std::string &username);
void exportSales(const std::string &username);
double calculateVAT(double amount);

#endif
