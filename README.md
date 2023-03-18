# DFA
A modest implementation of a DFA (Deterministic Finite Automata) that reads a user-supplied text input with the formal definition of a DFA. With the DFA read, the user can test whether words are part of the language defined by the provided ADF or not.

# Input
The input is a txt file named "input.txt". The file can be edited by the user to define any DFA following the format:

The first line has the initial state label; \
The second line has all end state labels separated by space; \
The third line has the labels of all the DFA states separated by space; \
The fourth line has all the symbols that make up the DFA alphabet separated by spaces; \
From the fifth line onwards, state transitions are defined as follows: "label of the state where the arrow comes from, label of the state that receives the arrow, symbol of the transition"

generic example

initial \
end1 end2 ... endN \
state1 state2 ... stateN \
symbol1 symbol2 ... symbolN \
state1 state 2 symbol1 \
. \
. \
. \
stateN stateM symbolN

example: Language defined by a DFA, such that the words are formed by a's and b's and contain an odd number of b's.

1 \
2 \
1 2 \
a b \
1 1 a \
1 2 b \
2 1 b \
2 2 a

DFA read:

M = ({ a b }, { 1 2 }, { ( 1 1 a ) ( 1 2 b ) ( 2 1 b ) ( 2 2 a ) }, 1, { 2 }) \
where \
M = (alphabet, set of all states, set of transitions, initial state, set of final states)

# Run
In the code directory, open the terminal and to compile run:

g++ dfa.cpp -o name_exec

to execute:

./name_exec path_of_input
