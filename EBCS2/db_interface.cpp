#include <curl/curl.h>
#include <iostream>
#include <string>
#include "db_interface.h"

// Callback function to handle response data
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realSize = size * nmemb;
    std::string *response = (std::string*)userp;
    response->append((char*)contents, realSize);
    return realSize;
}

// LOGIN METHOD
bool login(const std::string& username, const std::string& password) {
    CURL *curl = curl_easy_init();
    if (curl) {
        const std::string url = "http://44.205.141.40/api/auth/check-credentials";
        std::string responseData;
        long responseCode = 0;
        bool responseState = false;

        curl_mime *mime = curl_mime_init(curl);

        curl_mimepart *field1 = curl_mime_addpart(mime);
        curl_mime_name(field1, "username");
        curl_mime_data(field1, username.c_str(), CURL_ZERO_TERMINATED);

        curl_mimepart *field2 = curl_mime_addpart(mime);
        curl_mime_name(field2, "password");
        curl_mime_data(field2, password.c_str(), CURL_ZERO_TERMINATED);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            responseState = (responseCode == 200);
        }

        curl_easy_cleanup(curl);
        curl_mime_free(mime);
        return responseState;
    } else {
        return false;
    }
}

// POST METHOD for sales transaction
bool storeSale(const std::string& username, const std::string& product_id,
               double unit_price, double vat, int quantity,
               const std::string& client_name, const std::string& client_phone) {
    CURL *curl = curl_easy_init();
    if (curl) {
        char* encoded_username = curl_easy_escape(curl, username.c_str(), username.length());
        const std::string url = "http://44.205.141.40/api/sales/store-sales/" + std::string(encoded_username);
        std::string responseData;
        long responseCode = 0;
        bool responseState = false;

        curl_mime *mime = curl_mime_init(curl);

        curl_mimepart *field1 = curl_mime_addpart(mime);
        curl_mime_name(field1, "product_id");
        curl_mime_data(field1, product_id.c_str(), CURL_ZERO_TERMINATED);

        curl_mimepart *field2 = curl_mime_addpart(mime);
        curl_mime_name(field2, "unit_price");
        curl_mime_data(field2, std::to_string(unit_price).c_str(), CURL_ZERO_TERMINATED);

        curl_mimepart *field3 = curl_mime_addpart(mime);
        curl_mime_name(field3, "vat");
        curl_mime_data(field3, std::to_string(vat).c_str(), CURL_ZERO_TERMINATED);

        curl_mimepart *field4 = curl_mime_addpart(mime);
        curl_mime_name(field4, "quantity");
        curl_mime_data(field4, std::to_string(quantity).c_str(), CURL_ZERO_TERMINATED);

        curl_mimepart *field5 = curl_mime_addpart(mime);
        curl_mime_name(field5, "client_name");
        curl_mime_data(field5, client_name.c_str(), CURL_ZERO_TERMINATED);

        curl_mimepart *field6 = curl_mime_addpart(mime);
        curl_mime_name(field6, "client_phone");
        curl_mime_data(field6, client_phone.c_str(), CURL_ZERO_TERMINATED);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // <-- FIX: follow redirects
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
            responseState = (responseCode == 200);
        }

        curl_free(encoded_username);
        curl_easy_cleanup(curl);
        curl_mime_free(mime);
        curl_slist_free_all(headers);
        return responseState;
    } else {
        return false;
    }
}

// GET METHOD to retrieve sales data
std::string getSalesData(const std::string& username) {
    CURL *curl = curl_easy_init();
    if (curl) {
        std::cerr << "Failed to initialize CURL!" << std::endl;
        return "ERROR";
    }

    std::string url = "http://44.205.141.40/api/sales/get-sales/" + username;
    std::string responseData;
    long responseCode = 0;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);

    if (res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
        curl_easy_cleanup(curl);

        if (responseCode == 200) {
            if (responseData.find("\"message\":") != std::string::npos &&
                responseData.find("No more records") != std::string::npos) {
                return "ERROR";
            }

            size_t fieldPos = responseData.find("\"field\":");
            if (fieldPos != std::string::npos) {
                size_t valueStartPos = fieldPos + 8;
                while (valueStartPos < responseData.length() &&
                       (responseData[valueStartPos] == ' ' || responseData[valueStartPos] == '\t')) {
                    valueStartPos++;
                }
                if (responseData[valueStartPos] == '"') {
                    valueStartPos++;
                    size_t valueEndPos = responseData.find("\"", valueStartPos);
                    if (valueEndPos != std::string::npos) {
                        return responseData.substr(valueStartPos, valueEndPos - valueStartPos);
                    }
                } else {
                    size_t valueEndPos = responseData.find_first_of(",}", valueStartPos);
                    if (valueEndPos != std::string::npos) {
                        return responseData.substr(valueStartPos, valueEndPos - valueStartPos);
                    }
                }
            }
        }
    } else {
        curl_easy_cleanup(curl);
    }

    return "ERROR";
}
