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
void exibirString(char*,int);
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
                removeRegistro();
                break;
            case 5:
                listaRegistros();
                break;
            case 6:
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
    cout<<"[6] - Compactação do arquivo"<<endl;
    cout<<"[0] - Sair."<<endl;
}
void insereRegistro(){
    Aluno aluno1;
    Aluno alunoAtual;
    unsigned char *bloco;
    unsigned char *ponteiroBloco;
    int quantReg = 0;
    int sobraReg = 0;
    int j = 0;
    bool adicionado = false;
    bool adicionadoResto = false;
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    
    
    printf("Digite o RA do aluno:");
    fgets( aluno1.ra, 7, stdin );
    flush_in(); 
    printf("Digite o nome do aluno:");
    fgets( aluno1.nome, 38, stdin );
    printf("Digite a idade do aluno:");
    fgets( aluno1.idade, 4, stdin );
    
    //verifica quantidade de registros que tem no arquivo
    quantReg =quantBytes() / 46 ;
    
    fseek(arq, 0, SEEK_SET);
    printf("%d",quantBytes() );
    sobraReg = quantReg % 11;
    bloco = (unsigned char*) malloc(512);
    ponteiroBloco = bloco;
    if((quantReg - sobraReg) > 0){
        for(j=0;j<(quantReg - sobraReg) && !adicionado ; j++){
            if(j % 11 == 0){
                ponteiroBloco = bloco;
                fscanf(arq, "%506c",ponteiroBloco);
                
            }
            copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
            if(alunoAtual.ra[0] == ' ' && !adicionado){
                copiaParaUnsigned(aluno1.ra,ponteiroBloco,6);
                ponteiroBloco = ponteiroBloco + 6;
                copiaParaUnsigned(aluno1.nome,ponteiroBloco,37);
                ponteiroBloco = ponteiroBloco + 37;
                copiaParaUnsigned(aluno1.idade,ponteiroBloco,3);
                ponteiroBloco = ponteiroBloco + 3;
                adicionado = true;
                fseek(arq, - 506  , SEEK_CUR);
            }else{
                ponteiroBloco = ponteiroBloco + 46;
            }
            
        }
        if(!adicionado){
            ponteiroBloco = bloco;
            memset(bloco,0,512);
        }
    }
    if(sobraReg > 0 && !adicionado){
            ponteiroBloco = bloco;
            string tamanho = "%"+ std::to_string(sobraReg * 46) + "c";
            fscanf(arq, tamanho.c_str() ,ponteiroBloco);
            for(j=0;j<sobraReg; j++){
                copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
                if(alunoAtual.ra[0] == ' ' && !adicionadoResto){
                    copiaParaUnsigned(aluno1.ra,ponteiroBloco,6);
                    ponteiroBloco = ponteiroBloco + 6;
                    copiaParaUnsigned(aluno1.nome,ponteiroBloco,37);
                    ponteiroBloco = ponteiroBloco + 37;
                    copiaParaUnsigned(aluno1.idade,ponteiroBloco,3);
                    ponteiroBloco = ponteiroBloco + 3;
                    adicionadoResto = true;
                }else{
                    ponteiroBloco = ponteiroBloco + 46;
                }
            }
            fseek(arq, - sobraReg * 46  , SEEK_CUR);
    }
    if(!adicionado && !adicionadoResto){
        copiaParaUnsigned(aluno1.ra,ponteiroBloco,6);
        printf("%s",aluno1.ra);
        ponteiroBloco = ponteiroBloco + 6;
        copiaParaUnsigned(aluno1.nome,ponteiroBloco,37);
        ponteiroBloco = ponteiroBloco + 37;
        copiaParaUnsigned(aluno1.idade,ponteiroBloco,3);
        ponteiroBloco = ponteiroBloco + 3;        
    }
    ponteiroBloco = bloco;
    if(adicionado){
         fprintf(arq,"%506s",ponteiroBloco);
    }else{
        if(adicionadoResto){
            string tamanho = "%"+ std::to_string(sobraReg * 46) + "s";
            fprintf(arq, tamanho.c_str() ,ponteiroBloco);
        }else{
            string tamanho = "%"+ std::to_string((sobraReg + 1) * 46) + "s";
            printf("%s",tamanho.c_str());
            fprintf(arq, tamanho.c_str() ,ponteiroBloco);
        }
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
    int sobraReg = 0;
    int j = 0;
    bool achou = false;
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    
    
    printf("Digite o RA do aluno:");
    fgets(ra, 7, stdin );

    flush_in(); 
    //verifica quantidade de registros que tem no arquivo
    quantReg =quantBytes() / 46 ;
    
    fseek(arq, 0, SEEK_SET);
    sobraReg = quantReg % 11;
    bloco = (unsigned char*) malloc(512);
    memset(bloco,0,512);
    ponteiroBloco = bloco;
    
    if(quantReg > 0){
        for(j=0;j<quantReg && !achou ; j++){
            if(j == (quantReg - sobraReg) && sobraReg != 0){
                    ponteiroBloco = bloco;
                    string tamanho = "%"+ std::to_string(sobraReg * 46) + "c";
                    fscanf(arq, tamanho.c_str() ,ponteiroBloco);
            }else{
                if(j % 11 == 0){
                    ponteiroBloco = bloco;
                    fscanf(arq, "%506c",ponteiroBloco);
                }
            }
            copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
            printf("%s",alunoAtual.ra);
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
        if(achou){
            printf("---- Aluno encontrado ----\nRA do aluno:");
            exibirString(alunoAtual.ra,6);
            printf("\nNome do aluno:");
            exibirString(alunoAtual.nome,37);
            printf("\nIdade do aluno:");
            exibirString(alunoAtual.idade,3);
        }else{
            printf("---- Aluno nao encontrado ----");
        }
    }else{
        printf("O arquivo nao contem nenhum registro");
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
    int j = 0;
    bool achou = false;
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
    memset(bloco,0,512);
    ponteiroBloco = bloco;
    
    if(quantReg > 0){
        for(j=0;j<quantReg && !achou ; j++){
            if(j == (quantReg - sobraReg) && sobraReg != 0){
                    ponteiroBloco = bloco;
                    string tamanho = "%"+ std::to_string(sobraReg * 46) + "c";
                    fscanf(arq, tamanho.c_str() ,ponteiroBloco);
                    tipoBloco = 1;
            }else{
                if(j % 11 == 0){
                    ponteiroBloco = bloco;
                    fscanf(arq, "%506c",ponteiroBloco);
                    tipoBloco = 2;
                }
            }
            copiaParaChar(ponteiroBloco,alunoAtual.ra,6);
            printf("%s",alunoAtual.ra);
            if(comparaCadeiaChar(alunoAtual.ra,ra,6)){
                memset(ponteiroBloco,' ',46);
                achou = true;
                ponteiroBloco = bloco;
                if(tipoBloco == 1){
                    fseek(arq, - sobraReg * 46  , SEEK_CUR);
                    string tamanho = "%"+ std::to_string(sobraReg * 46) + "s";
                    fprintf(arq, tamanho.c_str() ,ponteiroBloco);
                }else{
                    fseek(arq, - 506  , SEEK_CUR);
                    fprintf(arq,"%506s",ponteiroBloco);
                }
            }else{
                ponteiroBloco = ponteiroBloco + 46;
            }
            
        }
        if(achou){
            printf("---- Aluno excluido ----");
        }else{
            printf("---- Aluno nao encontrado ----");
        }
    }else{
        printf("O arquivo nao contem nenhum registro");
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
    int j = 0;
    bool achou = false;
    FILE* arq = fopen(nomeArquivo.c_str(), "r+");
    

    //verifica quantidade de registros que tem no arquivo
    quantReg =quantBytes() / 46 ;
    
    fseek(arq, 0, SEEK_SET);
    sobraReg = quantReg % 11;
    bloco = (unsigned char*) malloc(512);
    memset(bloco,0,512);
    ponteiroBloco = bloco;
    
    if(quantReg > 0){
        for(j=0;j<quantReg && !achou ; j++){
            if(j == (quantReg - sobraReg) && sobraReg != 0){
                    ponteiroBloco = bloco;
                    string tamanho = "%"+ std::to_string(sobraReg * 46) + "c";
                    fscanf(arq, tamanho.c_str() ,ponteiroBloco);
            }else{
                if(j % 11 == 0){
                    ponteiroBloco = bloco;
                    fscanf(arq, "%506c",ponteiroBloco);
                }
            }
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
    }else{
        printf("O arquivo nao contem nenhum registro");
    }
        
    free(bloco);
    fclose(arq);
    
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
