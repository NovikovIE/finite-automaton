#include "nfa.h"
#include <iostream>

NFA::NFA(size_t sz, vector<char>&& new_alphabet, vector<pair<int, int>>&& new_paths, vector<string>&& new_word_on_paths,
         vector<bool>&& new_finish_state)
    : states_number(sz),
      alphabet(std::move(new_alphabet)),
      path(std::move(new_paths)),
      word_on_path(std::move(new_word_on_paths)),
      is_finish_state(std::move(new_finish_state)) {}

NFA::NFA(const string& regular_expression) {
    set<char> new_alphabet;
    size_t pos = 0;
    Node result = std::move(parse_regular_expression(regular_expression, pos, new_alphabet));
    alphabet = vector<char>(new_alphabet.begin(), new_alphabet.end());
    states_number = result.states_number;
    path = std::move(result.path);
    word_on_path = std::move(result.word_on_path);
    if (result.start_state != 0) {
        for (auto& p : path) {
            if (p.first == 0) {
                p.first = result.start_state;
            } else if (p.first == result.start_state) {
                p.first = 0;
            }
            if (p.second == 0) {
                p.second = result.start_state;
            } else if (p.second == result.start_state) {
                p.second = 0;
            }
        }
    }
    is_finish_state.assign(states_number, false);
    if (result.finish_state == -1) {
        return;
    }
    is_finish_state[result.finish_state] = true;
}

Node NFA::parse_regular_expression(const string& regular_expression, size_t& pos, set<char>& new_alphabet) {
    vector<Node> nodes;
    for (; pos < regular_expression.size(); ++pos) {
        if (regular_expression[pos] == '(') {
            ++pos;
            nodes.push_back(std::move(parse_regular_expression(regular_expression, pos, new_alphabet)));
        } else if (regular_expression[pos] == '*' || regular_expression[pos] == '+' || regular_expression[pos] == '.') {
            break;
        } else {
            nodes.emplace_back(regular_expression[pos]);
            if (regular_expression[pos] != '1') {
                new_alphabet.insert(regular_expression[pos]);
            }
        }
    }
    vector<char> operations;
    size_t temp_pos = pos;
    for (; temp_pos < regular_expression.size(); ++temp_pos) {
        if (regular_expression[temp_pos] == ')') {
            break;
        }
        if (regular_expression[temp_pos] == '*') {
            star(nodes[temp_pos - pos]);
        } else {
            operations.push_back(regular_expression[temp_pos]);
        }
    }
    pos = temp_pos;
    vector<bool> is_calculated(nodes.size(), false);
    temp_pos = 0;
    for (; temp_pos < operations.size(); ++temp_pos) {
        if (operations[temp_pos] == '.') {
            concatenate(nodes[temp_pos], nodes[temp_pos + 1]);
            is_calculated[temp_pos] = true;
        }
    }
    temp_pos = 0;
    for (; temp_pos < operations.size(); ++temp_pos) {
        if (operations[temp_pos] == '+') {
            size_t i = 1;
            while (is_calculated[temp_pos + i]) {
                ++i;
            }
            plus(nodes[temp_pos], nodes[temp_pos + i]);
            temp_pos += i - 1;
        }
    }
    if (nodes.empty()) {
        return Node();
    }
    return nodes.back();
}

Node::Node(char c) {
    states_number = 2;
    finish_state = 1;
    path.emplace_back(0, 1);
    if (c != '1') {
        word_on_path.emplace_back(1, c);
    } else {
        word_on_path.emplace_back();
    }
}

void shift(Node& from, Node& to) {
    for (size_t i = 0; i < from.path.size(); ++i) {
        auto p = std::move(from.path[i]);
        p.first += to.states_number;
        p.second += to.states_number;
        to.path.push_back(std::move(p));
        to.word_on_path.push_back(std::move(from.word_on_path[i]));
    }
}

void concatenate(Node& n1, Node& n2) {
    if (n1.states_number > n2.states_number) {
        shift(n2, n1);
        n1.path.emplace_back(n1.finish_state, n2.start_state + n1.states_number);
        n1.finish_state = n2.finish_state + n1.states_number;
        n1.states_number += n2.states_number;
        n1.word_on_path.emplace_back();
        n2 = std::move(n1);
    } else {
        n2.path.emplace_back(n1.finish_state + n2.states_number, n2.start_state);
        n2.word_on_path.emplace_back();
        n2.start_state = n1.start_state + n2.states_number;
        shift(n1, n2);
        n2.states_number += n1.states_number;
    }
}

void add_start_and_finish_states(Node& n1, const Node& n2) {
    size_t temp = n1.states_number;
    n1.states_number += n2.states_number + 2;
    n1.path.emplace_back(n1.states_number - 2, n1.start_state);
    n1.path.emplace_back(n1.states_number - 2, n2.start_state + temp);
    n1.path.emplace_back(n1.finish_state, n1.states_number - 1);
    n1.path.emplace_back(n2.finish_state + temp, n1.states_number - 1);
    for (size_t i = 0; i < 4; ++i) {
        n1.word_on_path.emplace_back();
    }
    n1.start_state = n1.states_number - 2;
    n1.finish_state = n1.states_number - 1;
}

void plus(Node& n1, Node& n2) {
    if (n1.states_number > n2.states_number) {
        shift(n2, n1);
        add_start_and_finish_states(n1, n2);
        n2 = std::move(n1);
    } else {
        shift(n1, n2);
        add_start_and_finish_states(n2, n1);
    }
}

void star(Node& node) {
    node.path.emplace_back(node.finish_state, node.start_state);
    node.word_on_path.emplace_back();
    node.finish_state = node.start_state;
}
