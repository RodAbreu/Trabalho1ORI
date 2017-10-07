
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

#opcao1
def criaArquivo():
    nomeArquivo = raw_input('Digite o nome do arquivo que voce deseja criar: ')
    arquivo = open(nomeArquivo+'.txt','w')
    arquivo.close()
    pass

#opcao2
def addRegistro():
    #acessando o arquivo
    nomeArquivo = raw_input('Digite o nome do arquivo: ') #lendo o nome do arquivo
    arquivo = open(nomeArquivo+'.txt','r') #abrindo para leitura
    conteudo = arquivo.readlines() #lendo os dados do arquivo
    nome = raw_input('Digite seu nome: ')
    ra = raw_input('Digite seu ra: ')
    idade = raw_input('Digite sua idade: ')
    conteudo.append(nome+'||'+ra+'||'+idade+'||') #concatenando
    arquivo.close()
    arquivo = open(nomeArquivo+'.txt','w') #abrindo para escrita
    if len(conteudo)<=99:
        arquivo.writelines(conteudo) #adicionando conteudo
    else:
        print('Conteudo muito grande.')
    #fechando
    arquivo.close()
    pass

#opcao3
def procuraRegistro():
    nomeArquivo = raw_input('Digite o nome do arquivo: ')
    busca = raw_input('Digite o que deseja buscar: ')
    arquivo = file(nomeArquivo+'.txt')
    for line in arquivo:
		if busca in line:
			found = True
			break
		else:
			found = False
    return found

#opcao4

#opcao5
def mostraRegistros():
    nome = raw_input('Digite o nome do arquivo que voce deseja acessar: ')
    print open(nome+'.txt').read()
    pass

#opcao6





print('Bem-vindo ao trabalho 1!!\n\n')
import sys
import os.path
x=1
while x!=0:
    abertura()
    x = input('Digite sua escolha: ')

    if x==1:
        criaArquivo()
        pass

    elif x==2:
        addRegistro()
        pass

    elif x==3:
        #bucando um registro
        encontrou = procuraRegistro()
        print(encontrou)
        pass

    elif x==4:
        pass

    elif x==5:
        mostraRegistros()
        pass

    elif x==6:
        pass
    
    else:
        x = 0
        pass

    print('\n\n')

print('Fim do programa.')
