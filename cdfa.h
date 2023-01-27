#ifndef DKA_CDFA_H
#define DKA_CDFA_H

#include "dfa.h"

class CDFA : public DFA {
private:
    void convert_to_complete_DFA();
    vector<int> split_to_equivalence_classes();

public:
    explicit CDFA(const NFA& nfa);
    explicit CDFA(const DFA& dfa);
    void minimize();
    void convert_to_CDFA_complement();
};

#endif  //DKA_CDFA_H
