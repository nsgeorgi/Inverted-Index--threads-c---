#include <vector>
#include <iostream>
#include <unordered_map>
#include <map>
#include <thread>
#include <algorithm>
#include <iterator>
#include <functional>
#include <queue>
using namespace std;
//void func(char *argv[],int start,int end_,const std::unordered_map<std::string,std::map<std::int,int> > &hashtable);
//void func(Word table[],int thread_num);





struct Word
{
    string name;
    int doc_id;
    int position;
} ;



struct Doc
{
    int id;
    int length;


};


struct Query
{
    std::vector<string>  query_string;
    int id_query;
    int num_answers;


};


struct TF
{
    int doc_id;
    int freq;
    int max_freq;


};




struct Weight
{
    int id ;
    double w;
    bool operator<(const Weight& rhs) const
    {
        return w < rhs.w;
    }

};


struct Word_2
{
    string name;
    int doc_id;
    std::vector<Weight> weights;

};

//  Word word;

void create_invertedindex(struct Word table[],int thread_num, int num_words_per_thread,  std::unordered_map<std::string, std::map<int,std::vector<int>> > * hashtable);

