#include "StringParser.cpp"
#include "CharParser.cpp"
#include <fstream>
#include <chrono>
using namespace std::chrono;

int main(){

//    StringParser parser("/home/daniil/CLionProjects/Kons_challenge/input.txt");
//    std::string word1 = "практики";
//    std::string word2 = "приложений";
//    int distance = 100;
//    unsigned count = parser.parse(word1, word2, distance);
//    std::cout << "ans: " << count << "\n";


    for (unsigned i = 0; i < 3; ++i) {
        auto start = high_resolution_clock::now();

        CharParser parser ("/home/daniil/CLionProjects/Kons_challenge/input.txt");
        std::string word1 = "Курс";
        std::string word2 = "состоит";
        int distance = 6;

        if (distance < 0){
            throw std::runtime_error("Distance between words must be non-negative");
        }
        unsigned count = parser.parse(word1, word2, distance);
        std::cout << "ans: " << count << "\n";

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout << duration.count() << std::endl;
    }



}