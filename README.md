# Trabalho 1 - Interfaces e Perifericos

## Alunos:
Gabriel Ferrari Cipriano
Héricles Bruno Quaresma Koelher
Mikaella Ferreira da Silva

## Objetivo:
implementar um driver, sob o formato de um Loadable Kernel Module (LKM) para o Sistema Operacional Linux que faça a interface com um dispositivo de hardware virtual que simula a operação de uma calculadora aritmética simples, com as 4 operações básicas (soma, subtração, adição e divisão) com números inteiros.

## Pré-requisitos:
- Permissões de super-usuário
- Pacote Build-Essential

## Instalação & execução:
### Compilar o driver e o programa testador:
'''sudo make'''

#### Compilar somente o driver:
'''sudo make Driver'''

#### Compilar somente o programa testador:
'''sudo make Main'''

### Carregar o driver no kernel:
'''sudo make install'''

### Executar o programa de teste:
'''sudo make run'''

### Desinstalar o driver:
'''sudo make uninstall'''

### Limpar (apagar) os arquivos gerados:
'''sudo make clean'''
