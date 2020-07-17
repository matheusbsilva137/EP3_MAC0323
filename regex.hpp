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
                        vector<int> opcoes;
                        opcoes.push_back(topo.second);
                        G->insereArco(topo.second, i); //arco para o próximo elem. após o '|' empilhado
                        while (p.top().first == '|'){
                            opcoes.push_back(p.top().second);
                            G->insereArco(p.top().second, i);
                            p.pop();
                        }
                        
                        ant = p.top().second; //elemento '('
                        p.pop();
                        for (int i = 0; i < (int)opcoes.size(); i++)
                            G->insereArco(ant, opcoes[i] + 1); //arco vindo do inicio da alternativa para a segunda alternativa
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
            expReg = converterRegex(exp);

            G = new Grafo(expReg.length()+1);
            constroiGrafo(expReg);
        }

        ~regex(){
            delete G;
        }

        //recebe uma string com estruturas adicionais (+, [] e -) e transforma-os na estrutura padrão
        //linear na quantidade de caracteres da expressão regular original
        string converterRegex(string regex){
            string convRegex = "";
            vector<bool> chars(95, false);
            int n = regex.length();
            bool inSet = false, complement = false;
            int setIni = -1;
            stack<string> p;

            for (int i = 0; i < n; i++){
                if (regex[i] == '(' || (regex[i] == '[' && !inSet)){
                    if (regex[i] == '[') {
                        inSet = true;
                        setIni = i;
                        if (regex[i+1] == '^'){
                            complement = true;
                            i++;
                        }
                    }
                    p.push("(");
                }else if (regex[i] == ')' || (regex[i] == ']' && inSet)){
                    if (regex[i] == ']') {
                        inSet = false;
                        setIni = -1;

                        if (complement){
                            int c = 0;
                            //insere os caracteres não marcados em chars
                            for (int i = 0; i < (int) chars.size(); i++){
                                if (!chars[i] && char(i+32) != '|' && char(i+32) != '(' && char(i+32) != ')' && char(i+32) != '[' && char(i+32) != ']' && char(i+32) != '.'){
                                    if (c == 0) p.top().push_back(char(i + 32));
                                    else{
                                        p.top().push_back('|');
                                        p.top().push_back(char(i + 32));
                                    }

                                    c++;
                                }
                            }

                            complement = false;
                            chars.clear();
                            chars.assign(95, false);
                        }
                    }
                    string aux = p.top() + ')';
                    p.pop();

                    if (i + 1 < n && regex[i+1] == '+'){
                        i++;
                        convRegex = convRegex.append(aux).append(aux).append("*");
                    }else convRegex += aux;
                }else if (regex[i] == '+' && !inSet){
                    convRegex += regex[i-1];
                    convRegex += '*';
                }else if (regex[i] == '-' && inSet){
                    //Concatenação de Intervalos conforme o formato padrão
                    if ( !(i - 2 >= 0 && i - 2 == setIni) && !complement){
                        if (p.empty()) convRegex.push_back('|');
                        else p.top().push_back('|');
                    }
                    
                    if (!complement){
                        if (p.empty()) convRegex.push_back('(');
                        else p.top().push_back('(');
                    }

                    char ini;
                    for (ini = regex[i-1]; ini <= regex[i+1]; ini++){
                        if (complement) chars[ini - ' '] = true;
                        else if (p.empty()){
                            convRegex.push_back(ini);
                            if (ini < regex[i+1]) convRegex = convRegex.append("|");
                        }else {
                            p.top().push_back(ini);
                            if (ini < regex[i+1]) p.top().append("|");
                        }
                    }

                    if (!complement){
                        if (p.empty()) convRegex.push_back(')');
                        else p.top().push_back(')');
                    }
                    
                    i++;
                }else if (p.empty()){
                    if ( !(i + 1 < n && regex[i+1] == '-' && inSet) ){
                        if (!inSet) convRegex.push_back(regex[i]);
                        else{
                            if (setIni != i-1 && !complement) convRegex = convRegex.append("|");
                            if (!complement) convRegex.push_back(regex[i]);
                            else{
                                chars[regex[i] - ' '] = true;
                            }
                        }
                    }
                }else{
                    if ( !(i + 1 < n && regex[i+1] == '-' && inSet) ){
                        if (!inSet) p.top().push_back(regex[i]);
                        else{
                            if (setIni != i-1 && !complement) p.top().append("|");
                            if (!complement) p.top().push_back(regex[i]);
                            else chars[regex[i] - ' '] = true;
                        }
                    }
                }
            }
            cout << "String convertida: " << convRegex << endl;
            return convRegex;
        }

        bool reconhece(string texto){
            bool* atingidos = new bool[G->vertices()];
            for (int i = 0; i < G->vertices(); i++) atingidos[i] = false;

            G->dfs(0, atingidos);
            for (int i = 0; i < (int) texto.length(); i++){
                bool* atual = new bool[G->vertices()];
                for (int j = 0; j < G->vertices(); j++) atual[j] = false;
                for (int j = 0; j < G->vertices(); j++)
                    if (atingidos[j] && (expReg[j] == texto[i] || (expReg[j] == '.' && texto[i] != '\n'))) atual[j+1] = true;
                
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