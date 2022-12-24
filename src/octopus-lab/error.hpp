#pragma once

#include <build-info.hpp>

#include <exception>
#include <experimental/source_location>
#include <filesystem>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

template <class T>
concept Streamable = requires(std::ostream output, T value)
{
    {output << value} -> std::same_as<std::ostream&>;
};

class Error : public std::exception {
public:
    using SourceLocation = std::experimental::source_location;

    explicit Error(
        std::string value = "",
        SourceLocation sourceLocation = SourceLocation::current())
        : _message(std::move(value))
        , _sourceLocation(sourceLocation)
    { }

    const char* what() const noexcept override
    {
        if (_cache.empty()) {
            auto relativeFilePath = std::filesystem::proximate(
                _sourceLocation.file_name(), bi::SOURCE_ROOT).string();

            auto stream = std::ostringstream{};
            stream <<
                relativeFilePath << ":" <<
                _sourceLocation.line() << ":" <<
                _sourceLocation.column() << " (" <<
                _sourceLocation.function_name() << ") " <<
                _message;
            _cache = stream.str();
        }
        return _cache.c_str();
    }

    template <Streamable T>
    Error& operator<<(T&& value) &
    {
        _cache.clear();
        _message = appendToString(std::move(_message), std::forward<T>(value));
        return *this;
    }

    template <Streamable T>
    Error operator<<(T&& value) &&
    {
        return Error{
            appendToString(std::move(_message), value),
            _sourceLocation};
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
    SourceLocation _sourceLocation;
    mutable std::string _cache;
};
