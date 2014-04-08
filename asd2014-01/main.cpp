/* 
 * File:   main.cpp
 * Author: giorgio
 *
 * Created on 7 aprile 2014, 15.40
 */

#include <iostream>
#include <fstream>

#include <vector>
#include <cstring>
#include <stack>

using namespace std;

struct node
{
    vector<int> adiacenti;
    int archiEntranti = 0;
    bool visitato = false;
};


/*
 * 
 */

int n = 0;              // numero di nodi
int m = 0;              // numero di archi
vector<node> grafo;     // grafo di input
stack<int> consiglieri;
int numeroConsiglieri = 0;
    

void leggiGrafo()
{
    ifstream f("/home/giorgio/NetBeansProjects/asd2014-01/asd2014-01/asd2014-01/DatasetP1/input5.txt");
    f >> n >> m;
    grafo.resize(n);
    
    cout << n << " " << m;
    
    for (int i = 0; i < m; i++)
    {
        int from, to;
        f >> from >> to;
        grafo[from].adiacenti.push_back(to);
        grafo[to].archiEntranti++;
        
        cout << "from: " << from << "   to: "<< to << endl;
    }
    
    f.close();
}

void ordineTopologico()
{
   ofstream f("/home/giorgio/NetBeansProjects/asd2014-01/asd2014-01/asd2014-01/DatasetP1/output.txt");
   vector<int> outputCons;
    
    for (int i = 0; i < n; i++)
    {
        if (grafo[i].archiEntranti == 0)
        {
            consiglieri.push(i);
            outputCons.push_back(i);
        }
    }
    
    numeroConsiglieri = consiglieri.size();
    
    stack<int> esplora;
    int i;
    
    cout << "numero consiglieri: " << numeroConsiglieri << endl;
    f << numeroConsiglieri << endl;
    
    for (int g = 0; g < outputCons.size(); g++)
    {
        f << outputCons[g] << " ";
    }
    f << endl;
    
    while (!consiglieri.empty())
    {
        i = consiglieri.top();
        
        cout << "consigliere: " << i << endl;
        consiglieri.pop();
        grafo[i].visitato = true;
        
        for (int j = 0; j < grafo[i].adiacenti.size(); j++)
        {
            //cout << "i -- grafo[i], adiacenti[j]: " << i << " -- " << grafo[i].adiacenti[j] << endl;
            if (!grafo[grafo[i].adiacenti[j]].visitato)
                {
                f << i << " " << grafo[i].adiacenti[j] << endl;
                
                esplora.push(grafo[i].adiacenti[j]);
                while(!esplora.empty())
                {
                    int k = esplora.top();
                    esplora.pop();
                    if(!grafo[k].visitato)
                    {
                        grafo[k].visitato = true;
                        for (int w = 0; w < grafo[k].adiacenti.size(); w++)
                        {
                            //cout << "k -- grafo[k], adiacenti[w]: " << k << " -- " << grafo[k].adiacenti[w] << endl;
                            if(!grafo[grafo[k].adiacenti[w]].visitato)
                            {
                                f << k << " " << grafo[k].adiacenti[w] << endl;
                                esplora.push(grafo[k].adiacenti[w]);
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv)
{
    leggiGrafo();
    ordineTopologico();
    return 0;
}

