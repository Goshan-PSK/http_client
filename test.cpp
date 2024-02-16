#include <iostream>
#include <functional>
#include <optional>
#include <system_error>
#include <string>
#include <boost/asio.hpp>
#include <dimon/http_client.hpp>

// class Client {
// public:
//     Client(boost::asio::io_context &io_context)
//         : mSocket(io_context)
//         , mResolver(io_context)
//     {
//     }

// public:
//     void open(std::function< void(std::error_code)> callBack) { // callback
//         // resolve -> onResolve -> connect -> onConnect -> callback;
//         mResolver.async_resolve(
//             "example.com",
//             "http",
//             [this, callBack](const auto &eror, const auto &result) {
//                 onResolve(eror, result, callBack);
//         });
//     }
// private:
//     void onResolve(
//         const boost::system::error_code &ec,
//         const boost::asio::ip::tcp::resolver::results_type &addrs,
//         std::function< void(std::error_code)> callBack) {
//         if (ec)
//             return callBack(ec);

//         boost::asio::async_connect(
//             mSocket,
//             addrs,
//             [this, callBack](const auto &ec, auto&&) { 
//                 callBack(ec);
//          });
//     }

// private:
//     boost::asio::ip::tcp::resolver mResolver;
//     boost::asio::ip::tcp::socket mSocket;
//     std::array< char, 1024 > buf;
// };

// int main () {
//     // boost::asio::io_context io_context;
//     // boost::asio::ip::tcp::resolver resolver(io_context);
//     // boost::asio::ip::tcp::resolver::results_type endpoints;
//     // boost::asio::ip::tcp::socket socket(io_context);
    
//     // resolver.async_resolve("example.com", "http", [&](const auto ec, const auto &endpoints){
//     //     std::cout << "resolve" << std::endl;
//     //     boost::asio::async_connect(socket, endpoints, [&](const auto &ec, const auto endpont) {
//     //         std::cout<< "connected" << std::endl;
//     //         boost::system::error_code eror;
//     //         std::string stroka = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
            
//     //         socket.async_send(boost::asio::buffer(stroka), [this](const auto &ec, const auto sz) {
//     //             std::cout<< "SEND" << std::endl;
//     //             std::array< char, 1024 > buf;
//     //             size_t len = socket.async_read(boost::asio::buffer(buf), [&](const auto &ec, const auto sz) {
//     //                 std::cout<< "READ" << std::endl;
//     //                 std::cout.write(buf.data(), len);
//     //                 std::cout<<std::endl; 
//     //             });
//     //         });
//     //     });
//     // });
    
//     // io_context.run();
// struct S {

// };
int main () {

    // const auto request = Request{
    //     .method = Method::GET,
    //     .target = "/",
    //     .headers = //...
    //     .body = "",
    // };
    my::Request r;

    r.method = my::Method::get;
    r.headers["Host"] = "example.com";
    // r.body = "SOSI PIZDU";
    r.target = "/";

    boost::asio::io_context ioc;


    my::makeSesion(ioc, my::Url("example.com"),[r](std::error_code error, std::shared_ptr<my::Sesion> ses) {
        if (error) {
            std::cout << error << error.message() << std::endl;
            return;
        }
        ses->write(r, [ses, r](std::error_code er,my::Response resp){
            // ses->write(r, [](std::error_code er, my::Response respon){
            //     std::cout << respon.body << std::endl;
            // });
            std::cout << resp.body << std::endl;
        });
    });

    // a.open("example.com",[&a, r](std::error_code ec){
    //     std::cout << "Connection : " << ec.message() << std::endl;
    //     a.send(r, [](const auto eror, const auto response){
    //         for (const auto[key, value] : response.headers) {
    //             std::cout << key << " : " << value << std::endl;
    //         }
    //         std::cout << "BODY = " <<response.body << std::endl << "status = " << response.status <<std::endl;
    //     });
    // });

    ioc.run();

    return 0;
}