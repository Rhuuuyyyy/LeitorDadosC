import json
import random

arquivo = 'dados.json'
sortear = 10

with open('dados.json', 'r', encoding='utf-8') as arquivo:
    categorias_unicas = list(set(item['Categoria'] for item in json.load(arquivo)))

sorteadas = random.sample(categorias_unicas, sortear)

opcoes_validas = ['a','b','c','d','e','f','g','h','i','j']
escolha = ''
menu = """
MENU DE OPÇÕES 
a. Listar todas as categorias de alimentos.
b. Listar alimentos de uma categoria (ordem alfabética).
c. Listar alimentos de uma categoria (ordem de energia).
d. Listar os N alimentos com maior umidade de uma categoria.
e. Listar os N alimentos com maior energia de uma categoria.
f. Listar os N alimentos com mais proteína de uma categoria.
g. Listar os N alimentos com mais carboidrato de uma categoria.
h. Listar os N alimentos com maior relação energia/proteína.
i. Listar os N alimentos com maior relação energia/carboidrato.
j. Encerrar o programa.
"""

while escolha not in opcoes_validas:
    escolha = str(input(f"{menu}\n Escolha sua opção: ")).strip().lower()