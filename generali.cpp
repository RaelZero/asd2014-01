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
    bool reached = false; // Verifica se è già raggiunto nella funzione terzalegge
    int cfc = 0;           // A quale CFC appartiene
};


int n = 0;                  // numero di nodi
int m = 0;                  // numero di archi
vector<node> grafo;         // grafo di input
vector<node> trasposto;     // Grafo Trasposto
stack<int> consiglieri;     // Vettore di consiglieri
int numeroConsiglieri = 0;  // Numero di consiglieri
ofstream of("output.txt");
stack<int> s;
vector<bool> v;
vector<int> ordine;

// Firme dei metodi
void leggiGrafo();
void cfc();
void dfsStack(int nodo);
void cc();
void ccdfs(int componente, int i);
void ordineTopologico(int nodo);
void tagliaCicli();
void terzaLegge();


int main(int argc, char** argv)
{
    leggiGrafo();
    
    cfc();
    tagliaCicli();
    terzaLegge();
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
    
    of << numeroConsiglieri << endl;
    
    // Cicla per scrivere sul file di output tutti i consiglieri
    for (int g = 0; g < outputCons.size(); g++)
    {   
        
        of << outputCons[g] << " ";
    }
    of << endl;
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

    // Crea e inizializza l'array di appoggio per la visita delle CFC
    v.resize(n);
    for(int i = 0; i < v.size(); i++)
        v[i] = false;
    
    for(int i = 0; i < grafo.size(); i++)
    {

      if (v[i] == false)
        dfsStack(i);
    }
    
    ordine.resize(n);

    for(int i = 0; i < ordine.size(); i++)
    {
        ordine[i] = s.top();
        s.pop();
    }
    
    cc();
}

// Funzione ausiliaria per caricare la pila di cfc()
void dfsStack(int nodo)
{
  v[nodo]=true;
    for (int i = 0; i < ((grafo[nodo]).adiacenti).size(); i++)
    {
        if(!v[((grafo[nodo]).adiacenti[i])])
        {
            dfsStack(grafo[nodo].adiacenti[i]);
        }
    }
    
    s.push(nodo);
}

// Funzione per le componenti connesse
void cc()
{
    int numComponente = 0;

    for (int i = 0; i < ordine.size(); i++)
    {
        if (trasposto[ordine[i]].cfc == 0)
        {
            numComponente++;
            ccdfs(numComponente, ordine[i]);
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
                
                of << nodo << " " << grafo[nodo].adiacenti[i] << endl;
                ordineTopologico(grafo[nodo].adiacenti[i]);
            }
        }
    }
}

// Taglia gli archi interni alla stessa CFC
void tagliaCicli()
{
    // Cicla su tutti i nodi
    for (int i = 0; i < grafo.size(); i++)
    {
        // Cicla sugli adiacenti al nodo i
        for (int j = 0; j < grafo[i].adiacenti.size(); j++)
        {
	  
            // Se il nodo i e il vicino j sono nella stessa CFC
            if (grafo[i].cfc == grafo[grafo[i].adiacenti[j]].cfc)
	        {
                // Riduci il numero di archi entranti
		        grafo[grafo[i].adiacenti[j]].archiEntranti--;
                
                // Sposta l'ultimo elemento del vettore dei vicini al posto del vicino j
                grafo[i].adiacenti[j] = grafo[i].adiacenti[grafo[i].adiacenti.size()-1];
                // Elimina l'ultimo elemento
                grafo[i].adiacenti.pop_back();
                
                // Correzione rispetto al fatto che è stato eliminato un elemento.
                j--;
            }
        }
    }
}

void terzaLegge()
{//scelgo a caso tanto vale partire dall'inizio
    for(int i=0;i<grafo.size();i++)
    {
        //parto dal primo nodo valido
        for(int j=0;j<grafo[i].adiacenti.size();j++)
        {
            
           if(!grafo[grafo[i].adiacenti[j]].reached)
            {
                int currentCFC=grafo[grafo[i].adiacenti[j]].cfc; // la cfc corrente
            
                bool perfectPick = false; //se il pick è su un nodo con 1 archi entranti PROFIT
            
                if(grafo[grafo[i].adiacenti[j]].archiEntranti==1)
                        perfectPick = true;     //controllo se il primo è perfetto
            
            
            
                 for(int k=j+1;k<grafo[i].adiacenti.size();k++)//controllo gli elementi successivi
                        {
                        if(grafo[grafo[i].adiacenti[k]].cfc==currentCFC) //i nodi appartengono alla stessa cfc TERZA LEGGE VIOLATA
                            {            
                                if(perfectPick) // se ho il perfetto rimuovo
                                {
                                grafo[grafo[i].adiacenti[k]].archiEntranti--;
                                grafo[i].adiacenti[k] = grafo[i].adiacenti[grafo[i].adiacenti.size()-1];
                                grafo[i].adiacenti.pop_back(); 
                                k--;
                                }
                                else if(grafo[grafo[i].adiacenti[k]].archiEntranti==1) //nuovo pick perfetto elimino il vecchio;
                                {
                                grafo[grafo[i].adiacenti[j]].archiEntranti--;
                                grafo[i].adiacenti[j] = grafo[i].adiacenti[grafo[i].adiacenti[k]];
                                grafo[i].adiacenti[k] = grafo[i].adiacenti[grafo[i].adiacenti.size()-1];
                                grafo[i].adiacenti.pop_back();
                                perfectPick = true;
                                k--;
                           
                                }
                                else // arco a caso lo rimuovo
                                {
                                grafo[grafo[i].adiacenti[k]].archiEntranti--;
                                grafo[i].adiacenti[k] = grafo[i].adiacenti[grafo[i].adiacenti.size()-1];
                                grafo[i].adiacenti.pop_back();
                                k--;
                                }
                             }
                    
                        }//endfor k
                
           // ho tenuto un nodo di una cfc lo segno come raggiunto per evitare di pickarlo di nuovo
            grafo[grafo[i].adiacenti[j]].reached = true;
            
                }// endif reached      
                else //rimuovo l'arco già raggiunto
                {
                grafo[grafo[i].adiacenti[j]].archiEntranti--;
                grafo[i].adiacenti[j]=grafo[i].adiacenti[grafo[i].adiacenti.size()-1];
                grafo[i].adiacenti.pop_back();
                j--; // riporto l'indice alla posizione precedente per controllare quello appena inserito
                }
       }//endfor j
        
    }//endfor i
    
    
}

