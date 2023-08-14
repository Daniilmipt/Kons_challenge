//
// Created by daniil on 12.08.23.
//
#ifndef KONS_CHALLENGE_PARSER_H
#define KONS_CHALLENGE_PARSER_H
#include <string>
#include <map>
#include <utility>


class Parser{
public:
    explicit Parser(std::string file_path): file_path(std::move(file_path)){}
    virtual int parse(const std::string &word1, const std::string &word2, int distance) const = 0;
protected:
    const std::string file_path;
    static const std::map<char, int> regChar;
    static const std::map<std::string, int> regStr;
    static const std::map<char, int> regNoSkip;
};

const std::map<char, int> Parser::regChar = {{' ', 1}, {'!', 2}, {'@', 2}, {'#', 2}, {'$', 2}, {'%', 2}, {'^', 2}, {'&', 2}, {'*', 2}, {'(', 1}, {')', 1}, {'[', 1}, {']', 1}, {'{', 1}, {'}', 1}, {':', 2}, {'\"', 2}, {'<', 2}, {'>', 2}, {'?', 2}, {'/', 2}, {'.', 2}, {',', 2}, {'\'', 2}, {';', 2}, {'_', 2}, {'+', 2}, {'=', 2}, {'0', 1}, {'9', 1}, {'8', 1}, {'7', 1}, {'6', 1}, {'5', 1}, {'4', 1}, {'3', 1}, {'2', 1}, {'1', 1}, {'~', 2}, {'`', 2}, {'\\', 2}, {'|', 2}, {'\n', 1}};
const std::map<std::string, int> Parser::regStr = {{"»", 2}, {"«", 2}, {"№", 3}, {"—", 3}};
const std::map<char, int> Parser::regNoSkip = {{'-', 1}};

#endif //KONS_CHALLENGE_PARSER_H
