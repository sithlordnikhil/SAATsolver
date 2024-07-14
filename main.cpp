// import the solver header file

#include "Solver.hpp"

int main(int argc, char *argv[]){

    string input_file="input.txt";      // The input file, default : input.txt
    
    if(argc==2){                        // if input file provided in argument
        input_file=argv[1];             // specify the file
    }else{
        cerr<<"\nWarning : No input file provided, taking input.txt as default\n\n";
    }

    Solver s;                           // initialize the solver
                
    s.take_input(input_file);           // take input from file

    s.decipher();                       // use decipher function to solve for the model
    
    return 0;
}