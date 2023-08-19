#ifndef KONS_CHALLENGE_CharParser_H
#define KONS_CHALLENGE_CharParser_H
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <set>
#include <regex>
#include"../Parser.cpp"

#ifdef _WIN32
#include <windows.h>
#include <cstring>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <tcl.h>
#include <fakesql.h>
#endif


class CharParser : public Parser{
public:
    using Parser::Parser;

    /*
     * открывает файл, отображает его в память, обрабатывает символьные данные, а затем закрывает файл
     */
    int parse(const std::string &word1, const std::string &word2, int distance) const override{
        auto [fileData, fileSize, fileMapping, fileHandle, fd] = openFileMmap(file_path);
        int count = formatCharFile(fileData, fileSize, word1, word2, distance);
        closeFile(fileData, fileSize, fileMapping, fileHandle, fd);
        return count;
    }

private:

    static void closeFile(char* fileData, size_t fileSize, HANDLE fileMapping, HANDLE fileHandle, int fd) {
        #ifdef _WIN32
            UnmapViewOfFile(fileData);
            CloseHandle(fileMapping);
            CloseHandle(fileHandle);
        #else
            close(fd);
            munmap(fileData, fileSize);
        #endif
    }


    // Открываем файл на чтение. В зависимости от ОС будет выполняться свой код открытия файла
    static std::tuple<char *, size_t, HANDLE, HANDLE, int> openFileMmap(const std::string &file_path) {
        size_t fileSize;
        char *fileData;

        #ifdef _WIN32
            HANDLE fileHandle = CreateFileA(file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (fileHandle == INVALID_HANDLE_VALUE) {
                std::cout << "Неправильный путь к файлу";
                std::exit(EXIT_SUCCESS);
            }

            HANDLE fileMapping = CreateFileMapping(fileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
            if (fileMapping == nullptr) {
                std::cout << "Ошибка при получении размера файла";
                std::exit(EXIT_SUCCESS);
            }

            LPVOID fileDataRaw = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
            if (fileDataRaw == nullptr) {
                std::cout << "Ошибка при получении размера файла";
                std::exit(EXIT_SUCCESS);
            }
            SetConsoleOutputCP(CP_UTF8);
            fileData = static_cast<char*>(fileDataRaw);

            LARGE_INTEGER fileSizeRaw;
            GetFileSizeEx(fileHandle, &fileSizeRaw);
            fileSize = fileSizeRaw.QuadPart;
            return{fileData, fileSize, fileMapping, fileHandle, 5};
        #else
            int fd = open(file_path.c_str(), O_RDONLY);
            if (fd != -1) {
                struct stat fileStat{};
                if (fstat(fd, &fileStat) == -1) {
                    close(fd);
                    std::cout << "Ошибка при получении размера файла";
                    std::exit(EXIT_SUCCESS);
                }
                // находим размер исходного файла
                fileSize = fileStat.st_size;
                fileData = (char *) mmap(nullptr, fileSize, PROT_READ, MAP_SHARED, fd, 0);
                HANDLE fileMapping;
                HANDLE fileHandle;
                return{fileData, fileSize, fileMapping, fileHandle, fd};
            }
            close(fd);
            std::cout << "Неправильный путь к файлу";
            std::exit(EXIT_SUCCESS);
        #endif
    }



    /*
     * Проверяем пары
     * charVector - вектор из символов слова
     * positionFirstWord - вектор из индексов вхождений первого слова в исходный файл
     * idW - позиция текущего слова в файле
    */
    static void updateCount(std::vector<char> &charVector, const std::string &word1, const std::string &word2,
                            std::vector<int > &positionFirstWord, int distance,
                            int &idW, int &count, int &offset){
        if(!charVector.empty()){
            int iOffset = offset;
            if(iOffset < positionFirstWord.size()
               && compareStringsByChar(&(*charVector.begin()), &(*charVector.end()), word2)){
                for (int j = iOffset; j < positionFirstWord.size(); ++j){
                    idW - positionFirstWord[offset] - 1 > distance ? offset = j + 1 : ++count;
                }
                offset = iOffset;
            }
            if (compareStringsByChar(&(*charVector.begin()), &(*charVector.end()), word1))
                positionFirstWord.push_back(idW);
            charVector.clear();
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
     * Посимвольно итерируемся по файлу
     * charSequence - последовательность символов из файла
     * distance - максимально возможное расстояние между словами
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
                    i += regStr.find(pChar) != regStr.end() ? regStr.at(pChar) : regStr.at(pStr);
                }
                else{
                    // символ может находиться внутри слова
                    if (regNoSkip.count(ch)){
                        // если вектор пустой, значит слова не было и символ сам по себе -> игнорируем его
                        if (!charVector.empty())
                            charVector.push_back(ch);
                        ++i;
                    }
                    if (isalpha(ch) || isdigit(ch)){
                        charVector.push_back(ch);
                        ++i;
                    }
                    else {
                        charVector.insert(charVector.end(), charSequence + i, charSequence + i + 2);
                        i += 2;
                    }
                }
            }
            else{
                if (ch == ' ' || ch == '\n' || ch == '\t')
                    updateCount(charVector, word1, word2, positionsWord1, distance, idW, count, offset);
                ++i;
            }
        }
        updateCount(charVector, word1, word2, positionsWord1, distance, idW, count, offset);
        return count;
    }
};
#endif //KONS_CHALLENGE_CharParser_H
