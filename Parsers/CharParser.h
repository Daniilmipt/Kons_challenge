#ifndef KONS_CHALLENGE_CharParser_H
#define KONS_CHALLENGE_CharParser_H
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

class CharParser : public Parser{
public:
    using Parser::Parser;

    int parse(const std::string &word1, const std::string &word2, int distance) const override{
        int fd = open(file_path.c_str(), O_RDONLY);
        if (fd != -1) {
            struct stat fileStat{};
            if (fstat(fd, &fileStat) == -1) {
                close(fd);
                std::cout << "Ошибка при получении размера файла";
                std::exit(0);
            }
            // находим размер исходного файла
            size_t fileSize = fileStat.st_size;
            char *fileData = (char *) mmap(nullptr, fileSize, PROT_READ, MAP_SHARED, fd, 0);
            int count = formatCharFile(fileData, fileSize, word1, word2, distance);
            close(fd);
            munmap(fileData, fileSize);
            return count;
        }
        std::cout << "Неправильный путь к файлу";
        std::exit(0);
    }

private:
    /*
     * Проверяем пары
     * subvector - вектор из символов слова
     * positionFirstWord - вектор из индексов вхождений первого слова в исходный файл
     * idW - позиция слова в файле
    */
    static void updateCount(std::vector<char> &subvector, const std::string &word1, const std::string &word2,
                                     std::vector<int > &positionFirstWord, int distance,
                                     int &idW, int &count, int &offset){
        if(!subvector.empty()){
            int iOffset = offset;
            if(iOffset < positionFirstWord.size()
                && compareStringsByChar(&(*subvector.begin()), &(*subvector.end()), word2)){
                    for (int j = iOffset; j < positionFirstWord.size(); ++j){
                        idW - positionFirstWord[offset] - 1 > distance ? offset = j + 1 : ++count;
                    }
                    offset = iOffset;
            }
            if (compareStringsByChar(&(*subvector.begin()), &(*subvector.end()), word1)){
                positionFirstWord.push_back(idW);
            }
            subvector.clear();
            ++idW;
        }
    }


    // Проверяем слова на совпадение
    static bool compareStringsByChar(const char* begin, const char* end, const std::string &stringTempl){
        if (end - begin != stringTempl.size()) return false;

        for (int i = 0; i < stringTempl.size(); ++i){
            if (*(begin + i) != stringTempl[i]) return false;
        }
        return true;
    }

    /*
        charSequence - последовательность символов из файла
        distance - максимально возможное расстояние между словами
    */
    static int formatCharFile(char* charSequence, size_t fileSize, const std::string &word1,
                               const std::string &word2, int distance){
        // вектор из символов слова
        std::vector<char> charVector;
        // вектор, содержащий индексы вхождений первого слова в исходный файл
        std::vector<int> positionsWord1;
        // idW - позиция слова в файле; count - число пар, на расстоянии меньшем чем distance
        int idW = 0, count = 0, offset = 0;

        for (int i = 0; i < fileSize;){
            char ch = charSequence[i];
            if (!regChar.count(ch)){
                // выделяем строку из следующих двух символов
                std::string pChar (charSequence + i, charSequence + i + 2);
                // выделяем строку из следующих трех символов
                std::string pStr (charSequence + i, charSequence + i + 3);

                if (regStr.find(pChar) != regStr.end() || regStr.find(pStr) != regStr.end()){
                    updateCount(charVector, word1, word2, positionsWord1, distance, idW, count, offset);
                    regStr.find(pChar) != regStr.end() ? i += regStr.at(pChar) : i += regStr.at(pStr);
                }
                else{
                    if (regNoSkip.find(ch) != regNoSkip.end())
                        charVector.push_back(charSequence[i++]);
                    else {
                        charVector.insert(charVector.end(), charSequence + i, charSequence + i + 2);
                        i += 2;
                    }
                }
            }
            else{
                updateCount(charVector, word1, word2, positionsWord1, distance, idW, count, offset);
                ++i;
            }
        }
        updateCount(charVector, word1, word2, positionsWord1, distance, idW, count, offset);
        return count;
    }
};

#endif //KONS_CHALLENGE_CharParser_H
