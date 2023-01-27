#ifndef DFA_H
#define DFA_H

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include "nfa.h"

using std::map;
using std::pair;
using std::queue;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

class DFA {
protected:
    vector<vector<int>> paths;
    unordered_map<char, size_t> alphabet_int;
    vector<bool> is_finish_state;
    vector<char> alphabet;

private:
    void bfs(vector<vector<vector<size_t>>>& temp_paths, size_t start);
    void initialize_alphabet_int(const vector<char>& alphabet);
    void remove_word_paths(const vector<pair<int, int>>& string_paths, const vector<string>& word_not_letter_paths,
                           vector<vector<vector<size_t>>>& temp_paths);
    void shorten_epsilon_paths(vector<vector<vector<size_t>>>& temp_paths);
    void shorten_epsilon_letter_paths(vector<vector<vector<size_t>>>& temp_paths) const;
    void remove_epsilon_paths(vector<vector<vector<size_t>>>& temp_paths);
    void build_dfa(vector<vector<vector<size_t>>>& temp_paths);
    void erase_vertex(int v, int end_state, vector<bool>& is_vertex_erased, vector<vector<bool>>& is_path_exists,
                      queue<int>& q, vector<vector<int>>& vertices_paths_to, vector<vector<string>>& m) const;
    void calculate_regular_paths(int end_state, vector<vector<bool>>& is_path_exists,
                                 vector<vector<int>>& vertices_paths_to, vector<vector<string>>& m) const;

public:
    explicit DFA(const NFA& nfa);
    void show_automaton();
    bool is_in(const string& s);
    size_t longest_substring(const string& s);
    string regular_expression();
    size_t size();
    size_t eps();
};

#endif  //DFA_H