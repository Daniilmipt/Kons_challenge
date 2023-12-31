#ifndef KONS_CHALLENGE_STRINGPARSER_H
#define KONS_CHALLENGE_STRINGPARSER_H
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include"../Parser.cpp"

class StringParser : public Parser{
public:
    using Parser::Parser;

    int parse(const std::string &word1, const std::string &word2, const int &distance) const override{
        std::fstream file;
        std::vector<int> positionsWord1;
        std::vector<std::string> stringVector;
        file.open(file_path, std::fstream::in);

        if (file.is_open()) {
            std::string str;
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
        std::exit(EXIT_SUCCESS);
    }

private:
    static void vectorByString(std::vector<char> &subCharVector, std::vector<std::string> &stringVector){
        if (!subCharVector.empty()) {
            std::string substring(subCharVector.begin(), subCharVector.end());
            stringVector.push_back(substring);
            subCharVector.clear();
        }
    }


    /*
     * Проверяем пары
     * word - прочитанное слово
     * positionFirstWord - вектор из индексов вхождений первого слова в исходный файл
     * idW - позиция слова в файле
    */
    static void updateCount(const std::string &word, const std::string &word1, const std::string &word2,
                            std::vector<int> &positionFirstWord, const int &distance,
                            const int &idW, int &count, int &offset){
        const int iOffset = offset;
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


    // Проверяем слова на совпадение
    static bool compareStringsByString(const std::string &word, const std::string &stringTempl){
        if (word.size() != stringTempl.size()) return false;

        for (int i = 0; i < stringTempl.size(); ++i){
            if (word[i] != stringTempl[i]) return false;
        }
        return true;
    }


    /*
     * Разбиваем строку на слова
     * charSequence - последовательность символов из файла
     * distance - максимально возможное расстояние между словами
    */
    static std::vector<std::string> formatString(const std::string &str){
        // вектор из символов слова
        std::vector<char> subCharVector;
        std::vector<std::string> stringVector;
        for (int i = 0; i < str.size();){
            const char ch = str[i];
            if (!regChar.count(ch)){
                // выделяем строку из следующих двух символов
                const std::string pChar (str.begin() + i, str.begin() + i + 2);
                // выделяем строку из следующих трех символов
                const std::string pStr (str.begin() + i, str.begin() + i + 3);

                if (regStr.find(pChar) != regStr.end() || regStr.find(pStr) != regStr.end()){
                    vectorByString(subCharVector, stringVector);
                    regStr.find(pChar) != regStr.end() ? i += regStr.at(pChar) : i += regStr.at(pStr);
                }
                else{
                    // символ может находиться внутри слова
                    if (regNoSkip.count(ch)){
                        // если вектор пустой, значит слова не было и символ сам по себе -> игнорируем его
                        if (!subCharVector.empty()) subCharVector.push_back(ch);
                        ++i;
                    }
                    else if (isalpha(ch) || isdigit(ch)){
                        subCharVector.push_back(ch);
                        ++i;
                    }
                    else {
                        subCharVector.insert(subCharVector.end(), str.begin() + i, str.begin() + i + 2);
                        i += 2;
                    }
                }
            }
            else{
                if (ch == ' ' || ch == '\n' || ch == '\t')
                    vectorByString(subCharVector, stringVector);
                ++i;
            }
        }
        vectorByString(subCharVector, stringVector);
        return stringVector;
    }

};

#endif //KONS_CHALLENGE_STRINGPARSER_H