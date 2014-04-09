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


int n = 0;              // numero di nodi
int m = 0;              // numero di archi
vector<node> grafo;     // grafo di input
stack<int> consiglieri;
stack<int> esplora;
int numeroConsiglieri = 0;
ofstream f("output.txt");
    

// Importa il grafo
void leggiGrafo()
{
    // Importa il numero di nodi e archi
    ifstream f("input.txt");
    f >> n >> m;
    grafo.resize(n);
    
    // Cicla sul numero di archi, importa il grafo
    //cout << n << " " << m; //DEBUG
    for (int i = 0; i < m; i++)
    {
        int from, to;
        f >> from >> to;
        grafo[from].adiacenti.push_back(to);
        grafo[to].archiEntranti++;
    }
    
    f.close();
}

void ordineTopologico(int nodo)
{
    if (!grafo[nodo].visitato)
    {
        grafo[nodo].visitato = true;
        for (int i = 0; i < grafo[nodo].adiacenti.size(); i++)
        {
            //cout << "nodo: " << nodo << " i: " << grafo[nodo].adiacenti[i] << "\n"; //DEBUG
            if(!grafo[grafo[nodo].adiacenti[i]].visitato)
            {
                f << nodo << " " << grafo[nodo].adiacenti[i] << endl;
                ordineTopologico(grafo[nodo].adiacenti[i]);
            }
        }
    }
}

int main(int argc, char** argv)
{
    leggiGrafo();
    
    vector<int> outputCons;
    
    // Cicla su tutti i nodi
    for (int i = 0; i < n; i++)
    {
        // Se ci sono 0 archi entranti, il nodo è un consigliere
        if (grafo[i].archiEntranti == 0)
        {
            consiglieri.push(i);
            outputCons.push_back(i);
            numeroConsiglieri++;
        }
    }
    
    //cout << "numero consiglieri: " << numeroConsiglieri << endl; //DEBUG
    // Scrive sul file di output il numero dei consiglieri
    f << numeroConsiglieri << endl;
    
    // Cicla per scrivere sul file di output tutti i consiglieri
    for (int g = 0; g < outputCons.size(); g++)
    {
        f << outputCons[g] << " ";
    }
    f << endl;
    numeroConsiglieri = consiglieri.size();
    
    while (!consiglieri.empty())
    {
        ordineTopologico(consiglieri.top());
        //cout << "---\n"; //DEBUG
        consiglieri.pop();
    }
    
    return 0;
}

