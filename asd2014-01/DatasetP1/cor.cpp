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

int OPTMID;
int OPTOPT;
int N,M,NL;

#define MUL 5
#define THIRDLAW MUL*0.4
#define SUBOPT MUL*0.05
#define MAX MUL*1.0

void ex(const char *msg, float res)
{
  if(msg) {
    fprintf(stderr, "%s ", msg);
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

int main(int argc, char *argv[])
{
  if(argc < 4)
    {
      cerr << "Usage: " << argv[0] << " <input> <helper> <test output>" << endl;
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

  (*fin)>>N>>M;
  vector<pair<int,int> > graph(M);
  vector<int> id(N);
  graph.resize(M);
  for(int i=0;i<M;i++){
    (*fin)>>graph[i].first>>graph[i].second;
  }
  (*fcor)>>OPTMID>>OPTOPT;
  for(int i=0;i<N;i++)
    (*fcor)>>id[i];  
  vector<int> cons;
  vector<pair<int,int> > tree;
  NL=safe_read(1,N);
  cons.resize(NL);
  set<int> done;
  for(int i=0;i<NL;i++){
    cons[i] = safe_read(0,N-1);
    done.insert(cons[i]);
  }
  if(((int)done.size())!=NL){
    ex("Consiglieri ripetuti", 0.0f);
  }
  tree.resize(N-NL);
  for(int i=0;i<N-NL;i++){
    tree[i].first = safe_read(0,N-1);
    tree[i].second = safe_read(0,N-1);
    done.insert(tree[i].second);
  }
  if(((int)done.size())!=N)
    ex("Alcuni generali sono scomparsi",0.0f);

  sort(tree.begin(),tree.end());
  sort(graph.begin(),graph.end());
  vector<pair<int,int>> result(tree.size());
  auto it = set_intersection(graph.begin(),graph.end(),tree.begin(),tree.end(),result.begin());

  if ( (it-result.begin()) != ((int)tree.size()) ){
    ex("Violata la prima legge",0.0f);
  }
  
  for(auto t:tree){
    if(id[t.first]==id[t.second])
      ex("Violata la seconda legge",0.0f);
  }
  
  vector<vector<int>>vic(N);
  vector<bool> vis(N,false);
  for(auto t:tree){
    vic[t.first].push_back(t.second);
  }
  vector<int> seq=cons;
  for(int l:seq)
    vis[l]=true;
  for(unsigned int i=0;i<seq.size();i++){
    for(int v:vic[seq[i]]){
      if(vis[v])
	ex("Ciclo nell'output",0.0f);
      vis[v]=true;
      seq.push_back(v);
    }
  }

  bool third = true;
  for(int i=0;i<N;i++){
    set<int> ids;
    for(int v:vic[i])
      ids.insert(id[v]);
    if(ids.size()!=vic[i].size())
      third=false;       
  }

  float score;
  if (third){
    if(OPTOPT==NL)
      ex("Tre leggi, Ottimo!",MAX);    
    score = MAX;
    score = score - (NL-OPTOPT)*SUBOPT;
    if(score<=0){
      ex("Tre leggi, lontano dall'ottimo",0.0f);
    }
    ex("Tre leggi, parziale",score);
  }else{
    if(OPTMID==NL)
      ex("Due leggi, ottimo",MAX-THIRDLAW);
    score = MAX-THIRDLAW;
    score = score - (NL-OPTMID)*SUBOPT;
    if(score<=0){
      ex("Due leggi, lontano dall'ottimo",0);
    }
    ex("Due leggi, parziale",score);
  }
  return 0;
}
