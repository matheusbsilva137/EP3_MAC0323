#include <stdlib.h>
#include "grafo.hpp"
#include <vector>
#include <stack>

class regex{
    private:
        void constroiGrafo(string exp){
            stack<pair<char, int>> p;
            int m = exp.length();
            for (int i = 0; i < m; i++){
                int ant = i;

                if (exp[i] == '(' || exp[i] == '|') p.push(make_pair(exp[i], i));
                else if(exp[i] == ')'){
                    pair<char, int> topo = p.top();
                    p.pop();
                    if (topo.first == '|'){
                        ant = p.top().second;
                        p.pop();
                        G->insereArco(topo.second, i); //arco para o próximo elem. após o '|' empilhado
                        G->insereArco(ant, topo.second + 1); //arco vindo do inicio da alternativa para a segunda alternativa
                    }else
                        ant = topo.second; //o operador era um '('
                }

                if (i < m - 1 && exp[i+1] == '*'){
                    G->insereArco(ant, i+1);
                    G->insereArco(i+1, ant);
                }

                if (exp[i] == '*' || exp[i] == '(' || exp[i] == ')')
                    G->insereArco(i, i+1);
            }
        }
    public:
        string expReg;
        Grafo* G;

        regex(string exp){
            expReg = exp;

            G = new Grafo(exp.length()+1);
            constroiGrafo(exp);
        }

        ~regex(){
            delete G;
        }

        bool reconhece(string texto){
            bool* atingidos = new bool[G->vertices()];
            for (int i = 0; i < G->vertices(); i++) atingidos[i] = false;

            G->dfs(0, atingidos);
            for (int i = 0; i < (int) texto.length(); i++){
                bool* atual = new bool[G->vertices()];
                for (int j = 0; j < G->vertices(); j++) atual[j] = false;
                for (int j = 0; j < G->vertices(); j++)
                    if (atingidos[j] && expReg[j] == texto[i]) atual[j+1] = true;
                
                bool* mark = new bool[G->vertices()];
                for (int j = 0; j < G->vertices(); j++) atingidos[j] = false;
                for (int j = 0; j < G->vertices(); j++){
                    if (atual[j]){
                        for (int k = 0; k < G->vertices(); k++) mark[k] = false;
                        G->dfs(j, mark);
                        for (int k = 0; k < G->vertices(); k++)
                            if (mark[k]) atingidos[k] = true;
                    }
                }
                /*for (int j = 0; j < G->vertices(); j++) cout << atingidos[j] << " ";
                cout << endl;*/

                delete[] atual;
                delete[] mark;
            }
            bool ans = atingidos[G->vertices() - 1];
            delete[] atingidos;

            return (ans);
        }
};