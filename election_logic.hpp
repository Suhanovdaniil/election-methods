#ifndef ELECTION_LOGIC_HPP
#define ELECTION_LOGIC_HPP

#include <string>
#include <vector>
#include <map>
#include <algorithm> // Для std::sort и std::unique, если потребуется

// Объявления функций
std::string solveBorda(int n, int k, const std::vector<std::vector<std::string>>& votes, const std::vector<std::string>& candidates);
std::string solveCondorcet(int n, int k, const std::vector<std::vector<std::string>>& votes, const std::vector<std::string>& candidates);

#endif // ELECTION_LOGIC_HPP