#pragma once

#include <build-info.hpp>

#include <exception>
#include <filesystem>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

#include <experimental/source_location>

namespace std { // NOLINT(cert-dcl58-cpp)

using source_location = ::std::experimental::source_location;

} // namespace std

template <class T>
concept Streamable = requires(std::ostream output, T value)
{
    {output << value} -> std::same_as<std::ostream&>;
};

class Error : public std::exception {
public:
    explicit Error(
        std::string_view value = "",
        std::source_location sourceLocation = std::source_location::current())
    {
        auto relativeFilePath = std::filesystem::proximate(
            sourceLocation.file_name(), SOURCE_ROOT).string();

        auto stream = std::ostringstream{};
        stream <<
            relativeFilePath << ": " <<
            sourceLocation.function_name() << ": " <<
            sourceLocation.line() << ":" <<
            sourceLocation.column() << ": " <<
            value;
        _message = stream.str();
    }

    const char* what() const noexcept override
    {
        return _message.c_str();
    }

    template <Streamable T>
    Error& operator<<(T&& value) &
    {
        _message = appendToString(std::move(_message), std::forward<T>(value));
        return *this;
    }

    template <Streamable T>
    Error operator<<(T&& value) &&
    {
        return Error{
            appendToString(std::move(_message), std::forward<T>(value))};
    }

private:
    template <Streamable T>
    static std::string appendToString(std::string&& string, T&& value)
    {
        auto stream =
            std::ostringstream{std::move(string), std::ios_base::ate};
        stream << std::forward<T>(value);
        return stream.str();
    }

    std::string _message;
};
