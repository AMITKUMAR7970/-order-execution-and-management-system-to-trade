#include "WebSocketServer.h"
#include <iostream>
#include <boost/beast/websocket.hpp>

WebSocketServer::WebSocketServer(boost::asio::io_context& ioc, int port)
    : acceptor_(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {}

void WebSocketServer::start() {
    serverThread_ = std::thread([this]() {
        acceptConnection();
    });
}

void WebSocketServer::stop() {
    acceptor_.close();
    if (serverThread_.joinable()) {
        serverThread_.join();
    }
}

void WebSocketServer::acceptConnection() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor().context());
    acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec) {
            auto ws = std::make_shared<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>>(std::move(*socket));
            ws->accept();
            handleClient(ws);
        }
        acceptConnection();
    });
}

void WebSocketServer::handleClient(std::shared_ptr<boost::beast::websocket::stream<boost::asio::ip::tcp::socket>> ws) {
    // Basic message handling for WebSocket clients
    auto buffer = std::make_shared<boost::beast::flat_buffer>();
    ws->async_read(*buffer, [this, ws, buffer](boost::system::error_code ec, std::size_t) {
        if (!ec) {
            std::string message = boost::beast::buffers_to_string(buffer->data());
            std::cout << "Received: " << message << std::endl;
            ws->text(ws->got_text());
            ws->write(boost::asio::buffer("Acknowledged: " + message));
        }
    });
}
