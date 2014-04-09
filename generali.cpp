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

// Struttura di ogni singolo nodo
struct node
{
    vector<int> adiacenti;
    int archiEntranti = 0;
    bool visitato = false;
    int cfc = 0;           // A quale CFC appartiene
};


int n = 0;                  // numero di nodi
int m = 0;                  // numero di archi
vector<node> grafo;         // grafo di input
vector<node> trasposto;     // Grafo Trasposto
stack<int> consiglieri;     // Vettore di consiglieri
int numeroConsiglieri = 0;  // Numero di consiglieri
ofstream f("output.txt");
    
// Firme dei metodi
void leggiGrafo();
void cfc();
void dfsStack(stack<int> &s, int nodo, vector<bool> &v);
void cc(vector<int> &v);
void ccdfs(int componente, int i);
void ordineTopologico(int nodo);
void tagliaCicli();


int main(int argc, char** argv)
{
    leggiGrafo();
    cfc();
    tagliaCicli();

    // Vettore per l'output dei consiglieri
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
    
    // Cicla sul vettore dei consiglieri, estraendone uno alla volta, e inizia la visita a partire da loro
    while (!consiglieri.empty())
    {
        ordineTopologico(consiglieri.top());
        //cout << "---\n"; //DEBUG
        consiglieri.pop();
    }
    
    return 0;
}

// Importa il grafo
void leggiGrafo()
{
    // Importa il numero di nodi e archi
    ifstream f("input.txt");
    f >> n >> m;
    grafo.resize(n);
    trasposto.resize(n);
    
    //cout << n << " " << m; //DEBUG
    // Cicla sul numero di archi importa il grafo, tracciando il numero di archi entranti per nodo
    for (int i = 0; i < m; i++)
    {
        int from, to;
        f >> from >> to;
        
        // Importa nel grafo
        grafo[from].adiacenti.push_back(to);
        grafo[to].archiEntranti++;
        
        // Importa nel grafo trasposto
        trasposto[to].adiacenti.push_back(from);
        trasposto[from].archiEntranti++;
    }
    
    f.close();
}

// Flagga ogni nodo con il numero della sua componente connessa
void cfc()
{
    stack<int> s;

    // Crea e inizializza l'array di appoggio per la visita delle CFC
    vector<bool> v;
    v.resize(n);
    for(int i = 0; i < v.size(); i++)
        v[i] = false;
    
    for(int i = 0; i < grafo.size(); i++)
    {
        dfsStack(s, i, v);
    }

    vector<int> ordine;
    ordine.resize(n);

    for(int i = 0; i < ordine.size(); i++)
    {
        ordine[i] = s.top();
        s.pop();
    }

    cc(ordine);
}

// Funzione ausiliaria per caricare la pila di cfc()
void dfsStack(stack<int> &s, int nodo, vector<bool> &v)
{
    for (int i = 0; i < ((grafo[nodo]).adiacenti).size(); i++)
    {
        if(!v[((grafo[nodo]).adiacenti[i])])
        {
            dfsStack(s, grafo[nodo].adiacenti[i], v);
        }
    }
    
    s.push(nodo);
}

// Funzione per le componenti connesse
void cc(vector<int> &v)
{
    int numComponente = 0;

    for (int i = 0; i < v.size(); i++)
    {
        if (trasposto[i].cfc == 0)
        {
            numComponente++;
            ccdfs(numComponente, i);
        }
    }
}

// Funzione ausiliaria per la ricerca delle componenti connesse
void ccdfs(int componente, int i)
{
    grafo[i].cfc = componente;
    trasposto[i].cfc = componente;
    
    for (int j = 0; j < (trasposto[i].adiacenti).size(); j++)
    {
        if (trasposto[trasposto[i].adiacenti[j]].cfc == 0)
        {
            ccdfs(componente, trasposto[i].adiacenti[j]);
        }
    }
}


// Soluzione da 30 punti
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

void tagliaCicli()
{
    for (int i = 0; i < grafo.size(); i++)
    {
        for (int j = 0; j < grafo[i].adiacenti.size(); j++)
        {
            if (grafo[i].cfc == grafo[grafo[i].adiacenti[j]].cfc)
            {
                grafo[grafo[i].adiacenti[j]].archiEntranti--;
                grafo[i].adiacenti.erase(j);
            }
        }
    }
}

