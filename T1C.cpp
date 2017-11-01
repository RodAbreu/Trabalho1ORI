#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

string nomeArquivo;

void apresentacao();                                                                     //Função para mostrar as opções
void criaArquvo();                                                                       //Função para criar um arquivo
void insereRegistro();                                                                   //Função para inserir um registro
void buscaRegistro();                                                                    //Função para buscar um registro
void removeRegistro();                                                                   //Função para remover um registro 
void listaRegistros();                                                                   //Função para listar os registros
void alterarRegistro();                                                                  //Função para alterar um registro
void exibirString(char*,int);                                                            //Função uma cadeia de char
int quantBytes();                                                                        //Função que retorna a quantidade de bytes do arquivo
void compactaArquivo();                                                                  //Função que compacta o arquivo
void setChar(char unsigned *ori,char valor,int tamanho);                                 //Função que insere um valor em uma cadeia de char
void copiaParaUnsigned(char *ori,char unsigned *dest, int tamanho);                      //Função que passa uma cadeia char para char unsigned  
void copiaParaChar(char unsigned *ori,char *dest, int tamanho);                          //Função que passa uma cadeia unsigned char para char 
void copiaUnsignedParaUnsigned(char unsigned *ori,char unsigned *dest, int tamanho);     //Função que passa uma cadeia unsigned char para outra
bool comparaCadeiaChar(char *p,char *d,int tamanho);                                     //Função que compara duas cadeias char

typedef struct aluno{                                                                    //Struct de aluno total de 36 bytes(ignorando o char \0)
    char nome[38];
    char idade[4];
    char ra[7];
} Aluno;

void flush_in(){                                                                         //Função para limpar o buffer
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
    cout<<"[2] - Insercao de um novo registro."<<endl;
    cout<<"[3] - Busca por um registro, dada uma chave."<<endl;
    cout<<"[4] - Remocao de um registro, dada uma chave."<<endl;
    cout<<"[5] - Listagem dos registros."<<endl;
    cout<<"[6] - Alteracao de um registro, dada uma chave."<<endl;
    cout<<"[7] - Compactacao do arquivo"<<endl;
    cout<<"[0] - Sair."<<endl;
}
void insereRegistro(){
    Aluno aluno1;                                                                         //Aluno que será lido
    Aluno alunoAtual;                                                                     //Aluno que será usado como auxiliar
    unsigned char *bloco;                                                                 //Bloco que será alocado na memória
    unsigned char *ponteiroBloco;                                                         //Ponteiro que será utilizado para gerenciar a memoria 
    int quantReg = 0;                                                                     //Variavel que armazena a quantidade de reg por bloco
    int quantBloco =0;                                                                    //Variavel que armazena a quantidade de blocos
    int j = 0,i=0;                                                                        //Contadores
    bool adicionado = false;                                                              //Armazena se o aluno foi armazenado
    
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");                                         //variavel para o arquivo
    
    printf("Digite o RA do aluno:");
    fgets( aluno1.ra, 7, stdin );
    flush_in(); 
    printf("Digite o nome do aluno:");
    fgets( aluno1.nome, 38, stdin );
    printf("Digite a idade do aluno:");
    fgets( aluno1.idade, 4, stdin );
    
    
    quantBloco = quantBytes() / 512 ;                                                    //Verifica quantos blocos tem no arquivo
    fseek(arq, 0, SEEK_SET);
    bloco = (unsigned char*) malloc(512);                                                //Aloca um bloco na memoria
    ponteiroBloco = bloco;                                                               //Posiciona o ponteiro par o inicio do bloco
    
    if(quantBytes() > 0 ){                                                               //Caso tenha já pelo menos um bloco
        for(j=0;j<quantBloco && !adicionado ; j++){                                      
            ponteiroBloco = bloco;
            fscanf(arq, "%512c",ponteiroBloco);                                          //Le um bloco do arquivo
            quantReg = 0;
            quantReg = (*ponteiroBloco - '0') * 10;                                      //Calculo para pegar a parte decimal do cabecalho
            ponteiroBloco++;
            quantReg = quantReg + *ponteiroBloco - '0';                                  //Calculo para pegar a parte da unidade do cabecalho
            ponteiroBloco= ponteiroBloco + 3;
            for(i=0;i< quantReg &&  quantReg != 11; i++){
                copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                
                if(alunoAtual.ra[0] == ' ' && !adicionado){                              //Condicao em que ele encontra um registro removido
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
            if(quantReg < 11 && !adicionado){                                          //Condição de inserir em um bloco que não esta cheio
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
    if(!adicionado){                                                                     //Condição em que se cria um novo bloco
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
    Aluno alunoAtual;                                                                     //Aluno que será usado como auxiliar
    unsigned char *bloco;                                                                 //Bloco que será alocado na memória
    unsigned char *ponteiroBloco;                                                         //Ponteiro que será utilizado para gerenciar a memoria 
    int quantReg = 0;                                                                     //variavel que armazena a quantidade de reg por bloco
    int quantBloco =0;                                                                    //Variavel que armazena a quantidade de blocos
    int j = 0,i=0;                                                                        //Contadores
    char ra[7];                                                                           //Variavel que irá pegar o ra
    bool achou = false;                                                                   //Variavel que que armazena se achou o aluno
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    
    
    printf("Digite o RA do aluno:");
    fgets(ra, 7, stdin );

    flush_in(); 
    //verifica quantidade de registros que tem no arquivo
    quantReg =quantBytes() / 46 ;
    quantBloco = quantBytes() / 512 ;
    fseek(arq, 0, SEEK_SET);
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
                do{                                                                      //Do-while para pular os blocos invalidos
                    copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                    if(alunoAtual.ra[0] == ' ')
                        ponteiroBloco = ponteiroBloco + 46;
                }while(alunoAtual.ra[0] == ' ');
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
        if(achou){                                                                       //Caso em que o aluno é encontrado
            printf("\n---- Aluno encontrado ----\nRA do aluno:");
            exibirString(alunoAtual.ra,6);
            printf("\nNome do aluno:");
            exibirString(alunoAtual.nome,37);
            printf("\nIdade do aluno:");
            exibirString(alunoAtual.idade,3);
            printf("Digite uma tecla para continuar.");
            getchar();
        }else{                                                                          //Caso em que o aluno nao é encontrado
            printf("\n---- Aluno nao encontrado ----\nDigite uma tecla para continuar.");
            getchar();
        }
    }else{
        printf("\nO arquivo nao contem nenhum registro\nDigite uma tecla para continuar.");
        getchar();
    }
        
    free(bloco);
    fclose(arq);
    
}
void removeRegistro(){
    Aluno alunoAtual;                                                                     //Aluno que será usado como auxiliar
    unsigned char *bloco;                                                                 //Bloco que será alocado na memória
    unsigned char *ponteiroBloco;                                                         //Ponteiro que será utilizado para gerenciar a memoria 
    int quantReg = 0;                                                                     //Variavel que armazena a quantidade de reg por bloco
    int quantBloco =0;                                                                    //Variavel que armazena a quantidade de blocos
    int j = 0,i=0;                                                                        //Contadores
    char ra[7];                                                                           //Variavel par apegar o ra
    bool achou = false;                                                                   //Verifica se o aluno foi achado
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");

    printf("Digite o RA do aluno:");
    fgets(ra, 7, stdin );
    flush_in(); 
    quantReg =quantBytes() / 46 ;
    
    fseek(arq, 0, SEEK_SET);
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
                if(comparaCadeiaChar(alunoAtual.ra,ra,6)){                                        //Caso em que acha o aluno que será excluido                              
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
            printf("Digite uma tecla para continuar.");
            getchar();
        }else{
            printf("\n---- Aluno nao encontrado ----\n");
            printf("Digite uma tecla para continuar.");
            getchar();
        }
    }else{
        printf("\nO arquivo nao contem nenhum registro\n");
        printf("Digite uma tecla para continuar.");
        getchar();
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
        printf("Digite uma tecla para continuar.");
        getchar();
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
            printf("Digite uma tecla para continuar.");
            getchar();
        }
    }else{
        printf("\nO arquivo nao contem nenhum registro\n");
        printf("Digite uma tecla para continuar.");
        getchar();
    }
        
    free(bloco);
    fclose(arq);
    
}
void compactaArquivo(){

    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    FILE* arqTemp = fopen("temp.txt", "w+");
    
    Aluno alunoAtual;                                                                     //Aluno que será usado como auxiliar
    unsigned char *bloco;                                                                 //Bloco que será alocado na memória
    unsigned char *ponteiroBloco;                                                         //Ponteiro que será utilizado para gerenciar a memoria 
    int quantReg = 0;                                                                     //Variavel que armazena a quantidade de reg por bloco
    int quantBloco =0;                                                                    //Variavel que armazena a quantidade de blocos
    int j = 0,i=0;                                                                        //Contadores
    unsigned char *blocoComp;                                                             //Bloco para o arquivo comprimido
    unsigned char *ponteiroBlocoComp;                                                     //Ponteiro para o bloco comprimido
    int quantRegComp = 0;                                                                 //Quantidade de registro do bloco do arquivo comprimido

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
                if(alunoAtual.ra[0] != ' '){                                            //Quando encontra um registro valido armazena no bloco
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
        printf("Digite uma tecla para continuar.");
        getchar();
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
        printf("Digite uma tecla para continuar.");
        getchar();
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
