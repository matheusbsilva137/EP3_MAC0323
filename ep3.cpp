/*
 * MAC0323 Algoritmos e Estruturas de Dados II
 * 
 * Reconhecimento de Expressões Regulares
 * Nome: Matheus Barbosa Silva - NUSP: 11221741
 */
#include<stdlib.h>
#include <iostream>
#include "regex.hpp"

using namespace std;
//inserir modo arquivo
int main(){
    string exp, str;
    regex* expReg;
    int op = 1;
    cout << "**** RECONHECIMENTO DE EXPRESSÕES REGULARES ****" << endl;
    cout << "Expressão Regular: ";
    getline(cin, exp);

    expReg = new regex(exp);
    cout << "Expressão a ser avaliada: ";
    getline(cin, str);

    if (expReg->reconhece(str)) cout << " - Resultado: True" << endl;
    else cout << " - Resultado: False" << endl;
    cout << endl;

    while (op != 0){
        cout << "Digite uma das opções a seguir: " << endl;
        cout << " 0 - SAIR;" << endl;
        cout << " 1 - Avaliar outra expressão usando a mesma regra;" << endl;
        cout << " 2 - Inserir outra expressão Regular." << endl;
        
        cout << "Opção: ";
        cin >> op;
        
        if (op == 1){
            cout << "Expressão a ser avaliada: ";
            cin.ignore();
            getline(cin, str);

            if (expReg->reconhece(str)) cout << " - Resultado: True\n";
            else cout << " - Resultado: False\n";
            cout << "\n";
        }else if (op == 2){
            cout << "Expressão Regular: ";
            cin.ignore();
            getline(cin, exp);
            delete expReg;

            expReg = new regex(exp);
            cout << "Expressão a ser avaliada: ";
            getline(cin, str);

            if (expReg->reconhece(str)) cout << " - Resultado: True" << endl;
            else cout << " - Resultado: False\n";
            cout << "\n";
        }
    }

    delete expReg;
    return 0;
}