#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

string nomeArquivo;

void apresentacao();
void criaArquvo();
void insereRegistro();
int quantBytes();

typedef struct aluno{
    char nome[38];
    char idade[4];
    char ra[7];
} Aluno;

void flush_in(){ 
    int ch;
    while( (ch = fgetc(stdin)) != EOF && ch != '\n' ){} 
}

int main(int argc, char const *argv[]) {
 
    int decisao = 0;
    do {
        apresentacao();
        fflush(stdin);
        scanf("%d",&decisao);
        flush_in(); 
        switch (decisao) {
            case 1:
                criaArquvo();
                break;
            case 2:
                insereRegistro();
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
void insereRegistro(){
    Aluno aluno1;
    int ra=0,idade=0;
    unsigned char *bloco;
    unsigned char *conjBlocos;
    int quantBlocos = 0;
    int quantReg = 0;
    int i,j,k,c = 0;
    
    FILE* arq = fopen(nomeArquivo.c_str(), "r+t");

    printf("Digite o RA do aluno:");
    fgets( aluno1.ra, 7, stdin );
    flush_in(); 
    printf("Digite o nome do aluno:");
    fgets( aluno1.nome, 38, stdin );
    printf("Digite a idade do aluno:");
    fgets( aluno1.idade, 4, stdin );
    
    quantReg =  (quantBytes() - 1 ) / 48;
    quantBlocos = (quantReg / 10) + 1;
    printf("%d",quantBytes());
    conjBlocos = (unsigned char*) malloc( quantBlocos * 512);
    bloco  = conjBlocos;
    if(quantBytes() > 0){
        for(k = 0; k < quantBlocos; k++){
            for(j = 0; j < 10; j++){
                for(i  = 0; i < 48; i ++){
                    fscanf(arq, "%c",bloco);
                    if(*bloco == ' ' && i == 0){
                        for(c =0; c< 6;c++){
                            *bloco = aluno1.ra[c];
                            bloco++;
                        }
                        for(c =0; c< 37;c++){
                            *bloco = aluno1.nome[c];
                            bloco++;
                        }
                        for(c =0; c< 3;c++){
                            *bloco = aluno1.idade[c];
                            bloco++;
                        }
                        i = 48;
                    }
                    bloco++;
                }
            }
            bloco = conjBlocos + 512 * (k+1);
        }
    }else{
        bool enter = false;
        printf("Foi");
        for(c =0; c< 6;c++){
            if(aluno1.ra[c] != '\n')
                *bloco = aluno1.ra[c];
            else
                *bloco = ' ';
            printf("%c",*bloco);
            bloco++;
        }
        
        for(c =0; c< 37;c++){
            if(aluno1.nome[c] == '\n' || enter){
                *bloco = ' ';
                enter = true;
            }else
                *bloco = aluno1.nome[c];
            
            printf("%c",*bloco);
            bloco++;
        }
        for(c =0; c< 3;c++){
            if(aluno1.idade[c] != '\n')
                *bloco = aluno1.idade[c];
            else
                *bloco = ' ';
            printf("%c",*bloco);
            bloco++;
        }
        bloco = bloco - 46;
    }
    printf("\n\nBlocos:%d\n\n",quantBlocos);
    fseek ( arq , 0 , SEEK_SET );
    for(k = 0; k < quantBlocos; k++){
        for(j = 0; j < 10; j++){
            for(i  = 0; i < 48; i ++){
                fprintf(arq,"%c",*bloco);
                bloco++;
            }
        }
        bloco = conjBlocos + 512 * (k+1);
    }
    fclose(arq);
}

void criaArquvo(){
    
    cout << "Digite o nome do arquivo que deseja criar: ";
    cin >> nomeArquivo;    
    flush_in(); 
    nomeArquivo = nomeArquivo + ".txt";
    FILE* arq = fopen(nomeArquivo.c_str(), "wt");
    fclose(arq);
}
int quantBytes(){

    FILE *arquivo; 
    int tamanho =0; 
    arquivo = fopen(nomeArquivo.c_str(), "rt");

    if (arquivo != NULL) {
        fseek(arquivo, 0, SEEK_END);
        tamanho = ftell(arquivo);
    } else {
        printf("Arquivo inexistente");
    }
    fclose(arquivo);
    return tamanho;


}
