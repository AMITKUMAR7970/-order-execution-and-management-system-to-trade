#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <thread>
#include <unordered_map>
#include <string>

class WebSocketServer {
public:
    WebSocketServer(boost::asio::io_context& ioc, int port);
    void start();
    void stop();

private:
    void acceptConnection();
    void handleClient(std::shared_ptr<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>> ws);

    boost::asio::ip::tcp::acceptor acceptor_;
    std::unordered_map<std::string, std::vector<std::shared_ptr<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>>>> subscriptions_;
    std::thread serverThread_;
};

#endif // WEBSOCKET_SERVER_H
