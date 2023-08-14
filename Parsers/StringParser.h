#ifndef KONS_CHALLENGE_STRINGPARSER_H
#define KONS_CHALLENGE_STRINGPARSER_H
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <set>
#include <regex>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include"../Parser.cpp"

class StringParser : public Parser{
public:
    using Parser::Parser;

    int parse(const std::string &word1, const std::string &word2, int distance) const override{
        std::fstream file;
        std::vector<int> positionsWord1;
        file.open(file_path, std::fstream::in);

        if (file.is_open()) {
            std::string str;
            std::vector<std::string> stringVector;
            int idW = 0, count = 0, offset = 0;

            while (file >> str) {
                stringVector = formatString(str);
                for (const std::string &word: stringVector) {
                    updateCount(word, word1, word2, positionsWord1, distance, idW, count, offset);
                    ++idW;
                }
            }
            file.close();
            return count;
        }
        std::cout << "Неправильный путь к файлу";
        std::exit(0);
    }

private:
    static void vectorByString(std::vector<char> &subCharVector, std::vector<std::string> &strVector){
        if (!subCharVector.empty()) {
            std::string substring(subCharVector.begin(), subCharVector.end());
            strVector.push_back(substring);
            subCharVector.clear();
        }
    }

    static void updateCount(const std::string &word, const std::string &word1, const std::string &word2,
                                       std::vector<int> &positionFirstWord, int distance,
                                       int &idW, int &count, int &offset){
        int iOffset = offset;
        if(iOffset < positionFirstWord.size() && compareStringsByString(word, word2)){
            for (int j = iOffset; j < positionFirstWord.size(); ++j){
                idW - positionFirstWord[offset] - 1 > distance ? offset = j + 1 : ++count;
            }
            offset = iOffset;
        }
        if (compareStringsByString(word, word1)){
            positionFirstWord.push_back(idW);
        }
    }

    static bool compareStringsByString(const std::string &word, const std::string &stringTempl){
        if (word.size() != stringTempl.size()) return false;

        for (int i = 0; i < stringTempl.size(); ++i){
            if (word[i] != stringTempl[i]) return false;
        }
        return true;
    }

    static std::vector<std::string> formatString(std::string str){
        std::vector<char> subCharVector;
        std::vector<std::string> strVector;

        for (int i = 0; i < str.size();){
            char ch = str[i];
            if (regChar.find(ch) == regChar.end()){
                std::string pChar (str.begin() + i, str.begin() + i + 2);
                std::string pStr (str.begin() + i, str.begin() + i + 3);

                if (regStr.find(pChar) != regStr.end() || regStr.find(pStr) != regStr.end()){
                    vectorByString(subCharVector, strVector);
                    regStr.find(pChar) != regStr.end() ? i += regStr.at(pChar) : i += regStr.at(pStr);
                }
                else{
                    if (regNoSkip.find(ch) != regNoSkip.end())
                        subCharVector.push_back(str[i++]);
                    else {
                        subCharVector.insert(subCharVector.end(), str.begin() + i, str.begin() + i + 2);
                        i += 2;
                    }
                }
            }
            else{
                vectorByString(subCharVector, strVector);
                i += regChar.at(ch);
            }
        }
        vectorByString(subCharVector, strVector);
        return strVector;
    }

};

#endif //KONS_CHALLENGE_STRINGPARSER_H
