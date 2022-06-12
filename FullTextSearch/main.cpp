#include "Source.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3)
        return 0;
    Fulltext fulltext;
    ifstream articles;
    articles.open(argv[1]);
    if (!articles.good())
        return 0;
    fulltext.process_links(articles);
    fulltext.output_id_pos(fulltext.database);
    if (argc == 2)
        fulltext.process_questions(cin);
    else if (argc == 3)
    {
        ifstream questions;
        questions.open(argv[2]);
        if (!questions.good())
            return 0;
        fulltext.process_questions(questions);
    }

}
