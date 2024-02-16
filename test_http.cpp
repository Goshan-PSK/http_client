#include <iostream>
#include <dimon/http_client.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace my;

TEST_CASE ("stroka ZAEBIS") {
    std::string strHttp = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: 20\r\nContent-Length: 3\r\nAccess-Control-AWWllow-Origin: 20\r\n\r\nxyu";
    Parser pa;
    const auto & response = pa.getResponse();
    bool res = pa.parse(strHttp);
    REQUIRE (res == true);
    REQUIRE (pa.getEror().value() == 0);
    
    REQUIRE (response.status == "HTTP/1.1 200 OK");
    REQUIRE (response.body == "xyu");

    REQUIRE (response.headers.find("Access-Control-Allow-Origin") != response.headers.end());
    REQUIRE (response.headers.at("Access-Control-Allow-Origin") == "20");
   
    REQUIRE (response.headers.find("Content-Length") != pa.getResponse().headers.end());
    REQUIRE (response.headers.at("Content-Length") == "3");
}

TEST_CASE ("stroka SLOMALAS") {
    std::string strHttp = "HTOPskTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: 20\r\ncontent-length: 3\r\n\r\nxyu";
    Parser pa;
    bool res = pa.parse(strHttp);
    REQUIRE (res == true); 
    REQUIRE (pa.getEror().value() != 0);
}

TEST_CASE ("STROKA WITHOUT BODY") {
    std::string strHttp = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: 20\r\nAccess-Control-AWWllow-Origin: 320\r\n\r\n";
    Parser pa;
    const auto & response = pa.getResponse();
    bool res = pa.parse(strHttp);
    REQUIRE (res == true);
    REQUIRE (pa.getEror().value() == 0);
    
    REQUIRE (response.status == "HTTP/1.1 200 OK");
    REQUIRE (response.body == "");

    REQUIRE (response.headers.find("Access-Control-Allow-Origin") != response.headers.end());
    REQUIRE (response.headers.at("Access-Control-Allow-Origin") == "20");
   
    REQUIRE (response.headers.find("Access-Control-AWWllow-Origin") != response.headers.end());
    REQUIRE (response.headers.at("Access-Control-AWWllow-Origin") == "320");

    REQUIRE (response.headers.find("Content-Length") == response.headers.end());
}

TEST_CASE ("WE NEED REED MOREEE") {
    std::string strHttp = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: 20\r\nContent-Length: 3\r\n\r\nxyu";
    Parser pa;
    for (int i = 0; i < strHttp.size(); ++i) {
        const std::string str = strHttp.substr(0, i);
        bool res = pa.parse(str);
        CAPTURE(i);
        CAPTURE(str);
        REQUIRE (res == false);
    }
    bool res = pa.parse(strHttp);
    REQUIRE (res == true);
}

TEST_CASE ("Requare") {
    Request r;
    Serialaizer req;

    Parser pa;
    const auto & response = pa.getResponse();

    r.method = my::Method::get;
    r.headers["Host"] = "example.com";
    r.body = "SOSI PIZDU";
    r.target = "/";

    std::string str = req.serialaiz(r);

    REQUIRE(str == "GET / HTTP/1.1\r\nContent-Length: 10\r\nHost: example.com\r\n\r\nSOSI PIZDU");
}

TEST_CASE("PARSE URL") {
    std::string url = "https://pass@example.com:443/path/to/my%2dfile.txt?id=42&name=John%20Doe+Jingleheimer%2DSchmidt";
    Url parseUrl(url);
    REQUIRE(parseUrl.getProtocol() == "https");
    REQUIRE(parseUrl.getHost() =="pass@example.com");
    REQUIRE(parseUrl.getPort() == "443");
    REQUIRE(parseUrl.getPath() == "/path/to/my%2dfile.txt");
    REQUIRE(parseUrl.getQuery() == "id=42&name=John%20Doe+Jingleheimer%2DSchmidt");
}

