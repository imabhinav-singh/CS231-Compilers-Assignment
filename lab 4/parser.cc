#include<bits/stdc++.h>

using namespace std;

struct variable 
{
    bool flag; // true means terminal, whereas false means non-terminal
    string value;
    bool compare(struct variable var) 
    {
        if (this->flag == var.flag && this->value.compare(var.value) == 0) return true;
        else return false;
    }
    int find(vector<struct variable> arr)
    {
        for (int i = 0; i < arr.size(); i++)
        {
            if (this->compare(arr.at(i))) return i;
        }
        return -1;        
    }
    void display() 
    {
        cout << this->value;
    }
    bool operator<(const variable& v) const
    { 
        if (this->flag == v.flag)
        {
            return (this->value.compare(v.value) < 0)? true: false;
        } 
        else return (this->flag)? true: false;
    } 
};

struct expr 
{     
    vector<struct variable> arr;
    void display()
    {
        for (int i = 0; i < this->arr.size(); i++)
        {
            arr.at(i).display();
        }        
    }
};

struct production 
{
    struct variable lhs;
    vector<struct expr> rhs;
    void display()
    {
        this->lhs.display();
        cout << " -> ";
        for (int i = 0; i < this->rhs.size(); i++)
        {
            this->rhs.at(i).display();
            if (i != this->rhs.size()-1) cout << " | ";
        }        
    }
};

int n_terminal, n_nonterminal, n_production;
vector<struct variable> terminals;
vector<struct variable> nonterminals;
vector<struct production> production_list;
struct variable epsilon = { true, "epsilon" };
struct variable endright = { true, "$" };
struct variable start;

unordered_map<string, vector<struct variable>> first;
unordered_map<string, vector<struct variable>> follow;

// code for left recursion elimination
void eliminate_left_recursion();
// code for first
void findfirst();
// code for follow
void findfollow();
// code for making parse table
unordered_map<string, unordered_map<string, struct expr>> generate_parse_table(); // no of rows = n_nonterminals no of columns = n_terminals
// code for parsing
bool parse(unordered_map<string, unordered_map<string, struct expr>> parse_table, string str);

vector<struct variable> remove_duplicates(vector<struct variable> vec)
{
    set<struct variable> s( vec.begin(), vec.end() );
    vec.assign( s.begin(), s.end() );
    return vec;
}

vector<string> split(string str, char delimiter) 
{
    //cout << "splitting " << str << " on " << delimiter << endl;
    vector <string> tokens; 
    stringstream check1(str); 
      
    string temp; 
    while(getline(check1, temp, delimiter)) 
    { 
        tokens.push_back(temp); 
    }
    return tokens;
}

string trim(const string str) 
{
    auto it = str.begin();
	while (it != str.end() && isspace(*it)) // isspace is a predefined function
		it++;
 
	auto rit = str.rbegin(); //reverse iterator
	while (rit.base() != it && isspace(*rit))
		rit++;
 
	return string(it, rit.base());
}

struct variable get_variable(string str, bool *flag)
{
    for (int i = 0; i < n_terminal; i++)
    {
        if(str.compare(terminals.at(i).value) == 0) 
        {
            *flag = true;
            return terminals.at(i);
        }
    }
    for (int i = 0; i < n_nonterminal; i++)
    {
        if(str.compare(nonterminals.at(i).value) == 0) 
        {
            *flag = true;
            return nonterminals.at(i);
        }
    }
    *flag = false;
    return { false, "" };    
}

struct expr build_expr(string str, bool *flag)
{
    vector<string> temp_str = split(str, ' ');
    struct expr temp_expr = {};
    for (int i = 0; i < temp_str.size(); i++)
    {
        struct variable temp_var = get_variable(temp_str.at(i), flag);
        if (!(*flag)) return temp_expr;
        temp_expr.arr.push_back(temp_var);
    }
    return temp_expr;    
}

void display(vector<struct production> var)
{
    for (int i = 0; i < var.size(); i++)
    {
        var.at(i).display();
        cout << endl;        
    }    
}

void first_util(struct production var, bool *done, bool *stack) 
{
    // cout << "Computing first of " ;
    // var.display();
    // cout << endl;
    // cout << var.rhs.size() << endl;
    int j = 0;
    for (int i = 0; i < var.rhs.size(); i++)
    {
        if (j == var.rhs.at(i).arr.size()) 
        {
            first[var.lhs.value].push_back(epsilon);
            j = 0;
        }
        else if (var.rhs.at(i).arr.at(j).compare(epsilon)) 
        {
            first[var.lhs.value].push_back(epsilon);
            j = 0;
        }
        else if (var.rhs.at(i).arr.at(j).flag == true) 
        {
            first[var.lhs.value].push_back(var.rhs.at(i).arr.at(j));
            j = 0;
        }
        else 
        {
            if (!(done[var.rhs.at(i).arr.at(j).find(nonterminals)]))
            {
                // cout << var.rhs.at(i).arr.at(j).value << " is not done." << endl;
                if(!(stack[var.rhs.at(i).arr.at(j).find(nonterminals)]))
                {
                    // cout << var.rhs.at(i).arr.at(j).value << " is not on stack." << endl;
                    for (int k = 0; k < n_production; k++)
                    {
                        if(production_list.at(k).lhs.compare(var.rhs.at(i).arr.at(j))) 
                        {
                            stack[var.lhs.find(nonterminals)] = true;
                            first_util(production_list.at(k), done, stack);
                            stack[var.lhs.find(nonterminals)] = false;
                            break;
                        }
                    }
                }              
            }

            int pos_eps = epsilon.find(first[var.rhs.at(i).arr.at(j).value]);
            first[var.lhs.value].insert(first[var.lhs.value].begin(), first[var.rhs.at(i).arr.at(j).value].begin(), first[var.rhs.at(i).arr.at(j).value].end());
            if (pos_eps >= 0)
            {
                first[var.lhs.value].erase(first[var.lhs.value].begin() + pos_eps);
                j++;
                i--;
                continue;
            }            
        }
    }
    done[var.lhs.find(nonterminals)] = true; 
    // var.lhs.display();
    // cout << " = ";   
    // for (int j = 0; j < first[var.lhs.value].size(); j++)
    // {
    //     first[var.lhs.value].at(j).display();   
    //     cout << " ";
    // } 
    // cout << endl;
}

void findfirst() 
{   
    bool done[n_nonterminal] = {false};
    bool stack[n_nonterminal] = {false};
    for (int i = 0; i < n_nonterminal; i++)
    {
        if (!(done[i]))
        {
            // cout << "Do first of ";
            // nonterminals[i].display();
            // cout << endl;
            for (int k = 0; k < n_production; k++)
            {
                if(production_list.at(k).lhs.compare(nonterminals[i])) 
                {
                    first_util(production_list.at(k), done, stack);
                    break;
                }
            }
        }
    } 
}

bool compare_vars(struct variable a, struct variable b)
{
    return a.compare(b);
}

vector<struct variable> followfirst_util(struct expr beta)
{
    vector<struct variable> temp_first;
    for (auto it = beta.arr.begin(); it != beta.arr.end(); it++)
    {
        if (it->flag) 
        {
            // terminal
            temp_first.insert(temp_first.begin(), *it);
            break;
        }
        else
        {
            // non-terminal
            temp_first.insert(temp_first.begin(), first[it->value].begin(), first[it->value].end());
            int pos = epsilon.find(first[it->value]);
            if (pos < 0) break;
            else 
            {
                if (!(it+1 == beta.arr.end())) temp_first.erase(temp_first.begin()+pos);
            }
        }
    }
    return remove_duplicates(temp_first);   
}

void follow_util(struct variable var, bool *done, bool *stack)
{
    for (auto it = production_list.begin(); it != production_list.end(); it++)
    {// for each production
        for (auto jt = it->rhs.begin(); jt != it->rhs.end(); jt++)
        {// for each expression in a production
            int pos = var.find(jt->arr);
            if (pos >= 0)
            {
                if (pos == jt->arr.size()-1)
                {
                    //find follow of lhs if not found yet
                    // update stack if calling a new function
                    // check stack if calling a new function
                    // append follow of lhs to follow of var
                    if (!(done[it->lhs.find(nonterminals)])) 
                    {
                        if(!(stack[var.find(nonterminals)]))
                        {
                            stack[var.find(nonterminals)] = true;
                            follow_util(it->lhs, done, stack);
                            stack[var.find(nonterminals)] = false;
                        }
                    }
                    follow[var.value].insert(follow[var.value].begin(), follow[it->lhs.value].begin(), follow[it->lhs.value].end());
                }
                else 
                {
                    // find first of beta
                    // append first of beta in follow of var
                    // if first of beta contains epsilon, then
                    // repeat the same process as in if part
                    vector<struct variable> beta (jt->arr.begin()+pos+1, jt->arr.end());
                    vector<struct variable> beta_first = followfirst_util({beta});
                    int pos_eps = epsilon.find(beta_first);
                    if (pos_eps >= 0)
                    {
                        beta_first.erase(beta_first.begin() + pos_eps);
                        follow[var.value].insert(follow[var.value].begin(), beta_first.begin(), beta_first.end());
                        if (!(done[it->lhs.find(nonterminals)])) 
                        {
                            if(!(stack[var.find(nonterminals)]))
                            {
                                stack[var.find(nonterminals)] = true;
                                follow_util(it->lhs, done, stack);
                                stack[var.find(nonterminals)] = false;
                            }
                        }
                        follow[var.value].insert(follow[var.value].begin(), follow[it->lhs.value].begin(), follow[it->lhs.value].end());
                    }
                    else follow[var.value].insert(follow[var.value].begin(), beta_first.begin(), beta_first.end());
                }
            }
        }        
    }
    follow[var.value] = remove_duplicates(follow[var.value]);
    done[var.find(nonterminals)] = true;    
}

void findfollow()
{
    bool done[n_nonterminal] = {false};
    bool stack[n_nonterminal] = {false};
    follow[start.value].push_back(endright);
    for (int i = 0; i < n_nonterminal; i++)
    {
        if (!(done[i]))
        {
            // cout << "Do first of ";
            // nonterminals[i].display();
            // cout << endl;
            follow_util(nonterminals[i], done, stack);
        }
    }
}

void eliminate_left_recursion()
{// A -> Aalpha_1 | Aalpha_2 |... | beta_1 | beta_2 | beta_3 | ...
// A -> beta_1A' | ....
// A' -> alpha_!A' |... | epsilon
    vector<struct production> temp_production_list;
    for (auto it = production_list.begin(); it != production_list.end(); it++)
    {
        vector<struct expr> startwithlhs;
        vector<struct expr> others;
        for (auto jt = it->rhs.begin(); jt != it->rhs.end(); jt++)
        {
            if(jt->arr.at(0).compare(it->lhs)) startwithlhs.push_back(*jt);
            else others.push_back(*jt);
        }
        if (startwithlhs.size() == 0) temp_production_list.push_back(*it);
        else 
        {
            struct variable new_nonterminal = { false, it->lhs.value };
            new_nonterminal.value.push_back('_');
            n_nonterminal += 1;
            nonterminals.push_back(new_nonterminal);
            first[new_nonterminal.value] = vector<struct variable> ();
            follow[new_nonterminal.value] = vector<struct variable> ();

            struct production new_prod = {};
            new_prod.lhs = it->lhs;
            for (auto jt = others.begin(); jt != others.end(); jt++)
            {
                jt->arr.push_back(new_nonterminal);
                new_prod.rhs.push_back(*jt);
            }
            temp_production_list.push_back(new_prod);   
            
            if(!(epsilon.find(terminals))) 
            {
                n_terminal += 1;
                terminals.push_back(epsilon);
            }
            new_prod = { new_nonterminal, vector<struct expr> () };
            new_prod.rhs.push_back({ { epsilon } });   
            for (auto jt = startwithlhs.begin(); jt != startwithlhs.end(); jt++)
            {
                jt->arr.erase(jt->arr.begin());
                jt->arr.push_back(new_nonterminal);
                new_prod.rhs.push_back(*jt);
            }     
            temp_production_list.push_back(new_prod);  
        }        
    } 
    production_list = temp_production_list;  
    n_production = temp_production_list.size(); 
}

unordered_map<string, unordered_map<string, struct expr>> generate_parse_table(bool *flag)
{
    unordered_map<string, unordered_map<string, struct expr>> parse_table;
    for (auto it = nonterminals.begin(); it != nonterminals.end(); it++)
    {
        parse_table[it->value] = unordered_map<string, struct expr> ();
        for (auto jt = terminals.begin(); jt != terminals.end(); jt++)
        {
            if (epsilon.compare(*jt)) continue;
            parse_table[it->value][jt->value] = {{}};
        }        
    }
    
    for (auto it = production_list.begin(); it != production_list.end(); it++)
    {
        for (auto jt = it->rhs.begin(); jt != it->rhs.end(); jt++)
        {
            vector<struct variable> temp_first = followfirst_util(*jt);
            for (auto kt = temp_first.begin(); kt != temp_first.end(); kt++)
            {
                if(epsilon.compare(*kt)) 
                {
                    for (auto lt = follow[it->lhs.value].begin(); lt != follow[it->lhs.value].end(); lt++)
                    {
                        if (parse_table[it->lhs.value][lt->value].arr.size() == 0) parse_table[it->lhs.value][lt->value] = *jt;
                        else {
                            *flag = false;
                            return parse_table;
                        }
                    }                    
                }
                else 
                {
                    if (parse_table[it->lhs.value][kt->value].arr.size() == 0) parse_table[it->lhs.value][kt->value] = *jt;
                    else {
                        *flag = false;
                        return parse_table;
                    }
                }
            }            
        }        
    }
    *flag = true;  
    return parse_table;    
}

bool parse(unordered_map<string, unordered_map<string, struct expr>> parse_table, vector<struct variable> str)
{
    str.push_back(endright);
    vector<struct variable> stack;
    stack.push_back(endright);
    stack.push_back(start);

    auto itr = str.begin();
    while (true)
    {
        struct variable top = stack.at(stack.size()-1);
        if (top.compare(*itr) && itr->compare(endright))
        {
            return true;
        }
        else if(top.compare(*itr))
        {
            stack.pop_back();
            itr++;
        }
        else if(top.flag)
        {
            cout << "Error: " << top.value << " expected instead of " << itr->value << " at position " << distance(str.begin(), itr) << endl; 
            return false;
        }
        else
        {
            if (parse_table[top.value][itr->value].arr.size() > 0)
            {
                cout << top.value << " -> ";
                parse_table[top.value][itr->value].display();
                cout << endl;
                stack.pop_back();
                if (!(epsilon.compare(parse_table[top.value][itr->value].arr.at(0))))
                {
                    for (auto it = parse_table[top.value][itr->value].arr.rbegin(); it != parse_table[top.value][itr->value].arr.rend(); it++)
                    {
                        stack.push_back(*it);
                    }
                }
            }
            else
            {
                cout << "Error: Cannot match " << top.value << " with " << itr->value << endl; 
                return false;
            }
        }
    }    
}

/*
epsilon is recognised as epsilon
when entering productions, give spaces between each variable
Also, all productions having same lhs will be on the same line with rhs(s) separated by |
For, example:-
E : E + T | E - T | T
T : T * F | T / F | F
F : id | num | ( E )
The above grammar has 8 terminals, 3 non-terminals, and 3 productions
If your grammar has epsilon, then count it as a terminal and write "epsilon" wherever it appears

Now, the input file must be in this format:
<number of terminals in your grammar> <number of non-terminals n your grammar> <number of productions>
<a space separated list of terminals>
<a space separated list of non-terminals>
<start symbol>
Following lines will contain productions where each production will be on a new line and in the above format.
*/


int main(int argc, char **argv) 
{
    if (argc != 3)
    {
        cout << "Too few arguments.\n";
        return 0;
    }
    // ifstream cin(argv[1]);
    freopen(argv[1], "r", stdin);
    // cout << "Enter number of terminals in your grammar: ";
    cin >> n_terminal;
    // cout << "Enter number of non-terminals n your grammar: ";
    cin >> n_nonterminal;
    // cout << "Enter number of productions: ";
    cin >> n_production;
    
    // cout << "Enter terminals: \n";
    for (int i = 0; i < n_terminal; i++)
    {
        struct variable temp = {};
        cin >> temp.value;
        temp.flag = true;
        terminals.push_back(temp);
    }
    n_terminal += 1;
    terminals.push_back(endright);
    
    // cout << "Enter non-terminals: \n";
    for (int i = 0; i < n_nonterminal; i++)
    {
        struct variable temp = {};
        cin >> temp.value;
        temp.flag = false;
        nonterminals.push_back(temp);
        first[temp.value] = vector<struct variable> ();
        follow[temp.value] = vector<struct variable> ();
    }

    // cout << "Enter start symbol:- ";
    cin >> start.value;
    start.flag = false;

    // check if nonterminals match terminals
    
    cin.ignore();
    // cout << "Enter productions: \n";
    for (int i = 0; i < n_production; i++)
    {
        string temp_str;
        bool flag;
        getline(cin, temp_str);
        // cout << temp_str << endl;
        vector<string> temp_vec = split(temp_str, ':');
        // cout << temp_vec.size() << endl;
        struct production temp_prod = {};
        temp_prod.lhs = get_variable(trim(temp_vec.at(0)), &flag);
        if (flag == false) 
        {
            cout << "Incorrect production. Non terminal " << trim(temp_vec.at(0)) << " not found.\n";
            return 0;
        }
        flag = true;
        temp_vec = split(trim(temp_vec.at(1)), '|');
        for (int i = 0; i < temp_vec.size(); i++)
        {
            struct expr temp_expr = build_expr(trim(temp_vec.at(i)), &flag);
            if (flag == false) 
            {
                cout << "Incorrect production. Variables don't match here " << trim(temp_vec.at(i)) << ".\n";
                return 0;
            }
            if (epsilon.compare(temp_expr.arr.at(0))) temp_prod.rhs.insert(temp_prod.rhs.begin(), temp_expr);
            else temp_prod.rhs.push_back(temp_expr);
        }
        production_list.push_back(temp_prod);       
    }

    cout << "You entered the following grammar :- " << endl;
    display(production_list);    
    
    eliminate_left_recursion();
    cout << "\nAfter eliminating left recursion: -"<< endl;
    display(production_list);    


    findfirst();
    findfollow();
    cout << "\nList of firsts:- " << endl;
    for (auto it = first.begin(); it != first.end(); it++)
    {
        cout << it->first << " = " ;
        for (int i = 0; i < it->second.size(); i++)
        {
            it->second.at(i).display();
            cout << " ";
        }
        cout << endl;        
    } 
    
    cout << "\nList of follows:- " << endl;
    for (auto it = follow.begin(); it != follow.end(); it++)
    {
        cout << it->first << " = " ;
        for (int i = 0; i < it->second.size(); i++)
        {
            it->second.at(i).display();
            cout << " ";
        }
        cout << endl;        
    }  

    bool flag;
    unordered_map<string, unordered_map<string, struct expr>> parse_table = generate_parse_table(&flag);
    if (flag) {
        cout << "\nParse table :-" << endl;
        cout << "\t";
        for (int i = 0; i < n_terminal; i++) 
        { 
            if (epsilon.compare(terminals.at(i))) continue;
            cout << terminals.at(i).value << "\t";
        }
        cout << endl;

        for (auto it = nonterminals.begin(); it != nonterminals.end(); it++)
        {
            cout << it->value << "\t";
            for (auto jt = terminals.begin(); jt != terminals.end(); jt++)
            {
                if (epsilon.compare(*jt)) continue;
                parse_table[it->value][jt->value].display();
                cout << "\t";
            }        
            cout << endl;
        }
        cout << "\n";

        //ifstream cin(argv[2]);
        freopen(argv[2], "r", stdin);
        string input;
        getline(cin, input);
        flag = parse(parse_table, build_expr(input, &flag).arr);
        if(flag) cout << "\nString accepted." << endl;  
    }
    else cout << "Incorrect grammar\n";
}


// 1. E − > E + T
// 2. E − > E − T
// 3. E − > T
// 4. T − > T ∗ F
// 5. T − > T /F
// 6. T − > F
// 8. F − > num
// 9. F − > id
// 10. F − > (E)