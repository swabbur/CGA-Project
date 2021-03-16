#pragma once

#include <iostream>

class Log {

    std::ostream & output;

    void print(std::string const & line);

public:

    explicit Log(std::ostream & output);

    Log(Log const &) = delete;

    Log(Log &&) = delete;

    ~Log();

    void trace(std::string const & message);

    void info(std::string const & message);

    void debug(std::string const & message);

    void warning(std::string const & message);

    void error(std::string const & message);

    void fatal(std::string const & message);
};

inline Log LOG(std::cout); // NOLINT(cert-err58-cpp,cppcoreguidelines-interfaces-global-init)
