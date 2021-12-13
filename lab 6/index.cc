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
        cout << this->value << " ";
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
    vector<string> action;
    void display()
    {
        for (int i = 0; i < this->arr.size(); i++)
        {
            arr.at(i).display();
            if(action.at(i).compare("") != 0) {
              cout << "$" << action.at(i) << " ";
            }
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
        cout << " : ";
        for (int i = 0; i < this->rhs.size(); i++)
        {
            this->rhs.at(i).display();
            if (i != this->rhs.size()-1) cout << "| ";
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
        if (temp_str.at(i).at(0) == '$') {
          temp_expr.action.push_back(temp_str.at(i).substr(1));
        }
        else {
          struct variable temp_var = get_variable(temp_str.at(i), flag);
          // cout << (temp_str.at(i)) << endl;
          if (!(*flag)) return temp_expr;
          temp_expr.arr.push_back(temp_var);
          
          if(i != temp_str.size()-1) {
            if (temp_str.at(i+1).at(0) != '$') {
              temp_expr.action.push_back("");
            }
          }
          else {
            temp_expr.action.push_back("");
          }
        }
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
                string new_action = new_nonterminal.value+".in=" + jt->action.at(jt->action.size()-1);
                jt->action.clear();
                jt->action.push_back(new_action);
                jt->action.push_back(new_nonterminal.value+".syn");
                new_prod.rhs.push_back(*jt);
            }
            temp_production_list.push_back(new_prod);   
            
            if(!(epsilon.find(terminals))) 
            {
                n_terminal += 1;
                terminals.push_back(epsilon);
            }
            new_prod = { new_nonterminal, vector<struct expr> () };
            new_prod.rhs.push_back({ { epsilon }, { new_nonterminal.value+".syn="+new_nonterminal.value+".in" } });   
            for (auto jt = startwithlhs.begin(); jt != startwithlhs.end(); jt++)
            {
                jt->arr.erase(jt->arr.begin());
                jt->arr.push_back(new_nonterminal);
                string new_action = new_nonterminal.value+".in="+jt->action.at(jt->action.size()-1).substr(0, 2)+new_nonterminal.value+jt->action.at(jt->action.size()-1).substr(3);
                jt->action.clear();
                jt->action.push_back(new_action);
                jt->action.push_back(new_nonterminal.value+".syn=2*"+new_nonterminal.value+".syn");
                new_prod.rhs.push_back(*jt);
            }     
            temp_production_list.push_back(new_prod);  
        }        
    } 
    production_list = temp_production_list;  
    n_production = temp_production_list.size(); 
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

// int main() {
//     n_terminal = 2;
//     n_nonterminal = 1;
//     vector<string> temp_ter = {"0", "1"};
//     insert_terminals(temp_ter, n_terminal);
//     vector<string> temp_nonter = {"B"};
//     insert_nonterminals(temp_nonter, n_nonterminal);

    
// }

int main(int argc, char **argv) 
{
    if (argc != 2)
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
        // cout << temp_vec.at(0) << endl;
        // cout << temp_vec.at(1) << endl;
        temp_vec = split(trim(temp_vec.at(1)), '|');
        for (int i = 0; i < temp_vec.size(); i++)
        {
            // cout << temp_vec.at(i) << endl;
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

    freopen(argv[1], "w", stdout);
    cout << n_terminal << " " << n_nonterminal << " " << n_production << endl;
    for (int i = 0; i < n_terminal-1; i++)
    {
      terminals.at(i).display();
    }
    epsilon.display();
    cout << endl;
    for (int i = 0; i < n_nonterminal; i++)
    {
      nonterminals.at(i).display();
    }
    cout << endl;
    cout << start.value;
    cout << endl;
    display(production_list);       
    
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