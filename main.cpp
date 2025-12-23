/*
Epifancev Artem
st142163@student.spbu.ru
LabWork1
*/

#include "bmp_image.h"
#include <iostream>
#include <string>

int main() {
    BMP image;
    std::string file_name = "";
    std::cout << "Введите путь к файлу. Для работы с файлом по умолчанию введите 0\n";
    std::cin >> file_name;
    if (file_name == "0") {
        image.Load("sample.bmp");
    } else {
        image.Load(file_name);
    }
    
    image.RotateCW().Save("clockwise.bmp");
    std::cout << "Сохранено: clockwise.bmp\n";
    
    image.RotateCCW().Save("counter_clockwise.bmp");
    std::cout << "Сохранено: counter_clockwise.bmp\n";
    
    image.RotateCW().GaussianFilter().Save("gaussian.bmp");
    std::cout << "Сохранено: gaussian.bmp\n";
    
    return 0;
}
