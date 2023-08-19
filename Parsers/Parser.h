#ifndef KONS_CHALLENGE_PARSER_H
#define KONS_CHALLENGE_PARSER_H
#include <string>
#include <map>
#include <utility>


class Parser{
public:
    explicit Parser(std::string file_path): file_path(std::move(file_path)) {}

    virtual int parse(const std::string &word1, const std::string &word2, const int &distance) const = 0;
protected:
    const std::string file_path;
    static const std::set<char> regChar;
    static const std::map<std::string, int> regStr;
    static const std::set<char> regNoSkip;
};


const std::set<char> Parser::regChar = {
        ' ', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '[', ']',
        '{', '}', ':', '\"', '<', '>', '?', '/', '.', ',', '\'', ';',
        '_', '+', '=', '~', '`', '\\', '|', '\n', '\r', '\t'
};

const std::map<std::string, int> Parser::regStr = {
        {"»", 2}, {"«", 2}, {"№", 3}, {"—", 3}, {"\r\n", 2}
};

const std::set<char> Parser::regNoSkip = {'-'};
#endif //KONS_CHALLENGE_PARSER_H