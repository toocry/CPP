#ifndef SOURCE_H_
#define SOURCE_H_
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

struct Id_Pos
{
    std::string id;
    int pos;

    bool operator<(const Id_Pos& x) const {return this->id < x.id;}
    bool operator!=(const Id_Pos& x) const {return this->id != x.id;}
};

struct Article
{
    //string id;
    std::string name;
    std::string text;
};


class Fulltext
{
public:
    void process_links(std::istream& Article);
    void output_id_pos(std::map<std::string, Article>& database);
    void process_questions(std::istream& question);
    std::map<std::string, std::vector<Id_Pos>> r_index;
    std::map<std::string, Article> database;
 
private:
    void search(std::vector<std::string> v);
    std::vector<std::string> parce_q_to_v(std::string& s);
    void go_through(const std::string& str, std::string& help, std::map<std::string, Article>::iterator& itr_map);
    void print_75_digits(Id_Pos& id_pos);
    void compare_itrs(std::vector<std::vector<Id_Pos>::iterator>& v_of_itr);
    bool contains(std::vector<Id_Pos>& vec, std::string& elem);
    void check_if_exist(Id_Pos rindex, std::string& str); 
    void add_fictive_end();
    bool all_equal(std::vector<std::vector<Id_Pos>::iterator>& v_of_itr);
    int find_min(std::vector<std::vector<Id_Pos>::iterator>& v_of_itr);

};

#endif