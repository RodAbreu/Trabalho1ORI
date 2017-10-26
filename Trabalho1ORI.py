import os
import sys
import fileinput
import Aluno

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
    os.system('clear')
    print('Arquivo criado com sucesso!')
    pass

#opcao2
def addRegistro():
    #acessando o arquivo
    nomeArquivo = raw_input('Digite o nome do arquivo: ') #lendo o nome do arquivo
    arquivo = open(nomeArquivo+'.txt','r') #abrindo para leitura
    conteudo = arquivo.readlines() #lendo os dados do arquivo
    nome = raw_input('\nDigite seu nome: ')
    ra = raw_input('\nDigite seu ra: ')
    idade = raw_input('\nDigite sua idade: ')
    conteudo.append(nome+'||'+ra+'||'+idade+'\n') #concatenando
    arquivo.close()
    arquivo = open(nomeArquivo+'.txt','w') #abrindo para escrita
    if len(conteudo)<=99:
        arquivo.writelines(conteudo) #adicionando conteudo
    else:
        print('Conteudo muito grande.')
    #fechando
    arquivo.close()
    os.system('clear')
    print('Registro adicionado com sucesso!')
    pass

#opcao3
def procuraRegistro():
    nomeArquivo = raw_input('Digite o nome do arquivo: ')
    busca = raw_input('Digite o que deseja buscar: ')
    arquivo = file(nomeArquivo+'.txt')
    for line in arquivo:
		if dado in line:
			found = True
			break
		else:
			found = False
    return found
    arquivo.close()
    os.system('clear')
    pass

#opcao4
def removeRegistro():
    nomeArquivo = raw_input('Digite o nome do arquivo: ')
    dado = raw_input('Digite o nome que deseja buscar: ')
    deci = raw_input('Deseja inserir outro nome, digite sim ou nao: ')
    if deci=='sim':
        novo_dado = raw_input('Digite o que deseja inserir no lugar: ')
        #Lendo do arquivo
        with open(nomeArquivo+'.txt', 'r') as file :
          filedata = file.read()
        #Atribuindo novo valor
        filedata = filedata.replace(dado,novo_dado)
        #Escrevendo no arquivo
        with open(nomeArquivo+'.txt', 'w') as file:
          file.write(filedata)
    elif deci=='nao':
        novo_dado = '*************'
        #Lendo do arquivo
        with open(nomeArquivo+'.txt', 'r') as file :
          filedata = file.read()
        #Atribuindo novo valor
        filedata = filedata.replace(dado,novo_dado)
        #Escrevendo no arquivo
        with open(nomeArquivo+'.txt', 'w') as file:
          file.write(filedata)
    else:
        print('Voce digitou errado')

    os.system('clear')
    pass


#opcao5
def mostraRegistros():
    nome = raw_input('\nDigite o nome do arquivo que voce deseja acessar: ')
    os.system('clear')
    print open(nome +'.txt').read()
    pass

#opcao6

#inicio do programa
print('Bem-vindo ao trabalho 1!!\n\n')
x=1
while x!=0:
    abertura()
    x = input('\nDigite sua escolha: ')

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
        #remocao de um registro
        removeRegistro()
        pass

    elif x==5:
        #exibicao de todos os registros
        mostraRegistros()
        pass

    elif x==6:
        pass

    else:
        x = 0
        pass

    print('\n\n')

print('Fim do programa.')
