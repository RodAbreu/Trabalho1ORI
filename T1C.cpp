#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

string nomeArquivo;

void apresentacao();
void criaArquvo();
void insereRegistro();
void buscaRegistro();
int quantBytes();
void copiaParaUnsigned(char *ori,char unsigned *dest, int tamanho);
void copiaParaChar(char unsigned *ori,char *dest, int tamanho);
bool comparaCadeiaChar(char *p,char *d,int tamanho);
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
                buscaRegistro();
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
    Aluno alunoAtual;
    int ra=0,idade=0;
    unsigned char *bloco;
    unsigned char *ponteiroBloco;
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
    
    
    bloco = (unsigned char*) malloc(512);
    memset(bloco,0,512);
    ponteiroBloco = bloco;
    if(quantReg > 0){
        
        
        for(j = 0; j < quantReg && !adicionado; j++){
            int x = quantReg - j;
            if(j % 11 == 0){
                ponteiroBloco= ponteiroBloco - 512;
                if(quantReg - j > 11)
                    fscanf(arq, "%506s",ponteiroBloco);
                else{
                    string tamanho = "%"+ std::to_string(x) + "s";
                    fscanf(arq, tamanho.c_str() ,ponteiroBloco);
                }
            }
            for(i  = 0; i < x; i ++){
                copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                if(alunoAtual.ra[0] == ' ' && !adicionado){
                    copiaParaUnsigned(aluno1.ra,ponteiroBloco,6);
                    ponteiroBloco = ponteiroBloco + 6;
                    copiaParaUnsigned(aluno1.nome,ponteiroBloco,37);
                    ponteiroBloco = ponteiroBloco + 37;
                    copiaParaUnsigned(aluno1.idade,ponteiroBloco,3);
                    ponteiroBloco = ponteiroBloco + 3;
                }
                    ponteiroBloco = ponteiroBloco + 46;
                }
            }
            
        }
    
    
    if(!adicionado){
        copiaParaUnsigned(aluno1.ra,ponteiroBloco,6);
        ponteiroBloco = ponteiroBloco + 6;
        copiaParaUnsigned(aluno1.nome,ponteiroBloco,37);
        ponteiroBloco = ponteiroBloco + 37;
        copiaParaUnsigned(aluno1.idade,ponteiroBloco,3);
        ponteiroBloco = ponteiroBloco + 3;
    }
    ponteiroBloco = bloco;
    if(adicionado)
        fprintf(arq,"%506s",bloco);
    else{
        int sobraRegistros = (quantReg + 1) % 11;
        string tamanho = "%"+ std::to_string(sobraRegistros) + "s";
        fprintf(arq, tamanho.c_str() ,bloco);
    }
    free(bloco);
    fclose(arq);
}
void buscaRegistro(){
    
    Aluno aluno1;
    unsigned char *bloco;
    unsigned char *conjBlocos;
    int quantBlocos = 0;
    int quantReg = 0;
    int i,j,k,c = 0;
    bool achou = false;
    char ra[7];
    FILE* arq = fopen(nomeArquivo.c_str(), "r+t");
    printf("Digite o ra do aluno:");
    fgets(ra, 7, stdin );
    //verifica quantidade de registros que tem no arquivo
    quantReg =quantBytes() / 46;
    
    //verifica a quantidade de blocos necessários para a quantidade de registros que tem no arquivo
    quantBlocos = (quantReg / 11) + 1;
    
    conjBlocos = (unsigned char*) malloc( quantBlocos * 512);
    bloco = conjBlocos;
    k = 0;
    j = 0;
    for(j = 0; j < quantReg; j++){
        if(j % 11 == 0){
            bloco = conjBlocos + 512 * k;
            k++;
        }
        for(i  = 0; i < 46; i ++){
            fscanf(arq, "%c",bloco);
            bloco++;
        }
            
    }
    bloco = conjBlocos;
    for(j = 0; j < quantReg && !achou; j++){
        if(j % 11 == 0){
            bloco = conjBlocos + 512 * k;
            k++;
        }
        copiaParaChar(bloco,aluno1.ra,6);
        printf("\nRA:%c",aluno1.ra);
        bloco = bloco + 6;
        if(comparaCadeiaChar(aluno1.ra,ra,6)){
            copiaParaChar(bloco,aluno1.nome,37);
           
            bloco = bloco + 37;
            copiaParaChar(bloco,aluno1.ra,3);
            bloco = bloco + 3;    
            achou = true;
        }
    }
    if(achou){
        printf("---- Aluno encontrado ----\nRA do aluno:%s",aluno1.ra);
        printf("Nome do aluno:%s",aluno1.nome);
        printf("Idade do aluno:%s",aluno1.idade);
    }else{
        printf("---- Aluno nao encontrado ----");
    }
    
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
void copiaParaUnsigned(char *ori,char unsigned *dest, int tamanho){
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
void copiaParaChar(char unsigned *ori,char *dest, int tamanho){
    int i=0;
    char  *destino;
    char unsigned *origem;
    bool terminou = false;
    origem = ori;
    destino = dest;
    for(i =0; i< tamanho;i++){
            *destino = *origem;
            origem++;
            destino++;
    }
}
bool comparaCadeiaChar(char *p,char *d,int tamanho){
    char *primeiro, *segundo;
    primeiro = p;
    segundo = d;
    int cont=0,i;
    for(i=0; i <tamanho;i++){
        if(primeiro[i] == segundo[i])
            cont++;
         printf("\np:%c s:%c\n",primeiro[i],segundo[i]);
    }
    if(cont == tamanho)
        return true;
    return false;
}
