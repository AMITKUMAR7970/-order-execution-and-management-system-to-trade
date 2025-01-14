#ifndef ORDER_MANAGER_H
#define ORDER_MANAGER_H

#include <string>

class OrderManager {
public:
    OrderManager(const std::string& api_key, const std::string& api_secret);
    void placeOrder(const std::string& instrument, double quantity, double price, const std::string& type);
    void cancelOrder(const std::string& orderId);
    void modifyOrder(const std::string& orderId, double newPrice);
    void getOrderBook(const std::string& instrument);
    void viewPositions();

private:
    std::string apiKey_;
    std::string apiSecret_;
    std::string generateAuthToken();
};

#endif // ORDER_MANAGER_H
