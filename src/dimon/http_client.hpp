#pragma once

#include <functional>
#include <optional>
#include <system_error>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace my {

enum class Method {
    options,
    get,
    head,
    post
};

struct IcaseCompare {
    bool operator() (std::string const &l, std::string const &r) const; 
};

struct Request {
    Method method;
    std::string target;
    std::map<std::string, std::string, IcaseCompare> headers;
    std::string body;
};

class Url {
    std::string parseProt(std::string);
    std::string parseHost(std::string);
    std::string parsePort(std::string);
    std::string parsePath(std::string);
    void parseQuery(std::string);
public:
    Url(std::string Ourl){
        std::string url;
        url = parseProt(Ourl);
        url = parseHost(url);
        url = parsePort(url);
        url = parsePath(url);
        parseQuery(url);
    }
    std::string const & getProtocol() const {
        return mProtocol;
    }
    std::string const & getHost() const {
        return mHost;
    }
    std::string const & getPort() const {
        return mPort;
    }
    std::string const & getPath() const {
        return mPath;
    }
    std::string const & getQuery() const {
        return mQuery;
    }
private:
    std::string mProtocol;
    std::string mHost;
    std::string mPort;
    std::string mPath;
    std::string mQuery;
};

class Serialaizer {
public:
    std::string serialaiz(Request);
};

struct Response {
    Response() = default;
    std::string status;
    std::map<std::string, std::string, IcaseCompare> headers;
    std::string body;
};

class Parser {
public:
    bool parse(std::string_view);
    const Response&  getResponse() const {
        return mResponse;
    }
    std::error_code getEror() {
        return mEror;
    }
private:
    std::error_code mEror;
    Response mResponse;
};

class Sesion {
public:
    using MessageHandler = std::function< void(std::error_code, Response response) >;
    virtual ~Sesion() = default;
    virtual void write(Request, MessageHandler) = 0;
    virtual bool isBusy() const = 0;
    virtual void close() = 0;
};

void makeSesion(boost::asio::io_context &, Url, std::function<void(std::error_code, std::shared_ptr<my::Sesion>)>);

} //namespace my closed
