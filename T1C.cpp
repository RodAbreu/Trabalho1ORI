#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

void apresentacao();
void criaArquvo();

int main(int argc, char const *argv[]) {

    int decisao;
    do {
        apresentacao();
        cin >> decisao;
        switch (decisao) {
            case 1:
                criaArquvo();
                break;
            case 2:
                //addRegisto();
                break;
            case 3:
                //buscaRegistro();
                break;
            case 4:
                //removeRegistro();
                break;
            case 5:
                //listaRegistros();
                break;
            case 6:
                //compactaArquivo();
                break;
            default:
                break;
        }
        cout<<endl<<endl<<endl;
    } while(decisao!=0);

    return 0;
}

void apresentacao(){
    cout<<"Bem-vindo ao sistema de gerenciamento de arquivos."<<endl<<endl;
    cout<<"Segue as funcionalidades do sistema:"<<endl;
    cout<<"[1] - Criacao de um arquivo vazio."<<endl;
    cout<<"[2] - Inserção de um novo registro."<<endl;
    cout<<"[3] - Busca por um registro, dada uma chave."<<endl;
    cout<<"[4] - Remoção de um registro, dada uma chave."<<endl;
    cout<<"[5] - Listagem dos registros."<<endl;
    cout<<"[6] - Compactação do arquivo"<<endl;
    cout<<"[0] - Sair."<<endl;
}

void criaArquvo(){
    string nomeArquivo;
    cout<<"Digite o nome do arquivo que deseja criar: ";
    cin>>nomeArquivo;
    FILE* arq = fopen(nomeArquivo.c_str(), "w");
    fclose(arq);
}
