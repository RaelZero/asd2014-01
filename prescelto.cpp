#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stack>

using namespace std;

// Struttura di ogni singolo nodo
struct node
{
    vector<int> figli;
    int padre;
    int grado = 0;
    bool visitato = false;
};


int n = 0;                  // numero di nodi
int m = 0;                  // numero di archi
vector<node> grafo;         // grafo di input
ofstream of("output.txt");

// Firme dei metodi
void leggiGrafo();

int main(int argc, char** argv)
{
    leggiGrafo();

    vector<int> sol;
    
    for(int i = 0; i < grafo.size(); i++){
        if(grafo[i].grado == 1)
            sol.push_back(i);
    }

    sol.pop_back();

    of << sol.size() << endl;

    for(int i = 0; i < sol.size(); i++){
        of << sol[i] << endl;
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
    
    //cout << n << " " << m; //DEBUG
    // Cicla sul numero di archi importa il grafo, tracciando il numero di archi entranti per nodo
    for (int i = 0; i < m; i++)
    {
        int from, to;
        f >> from >> to;
        
        // Importa nel grafo
        grafo[from].figli.push_back(to);
        grafo[to].padre = from;
        grafo[to].grado++;
        grafo[from].grado++;
    }
    
    f.close();
}
