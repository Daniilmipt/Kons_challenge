#include "StringParser.cpp"
#include "CharParser.cpp"
#include <fstream>
#include <chrono>
using namespace std::chrono;

bool is_number(const std::string& str)
{
    std::string::const_iterator it = str.begin();
    while (it != str.end() && std::isdigit(*it)) ++it;
    return !str.empty() && it == str.end();
}

int main(int argc, char* argv[]){

    if (argc < 5){
        std::cout << "Недостаточное число параметров. Должно быть 4 аргумента";
        std::exit(0);
    }
    if (argc > 5){
        std::cout << "Большое число аргументов. Должно быть 4 аргумента";
        std::exit(0);
    }

    std::string filePath = argv[1];
    std::string word1 = argv[2];
    if (is_number(word1)){
        std::cout << "Первое слово не должно быть числом";
        std::exit(0);
    }
    std::string word2 = argv[3];
    if (is_number(word2)){
        std::cout << "Второе слово не должно быть числом";
        std::exit(0);
    }
    std::string strDistance = argv[4];
    if (!is_number(strDistance) || std::stoi(strDistance) < 0){
        std::cout << "Расстояние между словами должно быть целым неотрицательным числом";
        std::exit(0);
    }
    int distance = std::stoi(strDistance);

    CharParser parser(filePath);
    int count = parser.parse(word1, word2, distance);
    std::cout << count;
}