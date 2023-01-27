# FiniteAutomaton

Contains header file finite_automaton.h, which includes 3 classes:

- NFA - has a constructor from (size_t, vector<char>, vector<pair<int, int>>, vector<string>, vector<bool>) :  
    number of states, alphabet, transitions (from which state to which),
    words on these transitions, an array showing which vertices are the final
  
    also has constructor from a string which is interpreted as a regular expression in reverse polish notation
  
- DFA - has a constructor from NFA
  
- CDFA - has a constructor from NFA and a constructor from DFA (is also the subclass of DFA)
All functionality of DFA:

(1) Turns transitions by words with length > 1 into transition chains with words of 1 symbol
(2) Removes eps-transitions
(3) Builds DFA
(4) Builds by DFA a regular expression
All functionality of CDFA - all, that has DFA +
  
(5) Builds CDFA by DFA
(6) Minimizes CDFA
(7) Buils by CDFA a complement of CDFA
