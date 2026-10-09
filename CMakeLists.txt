cmake_minimum_required(VERSION 3.10)
project(LaboratorioFATEC C)

set(CMAKE_C_STANDARD 11)

# Cria o executável do Exercício 1
add_executable(exe1 
    exercicio1/main.c
)

# Cria o executável do Exercício 2
add_executable(exe2 
    exercicio2/main.c
)

# Cria o executável do Exercício 3
add_executable(exe3 
    exercicio3/main.c
)
