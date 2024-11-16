#pragma once

#include <exception>

namespace Backend_plugins::exception {

struct Backend_plugins_ex : std::exception {
    explicit Backend_plugins_ex(std::string message_) : message{std::move(message_)} {};

    [[nodiscard]] const char *what() const noexcept override { return message.c_str(); }

   private:
    std::string message;
};

/* Ошибка при работе с clixon */
struct Clixon : Backend_plugins_ex {
    using Backend_plugins_ex::Backend_plugins_ex;
};

} // namespace Backend_plugins::exception
