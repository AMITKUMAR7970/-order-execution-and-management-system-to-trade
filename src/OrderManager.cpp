#include "OrderManager.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>

namespace http = boost::beast::http;
namespace json = boost::json;

OrderManager::OrderManager(const std::string& api_key, const std::string& api_secret)
    : apiKey_(api_key), apiSecret_(api_secret) {}

std::string OrderManager::generateAuthToken() {
    // This function should generate an authentication token for Deribit's API.
    // You must implement the exact Deribit authentication flow here.
    return "mocked-auth-token"; // Replace with actual token.
}

void OrderManager::sendRequest(const std::string& endpoint, const json::value& body) {
    try {
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::resolver resolver(ioc);
        boost::beast::tcp_stream stream(ioc);

        auto const results = resolver.resolve("test.deribit.com", "443");
        stream.connect(results);

        boost::asio::ssl::context ssl_context(boost::asio::ssl::context::tlsv12_client);
        boost::beast::ssl_stream<boost::beast::tcp_stream> ssl_stream(std::move(stream), ssl_context);
        ssl_stream.handshake(boost::asio::ssl::stream_base::client);

        // Create the HTTP POST request
        http::request<http::string_body> req(http::verb::post, endpoint, 11);
        req.set(http::field::host, "test.deribit.com");
        req.set(http::field::content_type, "application/json");
        req.body() = json::serialize(body);
        req.prepare_payload();

        // Send the request
        http::write(ssl_stream, req);

        // Receive the response
        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(ssl_stream, buffer, res);

        std::cout << "Response: " << res.body() << std::endl;

        // Gracefully close the connection
        boost::system::error_code ec;
        ssl_stream.shutdown(ec);
    } catch (const std::exception& ex) {
        std::cerr << "Error in sendRequest: " << ex.what() << std::endl;
    }
}

void OrderManager::placeOrder(const std::string& instrument, double quantity, double price, const std::string& type) {
    json::value body = {
        {"jsonrpc", "2.0"},
        {"id", 1},
        {"method", "private/buy"},
        {"params", {
            {"instrument_name", instrument},
            {"amount", quantity},
            {"price", price},
            {"type", type}
        }}
    };

    sendRequest("/api/v2/private/buy", body);
}

void OrderManager::cancelOrder(const std::string& orderId) {
    json::value body = {
        {"jsonrpc", "2.0"},
        {"id", 1},
        {"method", "private/cancel"},
        {"params", {{"order_id", orderId}}}
    };

    sendRequest("/api/v2/private/cancel", body);
}

void OrderManager::modifyOrder(const std::string& orderId, double newPrice) {
    json::value body = {
        {"jsonrpc", "2.0"},
        {"id", 1},
        {"method", "private/edit"},
        {"params", {
            {"order_id", orderId},
            {"price", newPrice}
        }}
    };

    sendRequest("/api/v2/private/edit", body);
}

void OrderManager::getOrderBook(const std::string& instrument) {
    json::value body = {
        {"jsonrpc", "2.0"},
        {"id", 1},
        {"method", "public/get_order_book"},
        {"params", {{"instrument_name", instrument}}}
    };

    sendRequest("/api/v2/public/get_order_book", body);
}

void OrderManager::viewPositions() {
    json::value body = {
        {"jsonrpc", "2.0"},
        {"id", 1},
        {"method", "private/get_positions"},
        {"params", {{"currency", "BTC"}, {"kind", "future"}}}
    };

    sendRequest("/api/v2/private/get_positions", body);
}
