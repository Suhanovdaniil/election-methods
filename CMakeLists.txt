cmake_minimum_required(VERSION 3.10) # Минимальная требуемая версия CMake
project(ElectionMethods CXX)          # Название проекта и язык

set(CMAKE_CXX_STANDARD 17)            # Используем стандарт C++17
set(CMAKE_CXX_STANDARD_REQUIRED True) # Требуем C++17

# Находим Google Test, установленный через Homebrew
# Homebrew устанавливает конфиг-файлы для find_package
find_package(GTest CONFIG REQUIRED)

# Если GTest найден, то переменные GTest_INCLUDE_DIRS и GTest_LIBRARIES
# будут установлены. Для GTest Config mode, они могут быть представлены через target-ы.

# Добавляем исполняемый файл для основной программы (с консольным вводом)
add_executable(election_app main.cpp election_logic.cpp
        election_logic.hpp
        election_logic.cpp)

# Добавляем исполняемый файл для тестов
# test.cpp содержит main() от Google Test, поэтому мы используем его как точку входа.
add_executable(election_tests test.cpp election_logic.cpp)

# Связываем библиотеки Google Test с нашим исполняемым файлом тестов
target_link_libraries(election_tests
        GTest::gtest        # Основная библиотека Google Test
        GTest::gtest_main   # Библиотека, предоставляющая main() функцию для тестов
        pthread             # Важно для macOS/Linux, так как GTest может использовать потоки
)

# Указываем, где искать заголовочные файлы для тестов, если find_package не сделал это автоматически.
# Обычно GTest::gtest и GTest::gtest_main уже содержат эту информацию.
# Но это безопасно добавить.
target_include_directories(election_tests PRIVATE ${GTest_INCLUDE_DIRS})