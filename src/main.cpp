#include "WebSocketServer.h"
#include "OrderManager.h"
#include <iostream>
#include <boost/asio.hpp>

int main() {
    boost::asio::io_context ioc;
    WebSocketServer wsServer(ioc, 8080);
    OrderManager orderManager("Client id", "Client Secret");

    wsServer.start();

    // Sample usage
    orderManager.placeOrder("BTC-PERPETUAL", 1.0, 20000.0, "limit");
    orderManager.getOrderBook("BTC-PERPETUAL");
    orderManager.viewPositions();

    std::cout << "Press Enter to exit." << std::endl;
    std::cin.get();
    wsServer.stop();

    return 0;
}
