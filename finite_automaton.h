#ifndef DKA_FINITE_AUTOMATON_H
#define DKA_FINITE_AUTOMATON_H

#include "cdfa.h"
#include "dfa.h"
#include "nfa.h"

size_t find_longest_substring_in_language(const string& regular_expression, const string& word) {
    NFA nfa(regular_expression);
    CDFA cdfa(nfa);
    cdfa.minimize();
    return cdfa.longest_substring(word);
}

#endif  //DKA_FINITE_AUTOMATON_H
