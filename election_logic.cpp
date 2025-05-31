#include "election_logic.hpp" // Включаем заголовочный файл с объявлениями
#include <iostream> // Для отладки, если нужна
#include <map>      // Для std::map
#include <vector>   // Для std::vector
#include <string>   // Для std::string
#include <algorithm> // Для std::find

// ---
// Метод Борда
// ---
std::string solveBorda(int n, int k, const std::vector<std::vector<std::string>>& votes, const std::vector<std::string>& candidates) {
    std::map<std::string, int> scores;
    for (const auto& candidate : candidates) {
        scores[candidate] = 0;
    }

    for (const auto& vote : votes) {
        // Проверка, что длина голосования соответствует n
        if (vote.size() != n) {
            // В идеале здесь должна быть более надежная обработка ошибок или исключение
            // Для целей этой задачи, предполагаем валидный ввод после проверки в main.cpp
            continue;
        }
        for (int i = 0; i < n; ++i) {
            // Проверка, что кандидат из голосования существует в списке кандидатов
            bool found_candidate = false;
            for (const auto& c : candidates) {
                if (c == vote[i]) {
                    found_candidate = true;
                    break;
                }
            }
            if (found_candidate) {
                scores[vote[i]] += (n - 1 - i); // n-1 баллов за 1-е место, 0 за последнее
            } else {
                // Неизвестный кандидат в голосовании. Игнорировать или обработать ошибку.
                // Для простоты, игнорируем баллы для этого кандидата.
            }
        }
    }

    std::string winner = "";
    int maxScore = -1;

    // В случае ничьей по Борда, возвращается тот, кто первый в алфавитном порядке,
    // так как std::map итерирует ключи в отсортированном порядке.
    // Если требуется другое правило разрешения ничьих, его нужно реализовать здесь.
    for (const auto& pair : scores) {
        if (pair.second > maxScore) {
            maxScore = pair.second;
            winner = pair.first;
        }
        // Если scores.empty() (неверный ввод), winner останется пустым.
    }
    return winner;
}

// ---
// Метод Кондорсе
// ---
std::string solveCondorcet(int n, int k, const std::vector<std::vector<std::string>>& votes, const std::vector<std::string>& candidates) {
    if (n == 0) return "Не существует победителя по Кондорсе"; // Защита от деления на ноль или пустых данных

    for (const auto& candidateA : candidates) {
        bool isCondorcetWinner = true;
        for (const auto& candidateB : candidates) {
            if (candidateA == candidateB) {
                continue;
            }

            int winsA_vs_B = 0;
            int winsB_vs_A = 0;

            for (const auto& vote : votes) {
                // Находим позиции A и B в текущем голосовании
                // Используем std::find для поиска элемента в векторе
                auto itA = std::find(vote.begin(), vote.end(), candidateA);
                auto itB = std::find(vote.begin(), vote.end(), candidateB);

                if (itA != vote.end() && itB != vote.end()) { // Убедимся, что оба кандидата найдены в голосовании
                    int posA = std::distance(vote.begin(), itA);
                    int posB = std::distance(vote.begin(), itB);

                    if (posA < posB) {
                        winsA_vs_B++;
                    } else if (posB < posA) {
                        winsB_vs_A++;
                    }
                    // Если posA == posB, это означает, что кто-то ошибся с вводом (дубликаты),
                    // но валидация ввода должна была это отловить.
                } else {
                    // Это может произойти, если в голосовании не было всех кандидатов,
                    // хотя валидация ввода должна это предотвратить.
                    // Для надежности можно здесь выкинуть исключение или проигнорировать голос.
                }
            }

            // Если B выиграл у A или была ничья (в парном сравнении), то A не является победителем Кондорсе
            if (winsB_vs_A >= winsA_vs_B) {
                isCondorcetWinner = false;
                break; // A не победитель Кондорсе, переходим к следующему candidateA
            }
        }
        if (isCondorcetWinner) {
            return candidateA; // Нашли победителя Кондорсе
        }
    }
    return "Не существует победителя по Кондорсе"; // Никто не выиграл у всех остальных
}