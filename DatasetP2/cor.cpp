#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility>
#include <climits>
#include <queue>
#include <set>
#include <algorithm>
#include <stack>
using namespace std;

ifstream *fin;
ifstream *fcor;
ifstream *fout;

int OPT;
int N,M;


void ex(const char *msg, float res)
{
  if(msg) {
    fprintf(stderr, "%s (Score= %.2f)", msg,res * 5.0);
  }
  printf("%f\n", res);
  exit(0);
}

int safe_read(int lowerBound, int upperBound){
  if (lowerBound > upperBound)
    {
      cerr << "safe_read chiamato con parametri errati: " << lowerBound << " " << upperBound << "\n";
      return 1;
    }
  int x;
  *fout >> x;  

  if (fout->fail())
    ex("Output malformato", 0.0f);
  if (x < lowerBound || x > upperBound)
    ex("Output invalido", 0.0f);
  return x;
}


struct node{
  vector<int> vic;
  bool vis;
};

vector<node> graph;
vector<vector<int> > ids; 

void visit(int nodo, int dist, int& it){
  graph[nodo].vis=true;
  ids[nodo][it]=dist;
  for(int v:graph[nodo].vic){
    if(!graph[v].vis)
      visit(v,dist+1,it);
  }
}


int main(int argc, char *argv[])
{
  if(argc < 4)
    {
      cerr << "Usage: " << argv[0] << " <input> <correct output> <test output>" << endl;
      return 1;
    }

  fin = new ifstream(argv[1]);
  fcor = new ifstream(argv[2]);
  fout = new ifstream(argv[3]);

  if(fin->fail())
    {
      cerr << "Impossibile aprire il file di input " << argv[1] << "." << endl;
      return 1;
    }
  if(fcor->fail())
    {
      cerr << "Impossibile aprire il file di output corretto " << argv[2] << "." << endl;
      return 1;
    }
  if(fout->fail())
    ex("Impossibile aprire il file di output generato dal codice sottoposto al problema.", 0.0);

  (*fin)>> N>>M;
  (*fcor)>> OPT;
  int SOL= safe_read(0,N);
  if(SOL!=OPT){
    ex("Soluzione non ottima",0.0f);
  }
  vector<int> choices(SOL);

  
  (*fout)>> choices[0];
  if(fout->fail()){
    ex("Soluzione ottima senza locazioni",0.8f);
  }


  if(choices[0] < 0 || choices[0] > N-1){
    ex("Output invalido",0.0f);
  }

  for(int i=1;i<SOL;i++){
    choices[i] = safe_read(0,N-1);
  }


  graph.resize(N);
  ids.resize(N);
  for(int i=0;i<M;i++){
    int f,t;
    *fin >> f >> t;
    graph[f].vic.push_back(t);
    graph[t].vic.push_back(f);
  }

  for(int i=0;i<N;i++)
    ids[i].resize(SOL);
  int it=0;
  for(int el:choices){
    for(auto& n:graph)
      n.vis=false;
    visit(el,0,it);
    it++;
  }
  /*  for(string s: ids)
      cout<<s<<endl;*/

  sort(begin(ids),end(ids));
  for(int i=1;i<N;i++)
    if(ids[i]==ids[i-1]){
      ex("Numero corretto ma locazioni non corrette",0.8f);
    }
  ex("Soluzione corretta",1.0f);
  return 0;
}
