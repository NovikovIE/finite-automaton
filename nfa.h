#ifndef NFA_H
#define NFA_H

#include <set>
#include <string>
#include <vector>

using std::pair;
using std::set;
using std::string;
using std::vector;

struct Node {
    size_t states_number = 1;
    size_t start_state = 0;
    size_t finish_state = -1;
    vector<pair<int, int>> path;
    vector<string> word_on_path;

    explicit Node(char c);
    Node() = default;
};

void shift(Node& from, Node& to);
void concatenate(Node& n1, Node& n2);
void add_start_and_finish_states(Node& n1, const Node& n2);
void plus(Node& n1, Node& n2);
void star(Node& node);

struct NFA {
private:
    Node parse_regular_expression(const string& regular_expression, size_t& pos, set<char>& new_alphabet);

public:
    size_t states_number = 0;
    vector<char> alphabet;
    vector<pair<int, int>> path;
    vector<string> word_on_path;
    vector<bool> is_finish_state;

    NFA(size_t sz, vector<char>&& new_alphabet, vector<pair<int, int>>&& new_paths, vector<string>&& new_word_on_paths,
        vector<bool>&& new_finish_state);
    NFA(const string& s);
    NFA() = delete;
};

#endif  //NFA_H
