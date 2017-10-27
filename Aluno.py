import os
import sys
import fileinput

class Aluno(object):
    contAlunos = 0

    def displayEmployee(self):
        print "Name : ", self.name,  ", RA: ", self.RA,  "Idade: ", self.idade

    def __init__(self, nome, RA, idade):
        self.nome = name
        self.RA = RA
        self.idade = idade
        Aluno.contAlunos += 1
