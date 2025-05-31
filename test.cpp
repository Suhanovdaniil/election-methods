#include "gtest/gtest.h"
#include "election_logic.hpp" // Включаем заголовочный файл с объявлениями функций

// ---
// Тесты для Метод Борда
// ---

TEST(BordaMethodTest, SimpleCase) {
    int n = 3;
    int k = 5;
    std::vector<std::string> candidates = {"Петя", "Вася", "Оля"};
    std::vector<std::vector<std::string>> votes = {
        {"Петя", "Вася", "Оля"},
        {"Петя", "Вася", "Оля"},
        {"Петя", "Оля", "Вася"},
        {"Вася", "Петя", "Оля"},
        {"Вася", "Петя", "Оля"}
    };
    // Петя: 2*3 + 1*0 + 0*0 = 6
    // Вася: 2*2 + 1*2 + 0*1 = 6
    // Оля:  2*0 + 1*1 + 0*4 = 1
    // Оба "Петя" и "Вася" набирают 6 баллов.
    // Из-за того, что std::map итерирует ключи в алфавитном порядке,
    // "Петя" (П) будет раньше "Васи" (В).
    ASSERT_EQ(solveBorda(n, k, votes, candidates), "Петя");
}

TEST(BordaMethodTest, CondorcetParadoxScenario) {
    int n = 3;
    int k = 3;
    std::vector<std::string> candidates = {"A", "B", "C"};
    std::vector<std::vector<std::string>> votes = {
        {"A", "B", "C"},
        {"B", "C", "A"},
        {"C", "A", "B"}
    };
    // Баллы по Борда:
    // A: (3-1)*1 (1-й голос) + (3-2)*1 (2-й голос) + (3-3)*1 (3-й голос) = 2 + 1 + 0 = 3
    // B: (3-1)*1 + (3-2)*1 + (3-3)*1 = 2 + 1 + 0 = 3
    // C: (3-1)*1 + (3-2)*1 + (3-3)*1 = 2 + 1 + 0 = 3
    // Все кандидаты набирают по 3 балла. Победитель будет зависеть от порядка в `std::map`.
    // В алфавитном порядке A, B, C. "A" будет первым.
    ASSERT_EQ(solveBorda(n, k, votes, candidates), "A");
}

TEST(BordaMethodTest, AllSamePreferences) {
    int n = 3;
    int k = 4;
    std::vector<std::string> candidates = {"Alpha", "Beta", "Gamma"};
    std::vector<std::vector<std::string>> votes = {
        {"Alpha", "Beta", "Gamma"},
        {"Alpha", "Beta", "Gamma"},
        {"Alpha", "Beta", "Gamma"},
        {"Alpha", "Beta", "Gamma"}
    };
    // Alpha: (3-1)*4 = 8
    // Beta:  (3-2)*4 = 4
    // Gamma: (3-3)*4 = 0
    ASSERT_EQ(solveBorda(n, k, votes, candidates), "Alpha");
}

TEST(BordaMethodTest, TwoCandidates) {
    int n = 2;
    int k = 5;
    std::vector<std::string> candidates = {"Кандидат1", "Кандидат2"};
    std::vector<std::vector<std::string>> votes = {
        {"Кандидат1", "Кандидат2"},
        {"Кандидат1", "Кандидат2"},
        {"Кандидат2", "Кандидат1"},
        {"Кандидат2", "Кандидат1"},
        {"Кандидат1", "Кандидат2"}
    };
    // Кандидат1: (2-1)*3 + (2-2)*2 = 3
    // Кандидат2: (2-1)*2 + (2-2)*3 = 2
    ASSERT_EQ(solveBorda(n, k, votes, candidates), "Кандидат1");
}

// ---
// Тесты для Метод Кондорсе
// ---

TEST(CondorcetMethodTest, SimpleCase) {
    int n = 3;
    int k = 5;
    std::vector<std::string> candidates = {"Петя", "Вася", "Оля"};
    std::vector<std::vector<std::string>> votes = {
        {"Петя", "Вася", "Оля"},
        {"Петя", "Вася", "Оля"},
        {"Петя", "Оля", "Вася"},
        {"Вася", "Петя", "Оля"},
        {"Вася", "Петя", "Оля"}
    };
    // Петя vs Вася: 3 (Петя) vs 2 (Вася) -> Петя выигрывает
    // Петя vs Оля: 4 (Петя) vs 1 (Оля) -> Петя выигрывает
    // Вася vs Оля: 4 (Вася) vs 1 (Оля) -> Вася выигрывает
    // Петя выигрывает у всех.
    ASSERT_EQ(solveCondorcet(n, k, votes, candidates), "Петя");
}

TEST(CondorcetMethodTest, CondorcetParadox) {
    int n = 3;
    int k = 3;
    std::vector<std::string> candidates = {"A", "B", "C"};
    std::vector<std::vector<std::string>> votes = {
        {"A", "B", "C"},
        {"B", "C", "A"},
        {"C", "A", "B"}
    };
    // Ручной анализ:
    // A vs B: A>B (Изб.1: A B C), B>A (Изб.2: B C A), A>B (Изб.3: C A B) -> A выигрывает (2:1)
    // B vs C: B>C (Изб.1: A B C), B>C (Изб.2: B C A), C>B (Изб.3: C A B) -> B выигрывает (2:1)
    // C vs A: C>A (Изб.1: A B C), C>A (Изб.2: B C A), C>A (Изб.3: C A B) -> C выигрывает (3:0)
    // ОШИБКА В МОЕМ ПРЕДЫДУЩЕМ РУЧНОМ АНАЛИЗЕ!
    // Давайте перепроверим Condorcet Paradox:
    // A vs B:
    // - Изб. 1: A (1) vs B (0)
    // - Изб. 2: A (0) vs B (1)
    // - Изб. 3: A (1) vs B (0) (потому что C>A>B)
    // Итого: A:2, B:1. A выигрывает у B.

    // B vs C:
    // - Изб. 1: B (1) vs C (0)
    // - Изб. 2: B (1) vs C (0)
    // - Изб. 3: B (0) vs C (1) (потому что C>A>B)
    // Итого: B:2, C:1. B выигрывает у C.

    // C vs A:
    // - Изб. 1: C (0) vs A (1)
    // - Изб. 2: C (1) vs A (0)
    // - Изб. 3: C (1) vs A (0)
    // Итого: C:2, A:1. C выигрывает у A.

    // Цикл: A выигрывает у B, B выигрывает у C, C выигрывает у A. Нет победителя Кондорсе.
    ASSERT_EQ(solveCondorcet(n, k, votes, candidates), "Не существует победителя по Кондорсе");
}

TEST(CondorcetMethodTest, DifferentResultsScenario) {
    int n = 4;
    int k = 5;
    std::vector<std::string> candidates = {"X", "Y", "Z", "W"};
    std::vector<std::vector<std::string>> votes = {
        {"X", "Y", "Z", "W"},
        {"X", "Y", "Z", "W"},
        {"Z", "W", "Y", "X"},
        {"W", "Z", "Y", "X"},
        {"Y", "X", "Z", "W"}
    };
    // Ручной анализ для Кондорсе:
    // Избиратели:
    // 1: X Y Z W
    // 2: X Y Z W
    // 3: Z W Y X
    // 4: W Z Y X
    // 5: Y X Z W

    // Проверяем Y:
    // Y vs X:
    // - Изб. 1: X>Y (0:1)
    // - Изб. 2: X>Y (0:1)
    // - Изб. 3: Y>X (1:0)
    // - Изб. 4: Y>X (1:0)
    // - Изб. 5: Y>X (1:0)
    // Итого: Y(3) vs X(2) -> Y выигрывает у X.

    // Y vs Z:
    // - Изб. 1: Y>Z (1:0)
    // - Изб. 2: Y>Z (1:0)
    // - Изб. 3: Z>Y (0:1)
    // - Изб. 4: Z>Y (0:1)
    // - Изб. 5: Y>Z (1:0)
    // Итого: Y(3) vs Z(2) -> Y выигрывает у Z.

    // Y vs W:
    // - Изб. 1: Y>W (1:0)
    // - Изб. 2: Y>W (1:0)
    // - Изб. 3: W>Y (0:1)
    // - Изб. 4: W>Y (0:1)
    // - Изб. 5: Y>W (1:0)
    // Итого: Y(3) vs W(2) -> Y выигрывает у W.

    // Y выигрывает у всех (X, Z, W). Y - победитель Кондорсе.
    ASSERT_EQ(solveCondorcet(n, k, votes, candidates), "Y");
}

TEST(CondorcetMethodTest, AllSamePreferences) {
    int n = 3;
    int k = 4;
    std::vector<std::string> candidates = {"Alpha", "Beta", "Gamma"};
    std::vector<std::vector<std::string>> votes = {
        {"Alpha", "Beta", "Gamma"},
        {"Alpha", "Beta", "Gamma"},
        {"Alpha", "Beta", "Gamma"},
        {"Alpha", "Beta", "Gamma"}
    };
    // Alpha выигрывает у всех.
    ASSERT_EQ(solveCondorcet(n, k, votes, candidates), "Alpha");
}

TEST(CondorcetMethodTest, TwoCandidates) {
    int n = 2;
    int k = 5;
    std::vector<std::string> candidates = {"Кандидат1", "Кандидат2"};
    std::vector<std::vector<std::string>> votes = {
        {"Кандидат1", "Кандидат2"},
        {"Кандидат1", "Кандидат2"},
        {"Кандидат2", "Кандидат1"},
        {"Кандидат2", "Кандидат1"},
        {"Кандидат1", "Кандидат2"}
    };
    // Кандидат1 vs Кандидат2: 3 (Кандидат1) vs 2 (Кандидат2) -> Кандидат1 выигрывает.
    ASSERT_EQ(solveCondorcet(n, k, votes, candidates), "Кандидат1");
}

// Это основной макрос для запуска всех тестов Google Test.
// Должен быть только один main() в программе.
// В данном случае Google Test предоставляет свой main(), который мы включаем.
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}