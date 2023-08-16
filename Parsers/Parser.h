#ifndef KONS_CHALLENGE_PARSER_H
#define KONS_CHALLENGE_PARSER_H
#include <string>
#include <map>
#include <utility>


class Parser{
public:
    explicit Parser(std::string file_path): file_path(std::move(file_path)) {}

    virtual int parse(const std::string &word1, const std::string &word2, int distance) const = 0;
protected:
    const std::string file_path;
    static const std::set<char> regChar;
    static const std::map<std::string, int> regStr;
    static const std::map<char, int> regNoSkip;
};


const std::set<char> Parser::regChar = {
        ' ', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '[', ']',
        '{', '}', ':', '\"', '<', '>', '?', '/', '.', ',', '\'', ';',
        '_', '+', '=', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        '~', '`', '\\', '|', '\n'
};

const std::map<std::string, int> Parser::regStr = {
        {"»", 2}, {"«", 2}, {"№", 3}, {"—", 3}
};

const std::map<char, int> Parser::regNoSkip = {
        {'-', 1}
};

#endif //KONS_CHALLENGE_PARSER_H
