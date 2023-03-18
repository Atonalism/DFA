#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <algorithm>

struct DFA
{
    std::vector<char> alphabet;
    std::vector<std::string> states;
    std::vector<std::vector<std::string>> transitions;
    std::string intital_state;
    std::vector<std::string> final_states;
};

typedef std::tuple<std::string, char> transition;
typedef std::unordered_map<std::string, std::vector<transition>> graph;

auto reading(const char *path) -> DFA
{

    DFA M;
    std::ifstream file;
    std::string line;
    char delimiter = ' ';

    file.open(path);
    if (file.is_open())
    {

        // getting initial state of DFA
        std::getline(file, line);
        M.intital_state = line;

        // getting the final state(s) of DFA
        std::getline(file, line);
        std::istringstream f_states(line);
        while (std::getline(f_states, line, delimiter))
        {
            M.final_states.push_back(line);
        }

        // getting the states of DFA
        std::getline(file, line);
        std::istringstream states(line);
        while (std::getline(states, line, delimiter))
        {
            M.states.push_back(line);
        }

        // getting the alphabet of DFA
        std::getline(file, line);
        std::istringstream alphabet(line);
        while (std::getline(alphabet, line, delimiter))
        {
            M.alphabet.push_back(line.front());
        }

        // getting the transitions of DFA
        std::vector<std::string> str;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            str.clear();
            while (std::getline(iss, line, delimiter))
            {
                str.push_back(line);
            }
            M.transitions.push_back(std::move(str));
        }
    }
    else
    {
        std::cout << "The file did not open.\n";
        std::exit(0);
    }

    return M;
}

auto create_graph(DFA &M) -> graph
{
    std::vector<std::vector<std::string>> trans;
    graph g;

    for (int i = 0; i < M.transitions.size(); i++)
    {
        std::string tail = M.transitions[i][0];
        std::string head = M.transitions[i][1];
        char symbol      = M.transitions[i][2][0];

        transition t = std::make_tuple(head, symbol);

        if (g.count(tail) == 0)
        {
            g.insert(std::make_pair(tail, std::vector<transition>()));

            if (std::none_of(g[tail].begin(), g[tail].end(), [&symbol](transition& tr) { return std::get<1>(tr) == symbol; })) {
                g[tail].push_back(t);

                trans.push_back(M.transitions[i]);
            }
        }
        else
        {
            if (std::none_of(g[tail].begin(), g[tail].end(), [&symbol](transition& tr) { return std::get<1>(tr) == symbol; })) {
                g[tail].push_back(t);

                trans.push_back(M.transitions[i]);
            }
        }

        if (g.count(head) == 0)
        {
            g.insert(make_pair(head, std::vector<transition>()));
        }
    }

    M.transitions = trans;
    
    return g;
}

template<typename T>
auto print_vector(const std::vector<T>& v, int i) -> void {
    std::string left_delimiter, right_delimiter;
    
    if (i == 0) {
        left_delimiter = "{ ";
        right_delimiter = "}";
    } else {
        left_delimiter = "( ";
        right_delimiter = ")";
    }

    if (!v.empty())
    {
        std::cout << left_delimiter;
        for (const T &t : v)
            std::cout << t << " ";
        std::cout << right_delimiter;
    }
}

auto print_dfa(DFA &M) -> void
{
    std::cout << "M = (";
    print_vector(M.alphabet, 0);
    std::cout << ", ";
    print_vector(M.states, 0);
    std::cout << ", { ";
    for (const auto &transition : M.transitions)
    {
        print_vector(transition, 1);
    }
    std::cout << " }, ";
    std::cout << M.intital_state << ", ";
    print_vector(M.final_states, 0);
    std::cout << ")\n";
}

auto next_state(graph &g, std::string state, char symbol) -> std::string
{
    for (const transition &t : g[state])
        if (std::get<1>(t) == symbol) return std::get<0>(t);
    return " ";
}

auto Pe(DFA &M, graph &g, std::vector<char> &word) -> bool
{
    auto state = M.intital_state;
    for (const char &c : word) {
        if (state == " ") return false;
        state = next_state(g, state, c);
    }

    return (std::count(M.final_states.begin(), M.final_states.end(), state))? true : false;
}

auto clear_screen() -> void
{
#ifdef WINDOWS
    std::system("cls");
#else
    // Assume POSIX
    std::system("clear");
#endif
}

auto main(int argc, char **argv) -> int
{
    if (argc != 2)
    {
        std::cout << "Erro na leitura dos comandos" << std::endl;
        exit(0);
    }

    for (int i = 0; i < argc; ++i)
    {
        if (argv[i] == NULL)
        {
            std::cout << "Erro na leitura dos comandos: algum argumento nulo\n";
            std::exit(0);
        }
    }

    const char *current_exec_name = argv[0];
    const char *input_path = argv[1];

    DFA M = reading(input_path);
    graph graph = create_graph(M);

    std::string valid_options("yYnN");
    bool start = true;
    bool illegal_input;
    std::string word;
    std::string input;

    do
    {
        clear_screen();
        std::cout << "DETERMINISTC FINITE AUTOMATA (DFA)\n\n";
        std::cout << "DFA read: \n";
        print_dfa(M);
        std::cout << "\n";
        std::cout << "Write your word: \n";
        std::cin >> word;
        std::vector<char> v(word.begin(), word.end());
        std::cout << "\n";

        if (Pe(M, graph, v))
        {
            std::cout << "Word successfully processed. '" << word << "' is part of the DFA-defined language.\n\n";
        }
        else
        {
            std::cout << "Unsuccessfully processed word. '" << word << "' is not part of the DFA-defined language.\n\n";
        }

        std::cout << "Try another word? (y/n)\n";
        std::cin >> input;

        while (input.size() != 1 || valid_options.find(input) == std::string::npos)
        {
            std::cout << "Try another word? (y/n)\n";
            std::cin >> input;
        }

        if (input == "n" || input == "N")
        {
            start = false;
        }

    } while (start);

    return 0;
}
