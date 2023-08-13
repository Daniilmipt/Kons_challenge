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
        if (fd) {
            struct stat fileStat{};
            if (fstat(fd, &fileStat) == -1) {
                perror("Error getting file size");
                close(fd);
                return 1;
            }
            size_t fileSize = fileStat.st_size;
            char *fileData = (char *) mmap(nullptr, fileSize, PROT_READ, MAP_SHARED, fd, 0);
            int count = formatCharFile(fileData, fileSize, word1, word2, distance);
            close(fd);
            munmap(fileData, fileSize);
            return count;
        }
        throw std::runtime_error("Wrong file path");
    }

private:
    static void updateCount(std::vector<char> &subvector, const std::string &word1, const std::string &word2,
                                     std::vector<int > &positionFirstWord, int distance,
                                     int &posW1, int &idW, int &count, int &offset){
        int iOffset = offset;
        if(!subvector.empty()){
            if(posW1 != -1){
                if (compareStringsByChar(&(*subvector.begin()), &(*subvector.end()), word2)){
                    for (int j = iOffset; j < positionFirstWord.size(); ++j){
                        idW - positionFirstWord[offset] - 1 > distance ? offset = j + 1 : ++count;
                    }
                    offset = iOffset;
                }
            }
            if (compareStringsByChar(&(*subvector.begin()), &(*subvector.end()), word1)){
                positionFirstWord.push_back(idW);
                posW1 = idW;
            }
            subvector.clear();
            ++idW;
        }
    }

    static bool compareStringsByChar(const char* begin, const char* end, const std::string &stringTempl){
        if (end - begin != stringTempl.size()) return false;

        for (int i = 0; i < stringTempl.size(); ++i){
            if (*(begin + i) != stringTempl[i]) return false;
        }
        return true;
    }

    static int formatCharFile(char* charSequence, size_t fileSize, const std::string &word1,
                               const std::string &word2, int distance){
        std::vector<char> charVector;
        std::vector<int> positionsWord1;
        int posW1 = -1, idW = 0, count = 0, offset = 0;

        for (int i = 0; i < fileSize;){
            char ch = charSequence[i];
            if (!regChar.contains(ch)){
                std::string pChar (charSequence + i, charSequence + i + 2);
                std::string pStr (charSequence + i, charSequence + i + 3);

                if (regStr.contains(pChar) || regStr.contains(pStr)){
                    updateCount(charVector, word1, word2, positionsWord1, distance, posW1, idW, count, offset);
                    regStr.contains(pChar) ? i += regStr.at(pChar) : i += regStr.at(pStr);
                }
                else{
                    if (isalpha(ch)){
                        charVector.push_back(charSequence[i++]);
                    }
                    else {
                        charVector.push_back(charSequence[i]);
                        charVector.push_back(charSequence[i + 1]);
                        i += 2;
                    }
                }
            }
            else{
                updateCount(charVector, word1, word2, positionsWord1, distance, posW1, idW, count, offset);
                i += regChar.at(ch);
            }
        }
        updateCount(charVector, word1, word2, positionsWord1, distance, posW1, idW, count, offset);

        return count;
    }
};

#endif //KONS_CHALLENGE_CharParser_H
