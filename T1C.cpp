#include <iostream>
#include <string>

using namespace std;

void apresentacao();

int main(int argc, char const *argv[]) {

    int decisao;
    do {
        apresentacao();
        cin >> decisao;
        switch (decisao) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
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
