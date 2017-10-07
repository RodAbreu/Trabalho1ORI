
def abertura():
    print('Define sua escolha:\n')
    print('[1] Criacao de um arquivo')
    print('[2] Insercao de um novo registro')
    print('[3] Buscar um registro')
    print('[4] Remocao de um registro')
    print('[5] Listagem dos registros')
    print('[6] Compactacao do arquivo')
    print('[0] Sair')
    pass

print('Bem-vindo ao trabalho 1!!\n\n')
import sys
import os.path
x=1
while x!=0:
    abertura()
    x = input('Digite sua escolha: ')

    if x==1:
        nomeArquivo = raw_input('Digite o nome do arquivo que voce deseja criar: ')
        arquivo = open(nomeArquivo+'.txt','w')
        arquivo.close()
        pass
    elif x==2:
        nomeArquivo = raw_input('Digite o nome do arquivo que voce deseja acessar: ')
        arquivo = open(nomeArquivo+'.txt','w')
        nome = raw_input('Digite seu nome: ')
        arquivo.write(nome+'||')
        ra = raw_input('Digite seu ra: ')
        arquivo.write(ra+'||')
        arquivo.close()

    elif x==3:
        pass
    elif x==4:
        pass
    elif x==5:
        nome = raw_input('Digite o nome do arquivo que voce deseja acessar: ')
        print open(nome+'.txt').read()
        pass
    elif x==6:
        pass
    else:
        x = 0
        pass

    print('\n\n')

print('Fim do programa.')
