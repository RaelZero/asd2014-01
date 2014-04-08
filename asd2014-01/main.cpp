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
    

// Importa il grafo
void leggiGrafo()
{
    // Importa il numero di nodi e archi
    ifstream f("input.txt");
    f >> n >> m;
    grafo.resize(n);
    
    // Cicla sul numero di archi, importa il grafo
    for (int i = 0; i < m; i++)
    {
        int from, to;
        f >> from >> to;
        grafo[from].adiacenti.push_back(to);
        grafo[to].archiEntranti++;
    }
    
    f.close();
}

// 
void ordineTopologico()
{
    // Inizializza il file di output e il vettore dei consiglieri 
    ofstream f("output.txt");
    vector<int> outputCons;
    
    // Cicla su tutti i nodi
    for (int i = 0; i < n; i++)
    {
        // Se ci sono 0 archi entranti, il nodo è un consigliere
        if (grafo[i].archiEntranti == 0)
        {
            consiglieri.push(i);
            outputCons.push_back(i);
        }
    }
    
    numeroConsiglieri = consiglieri.size();
   
    // Pila dei nodi da esplorare, come indici 
    stack<int> esplora;
    
    //cout << "numero consiglieri: " << numeroConsiglieri << endl; //DEBUG
    // Scrive sul file di output il numero dei consiglieri
    f << numeroConsiglieri << endl;
    
    // Cicla per scrivere sul file di output tutti i consiglieri
    for (int g = 0; g < outputCons.size(); g++)
    {
        f << outputCons[g] << " ";
    }
    f << endl;
   
    // Cicla sui consiglieri 
    while (!consiglieri.empty())
    {
        // Setta i all'indice del consigliere
        int i = consiglieri.top();
        
        // Estrae il consigliere i-esimo e lo flagga come visitato
        //cout << "consigliere: " << i << endl; //DEBUG
        consiglieri.pop();
        grafo[i].visitato = true;
       
        // Cicla su tutti gli adiacenti a quel consigliere 
        for (int j = 0; j < grafo[i].adiacenti.size(); j++)
        {
            //cout << "i -- grafo[i], adiacenti[j]: " << i << " -- " << grafo[i].adiacenti[j] << endl; //DEBUG

            // Se il j-esimo vicino non è ancora stato visitato, lo mette in pila
            if (!grafo[j].visitato)
            {
                // Scrive la riga del tipo "il generale i-esimo comanda il vicino j-esimo"
                f << i << " " << grafo[i].adiacenti[j] << endl;

                // Impila il vicino j-esimo per l'esplorazione
                esplora.push(grafo[i].adiacenti[j]);
    
                // Cicla finché la pila di esplorazione non è vuota
                while(!esplora.empty())
                {
                    // Estrae il primo elemento dalla pila
                    int k = esplora.top();
                    esplora.pop();
                
                    // Se il nodo estratto non è già stato visitato
                    if(!grafo[k].visitato)
                    {
                        // Flagga come visitato
                        grafo[k].visitato = true;

                        // Cicla sugli adiacenti a quel nodo
                        for (int w = 0; w < grafo[k].adiacenti.size(); w++)
                        {
                            // cout << "k -- grafo[k], adiacenti[w]: " << k << " -- " << grafo[k].adiacenti[w] << endl; //DEBUG
                            // Se l'adiacente non è già stato visitato
                            if(!grafo[w].visitato)
                            {
                                // Impila per l'esplorazione e scrive in output "Il vicino k comanda w"
                                esplora.push(grafo[k].adiacenti[w]);
                                f << k << " " << grafo[k].adiacenti[w] << endl;
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

