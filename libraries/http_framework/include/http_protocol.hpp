#ifndef HTTP_PROTOCOL_HPP
#define HTTP_PROTOCOL_HPP

#include <string_view>

namespace http_protocol {

namespace method {

static constexpr std::string_view OPTIONS = "OPTIONS",
                                  GET     = "GET",
                                  HEAD    = "HEAD",
                                  POST    = "POST",
                                  PUT     = "PUT",
                                  PATCH   = "PATCH",
                                  DELETE  = "DELETE",
                                  TRACE   = "TRACE",
                                  CONNECT = "CONNECT";

}  // namespace method

namespace version {

static constexpr std::string_view v09 = "0.9",
                                  v10 = "1.0",
                                  v11 = "1.1",
                                  v20 = "2.0";

}  // namespace version

enum class code : uint16_t {
    // Информационные
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    PROCESSING = 102,

    // Успех
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    ALREADY_REPORTED = 208,
    IM_USED = 226,

    // Перенаправление
    
}

}  // namespace http_protocol

#endif  // HTTP_PROTOCOL_HPP
