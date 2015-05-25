







// -------------------- Ergasia sto mathima Anaktisi Pliroforias --------------- //
//author Nikos Georgiadis
//aem 2043






#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <math.h>
#include <ctime>
#include <unistd.h>


#include "foo.h"

using namespace std;



int main (int argc,char *argv[])
{




    // ------------------------------ 1o MEROS ERGASIAS (XTISIMO KATALOGOU ME THREADS) ------------- //







    clock_t begin = clock();
    int num_of_docs=0; //arithmos documents

    int threads_num=0; //arithmos nimatwn
    if (sscanf (argv[2], "%i", &threads_num)!=1)
    {
        printf ("error - not an integer");
    }

    std::vector<std::thread> threads (threads_num) ;
//   cout<<threads_num<<endl;
    string line ;
    int doc_id=0;

    std::vector<std::unordered_map<std::string, std::map<int,std::vector<int>>  >> hashtables (threads_num);


    int start =1 ;
    int end_=0;
    ifstream myfile (argv[1]);

    Word word;
    Doc doc;
    std::vector<Word> words  ;


    std::vector <Doc> docs;

    int counter_words=0;


    if (myfile.is_open())
    {

        myfile>>num_of_docs;   // arithmos twn documents
        getline (myfile, line );
        //cout<<"Number of docs: "<<num_of_docs<<endl;
        myfile>>word.doc_id ;
        // cout<<"Number of doc id: "<<word.doc_id<<endl;
        while (getline(myfile, line))       //reading and parsing
        {


            // cout<<"Number of doc id "<<doc_id<<endl;

            //    cout << line << '\n';
            char*  pch;
            //cout<< "Splitting string "<<line <<"into tokens:\n"<<endl;
            // cout<<<<line <<<<endl;
            pch = strtok (const_cast<char*>(line.c_str())," ,.-");
            int count =0;
            while (pch != NULL)
            {
                count++;
                counter_words++;
                // printf ("%s\n",pch);
                // cout<<pch<<endl;
                word.name=pch;
                word.position=count;

                words.push_back(word) ;
                pch = strtok (NULL, " ,.-");

            }


            doc.length = count;
            doc.id=word.doc_id;

            docs.push_back(doc);
            myfile>>word.doc_id;


        }
        //  cout << "S T A R T" << endl;
        for (int i=0; i<words.size(); i++)
        {
            // cout << words[i].name <<"  Doc id  ---->  "<<words[i].doc_id<<"  Position  --->  "<<words[i].position<< endl;

        }


    }

    else cout << "Unable to open file";

//   cout<<"Counter words -->  "<<counter_words<<endl;
    int num_words_per_thread=counter_words / threads_num;
    //  cout<<"Number of words per thread --->  "<<num_words_per_thread<<endl;

    int rest = counter_words %  threads_num ;
    //  cout<<"Rest --->  "<<rest<<endl;
    int extra=0;


    Word** temps  = new Word*[threads_num];


    std::map<int,std::vector<int>>  OccurencesOfWord;

    std::unordered_map<std::string, std::map<int,std::vector<int>> >  hashtable;//o hashtable pou tha apothikeuei tis lekseis

    for(int i = 0; i < threads_num; ++i)
        temps[i] = new Word[num_words_per_thread+rest];
    //  cout<<"=====================================================    "<<num_words_per_thread+rest+1<<endl;

    for (int i=1; i<=threads_num; i++)
    {

        if (i==threads_num )
        {
            extra=rest;


        }

        vector<Word>::const_iterator first = words.begin() + (i-1)*num_words_per_thread;
        vector<Word>::const_iterator last = words.begin() + (i)*num_words_per_thread+extra;
        copy( first, last,temps[i-1] );

        // cout << "TEMP   S T A R T" << endl;
        for (int j=0; j<num_words_per_thread; j++)
        {
            //   cout << temps[i-1][j].name <<"  Doc id  ---->  "<<temps[i-1][j].doc_id<<"  Position  --->  "<<temps[i-1][j].position<< endl;

        }


        threads.push_back(std::thread(create_invertedindex,temps[i-1],i-1,num_words_per_thread+rest,&hashtables[i-1]));

    }


    /*if ( argc > 1 )
    {
        threads_num = atoi( argv[2] );
    }
    */

    // cout<<"telos!!!!!!!!!!!!! "<<endl;
    for(auto& t : threads)
    {
        try
        {
            if(t.joinable())
                t.join();
        }
        catch (std::system_error& e)
        {
            //   cout<< "False join "<<endl;
        }
    }

//   std::cout << "threads joined\n";
    // cout<<"===================================================================="<<endl;


    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;


    // --- PRINT TON INDEX ---- //



    for(int i = 0; i < threads_num; i++)
    {
        //   cout << " -------------------------------  "<<i<<"  ------------------------"<<endl;
        typedef std::unordered_map<std::string, std::map<int,std::vector<int>> >  ::iterator it_type;
        for(it_type iterator = hashtables[i].begin(); iterator != hashtables[i].end(); iterator++)
        {
            string  word ;
            std::map<int,std::vector<int>>  value ;
            word = iterator->first  ;
            value =iterator->second;
            //   cout<<" KEY -->    "<<word<<endl;
            //  cout<<word <<" VALUE --->   "<<cout<<word<<endl;
            //cout<<iterator->second<<endl;
            typedef  std::map<int,std::vector<int>>  ::iterator it_type_2;
            //std::map<int,std::vector<int>>  OccurencesOfWord;
            for(it_type_2 iterator = value.begin(); iterator != value.end(); iterator++)
            {
                //   cout  <<" DOC ID  --->   "<<iterator->first<<endl;
                //   cout  <<" POSITION --->   "<<iterator->second[1]<<endl;
                for (int i=0; i<iterator->second.size(); i++)
                {
                    //  cout <<" POSITION --->   "<< iterator->second[i] << "         ";
                }
                // cout<<endl;
            }


        }
    }





                //----------------------------- MERGING ------------------------------ //







    for (int i=0; i<threads_num-1; i++)
    {
        //  cout << " -------------------------------  "<<i<<"  ------------------------"<<endl;
        typedef std::unordered_map<std::string, std::map<int,std::vector<int>> >  ::iterator it_type;
        for(it_type iterator = hashtables[i].begin(); iterator != hashtables[i].end(); iterator++)
        {
            string  word ;
            std::map<int,std::vector<int>>  value ;
            word = iterator->first  ;
            value =iterator->second;
            //  cout<<" KEY -->    "<<word<<endl;

            if (hashtables[threads_num-1].count(word)==1 )
            {

                typedef  std::map<int,std::vector<int>>  ::iterator it_type_2;
                //std::map<int,std::vector<int>>  OccurencesOfWord;
                for(it_type_2 iterator = value.begin(); iterator != value.end(); iterator++)
                {
                    //  cout  <<" DOC ID  --->   "<<iterator->first<<endl;
                    //   cout  <<" POSITION --->   "<<iterator->second[1]<<endl;
                    if (hashtables[threads_num-1][word].count(iterator->first)==1)
                    {


                        for (int i=0; i<iterator->second.size(); i++)
                        {
                            //  cout <<" POSITION --->   "<< iterator->second[i] << "         ";
                            hashtables[threads_num-1][word][iterator->first].push_back(iterator->second[i]) ;
                        }
                        //   cout<<endl;

                        //----- tha prosthesw to id doc kai to position tis leksis ---- //
                    }
                    else
                    {
                        std::vector<int> position_in_text; //dimiourgia vector opou apothikeuei tin thesi tis leksi sto doc

                        for (int i=0; i<iterator->second.size(); i++)
                        {
                            // cout <<" POSITION --->   "<< iterator->second[i] << "         ";
                            position_in_text.push_back(iterator->second[i]);
                        }
                        //   cout<<endl;



                        hashtables[threads_num-1][word][iterator->first]=position_in_text;


                    }
                }

                //--- tha eisagw tin leksi mesa ston antestrammeno katalogo kai tha ftiaksw map kai vector gia na eisagw  to doc id kai to position tis leksis antistoixa --- //
            }
            else
            {
                typedef  std::map<int,std::vector<int>>  ::iterator it_type_2;
                //std::map<int,std::vector<int>>  OccurencesOfWord;
                std::map<int,std::vector<int>>  OccurencesOfWord; // dimourgia map to opoio periexei tis emfaniseis tis leksis sto doc

                for(it_type_2 iterator = value.begin(); iterator != value.end(); iterator++)
                {
                    //  cout  <<" DOC ID  --->   "<<iterator->first<<endl;
                    std::vector<int> position_in_text; //dimiourgia vector opou apothikeuei tin thesi tis leksi sto doc

                    for (int i=0; i<iterator->second.size(); i++)
                    {
                        //   cout <<" POSITION --->   "<< iterator->second[i] << "         ";
                        position_in_text.push_back(iterator->second[i]);
                    }
                    // cout<<endl;


                    OccurencesOfWord[iterator->first]=position_in_text;




                }


                hashtables[threads_num-1][word]=OccurencesOfWord;



            }

            // iterator->second = value
            // Repeat if you also want to iterate through the second map.
        }



    }








// free dynamically allocated memory
    for( int i = 0 ; i < threads_num ; i++ )
    {
        delete[] temps[i]; // delete array within matrix
    }
// delete actual matrix
    delete[] temps;


    // cout << " =========================== TELOS MERGING ========================"<<endl;


// --- PRINT TON INDEX ---- //

    //  cout << " -------------------------------  "<<threads_num-1<<"  ------------------------"<<endl;
    typedef std::unordered_map<std::string, std::map<int,std::vector<int>> >  ::iterator it_type;
    for(it_type iterator = hashtables[threads_num-1].begin(); iterator != hashtables[threads_num-1].end(); iterator++)
    {
        string  word ;
        std::map<int,std::vector<int>>  value ;
        word = iterator->first  ;
        value =iterator->second;
        //  cout<<" KEY -->    "<<word<<endl;
        //  cout<<word <<" VALUE --->   "<<cout<<word<<endl;
        //cout<<iterator->second<<endl;
        typedef  std::map<int,std::vector<int>>  ::iterator it_type_2;
        //std::map<int,std::vector<int>>  OccurencesOfWord;
        for(it_type_2 iterator = value.begin(); iterator != value.end(); iterator++)
        {
            //   cout  <<" DOC ID  --->   "<<iterator->first<<endl;
            //   cout  <<" POSITION --->   "<<iterator->second[1]<<endl;
            for (int i=0; i<iterator->second.size(); i++)
            {
                //  cout <<" POSITION --->   "<< iterator->second[i] << "         ";
            }
            //  cout<<endl;
        }

        // iterator->second = value
        // Repeat if you also want to iterate through the second map.
    }






    //-------------------   DIAVASMA TO ARXEIO ---------------- //


    clock_t begin2 = clock();

    Query query;
    std::vector<Query> queries  ;
    ifstream myfile2 (argv[3]);
    if (myfile2.is_open())
    {

        int num_queries;

        string line ;
        //  cout <<" ANOIKSE KAI TO DEUTERO ! "<<endl;
        myfile2>>num_queries;   // arithmos twn documents
        getline (myfile2, line );
        // cout<<"Number of queries: "<<num_queries<<endl;
        myfile2>>query.id_query ;

        myfile2>>query.num_answers ;



        while (getline(myfile2, line))       //reading and parsing
        {


            // cout<<"Number of query id: "<<query.id_query<<endl;

            //cout<<"Number of answers: "<<query.num_answers<<endl;
            // getline (myfile,line);
            //  cout << line << '\n';
            char*  pch;
            //  cout<< "Splitting string "<<line <<"into tokens:\n"<<endl;
            pch = strtok (const_cast<char*>(line.c_str())," ,.-");

            int count =0;
            while (pch != NULL)
            {

                //  count++;
                // counter_words++;
                // printf ("%s\n",pch);
                //  cout<<pch<<endl;


//std::string str(pch);
                query.query_string.push_back(pch) ;

                //cout<<   query.query_string.back()<<endl;
                //   cout<<    query.query_string.push_back(pch) ;
                pch = strtok (NULL, " ,.-");

            }
            queries.push_back(query);
            query.query_string.clear();
            myfile2>>query.id_query ;
            myfile2>>query.num_answers ;
            //Query query;
            //  queries.push_back(query);
        }


    }
    else cout << "Unable to open file "<<argv[3]<<endl;







    // ---------------------- QUERIES (2o meros ergasias)  --------------------------- //












    for(int i = 0; i <queries.size(); i++)
    {
        cout<<endl;
        if ( i!=0)
        {
            cout<<endl;
            cout<<"------------------------------------------------------"<<endl;
            cout<<endl;
        }
        cout<<endl;
        cout<<"Ερώτημα ID: "<<queries[i].id_query<<endl;


        cout<<"Aριθμός απαντήσεων: "<<queries[i].num_answers<<endl;
        cout<<"Ερώτημα :    ";
        //cout<<"====================Pame sto epomeno arxeio============"<<endl;
        std::map<int,double> weights;
        for ( int j=0; j<queries[i].query_string.size(); j++)
        {

            cout<<queries[i].query_string[j]<<"  ";
            //  cout<<endl;

            //cout << " -------------------------------  "<<queries[i].query_string[j]<<"  ------------------------"<<endl;
            //  cout <<"Arithmos leksewn sto doc ===   "<<queries[i].query_string.size()<<endl;
            double IDF=0;
            //----------------- YPOLOGISMOS IDF  ----------------------//

            if (hashtables[threads_num-1].count(queries[i].query_string[j])==1)
            {


                IDF = (double)log2((double)num_of_docs/(double) hashtables[threads_num-1][queries[i].query_string[j]].size()  );
                //   cout<<"Arithmos docs pou periexoun tin leksi ----> "<<hashtables[threads_num-1][queries[i].query_string[j]].size()<<endl;
                //   cout<<"Sunolikos arithmos docs ---->    "<<num_of_docs<<endl;
                //   cout<<"IDF ---> "<<IDF<<endl;

            }

            //----------------- YPOLOGISMOS TF  ----------------------//

            TF tf ;
            std::vector<TF> tfs;


            typedef  std::map<int,std::vector<int>>  ::iterator it_type_2;

            for(it_type_2 iterator =  hashtables[threads_num-1][queries[i].query_string[j]].begin(); iterator !=  hashtables[threads_num-1][queries[i].query_string[j]].end(); iterator++)
            {
                tf.doc_id= iterator->first;
                tf.freq=iterator->second.size();
                tf.max_freq=0;
                tfs.push_back(tf);
                // cout<<"Doc id pou periexei tin leksi --->  "<<tf.doc_id<<endl;
                // cout<<"Freq ---->   "<<tf.freq<<endl;
            }

            typedef std::unordered_map<std::string, std::map<int,std::vector<int>> >  ::iterator it_type;
            for(it_type iterator = hashtables[threads_num-1].begin(); iterator != hashtables[threads_num-1].end(); iterator++)
            {
                for (int z=0; z<tfs.size(); z++ )
                {

                    if (iterator->second.count(tfs[z].doc_id)==1)
                    {
                        if (tfs[z].max_freq<iterator->second[tfs[z].doc_id].size() )
                        {
                            tfs[z].max_freq= iterator->second[tfs[z].doc_id].size();

                        }

                    }


                }



            }
            // Word_2 words_2 ;
            // words_2.name = queries[i].query_string[j];
            // Weight weight;
            //   words_2.doc_id =
            for (int z=0; z<tfs.size(); z++ )
            {
                //  cout<<"Doc id --->  "<<tfs[z].doc_id<<endl;
                //  cout <<"Max Freq --->   "<<tfs[z].max_freq<<endl;
                //  Word_2 words_2 ;
                double TF_= (double)tfs[z].freq/(double)tfs[z].max_freq;
                double weight=IDF * TF_;

                // cout <<"TF --->   "<<TF_<<"  IDF --->   "<<IDF<<"   Weight --->  "<<weight<<endl;

                int  id=tfs[z].doc_id ;
                //words_2.weights.push_back(weight ) ;
                if (weights.count(id)==1)
                {

                    weights[id]=weights[id]+weight;

                }
                else
                {
                    weights[id]=weight ;

                }

            }



        }

        std::priority_queue<Weight> q;
        //Weight w;
        typedef  std::map<int,double>  ::iterator it_type_3;
        for(it_type_3 iterator = weights.begin(); iterator != weights.end(); iterator++)
        {
            Weight w;
            // cout<<"Weight ID--> "<<iterator->first<<" value -->"<<iterator->second<<endl ;
            for (int x=0; x<docs.size(); x++)
            {
                if (docs[x].id== iterator->first )
                {
                    w.w=(double)iterator->second/(double)docs[x].length;
                    //   cout<<"Length Doc ---> "<<docs[x].length<<endl;
                    break;
                }
            }
            w.id=iterator->first;
            // w.w=iterator->second;
            q.push(w);
        }
        cout<<endl;
        cout<<endl;
        cout<<"===========    Λίστα με τα αποτελέσματα αναζήτησης    ==========="<<endl;
        //   cout<<"====================================="<<endl;
        for (int  k=0; k<queries[i].num_answers; k++ )
        {
            Weight  w_ = q.top();

            cout<<k+1<<".    ID --->   "<<  w_.id<<endl;
            //  cout<<"Weight --->   "<<  w_.w<<endl;
            q.pop();
            //  cout << weight.w << endl;

        }

        weights.clear();




    }





    clock_t end2 = clock();
    double elapsed_secs2 = double(end2 - begin2) / CLOCKS_PER_SEC;










    //------------------- BONUS (Anaktisi frasewn mesa se docs )  ---------------------- //









    cout<<endl;
    cout<<endl;
    cout<<"============ *** BONUS *** ==============="<<endl;
    //  cout <<"----------------   "<<queries.size()<<endl;



    for(int i = 0; i <queries.size(); i++)
    {
        int document_id=0;
        // cout<<"====================Pame sto epomeno arxeio============"<<endl;
        //  std::map<int,double> weights;
        bool flag=true;
        int x=1;
        while (flag!=false && x!=queries[i].query_string.size() )
        {
            //   cout<<"MpiKA -->  "<<i<<endl;
            //   cout<<"leksi --->   "<<queries[i].query_string[x-1]<<endl;
            //   cout<<"epomeni leksi --->   "<<queries[i].query_string[x]<<endl;
            //   cout<<"megethos ===> " <<queries[i].query_string.size()<<endl;
            flag=false;
            typedef  std::map<int,std::vector<int>>  ::iterator it_type_4;
            for(it_type_4 iterator =  hashtables[threads_num-1][queries[i].query_string[x-1]].begin(); iterator !=  hashtables[threads_num-1][queries[i].query_string[x-1]].end(); iterator++)
            {

                //    cout<<"ID-->    "<<iterator->first<<endl;
                //cout<<"Position --->    "iterator -> second<<endl;
                // id  = iterator->first
                //position = iterator -> second
                //An i  epomeni leksi tou query vrisketai sto idio doc me tin  trexon leksi
                if (hashtables[threads_num-1][queries[i].query_string[x]].count(iterator->first )==1)
                {
                    //     cout<<"vRISKONTAI STO IDIO DOC  ! "<<endl;
                    document_id =iterator->first;
                    for (int k=0; k<iterator->second.size(); k++)
                    {
                        //   cout<<"Position --->    "<<iterator -> second[k]<<endl;
                        //an i epomeni leksi tou query vrisketai sto position +1 apo tin trexon leksi
                        if(std::find(hashtables[threads_num-1][queries[i].query_string[x]][iterator->first].begin(), hashtables[threads_num-1][queries[i].query_string[x]][iterator->first].end(), (iterator->second[k])+1)  != hashtables[threads_num-1][queries[i].query_string[x]][iterator->first].end() )
                        {
                            //    cout<<"Success !!"<<endl;
                            flag=true;
                            break ;

                        }


                    }

                    //     cout<<"Den einai i epomeni leksi !"<<endl;


                }


                if (flag==true)
                {

                    //  cout<<"Success !!"<<endl;
                    break;
                }




            }
            x++;

        }
        // cout<<"vgika apo to while !"<<endl;

        if ( x==queries[i].query_string.size() && flag==true)
        {

            cout<< "Η φράση "<<"** ";
            for ( int p=0; p<queries[i].query_string.size(); p++)
            {
                cout<<queries[i].query_string[p]<<"  ";
            }
            cout <<" ** βρέθηκε αυτούσια στο αρχείο  με ID "<<  document_id <<endl;
        }
        else {
          cout<< "Η φράση "<<"** ";
            for ( int p=0; p<queries[i].query_string.size(); p++)
            {
                cout<<queries[i].query_string[p]<<"  ";
            }
        cout <<" ** δεν βρέθηκε αυτούσια σε κανένα αρχείο !   "<<  document_id <<endl;

        }

    }

    cout <<endl;
    cout<<"============ *** ΧΡΟΝΟΣ *** ==============="<<endl;

    cout<<"Χρόνος για το χτίσιμο του αντεστραμμένου καταλόγου ---> "<<elapsed_secs<<"  secs"<<endl;
    cout<<"Χρόνος για την επεξεργασία ερωτημάτων --->  "<<elapsed_secs2<<"  secs"<<endl;

    // cout<<"Telos ergasias :) "<<endl;
    return 0;
}
