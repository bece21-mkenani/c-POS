#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include <string>

bool login(const std::string& username, const std::string& password);
bool storeSale(const std::string& username, const std::string& product_id,
               double unit_price, double vat, int quantity,
               const std::string& client_name, const std::string& client_phone);
std::string getSalesData(const std::string& username);

#endif
