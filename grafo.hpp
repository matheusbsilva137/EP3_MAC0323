#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <limits.h>
using namespace std;

class Grafo{
    private: 
        bool* mrk;

    public:
        vector<vector<int>> adj;
        int v;
        int a;

        Grafo(int size){
            v = size;
            a = 0;
            adj.resize(size);
        }

        void insereArco(int i, int j){
            adj[i].push_back(j);
            a++;
        }

        void dfs(int vert, bool* mrk){
            //dfs(i) retorna a quantidade de vértices atingíveis por i em sua componente conexa
            if (mrk[vert] == true) return;
            else mrk[vert] = true;

            for(int i = 0; i < (int) adj[vert].size(); i++) dfs(adj[vert][i], mrk);
        }

        int vertices(){
            /* Retorna o número de vértices do grafo*/
            return v;
        }

        int arcos(){
            /* Retorna o número de arcos do grafo*/
            return a;
        }
};