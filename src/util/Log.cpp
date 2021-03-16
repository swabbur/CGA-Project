#include <util/Log.hpp>

Log::Log(std::ostream & output) : output(output) {}

Log::~Log() = default;

void Log::print(std::string const & line) {
    output << line << "\u001b[0m" << std::endl;
}

void Log::trace(std::string const & message) {
    print("\u001b[37m" + message);
}

void Log::info(std::string const & message) {
    print(message);
}

void Log::debug(std::string const & message) {
    print("\u001b[34m" + message);
}

void Log::warning(std::string const & message) {
    print("\u001b[33m" + message);
}

void Log::error(std::string const & message) {
    print("\u001b[31m" + message);
}

void Log::fatal(std::string const & message) {
    print("\u001b[1;31m" + message);
    std::exit(1);
}