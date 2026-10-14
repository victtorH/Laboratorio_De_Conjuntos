cmake_minimum_required(VERSION 3.10)
project(LaboratorioFATEC C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

# Cria o executável do Exercício 1
add_executable(exe1 
    exercicio1/main.c
    exercicio1/conjuntos.c
    exercicio1/terminal_ui.c
)

# Cria o executável do Exercício 2
add_executable(exe2 
    exercicio2/main.c
    exercicio2/familias.c
    exercicio2/terminal_ui.c

)

# Cria o executável do Exercício 3
add_executable(exe3 
    exercicio3/main.c
)
