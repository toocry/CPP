#include<iostream>
#include "Source.h"

using namespace std;



void Fulltext :: process_links(istream& clanek)
{
    Article clanky;
    string idetification;
    string name;
    string text;

    while (getline(clanek, idetification))
    {
        if (idetification.empty()) {
            break;
        }
        getline(clanek, name);
        getline(clanek, text);
        clanky.name = name;
        clanky.text = text;
        database.insert({ idetification, clanky });
        clanky.name = "";
        clanky.text = "";
    }

}

bool Fulltext::contains(vector<Id_Pos>& vec, string& elem)
{
    bool result = false;
    for (auto itr_vec = vec.begin(); itr_vec != vec.end(); ++itr_vec)
    {
        if (itr_vec->id == elem)
            result = true;
    }
    return result;
}

void Fulltext :: check_if_exist(Id_Pos rindex, string& str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    auto itr_pair = r_index.find(str);

    //not found
    if (itr_pair == r_index.end())
    {
        r_index.insert({ str, vector<Id_Pos>{ rindex } });
    }
    //found
    else
    {
        if (contains(itr_pair->second, rindex.id))
        {
            return;
        }
        else
        {
            itr_pair->second.push_back(rindex);
        }

    }
}



void Fulltext:: go_through(const string& str, string& help, map<std::string, Article>::iterator& itr_map)
{
    Id_Pos rindex;
    for (size_t index_ch = 0; index_ch != str.size(); ++index_ch)
    {
        if (isalpha(str[index_ch]))
        {
            help += str[index_ch];
        }
        else
        {
            rindex.pos = index_ch - help.size();
            rindex.id = itr_map->first;
            if (!help.empty())
                check_if_exist(rindex, help);
            help = "";
        }
    }
    if (!help.empty()) {
        rindex.pos = str.size() - help.size() + 1;
        rindex.id = itr_map->first;
        check_if_exist(rindex, help);
    }
}


//takes database and returns id and pos where it occurs
void Fulltext::output_id_pos(map<string, Article>& database)
{
    string help;

    for (auto itr_map = database.begin(); itr_map != database.end(); ++itr_map)
    {
        auto text = itr_map->second.text;
        go_through(text, help, itr_map);

    }
    add_fictive_end();

}

void Fulltext::add_fictive_end() {
    for (auto itr_map = r_index.begin(); itr_map != r_index.end(); ++itr_map)
        itr_map->second.push_back(Id_Pos{"", -1});
}


vector<string> Fulltext:: parce_q_to_v(string& s)
{
    string help;
    vector<string> help_v;
    for (auto itr = s.begin(); itr < s.end(); ++itr)
    {
        if (isalpha(*itr))
        {
            help += *itr;
        }
        else
        {
            if (!help.empty())
            {
                transform(help.begin(), help.end(), help.begin(), ::tolower);
                help_v.push_back(help);
                help = "";
            }
        }
    }
    if (!help.empty()) {
        transform(help.begin(), help.end(), help.begin(), ::tolower);
        help_v.push_back(help);
    }
    return help_v;
}


bool Fulltext::all_equal(vector<vector<Id_Pos>::iterator>& v_of_itr) 
{
    for (auto&& x : v_of_itr) 
    {
        if (*x != *v_of_itr[0])
            return false;
    }
    return true;
}

int Fulltext::find_min(vector<vector<Id_Pos>::iterator>& v_of_itr)
{
    int min = 0;
    for (size_t i = 0; i < v_of_itr.size(); ++i)
    {
        if (*v_of_itr[i] < *v_of_itr[min])
            min = i;

    } return min;
}

void Fulltext::print_75_digits(Id_Pos& id_pos)
{
    auto link = database.find(id_pos.id)->second;
    cout << '[' << id_pos.id << ']' << ' ' << link.name << endl
        << link.text.substr(id_pos.pos, 75) << "..." << endl;
}

void Fulltext::compare_itrs(vector<vector<Id_Pos>::iterator>& v_of_itr)
{
   
    bool intersect = false;
    int pos_of_min;
    while (true)
    {


        if (all_equal(v_of_itr))
        {
            print_75_digits(*v_of_itr[0]);
            intersect = true;
            for (auto&& x : v_of_itr)
            {
                x = next(x, 1);

                if (x->pos == -1) {
                    return;
                }
            }
        }
        else
        {
            pos_of_min = find_min(v_of_itr);
            v_of_itr[pos_of_min] = next(v_of_itr[pos_of_min],1);

            if (v_of_itr[pos_of_min]->pos == -1) {
                if (intersect == false)
                {
                    cout << "No results" << endl;
                }
                return;
            }
        }
    }
}

void Fulltext::search(vector<string> v)
{
    if (v.empty())
    {
        cout << "No results" << endl;
        return;
    }
    vector<vector<Id_Pos>::iterator> v_of_itr;
        
    for (auto itr = v.begin(); itr != v.end(); ++itr)
    {
        auto itr_pair = r_index.find(*itr);
        if (itr_pair == r_index.end())
        {
            cout << "No results" << endl;
            return;
        }
        v_of_itr.push_back(itr_pair->second.begin());
    }
    compare_itrs(v_of_itr);

}

void Fulltext::process_questions(istream& question)
{
    string line;
    vector <string> v_of_str;
    while (getline(question, line))
    {
        if (line.empty())
            return;
        v_of_str = parce_q_to_v(line);
        search(v_of_str);
        v_of_str.clear();
        cout << endl;
    }

}






