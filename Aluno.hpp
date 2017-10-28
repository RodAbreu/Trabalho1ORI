#include <iostream>
#include <string>

using namespace std;

class Aluno {
private:
    string nome;
    int idade;
    int ra;

public:
    Aluno (string nome, int idade, int ra){
        this->nome = nome;
        this->idade = idade;
        this->ra = ra;
    }
    void setNome(string nome){this->nome = nome;}
    void setIdade(int idade){this->idade = idade;}
    void setRA(int ra){this->ra = ra;}
    string getNome(){return this->nome;}
    int getIdade(){return this->idade;}
    int getRA(){return this->ra;}

};
