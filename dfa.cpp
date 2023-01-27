#include "dfa.h"

void DFA::bfs(vector<vector<vector<size_t>>>& temp_paths, size_t start) {
    size_t eps = alphabet_int.size();
    queue<size_t> q;
    q.push(start);
    vector<bool> used(temp_paths.size());
    used[start] = true;
    while (!q.empty()) {
        size_t v = q.front();
        q.pop();
        vector<size_t> temp;
        for (size_t to : temp_paths[v][eps]) {
            if (!used[to]) {
                used[to] = true;
                q.push(to);
                temp.push_back(to);
                if (is_finish_state[to])
                    is_finish_state[start] = true;
            }
        }
        for (size_t& i : temp) {
            temp_paths[start][eps].push_back(i);
        }
    }
}

void DFA::initialize_alphabet_int(const vector<char>& alphabet) {
    for (int i = 0; i < alphabet.size(); ++i) {
        alphabet_int[alphabet[i]] = i;
    }
}

void DFA::remove_word_paths(const vector<pair<int, int>>& string_paths, const vector<string>& word_string_paths,
                            vector<vector<vector<size_t>>>& temp_paths) {
    for (size_t i = 0; i < string_paths.size(); ++i) {
        size_t k = string_paths[i].first;
        string s = word_string_paths[i];
        for (int j = 0; j < s.size() - 1; ++j) {
            temp_paths.emplace_back(alphabet_int.size() + 1, vector<size_t>());
            temp_paths[k][alphabet_int[s[j]]].push_back(temp_paths.size() - 1);
            is_finish_state.push_back(false);
            k = temp_paths.size() - 1;
        }
        temp_paths[k][alphabet_int[s[s.size() - 1]]].push_back(string_paths[i].second);
    }
}

void DFA::shorten_epsilon_paths(vector<vector<vector<size_t>>>& temp_paths) {
    for (size_t i = 0; i < temp_paths.size(); ++i) {
        bfs(temp_paths, i);
    }
}

void DFA::shorten_epsilon_letter_paths(vector<vector<vector<size_t>>>& temp_paths) const {
    for (size_t from = 0; from < temp_paths.size(); ++from) {
        for (size_t to_eps = 0; to_eps < temp_paths[from][alphabet_int.size()].size(); ++to_eps) {
            if (from == temp_paths[from][alphabet_int.size()][to_eps])
                continue;
            size_t to = temp_paths[from][alphabet_int.size()][to_eps];
            for (size_t letter = 0; letter < temp_paths[to].size() - 1; ++letter) {
                vector<bool> existing_paths_from_i_by_letter(temp_paths.size(), false);
                if (temp_paths[to][letter].empty())
                    continue;
                for (size_t to_by_letter_from_i : temp_paths[from][letter]) {
                    existing_paths_from_i_by_letter[to_by_letter_from_i] = true;
                }
                for (size_t l = 0; l < temp_paths[to][letter].size(); ++l) {
                    if (!existing_paths_from_i_by_letter[l])
                        temp_paths[from][letter].push_back(temp_paths[to][letter][l]);
                }
            }
        }
    }
}

void DFA::remove_epsilon_paths(vector<vector<vector<size_t>>>& temp_paths) {
    shorten_epsilon_paths(temp_paths);
    shorten_epsilon_letter_paths(temp_paths);
    for (auto& temp_path : temp_paths) {
        temp_path[alphabet_int.size()].clear();
    }
}

void DFA::build_dfa(vector<vector<vector<size_t>>>& temp_paths) {
    paths = vector<vector<int>>(1, vector<int>(alphabet_int.size(), -1));
    vector<bool> new_finish_states(1, is_finish_state[0]);
    map<set<size_t>, size_t> m;
    queue<set<size_t>> q;
    q.push({0});
    m[{0}] = 0;
    int counter = 1;
    while (!q.empty()) {
        auto states = q.front();
        q.pop();
        for (size_t letter = 0; letter < alphabet_int.size(); ++letter) {
            set<size_t> combined_states;
            bool is_finish = false;
            for (size_t state : states) {
                for (size_t j : temp_paths[state][letter]) {
                    combined_states.insert(j);
                    is_finish = is_finish || is_finish_state[j];
                }
            }
            if (combined_states.empty())
                continue;
            if (m.find(combined_states) == m.end()) {
                m[combined_states] = counter++;
                paths.emplace_back(alphabet_int.size(), -1);
                paths[m[states]][letter] = static_cast<int>(m[combined_states]);
                new_finish_states.push_back(is_finish);
                q.push(combined_states);
            } else {
                paths[m[states]][letter] = static_cast<int>(m[combined_states]);
                new_finish_states[m[combined_states]] = new_finish_states[m[combined_states]] || is_finish;
            }
        }
    }
    is_finish_state = new_finish_states;
}

DFA::DFA(const NFA& nfa) : is_finish_state(nfa.is_finish_state), alphabet(nfa.alphabet) {
    initialize_alphabet_int(nfa.alphabet);
    vector<vector<vector<size_t>>> temp_paths(nfa.states_number,
                                              vector<vector<size_t>>(nfa.alphabet.size() + 1, vector<size_t>()));
    vector<pair<int, int>> string_paths;
    vector<string> word_string_paths;
    for (size_t i = 0; i < nfa.path.size(); ++i) {
        if (nfa.word_on_path[i].size() > 1) {
            string_paths.push_back(nfa.path[i]);
            word_string_paths.push_back(nfa.word_on_path[i]);
        } else if (nfa.word_on_path[i].size() == 1) {
            temp_paths[nfa.path[i].first][alphabet_int[nfa.word_on_path[i][0]]].push_back(nfa.path[i].second);
        } else {
            temp_paths[nfa.path[i].first][alphabet_int.size()].push_back(nfa.path[i].second);
        }
    }
    remove_word_paths(string_paths, word_string_paths, temp_paths);
    remove_epsilon_paths(temp_paths);
    build_dfa(temp_paths);
}

vector<int> DFA::split_to_equivalence_classes() {
    vector<int> equivalence_classes1(paths.size());
    for (size_t i = 0; i < equivalence_classes1.size(); ++i) {
        if (is_finish_state[i])
            equivalence_classes1[i] = 1;
        else
            equivalence_classes1[i] = 0;
    }
    vector<int> equivalence_classes2(paths.size());
    //      number      path_to char    class
    map<pair<int, set<pair<int, int>>>, int> equivalence_classes_paths;
    while (true) {
        equivalence_classes_paths.clear();
        int counter = 0;
        for (size_t i = 0; i < equivalence_classes1.size(); ++i) {
            set<pair<int, int>> paths_to_classes;
            for (size_t j = 0; j < alphabet_int.size(); ++j) {
                if (paths[i][j] != -1)
                    paths_to_classes.insert({equivalence_classes1[paths[i][j]], j});
            }
            if (equivalence_classes_paths.find({equivalence_classes1[i], paths_to_classes}) !=
                equivalence_classes_paths.end()) {
                equivalence_classes2[i] = equivalence_classes_paths[{equivalence_classes1[i], paths_to_classes}];
            } else {
                equivalence_classes2[i] = counter;
                equivalence_classes_paths[{equivalence_classes1[i], paths_to_classes}] = counter++;
            }
        }
        bool is_same_classes = true;
        for (size_t i = 0; i < equivalence_classes1.size(); ++i) {
            if (equivalence_classes1[i] != equivalence_classes2[i])
                is_same_classes = false;
        }
        if (is_same_classes)
            break;
        equivalence_classes1 = equivalence_classes2;
    }
    return equivalence_classes2;
}

void DFA::minimize() {
    vector<int> new_states = split_to_equivalence_classes();
    int max_value = 0;
    for (int new_state : new_states) {
        if (max_value < new_state)
            max_value = new_state;
    }
    vector<vector<int>> new_paths(max_value + 1, vector<int>(alphabet_int.size()));
    vector<bool> new_finish_states(max_value + 1);
    for (size_t i = 0; i < new_states.size(); ++i) {
        for (size_t j = 0; j < alphabet_int.size(); ++j) {
            if (paths[i][j] == -1)
                new_paths[new_states[i]][j] = -1;
            else
                new_paths[new_states[i]][j] = new_states[paths[i][j]];
        }
        new_finish_states[new_states[i]] = is_finish_state[i];
    }
    paths = new_paths;
    is_finish_state = new_finish_states;
}

void DFA::show_automaton() {
    std::cout << "showstart\n";
    for (size_t i = 0; i < paths.size(); ++i) {
        for (size_t j = 0; j < alphabet_int.size(); ++j) {
            std::cout << paths[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "end states: ";
    for (size_t i = 0; i < is_finish_state.size(); ++i) {
        if (is_finish_state[i])
            std::cout << i << " ";
    }
    std::cout << "\nshowend\n";
}

size_t DFA::size() {
    return paths.size();
}

size_t DFA::eps() {
    return alphabet_int.size();
}

bool DFA::is_in(const string& s) {

    if (paths[0].empty()) {
        if (is_finish_state[0]) {
            return s.empty();
        } else {
            return false;
        }
    }
    int v = 0;
    int pointer = 0;
    while (pointer != s.size()) {
        if (paths[v][alphabet_int[s[pointer]]] == -1) {
            return false;
        } else {
            v = paths[v][alphabet_int[s[pointer++]]];
        }
    }
    return is_finish_state[v];
}

string DFA::regular_expression() {
    int end_state = paths.size();
    vector<vector<string>> m(paths.size() + 1, vector<string>(paths.size() + 1, ""));
    vector<vector<bool>> is_path_exists(paths.size() + 1, vector<bool>(paths.size() + 1, false));
    vector<vector<int>> vertices_paths_to(paths.size() + 1);
    calculate_regular_paths(end_state, is_path_exists, vertices_paths_to, m);
    vector<bool> is_vertex_erased(paths.size(), false);
    queue<int> q;
    while (true) {
        for (int i = 1; i < paths.size(); ++i) {
            if (is_path_exists[0][i] && !is_vertex_erased[i])
                q.push(i);
        }
        if (q.empty())
            break;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            if (is_vertex_erased[v])
                continue;
            erase_vertex(v, end_state, is_vertex_erased, is_path_exists, q, vertices_paths_to, m);
        }
    }
    string result;
    string temp1 = "";
    if (is_path_exists[0][0] && !m[0][0].empty())
        temp1 = "(" + m[0][0] + ")*";
    if (is_path_exists[0][end_state]) {
        string temp2 = "";

        if (is_finish_state[0])
            temp2 = "1+";
        result = temp1 + "(" + temp2 + m[0][end_state] + ")";
    } else {
        result = temp1;
    }
    return result;
}

void DFA::erase_vertex(int v, int end_state, vector<bool>& is_vertex_erased, vector<vector<bool>>& is_path_exists,
                       queue<int>& q, vector<vector<int>>& vertices_paths_to, vector<vector<string>>& m) const {
    string path_to_itself = "";
    if (is_path_exists[v][v] && m[v][v] != "1")
        path_to_itself = m[v][v];
    if (!path_to_itself.empty())
        path_to_itself = "(" + path_to_itself + ")*";
    for (size_t to = 0; to < m.size(); ++to) {
        if (to == v || is_vertex_erased[to])
            continue;
        if (is_path_exists[v][to]) {
            if (to != 0 && to != end_state && !is_vertex_erased[to])
                q.push(to);
            for (int from : vertices_paths_to[v]) {
                if (is_vertex_erased[from])
                    continue;
                string elem1 = (m[from][v] == "1") ? ("") : (m[from][v]);
                string elem2 = (m[v][to] == "1") ? ("") : (m[v][to]);
                if (elem1.size() > 1)
                    elem1 = "(" + elem1 + ")";
                if (elem2.size() > 1)
                    elem2 = "(" + elem2 + ")";
                if (m[from][to].empty())
                    m[from][to] = elem1 + path_to_itself + elem2;
                else
                    m[from][to] += "+" + elem1 + path_to_itself + elem2;
                if (!is_path_exists[from][to]) {
                    is_path_exists[from][to] = true;
                    vertices_paths_to[to].push_back(from);
                }
            }
        }
    }
    is_vertex_erased[v] = true;
}

void DFA::calculate_regular_paths(int end_state, vector<vector<bool>>& is_path_exists,
                                  vector<vector<int>>& vertices_paths_to, vector<vector<string>>& m) const {
    for (size_t i = 0; i < paths.size(); ++i) {
        if (is_finish_state[i]) {
            is_path_exists[i][end_state] = true;
            vertices_paths_to[end_state].push_back(i);
            m[i][end_state] = "1";
        }
        bool is_path_to_itself_exists = false;
        for (size_t j = 0; j < alphabet_int.size(); ++j) {
            if (paths[i][j] != -1) {
                if (paths[i][j] == i)
                    is_path_to_itself_exists = true;
                if (!is_path_exists[i][paths[i][j]]) {
                    m[i][paths[i][j]] = alphabet[j];
                    is_path_exists[i][paths[i][j]] = true;
                } else {
                    m[i][paths[i][j]] += "+" + string(1, alphabet[j]);
                }
                if (paths[i][j] != i)
                    vertices_paths_to[paths[i][j]].push_back(i);
            }
        }
        if (is_path_to_itself_exists)
            vertices_paths_to[i].push_back(i);
    }
}

size_t DFA::longest_substring(const string& s) {
    if (paths[0].empty()) {
        return 0;
    }
    size_t max_len = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        size_t v = 0;
        size_t j = i;
        while (j != s.size()) {
            if (alphabet_int.find(s[j]) == alphabet_int.end()) {
                break;
            } else {
                v = paths[v][alphabet_int[s[j++]]];
            }
            if (is_finish_state[v]) {
                max_len = std::max(max_len, j - i);
            }
        }
    }
    return max_len;
}
