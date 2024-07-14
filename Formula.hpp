#include "Utils.hpp"

class Formula{
    public:
        set<vector<int> > clauses;              // set of clauses
        vector<int> literals;                   // truth vaue of literals, 1 for pos, 0 for neg, -1 for not assigned
        vector<int> freq;                       // frequency of signed literal
        vector<int> freq_lit;                   // frequency of unsigned literal

        Formula(){
                                                // constructor
        }

        Formula(Formula & formula){             // copy constructor
            clauses=formula.clauses;
            literals=formula.literals;
            freq=formula.freq;
            freq_lit=formula.freq_lit;
        }
};