// import the Formula header file

#include "Formula.hpp"

class Solver{
    private:
        Formula formula;                            // The formula to be solved
        int no_of_literals;                         // The number of literals
        int unit_propagate(Formula &);              // function for unit propagation
        int eliminate_pure_literals(Formula &);     // function for elimination of pure literals
        int update_formula(Formula &, int, int);    // update the formula with a given truth value of given literal
        void print_model(Formula &, int);           // print the model solution
        int solve(Formula &);                       // solve function for different optimizations and function calls
    
    public:
        Solver(){                                   // constructor function

        }
        void take_input(string &);                  // public function for taking input from file given
        void decipher();                            // public function for solving the formula
};

void Solver::take_input(string & input_file){

    freopen(input_file.c_str(), "r", stdin);                // read stdin from given file

    string str;
    getline(cin, str);
    while(str[0]!='p'){                                     // read and ignore comment lines
        getline(cin, str);                      
    }

    string temp_literal_count="", temp_clause_count="";     // first store variables in string format
    
    int in=6;                                               // ignore "p cnf ", first 6 characters 
    
    while(str[in]!=' '){
        temp_literal_count+=str[in];
        in++;
    }
    
    for(int i=in+1;i<str.size();i++){
        temp_clause_count+=str[i];
    }
    
    no_of_literals=stoi(temp_literal_count);                // convert string to integer

    formula.freq.resize(2*no_of_literals, 0);               // resize double of number of literals, positive and negative
    formula.freq_lit.resize(no_of_literals, 0);             // resize to number of literals
    formula.literals.resize(no_of_literals, -1);            // resize to number of literals
    
    int no_of_clauses=stoi(temp_clause_count);              // convert string to integer
    

    for(int i=0;i<no_of_clauses;i++){                   // run the loop till number of clauses times                       
        vector<int> v;
        set<int> s;                                     // set for removing the duplicates
        int x;
        cin>>x;
        bool both_pos_and_neg_found=false;              // flag for if both pos and neg polarity of same literal appear in same clause
        


        // Consideration : for frequency to be maintained throughout, we keep a 0 based indexing for every literal,
        // so for literals 1,2,3,4.....,n we assume them 0,1,2,3.....,n-1
        // for keeping index for negative and positive literal, we assigne 2*x+1 to positive literal and 2*x to negative literal in 0 based indexing



        while(x!=0){                                    //run the loop till we reach 0
            if(x>0){
                s.insert(2*x-1);
                if(s.find(2*x-2)!=s.end()){             // check for the flag
                    both_pos_and_neg_found=true;
                }
            }else if(x<0){
                s.insert(2*(-1*x)-2);
                if(s.find(2*(-1*x)-1)!=s.end()){        // check for the flag
                    both_pos_and_neg_found=true;
                }
            }
            cin>>x;
        }
        if(both_pos_and_neg_found){                     // if both pos and neg polalrity of a literal is present, do not consider this clause, default true
            continue;
        }
        for(auto i:s){
            v.push_back(i);                             // copy set into vector
        }
        formula.clauses.insert(v);                      // insert the clause into clauses set
    }
    
    for(auto v:formula.clauses){                
        for(auto it:v){
            formula.freq[it]++;                         // update the frequencies
            formula.freq_lit[it/2]++;
        }
    }
}

// function for unit propagation in the formula
// function returns 1 if after unit propagation, SAT is proven
// function returns 0 if after unit propagation, UNSAT is proven
// function returns 2 if after unit propagation, neither can be proven
int Solver::unit_propagate(Formula & formula){
    
    bool unit_found=true;                           // initial condition for running the loop
    
    while(unit_found){                              // run loop till we are finding unit clauses

        unit_found=false;                           // assume that unit clause is not found
        
        for(auto it=formula.clauses.begin(); it!=formula.clauses.end(); it++){  // iterate through the clauses    

            if((*it).size()==1){                    // unit clause found
                unit_found=true;
                
                int literal=*((*it).begin());       // the first and only element will be the literal
                
                formula.literals[literal/2]=literal&1;                          // the even literals are for neg and odd for pos polarity 
                
                int res=update_formula(formula, literal&1, literal/2);          // update the formula with  the truth value assigned to present literal
                
                // res==1 : SAT
                // res==0 : UNSAT
                // res==2 : Nothing proved yet

                if(res==1 || res==0){
                    return res;
                }
                
                break;
            }
            
            if((*it).size()==0){                    // if a clause is empty, the present formula is UNSAT
                return 0;
            }
        }
    }
    return 2;                                       // Nothing has been proven yet
}


// function for elimination of pure literals in the formula
// function returns 1 if after elimination, SAT is proven
// function returns 0 if after elimination, UNSAT is proven
// function returns 2 if after elimination, neither can be proven
int Solver::eliminate_pure_literals(Formula & formula){
    
    for(int literal=0;literal<formula.literals.size();literal++){       // iterate throught the clauses
        
        int pos=2*literal+1;                                            // the postive polarity of literal
        int neg=2*literal;                                              // the negative polarity of literal
        

        if(formula.freq[pos]==0 && formula.freq[neg]!=0){               // if frequency of positive polarity is zero and that of negative is non-zero               
            formula.literals[literal]=0;                                // assign truth value according to negative polarity
        
            int res=update_formula(formula, 0, literal);                // update the formula with  the truth value assigned to negative polarity      
            
            // res==1 : SAT
            // res==0 : UNSAT
            // res==2 : Nothing proved yet
            if(res==1 || res==0){
                return res;
            }
        }
        
        if(formula.freq[pos]!=0 && formula.freq[neg]==0){               // if frequency of positive polarity is non-zero and that of negative is zero              
            formula.literals[literal]=1;                                // assign truth value according to postive polarity

            int res=update_formula(formula, 1, literal);                // update the formula with  the truth value assigned to positive polarity
            
            // res==1 : SAT
            // res==0 : UNSAT
            // res==2 : Nothing proved yet
            if(res==1 || res==0){
                return res;
            }
        }
    }
    return 2;                                                           // Nothing has been proven yet
}

// function for updation of formula with given literal and truth value assigned to that literal
// function returns 1 if after updation, SAT is proven
// function returns 0 if after updation, UNSAT is proven
// function returns 2 if after updation, neither can be proven
int Solver::update_formula(Formula & formula, int boolean, int literal){

    for(auto it=formula.clauses.begin(); it!=formula.clauses.end();){   // iterate through the clauses
        
        vector<int> v=*it;
        bool done=false;                                // flag for updation of current clause
        
        for(int i=0;i<v.size();i++){                    // iterate through the current clause

            if(v[i]==2*literal+boolean){                // if clause contains the literal
                it=formula.clauses.erase(it);           // erase the clause
        
                for(auto lit:v){
                    formula.freq[lit]--;                // update the frequencies
                    formula.freq_lit[lit/2]--;
                }
                v.clear();
                done=true;                              // updation done
        
                if(formula.clauses.size()==0){          // if no clauses left, SAT proved
                    return 1;
                }
        
                break;
            }
        }

        for(int i=0;i<v.size();i++){                    // if updation not done yet, again iterate
            if(v[i]==2*literal+1-boolean){              // if negation of literal is present
            
                formula.freq[v[i]]--;
                formula.freq_lit[v[i]/2]--;             // update the frequencies
            
                it=formula.clauses.erase(it);
            
                v.erase(v.begin()+i);                   // erase the negation from the clause
                formula.clauses.insert(v);
            
                done=true;                              //updation done
                
                if(v.size()==0){                        // if size of clause is zero, UNSAT proved
                    return 0;
                }
            
                break;
            }
        }
        if(!done){                                      // if updation not done, move to next element in the set
            it++;
        }
    }
    return 2;
}

// recursive function for solving the formula
// function returns 1 if current formula is solved and model is achieved
// function returns 0 if current formula is UNSAT and we need to backtrack or move to another formula
int Solver::solve(Formula & formula){
    
    if(formula.clauses.size()==0){                  // base case, if there are no clauses, SAT
        print_model(formula, 1);                    // print the model, (print_model exits program )
    }
    
    int done_unit=unit_propagate(formula);          // unit propagation
    
    if(done_unit==1){                               
        print_model(formula, done_unit);            // if SAT, print model
    }else if(done_unit==0){
        return 0;                                   // if UNSAT, end this branch
    }
    
    int done_elim=eliminate_pure_literals(formula); // elimination of pure literals
    
    if(done_elim==1){
        print_model(formula, done_elim);            // if SAT, print model
    }else if(done_elim==0){
        return 0;                                   // if UNSAT, end this branch
    }

    
    // We will choose the literal whose frequency is maximum. This will maximise the clauses to be checked in a single go.

    int literal=max_element(formula.freq_lit.begin(), formula.freq_lit.end())-formula.freq_lit.begin();

    if(formula.freq_lit[literal]==0){               // if maximum frequency is zero, formula is already solved
        print_model(formula, 1);
    }

    Formula formula_copy(formula);                  // copy the formula

    Formula formula_copy_copy(formula_copy);        // make another copy for another truth value

    
    // The literal will be first assigned a true value if the frequency of positive polarity will be more in the formula than negative, and vice versa.
    if(formula.freq[2*literal+1]>formula.freq[2*literal]){

        formula_copy.literals[literal]=1;
        int res=update_formula(formula_copy, 1, literal);

        if(res==1){
            print_model(formula_copy, 1);           // print model if SAT
        }else if(res==2){
            res=solve(formula_copy);                // recursively solve 
            if(res==1){
                print_model(formula_copy, 1);       // print model if SAT
            }
        }
    }else{

        formula_copy.literals[literal]=0;
        int res=update_formula(formula_copy, 0, literal);

        if(res==1){
            print_model(formula_copy, 1);           // print model if SAT
        }else if(res==2){
            res=solve(formula_copy);                // recursively solve 
            if(res==1){
                print_model(formula_copy, 1);       // print model if SAT
            }
        }
    }
    if(formula.freq[2*literal+1]>formula.freq[2*literal]){
        formula_copy_copy.literals[literal]=0;
        int res=update_formula(formula_copy_copy, 0, literal);
        if(res==1){
            print_model(formula_copy_copy, 1);      // print model if SAT
        }else if(res==2){
            res=solve(formula_copy_copy);           // recursively solve 

            if(res==1){
                print_model(formula_copy_copy, 1);  // print model if SAT
            }
        }
    }else{
        formula_copy.literals[literal]=1;

        int res=update_formula(formula_copy_copy, 1, literal);

        if(res==1){
            print_model(formula_copy_copy, 1);      // print model if SAT
        }else if(res==2){
            res=solve(formula_copy_copy);           // recursively solve 

            if(res==1){
                print_model(formula_copy_copy, 1);  // print model if SAT
            }
        }
    }
    return 0;
}

void Solver::print_model(Formula & formula, int boolean){
    
    if(boolean){
        cout<<"\nThe formula is SAT\n\n{";
        
        for(int i=0;i<formula.literals.size();i++){
            
            if(formula.literals[i]){                    // if literals[i] is 1 or -1
                cout<<i+1;
            }else{
                cout<<-1*(i+1);                         // if literals[i] is 0
            }

            if(i!=formula.literals.size()-1){
                cout<<", ";
            }
        }
        cout<<"}\n";

    }else{
        cout<<"\nThe formula is UNSAT";
    }

    time_taken();
    
    exit(0);
}

void Solver::decipher(){
    cerr<<"deciphering...\n";                   // print message that solver is running
    int res=solve(formula);                     
    if(res!=1){                                 // if UNSAT
        print_model(formula, 0);                // print with UNSAT message
    }
}
