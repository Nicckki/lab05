#include <print.hpp>
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <cstdio>

TEST(Print, InFileStream)
{
    std::string filepath = "/tmp/test_output.txt";
    std::string text = "Hello, GTest!";
    
    // Записываем в файл
    std::ofstream out{filepath};
    ASSERT_TRUE(out.is_open()) << "Failed to open file for writing";
    print(text, out);
    out.close();
    
    // Читаем из файла (читаем всю строку целиком)
    std::string result;
    std::ifstream in{filepath};
    ASSERT_TRUE(in.is_open()) << "Failed to open file for reading";
    std::getline(in, result);  // <-- используем getline вместо >>
    in.close();
    
    // Проверяем
    EXPECT_EQ(result, text);
    
    // Удаляем временный файл
    std::remove(filepath.c_str());
}

TEST(Print, ToStringStream)
{
    std::stringstream buffer;
    std::string text = "Testing stringstream";
    
    print(text, buffer);
    
    std::string result = buffer.str();
    EXPECT_EQ(result, text);
}
