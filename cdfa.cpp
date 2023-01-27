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

CDFA::CDFA(const NFA& nfa) : DFA(nfa) {
    convert_to_complete_DFA();
}

CDFA::CDFA(const DFA& dfa) : DFA(dfa) {
    convert_to_complete_DFA();
}
