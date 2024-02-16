#include "http_client.hpp"

#include <sstream>

namespace my {

class HttpSession : public Sesion {
public:
    HttpSession(boost::asio::ip::tcp::socket socket)
        : mSocket{std::move(socket)}
    {
        mBuf.resize(1024);
    }
private:
    void read(MessageHandler callBack) {
        mSocket.async_read_some(boost::asio::buffer(mBuf.data() + mRecived, mBuf.size() - mRecived), [this, callBack](const auto error, const auto sz){
            Parser pa;
            if(error) {
                callBack(error, pa.getResponse());
                mBusy = false;
                return;
            }
            mRecived += sz;
            if (mRecived >= mBuf.size()) {
                mBuf.resize(mBuf.size() + 1024);
            }
            std::string_view strPodPars(mBuf.data(), mRecived);
            bool result = pa.parse(strPodPars);
            if (result) {
                callBack(pa.getEror(), pa.getResponse());
                mBusy = false;
            }
            else {
                read(callBack);
            }
        });
    }

    void send(const Request & req, MessageHandler callBack) {
    Serialaizer makeReq;
    boost::asio::async_write(mSocket, boost::asio::buffer(makeReq.serialaiz(req)), [this, callBack](const auto & error, const auto) {
        if (error) {
            callBack(error, Response());
            mBusy = false;
            return;
        }
        read(callBack);
    });
    }
public:
    void close() override {
        mSocket.close();
    }

    bool isBusy() const override {
        return mBusy;
    }

    void write(Request req, MessageHandler callBack) override {
        mBusy = true;
        send(req, callBack);
    }
private:
    size_t mRecived = 0;
    boost::asio::ip::tcp::socket mSocket;
    std::string mBuf;
    bool mBusy = false;
};

class HttpsSession : public Sesion,  public std::enable_shared_from_this <HttpsSession> {
public:
    HttpsSession(boost::asio::ip::tcp::socket socket)
        : mCtx{boost::asio::ssl::context::sslv23}
        , mStream{std::move(socket), mCtx}
    {
        mBuf.resize(1024);
    }
public:
    void handShake(std::function<void(std::error_code, std::shared_ptr<Sesion>)> callBack) {
        mStream.async_handshake(boost::asio::ssl::stream_base::client, [self = shared_from_this(), this, callBack](const auto error){callBack(error, shared_from_this());});
    }

    void sendHttps(const my::Request &req, my::Sesion::MessageHandler callBack) {
    Serialaizer makeReq;
    boost::asio::async_write(mStream, boost::asio::buffer(makeReq.serialaiz(req)), [self = shared_from_this(), this, callBack](const auto &error, const auto sz) {
        if (error) {
            callBack(error, Response());
            return;
        }
        self->readHttps(callBack);
    });
    }

    void readHttps(my::Sesion::MessageHandler callBack) {
        mStream.async_read_some(boost::asio::buffer(mBuf.data() + mRecived, mBuf.size() - mRecived), [self = shared_from_this(), this, callBack](const auto error, const auto sz){
            Parser pa;
            if(error) {
                callBack(error, pa.getResponse());
                return;
            }
            mRecived += sz;
            if (mRecived >= mBuf.size()) {
                mBuf.resize(mBuf.size() + 1024);
            }
            std::string_view strPodPars(mBuf.data(), mRecived);
            bool result = pa.parse(strPodPars);
            if (result) {
                callBack(pa.getEror(), pa.getResponse());
            }
            else {
                self->readHttps(callBack);
            }
        });
    }

    void write(Request req, MessageHandler callBack) override {
    mBusy = true;
    sendHttps(req, callBack);
    }

    bool isBusy() const override {
        return mBusy;
    }

    void close() override {
        mStream.async_shutdown([itself = shared_from_this()](const auto error) {});
    }
private:
    size_t mRecived;
    boost::asio::ssl::context mCtx;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> mStream;
    std::string mBuf;
    bool mBusy = false;
};
} // namespase end;

using namespace my;

bool IcaseCompare::operator() (std::string const &l, std::string const &r) const {
    auto lowLeter = [](char letter) -> char {
        if (letter >= 'A' && letter <= 'Z') {
            return letter + ('a' - 'A');
        }
        return letter;
    };
    if (l.size() == r.size()) {
        std::string::const_iterator i = l.begin();
        std::string::const_iterator j = r.begin();
        for ( ; i != l.end(); ++i, ++j) {
            const char lSt = lowLeter(*i);
            const char rSt = lowLeter(*j);
            if (lSt == rSt) {
                continue;
            }
        return rSt < lSt;
        }
    return false;
    }
    else {
        return l.size() < r.size();
    }
} 

std::string toString(const Method meth) {
    switch (meth) {
        case Method::options: return "OPTIONS";
        case Method::get:     return "GET";
        case Method::head:    return "HEAD";
        case Method::post:    return "POST";
    }
    throw std::runtime_error("Unknown method");
}

std::string Url::parseProt(std::string url) {
    std::string_view prot = "://";
    size_t protAdr = url.find(prot);
    if (protAdr == url.npos) {
        return url;
    }
    mProtocol = url.substr(0, protAdr);
    return url.substr(protAdr + prot.size());
}

std::string Url::parseHost(std::string url) {
    size_t firs = url.find_first_of(":/");
    mHost = url.substr(0, firs);
    if (firs == url.npos) {
        return {};
    }
    return url.substr(firs);
}

std::string Url::parsePort(std::string url) {
    size_t portId = url.find(":");
    if (portId == url.npos) {
        return url;
    }
    else {
        size_t sleshId = url.find("/");
        size_t portSz = sleshId - portId;
        mPort = url.substr(portId + 1, portSz - 1);
        return url.substr(portSz);
    }
}

std::string Url::parsePath(std::string url) {
    size_t first = url.find_first_of("?#");
    mPath = url.substr(0,first);
    return url.substr(mPath.size());
}

void Url::parseQuery(std::string url) {
    size_t questId = url.find("?");
    if (questId == url.npos) {
        return;
    }
    mQuery = url.substr(questId + 1);
}

std::string Serialaizer::serialaiz(Request rec) {
    std::string req;
    req += toString(rec.method);
    req += " / HTTP/1.1\r\n";
    if (!rec.body.empty() && rec.headers.find("Content-Length") == rec.headers.end()) {
        req += "Content-Length: ";
        req += std::to_string(rec.body.size());
        req += "\r\n";
    }
    for (auto it = rec.headers.begin(); it != rec.headers.end(); ++it) {
        req += it->first;
        req += ": ";
        req += it->second;
        req += "\r\n";
    }
    req += "\r\n";
    req += rec.body;
    return req;
}

bool Parser::parse(std::string_view str) {
    constexpr static std::string_view endLine = "\r\n\r\n";
    constexpr static std::string_view CRLF = "\r\n";
    constexpr static std::string_view markerStatus = "HTTP/1.1";
    constexpr static std::string_view markerBody = "Content-Length: ";

    if (str.find(endLine) == str.npos) {
        return false;
    }
    if (!str.starts_with(markerStatus)) {
        mEror = std::make_error_code(std::errc::protocol_error);
        return true;
    }
    const size_t statusId = str.find(CRLF);
    mResponse.status = std::string(str.substr(0, statusId));
    str = str.substr(statusId + CRLF.size());
    size_t pos = str.find(CRLF);
    while (pos != 0) {
        const size_t colon = str.find(':');
        if (colon == str.npos) {
            mEror = std::make_error_code(std::errc::protocol_error);
            return true;
        }
        mResponse.headers[std::string(str.substr(0, colon))] = std::string(str.substr(colon + 2, pos - colon - 2));
        str = str.substr(pos  + CRLF.size());
        pos = str.find(CRLF);
    }
    str = str.substr(CRLF.size());
    const auto contentLength = mResponse.headers.find("Content-Length");
    if (contentLength == mResponse.headers.end())
        return true;
    try {
        const auto bodySize = std::stoi(contentLength->second);
        if (bodySize > str.size())
            return false;
        mResponse.body = str.substr(0, bodySize);
    } catch(const std::exception&) {
        mEror = std::make_error_code(std::errc::protocol_error);
    }
    return true;
}

void my::makeSesion(boost::asio::io_context & ioc, Url url, std::function<void(std::error_code, std::shared_ptr<Sesion>)> callBack) {
    auto resolver = std::make_shared<boost::asio::ip::tcp::resolver>(ioc);
    const std::string protocol = url.getProtocol().empty() ? "https" : url.getProtocol();
    resolver->async_resolve(url.getHost(), protocol, [protocol, resolver, &ioc, callBack](const auto &error, const auto &result) {
        if(error) {
            callBack(error, nullptr);
            return;
        }

        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioc);
        boost::asio::async_connect(*socket, result, [protocol, socket, callBack](const auto &errors, auto&&){
            if (protocol == "http") {
                auto HttpPt = std::make_shared<my::HttpSession>(std::move(*socket));
                callBack(errors, HttpPt);
            }
            else {
                auto HttpsPt = std::make_shared<HttpsSession>(std::move(*socket));
                HttpsPt->handShake(callBack);
            }
        });
    });
}