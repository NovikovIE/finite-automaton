#ifndef DKA_CDFA_H
#define DKA_CDFA_H

#include "dfa.h"

class CDFA : public DFA {
private:
    void convert_to_complete_DFA();
public:
    explicit CDFA(const NFA& nfa);
    explicit CDFA(const DFA& dfa);
    void convert_to_CDFA_complement();
};

#endif  //DKA_CDFA_H
