#include <iostream>
#include <thread>
#include <chrono>

#include "boost/asio.hpp"
#include "boost/beast.hpp"
#include "boost/asio/ssl.hpp"
#include "boost/beast/ssl.hpp"

using std::chrono_literals::operator""s;

boost::asio::ssl::context MakeDefaultSslContext() {
  auto ssl_context = boost::asio::ssl::context(boost::asio::ssl::context::tlsv12_client);
  ssl_context.set_verify_mode(boost::asio::ssl::verify_peer);
  ssl_context.set_default_verify_paths();
  return ssl_context;
}

int main() {
  while(true) {
    try {
      boost::asio::io_context ioc;
      boost::asio::ssl::context ssl_context = MakeDefaultSslContext();
      auto work = boost::asio::make_work_guard(ioc);
      std::jthread thread([&ioc]() { ioc.run(); });

      boost::asio::ip::tcp::resolver resolver{ioc};
      boost::asio::ip::tcp::socket socket{ioc};
      auto future = boost::asio::async_connect(socket, resolver.resolve("example.com", "443"), boost::asio::use_future);
      future.get();
      boost::beast::tcp_stream stream{std::move(socket)};

      boost::beast::ssl_stream<boost::beast::tcp_stream> ssl_stream{std::move(stream), ssl_context};
      ssl_stream.set_verify_callback(boost::asio::ssl::host_name_verification("example.com"));
      auto ssl_future = ssl_stream.async_handshake(boost::asio::ssl::stream_base::client, boost::asio::use_future);
      ssl_future.get();

      std::cout << "Successfully connected" << std::endl;
      ioc.stop();
      std::this_thread::sleep_for(1s);
    }
    catch (const std::exception& exc) {
      std::cout << exc.what() << std::endl;
    }
  }
}
