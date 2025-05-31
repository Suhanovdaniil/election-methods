#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <limits> // Для std::numeric_limits
#include <sstream> // Для std::stringstream
#include "election_logic.hpp" // Включаем заголовочный файл с объявлениями функций

// ---
// Функция для вывода результатов
// ---
void printResults(const std::string& methodName, const std::string& winner, const std::string& additionalComment = "") {
    std::cout << "\n--- " << methodName << " ---" << std::endl;
    std::cout << "Победитель: " << winner << std::endl;
    if (!additionalComment.empty()) {
        std::cout << additionalComment << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // setlocale(LC_ALL, "ru_RU.UTF-8"); // Может понадобиться для корректного отображения кириллицы в некоторых терминалах

    int n, k;

    // Ввод количества кандидатов (n)
    std::cout << "Введите количество кандидатов (n): ";
    while (!(std::cin >> n) || n <= 0) {
        std::cout << "Некорректный ввод. Введите положительное целое число для n: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера после ввода числа

    // Ввод имен кандидатов
    std::vector<std::string> candidates(n);
    std::cout << "Введите имена кандидатов (по одному на строку):" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << "Кандидат " << i + 1 << ": ";
        std::string name;
        std::getline(std::cin, name);
        while (name.empty() || std::all_of(name.begin(), name.end(), isspace)) { // Проверка на пустые строки
            std::cout << "Имя кандидата не может быть пустым. Введите Кандидат " << i + 1 << ": ";
            std::getline(std::cin, name);
        }
        candidates[i] = name;
    }

    // Ввод количества избирателей (k)
    std::cout << "Введите количество избирателей (k): ";
    while (!(std::cin >> k) || k <= 0) {
        std::cout << "Некорректный ввод. Введите положительное целое число для k: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера после ввода числа

    // Ввод голосов (ранжирований) избирателей
    std::vector<std::vector<std::string>> votes(k, std::vector<std::string>(n));
    std::cout << "\nВведите ранжирование кандидатов для каждого избирателя." << std::endl;
    std::cout << "Используйте пробелы между именами кандидатов. Например: Петя Вася Оля" << std::endl;
    std::cout << "Порядок важен: первый кандидат - самый предпочтительный." << std::endl;

    for (int i = 0; i < k; ++i) {
        bool validInput = false;
        while (!validInput) {
            std::cout << "\nГолос избирателя " << i + 1 << " (введите " << n << " имен, разделенных пробелами): ";
            std::string line;
            std::getline(std::cin, line);

            std::stringstream ss(line);
            std::string candidateName;
            std::vector<std::string> currentVote;

            while (ss >> candidateName) {
                currentVote.push_back(candidateName);
            }

            if (currentVote.size() != n) {
                std::cout << "Ошибка: Вы ввели " << currentVote.size() << " имен, а ожидалось " << n << ". Попробуйте снова." << std::endl;
                continue; // Повторить ввод для текущего избирателя
            }

            // Проверка на то, что все введенные имена являются существующими кандидатами
            bool allCandidatesValid = true;
            for (const auto& name : currentVote) {
                bool found = false;
                for (const auto& c : candidates) {
                    if (c == name) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    std::cout << "Ошибка: Кандидат '" << name << "' не найден в списке введенных кандидатов. Попробуйте снова." << std::endl;
                    allCandidatesValid = false;
                    break;
                }
            }
            if (!allCandidatesValid) {
                continue; // Повторить ввод
            }

            // Проверка на дубликаты в ранжировании текущего избирателя
            std::vector<std::string> tempVote = currentVote; // Создаем копию для сортировки
            std::sort(tempVote.begin(), tempVote.end());
            if (std::unique(tempVote.begin(), tempVote.end()) - tempVote.begin() != n) {
                std::cout << "Ошибка: В ранжировании присутствуют повторяющиеся кандидаты. Каждый кандидат должен быть упомянут один раз. Попробуйте снова." << std::endl;
                continue; // Повторить ввод
            }

            votes[i] = currentVote;
            validInput = true; // Ввод валиден, выходим из внутреннего цикла
        }
    }

    // ---
    // Расчет и вывод победителей
    // ---
    std::string bordaWinner = solveBorda(n, k, votes, candidates);
    std::string condorcetWinner = solveCondorcet(n, k, votes, candidates);

    printResults("Метод Борда", bordaWinner);
    printResults("Метод Кондорсе", condorcetWinner,
                  (condorcetWinner == "Не существует победителя по Кондорсе" ? "В данном случае не существует победителя по Кондорсе (возможно, парадокс Кондорсе)." : ""));

    if (bordaWinner != condorcetWinner && condorcetWinner != "Не существует победителя по Кондорсе") {
        std::cout << "Комментарий: Результаты разнятся. Разные алгоритмы могут давать разные результаты в зависимости от специфики голосования." << std::endl;
    } else if (bordaWinner != condorcetWinner && condorcetWinner == "Не существует победителя по Кондорсе") {
        std::cout << "Комментарий: Результаты разнятся. Метод Борда выбрал победителя, тогда как метод Кондорсе не смог определить однозначного победителя из-за циклической зависимости или парадокса Кондорсе." << std::endl;
    }

    return 0;
}