#include "foo.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <thread>
#include <string.h>
#include <fstream>
#include <mutex>
#include <algorithm>
#include <iterator>
#include <functional>
#include <queue>
using namespace std;
//void func(char *argv[],int start,int end_,const std::unordered_map<std::string,std::map<std::int ,int>   >  &hashtable)



void create_invertedindex(struct Word table[],int thread_num, int num_words_per_thread,  std::unordered_map<std::string, std::map<int,std::vector<int>> > *hashtable)
{
    std::thread::id this_id = std::this_thread::get_id();
    std::mutex mutex;
    std::lock_guard<std::mutex> guard(mutex);
    int doc_id=0;
    int num_of_docs;
    //cout<<"inside thread ID "<<  thread_num<<endl;
    std::unordered_map<std::string, std::map<int,std::vector<int>>> &myhashtable=*hashtable;


    for (int j=0; j<num_words_per_thread; j++)
    {
        string key=table[j].name;


        //  ---- EAN H LEKSI DEN YPARXEI STON ANTESTRAMMENO KATALOGO ----



        if ( myhashtable.count(key)==0 )
        {

            std::vector<int> position_in_text; //dimiourgia vector opou apothikeuei tin thesi tis leksi sto doc
            position_in_text.push_back(table[j].position);

            std::map<int,std::vector<int>>  OccurencesOfWord; // dimourgia map to opoio periexei tis emfaniseis tis leksis sto doc
            OccurencesOfWord[table[j].doc_id]=position_in_text;
            myhashtable[key]=OccurencesOfWord;

        }

        //  ---- EAN H LEKSI YPARXEI STON ANTESTRAMMENO KATALOGO ----

        else
        {

            //-- EAN YPARXEI IDI TO SUGKEKRIMENO DOC_ID (KEY) TOTE THA TO EKXWRISOUME STO DIKO TOU MAP

            if (myhashtable[key].count(table[j].doc_id)==1)
            {

                myhashtable[key][table[j].doc_id].push_back(table[j].position);


            }

            //-- EAN DEN YPARXEI  TO SUGKEKRIMENO DOC_ID (KEY) TOTE THA TO EKXWRISOUME STO DIKO TOU MAP

            else
            {


                std::vector<int> position_in_text; //dimiourgia vector opou apothikeuei tin thesi tis leksi sto doc
                position_in_text.push_back(table[j].position);
                myhashtable[key][table[j].doc_id]=position_in_text;

            }


        }


    }


    // --- PRINT TON INDEX ---- //

    typedef std::unordered_map<std::string, std::map<int,std::vector<int>> >  ::iterator it_type;
    for(it_type iterator = myhashtable.begin(); iterator != myhashtable.end(); iterator++)
    {
        string  word ;
        std::map<int,std::vector<int>>  value ;
        word = iterator->first  ;
        value =iterator->second;
       // cout<<" WORD -->    "<<word<<endl;
        //  cout<<word <<" VALUE --->   "<<cout<<word<<endl;
        //cout<<iterator->second<<endl;
        typedef  std::map<int,std::vector<int>>  ::iterator it_type_2;
        //std::map<int,std::vector<int>>  OccurencesOfWord;
        for(it_type_2 iterator = value.begin(); iterator != value.end(); iterator++)
        {
           // cout  <<" DOC ID  --->   "<<iterator->first<<endl;
            //   cout  <<" POSITION --->   "<<iterator->second[1]<<endl;
            for (int i=0; i<iterator->second.size(); i++)
            {
             //   cout <<" POSITION --->   "<< iterator->second[i] << "         ";
            }
         //   cout<<endl;
        }

        // iterator->second = value
        // Repeat if you also want to iterate through the second map.
    }




}


