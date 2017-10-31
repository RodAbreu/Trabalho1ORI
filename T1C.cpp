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
void removeRegistro();
void listaRegistros();
void alterarRegistro();
void exibirString(char*,int);
int quantBytes();
void compactaArquivo();
void setChar(char unsigned *ori,char valor,int tamanho);
void copiaParaUnsigned(char *ori,char unsigned *dest, int tamanho);
void copiaParaChar(char unsigned *ori,char *dest, int tamanho);
void copiaUnsignedParaUnsigned(char unsigned *ori,char unsigned *dest, int tamanho);
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
                removeRegistro();
                break;
            case 5:
                listaRegistros();
                break;
            case 6:
                alterarRegistro();
                break;
            case 7:
                compactaArquivo();
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
    cout<<"[6] - Alteracao de um registro, dada uma chave."<<endl;
    cout<<"[7] - Compactação do arquivo"<<endl;
    cout<<"[0] - Sair."<<endl;
}
void insereRegistro(){
    Aluno aluno1;
    Aluno alunoAtual;
    unsigned char *bloco;
    unsigned char *ponteiroBloco;
    int quantReg = 0;
    int sobraReg = 0;
    int quantBloco =0;
    int j = 0,i=0;
    bool adicionado = false;
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    
    printf("Digite o RA do aluno:");
    fgets( aluno1.ra, 7, stdin );
    flush_in(); 
    printf("Digite o nome do aluno:");
    fgets( aluno1.nome, 38, stdin );
    printf("Digite a idade do aluno:");
    fgets( aluno1.idade, 4, stdin );
    
    //verifica quantidade de registros que tem no arquivo
    quantBloco = quantBytes() / 512 ;
    fseek(arq, 0, SEEK_SET);
    bloco = (unsigned char*) malloc(512);
    ponteiroBloco = bloco;
    
    if(quantBytes() > 0 ){
        for(j=0;j<quantBloco && !adicionado ; j++){
            ponteiroBloco = bloco;
            fscanf(arq, "%512c",ponteiroBloco);
            quantReg = 0;
            quantReg = (*ponteiroBloco - '0') * 10;
            ponteiroBloco++;
            quantReg = quantReg + *ponteiroBloco - '0';
            ponteiroBloco= ponteiroBloco + 3;
            for(i=0;i< quantReg &&  quantReg != 11; i++){
                copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                if(alunoAtual.ra[0] == ' ' && !adicionado){
                    copiaParaUnsigned(aluno1.ra,ponteiroBloco,6);
                    ponteiroBloco = ponteiroBloco + 6;
                    copiaParaUnsigned(aluno1.nome,ponteiroBloco,37);
                    ponteiroBloco = ponteiroBloco + 37;
                    copiaParaUnsigned(aluno1.idade,ponteiroBloco,3);
                    ponteiroBloco = ponteiroBloco + 3;
                    adicionado = true;
                    ponteiroBloco = bloco;
                    fseek(arq, - 512, SEEK_CUR);
                    *ponteiroBloco =  ((quantReg + 1) / 10) + 48;
                    ponteiroBloco++;
                    *ponteiroBloco = ((quantReg + 1) % 10) + 48;
                    ponteiroBloco = bloco;
                    fprintf(arq,"%512s",ponteiroBloco);
                }else{
                    ponteiroBloco = ponteiroBloco + 46;
                }
            }
            if(quantReg < 11 && !adicionado){
                copiaParaUnsigned(aluno1.ra,ponteiroBloco,6);
                ponteiroBloco = ponteiroBloco + 6;
                copiaParaUnsigned(aluno1.nome,ponteiroBloco,37);
                ponteiroBloco = ponteiroBloco + 37;
                copiaParaUnsigned(aluno1.idade,ponteiroBloco,3);
                ponteiroBloco = ponteiroBloco + 3;
                adicionado = true;
                ponteiroBloco = bloco;
                fseek(arq, - 512, SEEK_CUR);
                *ponteiroBloco =  ((quantReg + 1) / 10) + 48;
                ponteiroBloco++;
                *ponteiroBloco = ((quantReg + 1) % 10) + 48;
                ponteiroBloco = bloco;
                fprintf(arq,"%512s",ponteiroBloco);
            }
        }
    }    
    if(!adicionado){
        ponteiroBloco = bloco;
        memset(ponteiroBloco,'*',512);
        ponteiroBloco = ponteiroBloco;
        *ponteiroBloco='0';
        ponteiroBloco++;
        *ponteiroBloco='1';
        ponteiroBloco++;
        *ponteiroBloco='B';
        ponteiroBloco++;
        *ponteiroBloco='K';
        ponteiroBloco++;
        copiaParaUnsigned(aluno1.ra,ponteiroBloco,6);
        ponteiroBloco = ponteiroBloco + 6;
        copiaParaUnsigned(aluno1.nome,ponteiroBloco,37);
        ponteiroBloco = ponteiroBloco + 37;
        copiaParaUnsigned(aluno1.idade,ponteiroBloco,3);
        ponteiroBloco = ponteiroBloco + 3; 
        ponteiroBloco = bloco;
        fprintf(arq,"%512s",ponteiroBloco);
    }    
    free(bloco);
    fclose(arq);
}    
void buscaRegistro(){
    Aluno alunoAtual;
    unsigned char *bloco;
    unsigned char *ponteiroBloco;
    int quantReg = 0;
    char ra[7];
    int quantBloco=0;
    int sobraReg = 0;
    int j = 0,i=0;
    bool achou = false;
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    
    
    printf("Digite o RA do aluno:");
    fgets(ra, 7, stdin );

    flush_in(); 
    //verifica quantidade de registros que tem no arquivo
    quantReg =quantBytes() / 46 ;
    quantBloco = quantBytes() / 512 ;
    fseek(arq, 0, SEEK_SET);
    sobraReg = quantReg % 11;
    bloco = (unsigned char*) malloc(512);
    ponteiroBloco = bloco;
    
    if(quantBytes() > 0 ){
        for(j=0;j<quantBloco && !achou ; j++){
            ponteiroBloco = bloco;
            fscanf(arq, "%512c",ponteiroBloco);
            quantReg = 0;
            quantReg = (*ponteiroBloco - '0') * 10;
            ponteiroBloco++;
            quantReg = quantReg + *ponteiroBloco - '0';
            ponteiroBloco= ponteiroBloco + 3;
            for(i=0;i< quantReg; i++){
                copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                if(comparaCadeiaChar(alunoAtual.ra,ra,6)){
                    ponteiroBloco = ponteiroBloco + 6;
                    copiaParaChar(ponteiroBloco,alunoAtual.nome,37);
                    ponteiroBloco = ponteiroBloco + 37;
                    copiaParaChar(ponteiroBloco,alunoAtual.idade,3);
                    ponteiroBloco = ponteiroBloco + 3;
                    achou = true;
                }else{
                    ponteiroBloco = ponteiroBloco + 46;
                }
            }
        }
        if(achou){
            printf("\n---- Aluno encontrado ----\nRA do aluno:");
            exibirString(alunoAtual.ra,6);
            printf("\nNome do aluno:");
            exibirString(alunoAtual.nome,37);
            printf("\nIdade do aluno:");
            exibirString(alunoAtual.idade,3);
        }else{
            printf("\n---- Aluno nao encontrado ----\n");
        }
    }else{
        printf("\nO arquivo nao contem nenhum registro\n");
    }
        
    free(bloco);
    fclose(arq);
    
}
void removeRegistro(){
    Aluno alunoAtual;
    unsigned char *bloco;
    unsigned char *ponteiroBloco;
    int quantReg = 0;
    char ra[7];
    int sobraReg = 0;
    int j = 0,i=0;
    bool achou = false;
    int quantBloco =0;
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    int tipoBloco = 0;
    
    printf("Digite o RA do aluno:");
    fgets(ra, 7, stdin );

    flush_in(); 
    //verifica quantidade de registros que tem no arquivo
    quantReg =quantBytes() / 46 ;
    
    fseek(arq, 0, SEEK_SET);
    sobraReg = quantReg % 11;
    bloco = (unsigned char*) malloc(512);
    ponteiroBloco = bloco;
    quantBloco = quantBytes() / 512 ;
    
    if(quantBytes() > 0 ){
        for(j=0;j<quantBloco && !achou ; j++){
            ponteiroBloco = bloco;
            fscanf(arq, "%512c",ponteiroBloco);
            quantReg = 0;
            quantReg = (*ponteiroBloco - '0') * 10;
            ponteiroBloco++;
            quantReg = quantReg + *ponteiroBloco - '0';
            ponteiroBloco= ponteiroBloco + 3;
            for(i=0;i< quantReg; i++){
                do{
                    copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                    if(alunoAtual.ra[0] == ' ')
                        ponteiroBloco = ponteiroBloco + 46;
                }while(alunoAtual.ra[0] == ' ');
                printf("%s",alunoAtual.ra);
                if(comparaCadeiaChar(alunoAtual.ra,ra,6)){
                    memset(ponteiroBloco,' ',46);
                    achou = true;
                    ponteiroBloco = bloco;
                    fseek(arq, - 512, SEEK_CUR);
                    *ponteiroBloco =  ((quantReg - 1) / 10) + 48;
                    ponteiroBloco++;
                    *ponteiroBloco = ((quantReg - 1) % 10) + 48;
                    ponteiroBloco = bloco;
                    fprintf(arq,"%512s",ponteiroBloco);
                }else{
                    ponteiroBloco = ponteiroBloco + 46;
                }
            }
            
        }
        if(achou){
            printf("\n---- Aluno excluido ----\n");
        }else{
            printf("\n---- Aluno nao encontrado ----\n");
        }
    }else{
        printf("\nO arquivo nao contem nenhum registro\n");
    }
        
    free(bloco);
    fclose(arq);
}
void listaRegistros(){
    Aluno alunoAtual;
    unsigned char *bloco;
    unsigned char *ponteiroBloco;
    int quantReg = 0;
    char ra[7];
    int sobraReg = 0;
    int j = 0,i=0;
    bool achou = false;
    int quantBloco =0;
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    

    quantBloco = quantBytes() / 512 ;
    fseek(arq, 0, SEEK_SET);
    sobraReg = quantReg % 11;
    bloco = (unsigned char*) malloc(512);
    ponteiroBloco = bloco;
    
    if(quantBytes() > 0 ){
        for(j=0;j<quantBloco ; j++){
            ponteiroBloco = bloco;
            fscanf(arq, "%512c",ponteiroBloco);
            quantReg = 0;
            quantReg = (*ponteiroBloco - '0') * 10;
            ponteiroBloco++;
            quantReg = quantReg + *ponteiroBloco - '0';
            ponteiroBloco= ponteiroBloco + 3;
            for(i=0;i< quantReg; i++){
                do{
                    copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                    if(alunoAtual.ra[0] == ' ')
                        ponteiroBloco = ponteiroBloco + 46;
                }while(alunoAtual.ra[0] == ' ');
                copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                ponteiroBloco = ponteiroBloco + 6;
                copiaParaChar(ponteiroBloco,alunoAtual.nome,37);
                ponteiroBloco = ponteiroBloco + 37;
                copiaParaChar(ponteiroBloco,alunoAtual.idade,3);
                ponteiroBloco = ponteiroBloco + 3;
                printf("\n---- Aluno ----\nRA do aluno:");
                exibirString(alunoAtual.ra,6);
                printf("\nNome do aluno:");
                exibirString(alunoAtual.nome,37);
                printf("\nIdade do aluno:");
                exibirString(alunoAtual.idade,3);
            }
            
        }
    }else{
        printf("O arquivo nao contem nenhum registro");
    }
        
    free(bloco);
    fclose(arq);
    
}
void alterarRegistro(){
    Aluno alunoAtual;
    Aluno aluno1;
    unsigned char *bloco;
    unsigned char *ponteiroBloco;
    int quantReg = 0;
    char ra[7];
    int quantBloco=0;
    int sobraReg = 0;
    int j = 0,i=0;
    bool achou = false;
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    
    
    printf("Digite o RA do aluno:");
    fgets(ra, 7, stdin );

    flush_in(); 
    //verifica quantidade de registros que tem no arquivo
    quantReg =quantBytes() / 46 ;
    quantBloco = quantBytes() / 512 ;
    fseek(arq, 0, SEEK_SET);
    sobraReg = quantReg % 11;
    bloco = (unsigned char*) malloc(512);
    ponteiroBloco = bloco;
    
    if(quantBytes() > 0 ){
        for(j=0;j<quantBloco && !achou ; j++){
            ponteiroBloco = bloco;
            fscanf(arq, "%512c",ponteiroBloco);
            quantReg = 0;
            quantReg = (*ponteiroBloco - '0') * 10;
            ponteiroBloco++;
            quantReg = quantReg + *ponteiroBloco - '0';
            ponteiroBloco= ponteiroBloco + 3;
            for(i=0;i< quantReg ; i++){
                do{
                    copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                    if(alunoAtual.ra[0] == ' ')
                        ponteiroBloco = ponteiroBloco + 46;
                }while(alunoAtual.ra[0] == ' ');
                if(comparaCadeiaChar(alunoAtual.ra,ra,6)){
                    
                    printf("\n---- Aluno encontrado ----\nDigite o novo ra:");
                    fgets( aluno1.ra, 7, stdin );
                    flush_in(); 
                    printf("Digite o novo nome do aluno:");
                    fgets( aluno1.nome, 38, stdin );
                    printf("Digite a nova idade do aluno:");
                    fgets( aluno1.idade, 4, stdin );
                    copiaParaUnsigned(aluno1.ra,ponteiroBloco,6);
                    ponteiroBloco = ponteiroBloco + 6;
                    copiaParaUnsigned(aluno1.nome,ponteiroBloco,37);
                    ponteiroBloco = ponteiroBloco + 37;
                    copiaParaUnsigned(aluno1.idade,ponteiroBloco,3);
                    ponteiroBloco = ponteiroBloco + 3;
                    achou = true;
                    ponteiroBloco = bloco;
                    fseek(arq, - 512, SEEK_CUR);
                    *ponteiroBloco =  ((quantReg + 1) / 10) + 48;
                    ponteiroBloco++;
                    *ponteiroBloco = ((quantReg + 1) % 10) + 48;
                    ponteiroBloco = bloco;
                    fprintf(arq,"%512s",ponteiroBloco);
                }else{
                    ponteiroBloco = ponteiroBloco + 46;
                }
            }
        }
        if(!achou){
            printf("\n---- Aluno nao encontrado ----\n");
        }
    }else{
        printf("\nO arquivo nao contem nenhum registro\n");
    }
        
    free(bloco);
    fclose(arq);
    
}
void compactaArquivo(){

    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    FILE* arqTemp = fopen("temp.txt", "w+");

    Aluno alunoAtual;
    unsigned char *bloco;
    unsigned char *blocoComp;
    unsigned char *ponteiroBloco;
    unsigned char *ponteiroBlocoComp;
    int quantReg = 0;
    char ra[7];
    int sobraReg = 0;
    int j = 0,i=0;
    int quantRegComp = 0;
    int quantBloco =0;    

    bloco = (unsigned char*) malloc(512);
    blocoComp = (unsigned char*) malloc(512);
    
    quantBloco = quantBytes() / 512 ;
    fseek(arq, 0, SEEK_SET);
    memset(blocoComp,'*',512);
    ponteiroBloco = bloco;
    ponteiroBlocoComp = blocoComp + 4;
    if(quantBytes() > 0 ){
        for(j=0;j<quantBloco ; j++){
            ponteiroBloco = bloco;
            fscanf(arq, "%512c",ponteiroBloco);
            quantReg = 0;
            quantReg = (*ponteiroBloco - '0') * 10;
            ponteiroBloco++;
            quantReg = quantReg + *ponteiroBloco - '0';
            ponteiroBloco= ponteiroBloco + 3;
            for(i=0;i< quantReg  ; i++){
                do{
                    copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                    if(alunoAtual.ra[0] == ' ')
                        ponteiroBloco = ponteiroBloco + 46;
                }while(alunoAtual.ra[0] == ' ');
                if(alunoAtual.ra[0] != ' '){
                    quantRegComp++;
                    copiaUnsignedParaUnsigned(ponteiroBloco,ponteiroBlocoComp,46);
                    ponteiroBloco = ponteiroBloco + 46;
                    if(quantRegComp == 11){
                        ponteiroBlocoComp = blocoComp;
                        *ponteiroBlocoComp='1';
                        ponteiroBlocoComp++;
                        *ponteiroBlocoComp='1';
                        ponteiroBlocoComp++;
                        *ponteiroBlocoComp='B';
                        ponteiroBlocoComp++;
                        *ponteiroBlocoComp='K';
                        ponteiroBlocoComp++;
                        ponteiroBlocoComp = blocoComp;
                        fprintf(arqTemp,"%512s",ponteiroBlocoComp);
                        quantRegComp=0;
                        memset(ponteiroBlocoComp,'*',512);
                        ponteiroBlocoComp = blocoComp + 4;
                    }
                }
            }
            if(j == quantBloco - 1){
                ponteiroBlocoComp = blocoComp;
                *ponteiroBlocoComp =  (quantRegComp / 10) + 48;
                ponteiroBlocoComp++;
                *ponteiroBlocoComp = (quantRegComp % 10) + 48;
                ponteiroBlocoComp++;
                *ponteiroBlocoComp='B';
                ponteiroBlocoComp++;
                *ponteiroBlocoComp='K';
                ponteiroBlocoComp++;
                ponteiroBlocoComp = blocoComp;
                fprintf(arqTemp,"%512s",ponteiroBlocoComp);
                quantRegComp=0;
                memset(ponteiroBlocoComp,'*',512);
                ponteiroBlocoComp = blocoComp + 4;
            }
        }
    }else{
        printf("O arquivo nao contem nenhum registro");
    }
     
    free(bloco);
    fclose(arqTemp);
    fclose(arq);
    remove(nomeArquivo.c_str());
    rename("temp.txt",nomeArquivo.c_str());
}
void criaArquvo(){
    
    cout << "Digite o nome do arquivo que deseja criar: ";
    cin >> nomeArquivo;    
    flush_in(); 
    nomeArquivo = nomeArquivo + ".txt";
    FILE* arq = fopen(nomeArquivo.c_str(), "w");
    fclose(arq);
}
int quantBytes(){

    FILE *arquivo; 
    int tamanho =0; 
    arquivo = fopen(nomeArquivo.c_str(), "r");

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
    origem = ori;
    destino = dest;
    for(i =0; i< tamanho;i++)
            if(origem[i] != '\0'){
                destino[i] = origem[i];
            }else
                i = tamanho;
            
            
}
bool comparaCadeiaChar(char *p,char *d,int tamanho){
    char *primeiro, *segundo;
    primeiro = p;
    segundo = d;
    int cont=0,i;
    for(i=0; i <tamanho;i++){
        if(primeiro[i] == segundo[i])
            cont++;
    }
    if(cont == tamanho)
        return true;
    return false;
}
void exibirString(char* string,int tamanho){
    int cont = 0;
    for(cont = 0; cont < tamanho;cont++)
        printf("%c",string[cont]);
    
}
void setChar(char unsigned *ori,char valor,int tamanho){
    int cont = 0;
    for(cont = 0; cont < tamanho;cont++)
        ori[cont]= valor;
}
void copiaUnsignedParaUnsigned(char unsigned *ori,char unsigned *dest, int tamanho){
    int i=0;
    char unsigned *destino;
    char unsigned *origem;
    origem = ori;
    destino = dest;
    for(i =0; i< tamanho;i++)
        destino[i] = origem[i];
}
