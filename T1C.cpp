#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

string nomeArquivo;

void apresentacao();
void criaArquvo();
void insereRegistro();
int quantBytes();
void copia(char *origem,char unsigned *destino, int tamanho);

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
    bool adicionado = false;
    
    FILE* arq = fopen(nomeArquivo.c_str(), "r+t");

    printf("Digite o RA do aluno:");
    fgets( aluno1.ra, 7, stdin );
    flush_in(); 
    printf("Digite o nome do aluno:");
    fgets( aluno1.nome, 38, stdin );
    printf("Digite a idade do aluno:");
    fgets( aluno1.idade, 4, stdin );
    
    //verifica quantidade de registros que tem no arquivo
    quantReg =quantBytes() / 46;
    
    //verifica a quantidade de blocos necessários para a quantidade de registros que tem no arquivo
    quantBlocos = (quantReg + 1 / 11) + 1;
    
    printf("Quantidade de registros: %d",quantBytes() );
    conjBlocos = (unsigned char*) malloc( quantBlocos * 512);
    bloco = conjBlocos;
    if(quantReg > 0){
        k = 0;
        j = 0;
        for(j = 0; j < quantReg; j++){
            if(j % 11 == 0){
                bloco = conjBlocos + 512 * k;
                k++;
            }
            for(i  = 0; i < 46; i ++){
                fscanf(arq, "%c",bloco);
                if(*bloco == ' ' && i == 0 && !adicionado){
                    fseek ( arq , 45 , SEEK_CUR );
                    copia(aluno1.ra,bloco,6);
                    bloco = bloco + 6;
                    copia(aluno1.nome,bloco,37);
                    bloco = bloco + 37;
                    copia(aluno1.idade,bloco,3);
                    bloco = bloco + 3;
                    i = 46;
                    adicionado = true;
                }else{
                    bloco++;
                }
            }
            
        }
    }
    
    if(!adicionado){
        bool enter = false;
        printf("Adicionado no fim");
        copia(aluno1.ra,bloco,6);
        bloco = bloco + 6;
        copia(aluno1.nome,bloco,37);
        bloco = bloco + 37;
        copia(aluno1.idade,bloco,3);
        bloco = bloco + 3;
        bloco = bloco - 46;
        
    }
    quantReg++;
    fseek ( arq , 0 , SEEK_SET );
    k = 0;
    j = 0;
    for(j = 0; j < quantReg; j++){
        if(j % 11 == 0){
            bloco = conjBlocos + 512 * k;
            k++;
        }
        for(i  = 0; i < 46; i ++){
            fprintf(arq,"%c",*bloco);
            bloco++;
        }
        
    }
    free(conjBlocos);
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
void copia(char *ori,char unsigned *dest, int tamanho){
    int i=0;
    char  *origem;
    char unsigned *destino;
    bool terminou = false;
    origem = ori;
    destino = dest;
    for(i =0; i< tamanho;i++){
             if(*origem == '\n' || *origem == '\0' || terminou){
                *destino = ' ';
                terminou = true;
             }else{
                *destino = *origem;
             }
             origem++;
             destino++;
    }
}
