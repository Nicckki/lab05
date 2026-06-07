
#include <iostream> //для ввода-вывода
#include <string>   //для работ со строками

int
main()
{
  std::string name; //переменная для хранения имени пользователя
  std::cout << "Enter your name: "; //запрос имени
  std::cin >> name; //ввод имени пользователя
  std::cout << "Hello world from " << name << "!" << std::endl; //приветсвие
  return 0; //успешное завершение программы
}
