#include "cdfa.h"

void CDFA::convert_to_complete_DFA() {
    paths.emplace_back(alphabet_int.size(), -1);
    is_finish_state.push_back(false);
    bool is_new_path_added = false;
    for (size_t i = 0; i < paths.size() - 1; ++i) {
        for (size_t j = 0; j < alphabet_int.size(); ++j) {
            if (paths[i][j] == -1) {
                is_new_path_added = true;
                paths[i][j] = paths.size() - 1;
            }
        }
    }
    if (!is_new_path_added) {
        paths.pop_back();
        is_finish_state.pop_back();
        return;
    }
    for (size_t i = 0; i < alphabet_int.size(); ++i) {
        paths[paths.size() - 1][i] = paths.size() - 1;
    }
}

void CDFA::convert_to_CDFA_complement() {
    for (int i = 0; i < is_finish_state.size(); ++i) {
        is_finish_state[i] = !is_finish_state[i];
    }
}

vector<int> CDFA::split_to_equivalence_classes() {
    vector<int> equivalence_classes1(paths.size());
    for (size_t i = 0; i < equivalence_classes1.size(); ++i) {
        if (is_finish_state[i])
            equivalence_classes1[i] = 1;
        else
            equivalence_classes1[i] = 0;
    }
    vector<int> equivalence_classes2(paths.size());
    //      number      path_to char    class
    map<pair<int, set<pair<int, int>>>, int> equivalence_classes_paths;
    while (true) {
        equivalence_classes_paths.clear();
        int counter = 0;
        for (size_t i = 0; i < equivalence_classes1.size(); ++i) {
            set<pair<int, int>> paths_to_classes;
            for (size_t j = 0; j < alphabet_int.size(); ++j) {
                if (paths[i][j] != -1)
                    paths_to_classes.insert({equivalence_classes1[paths[i][j]], j});
            }
            if (equivalence_classes_paths.find({equivalence_classes1[i], paths_to_classes}) !=
                equivalence_classes_paths.end()) {
                equivalence_classes2[i] = equivalence_classes_paths[{equivalence_classes1[i], paths_to_classes}];
            } else {
                equivalence_classes2[i] = counter;
                equivalence_classes_paths[{equivalence_classes1[i], paths_to_classes}] = counter++;
            }
        }
        bool is_same_classes = true;
        for (size_t i = 0; i < equivalence_classes1.size(); ++i) {
            if (equivalence_classes1[i] != equivalence_classes2[i])
                is_same_classes = false;
        }
        if (is_same_classes)
            break;
        equivalence_classes1 = equivalence_classes2;
    }
    return equivalence_classes2;
}

void CDFA::minimize() {
    vector<int> new_states = split_to_equivalence_classes();
    int max_value = 0;
    for (int new_state : new_states) {
        if (max_value < new_state)
            max_value = new_state;
    }
    vector<vector<int>> new_paths(max_value + 1, vector<int>(alphabet_int.size()));
    vector<bool> new_finish_states(max_value + 1);
    for (size_t i = 0; i < new_states.size(); ++i) {
        for (size_t j = 0; j < alphabet_int.size(); ++j) {
            if (paths[i][j] == -1)
                new_paths[new_states[i]][j] = -1;
            else
                new_paths[new_states[i]][j] = new_states[paths[i][j]];
        }
        new_finish_states[new_states[i]] = is_finish_state[i];
    }
    paths = new_paths;
    is_finish_state = new_finish_states;
}


CDFA::CDFA(const NFA& nfa) : DFA(nfa) {
    convert_to_complete_DFA();
}

CDFA::CDFA(const DFA& dfa) : DFA(dfa) {
    convert_to_complete_DFA();
}
