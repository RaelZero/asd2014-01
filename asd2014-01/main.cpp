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

using namespace std;


struct node
{
    vector<int> adiacenti;
    int archiEntranti = 0;
    bool visitato;
};


/*
 * 
 */

int n = 0;              // numero di nodi
int m = 0;              // numero di archi
vector<node> grafo;     // grafo di input


vector<node> leggiGrafo(string fileName)
{
    vector<node> grafo;
    node nodo;
    
    ifstream f("/home/giorgio/NetBeansProjects/asd2014-01/asd2014-01/asd2014-01/DatasetP1/input1.txt");
    f >> n >> m;
    graph.resize(n);
    
    for (int i = 0; i < m; i++)
    {
        int from, to;
        f >> from >> to;
        grafo[from].adiacenti.push_back(to);
        grafo[to].archiEntranti++;
    }
    
    f.close();
    return grafo;
}

void topologicOrder()
{
    
}

int main(int argc, char** argv)
{
    grafo = leggiGrafo();
    
    return 0;
}

