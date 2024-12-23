#pragma once

#include <fstream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "exception.h"

struct Config {
    char sepCol;
    char sepRow;
    char escapeChar;

    Config(char sepCol = ',', char sepRow = '\n', char escapeChar = '"') {
        this->sepCol = sepCol;
        this->sepRow = sepRow;
        this->escapeChar = escapeChar;
    }
};


template <typename... args>
class csvParser {
private:
    std::ifstream& file;
    size_t linesToSkip;
    Config& config;

public:
    class Iterator {
    private:
        std::ifstream* file;
        size_t linesToSkip;
        size_t curLine;
        Config& config;
        std::tuple<args...> line;


        std::vector<std::string> splitLine(std::string str) {
            std::vector<std::string> res;
            std::string tmp;
            bool escapeCharacterMet;

            escapeCharacterMet = false;

            for (char c : str) {

                if (c == this->config.escapeChar) {
                    escapeCharacterMet = !escapeCharacterMet;
                }
                else if (c != this->config.sepCol) {
                    tmp += c;
                }
                else {
                    res.push_back(tmp);
                    tmp.clear();
                }
            }

            res.push_back(str.substr(str.rfind(this->config.sepCol) + 1, str.length()));

            return res;
        }


        void readLine() {
            std::string tmp;

            for (char c = file->get(); c != this->config.sepRow && !file->eof(); c = file->get()) {
                tmp += c;
            }

            this->line = parseLine(tmp);
        }


        std::tuple<args...> parseLine(std::string line) {
            std::vector<std::string> splittedLine = splitLine(line);

            if (splittedLine.size() != sizeof...(args)) {
                throw RuntimeException(std::string("csvParser::Iterator::parseLine"), std::string("Bad number of columns in line: ") + std::to_string(this->curLine));
            }

            return createTuple(splittedLine, std::index_sequence_for<args...>{});
        }


        template<std::size_t... Indices>
        std::tuple<args...> createTuple(const std::vector<std::string>& splittedStr, std::index_sequence<Indices...>) {
            return std::make_tuple(fitToType<args>(splittedStr[Indices])...);
        }


        template<typename T>
        T fitToType(const std::string& val) {
            if (val.empty()) {
                if constexpr (std::is_arithmetic_v<T>) {
                    return T();
                }
                else if constexpr  (std::is_same_v<std::string, T>) {
                    return std::string("");
                }
                else {
                    throw InvalidArgumentException(std::stirng("csvParser::Iterator::firToType"), std::string("Can not convert empty val to such type in line: ") + std::to_string(curLine));
                }
            }
            else {
                if constexpr (std::is_same_v<std::string, T>) {
                    return val;
                }
                else {
                    T value;
                    std::istringstream strStream(val);
                    strStream >> value;

                    if (strStream.fail()) {
                        throw InvalidArgumentException(std::string("csvParser::Iterator::firToType"), std::string("Failed to convert to type in line: ") + std::to_string(curLine));
                    }

                    return value;
                }
            }
            
        }


    public:

        Iterator(std::ifstream* file = nullptr, size_t linesToSkip = 0, Config& conf = Config()) : file(file), linesToSkip(linesToSkip), config(conf) {
            this->curLine = linesToSkip;

            if (this->file) {
                this->file->seekg(0, std::ios::beg);

                std::string tmp;

                for (size_t i = 0; i < linesToSkip; i++) {
                    std::getline(*file, tmp);
                }

                readLine();
            }
            else {
               this->file = nullptr;
            }
        }


        std::tuple<args...> operator*() {
            return this->line;
        }


        Iterator& operator++() {
            if (this->file && !file->eof()) {
                readLine();
                this->curLine++;
                return *this;
            }

            this->file = nullptr;
            return *this;
        }


        bool operator==(const Iterator& iter) {
            return this->file == iter.file;
        }


        bool operator!=(const Iterator& iter) {
            return !(*this == iter);
        }

    };


    csvParser(std::ifstream& file, size_t linesToSkip = 0, Config& conf = Config()) : file(file), linesToSkip(linesToSkip), config(conf) {

    }


    Iterator begin() {
        return Iterator(&(this->file), this->linesToSkip, this->config);
    }


    Iterator end() {
        return Iterator();
    }
};


template<typename Tuple, std::size_t Index>
struct TuplePrinter
{
    static void print(std::ostream& os, const Tuple& tuple)
    {
        TuplePrinter<Tuple, Index - 1>::print(os, tuple);
        os << ", " << std::get<Index>(tuple);
    }
};


template<typename Tuple>
struct TuplePrinter<Tuple, 0>
{
    static void print(std::ostream& os, const Tuple& tuple) { os << std::get<0>(tuple); }
};


template<typename... args>
std::ostream& operator<<(std::ostream& os, const std::tuple<args...>& tuple)
{
    os << "(";
    if constexpr (sizeof...(args) > 0)
        TuplePrinter<std::tuple<args...>, sizeof...(args) - 1>::print(os, tuple);
    os << ")";
    return os;
}