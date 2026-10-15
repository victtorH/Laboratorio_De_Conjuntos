# Laboratório de Conjuntos e Conjuntos Numéricos
**Curso:** Desenvolvimento de Software Multiplataforma - FATEC Osasco  
**Disciplina:** Matemática para Computação  
**Integrantes:** Matheus Santos, Victor Hugo

---

## 🛠️ Tecnologias e Ambiente
- **Linguagem:** C (Padrão C11)
- **Compilação e Execução:** Docker e CMake

### Como Preparar e Executar os Exercícios:

#### Opção 1: Execução Direta no Windows (Sem Docker)
*Opção recomendada para avaliação do Professor.*

**1. Pré-requisito (GCC):**
Caso não possua o compilador C (GCC) instalado, abra o **PowerShell como Administrador** e instale rapidamente com este comando:
```powershell
winget install MSYS2.MSYS2

```

**2. Clonar e Executar:**
Clone o repositório e compile cada exercício manualmente.

```powershell
# Clonar o repositório
git clone https://github.com/victtorH/Laboratorio_De_Conjuntos.git
cd Laboratorio_De_Conjuntos

# Compilar e rodar o Exercício 1
gcc exercicio1/*.c -Iexercicio1 -o exe1.exe
.\exe1.exe

# Compilar e rodar o Exercício 2
gcc exercicio2/*.c -Iexercicio2 -o exe2.exe -lm
.\exe2.exe

# Compilar e rodar o Exercício 3
gcc exercicio3/*.c -Iexercicio3 -o exe3.exe
.\exe3.exe

```

#### 🐳 Opção 2: Execução via Docker (Ambiente Isolado)

*Utiliza o container com CMake.*

```bash
# -- Preparando o Ambiente --
git clone https://github.com/victtorH/Laboratorio_De_Conjuntos.git
cd Laboratorio_De_Conjuntos

docker compose build

# -- Executando os Exercicios --

# Para rodar o Exercício 1:
docker compose run --rm compiler sh -c "mkdir -p build && cd build && cmake .. && make && ./exe1"

# Para rodar o Exercício 2:
docker compose run --rm compiler sh -c "mkdir -p build && cd build && cmake .. && make && ./exe2"

# Para rodar o Exercício 3:
docker compose run --rm compiler sh -c "mkdir -p build && cd build && cmake .. && make && ./exe3"

```

---

## 📌 Parte 1: Teoria dos Conjuntos (Básico)

*Responsável: Victor Hugo*

### Decisões de Implementação

### Decisões de Implementação

- **Modularização do Código:** O módulo foi dividido em `conjuntos.c`/`conjuntos.h` para a lógica matemática de conjuntos e `terminal_ui.c`/`terminal_ui.h` para o menu e interface visual (TUI) com cores ANSI.
- **Gerenciamento de Memória e Unicidade:** Utilização de alocação dinâmica (`struct Set`) com redimensionamento sob demanda (`realloc`). Para garantir a definição matemática de conjunto, a função de adição (`set_add`) realiza uma verificação prévia de pertinência (`set_contains`), impedindo a inserção de elementos duplicados.
- **Operações e Casos de Borda:** As operações de união, interseção, diferença e inclusão foram implementadas utilizando laços de comparação direta de strings (`strcmp`), tratando com segurança os casos de conjuntos vazios (tamanho 0) e liberando a memória alocada (`set_free`) ao final de cada execução.

### Prints de Execução e Casos de Borda

#### Caso Geral: Menu e Operações Principais

![Caso Geral](imgs/exe1_caso_geral.png)

#### Caso de Teste 1: Conjunto Vazio (A = ∅, B = {1, 2})

![Conjunto Vazio](imgs/exe1_vazio.png)

#### Caso de Teste 2: Conjuntos Iguais (A = B = {1, 2, 3})

![Conjuntos Iguais](imgs/exe1_iguais.png)


#### Caso de Teste 3: Conjuntos Disjuntos (A = {1, 2}, B = {3, 4})

![Conjuntos Disjuntos](imgs/exe1_disjuntos.png)


#### Caso de Teste 4: Elementos Repetidos na Entrada (A = {1, 1, 1, 2})

![Elementos Repetidos ](imgs/exe1_repetidos.png)

---

## 📌 Parte 2: Conjuntos Numéricos (Intermediário)

*Responsável: Victor Hugo*

### Decisões de Implementação

### Decisões de Implementação

- **Parsing e Leitura de Entrada:** O sistema processa o número recebido como string, tratando vírgula ou ponto decimal e identificando padrões de formato: números inteiros, frações (`a/b`), raízes (`sqrt` ou `√`) e dízimas periódicas indicadas por parênteses (`parte_nao_periodica(periodo)`).
- **Cálculo da Fração Geratriz e Simplificação:** Para dízimas compostas, o algoritmo separa a parte inteira, a não periódica e o período, aplicando a fórmula algébrica para montar numerador e denominador. Em seguida, aplica o Algoritmo de Euclides para calcular o MDC e simplificar a fração até sua forma irredutível.
- **Hierarquia dos Conjuntos Numéricos:** A classificação segue a cadeia $\mathbb{N} \subset \mathbb{Z} \subset \mathbb{Q} \subset \mathbb{R}$. Se um número racional simplificado possui denominador 1, ele é promovido a $\mathbb{Z}$ (e a $\mathbb{N}$ se não for negativo). Raízes de inteiros que não geram quadrados perfeitos são detectadas e classificadas diretamente no conjunto dos irracionais ($\mathbb{I}$ e $\mathbb{R}$).
### Prints de Execução

#### Teste 1: Dízima Periódica e Fração Geratriz (Entrada: 1,2(5))

![Dizima Periodica e Fração Geratriz](imgs/exe2_dizima.png)

#### Teste 2: Inteiros Negativos e Frações (-3, 3/4, -5/2, 6/3)

![Inteiros Negativos e Fração](imgs/exe2_fracoes.png)


#### Teste 3: Raízes Exatas e Irracionais (√9 e √2)

![Raízes Exatas e Irracionais](imgs/exe2_raizes.png)

---

## 📌 Parte 3: Sistema de Recomendação (Índice de Jaccard)

*Responsável: Matheus da Silva Santos*

### Decisões de Implementação

* **Modelagem Matemática:** Perfis de usuário representados como conjuntos finitos de texto. A regra formal de unicidade foi garantida verificando a pertinência antes de cada inserção.


* **Estrutura de Dados:** Utilização de vetores estáticos em memória local (`struct Conjunto`) para evitar vazamentos de memória e manter o algoritmo direto e seguro.
* **Métrica Aplicada:** Fórmula do Índice de Jaccard:

$$\text{Jaccard}(A, B) = \frac{\vert{}A \cap B\vert{}}{\vert{}A \cup B\vert{}}$$




### Demonstração de Execução

![Print do Resultado](imgs/exe3_jaccard.png)

### Saída da Execução (Log Oficial)

```text
=== EXERCICIO 3: RECOMENDACAO POR INTERESSES (JACCARD) ===

[Entrada - Interesses]
- Ana   { rock, jazz, cinema, games } (4 itens)
- Bia   { jazz, cinema, series, viagens } (4 itens)
- Caio  { games, tecnologia, cinema } (3 itens)

----------------------------------------------------------
[Calculos de Afinidade]

Par 1: Ana e Bia
  Intersecao: { jazz, cinema } -> |A ∩ B| = 2
  Uniao:      { rock, jazz, cinema, games, series, viagens } -> |A ∪ B| = 6
  Jaccard:    2 / 6 = 0.33

Par 2: Ana e Caio
  Intersecao: { cinema, games } -> |A ∩ B| = 2
  Uniao:      { rock, jazz, cinema, games, tecnologia } -> |A ∪ B| = 5
  Jaccard:    2 / 5 = 0.40

Par 3: Bia e Caio
  Intersecao: { cinema } -> |A ∩ B| = 1
  Uniao:      { jazz, cinema, series, viagens, games, tecnologia } -> |A ∪ B| = 6
  Jaccard:    1 / 6 = 0.17

----------------------------------------------------------
[Resultado]
Maior afinidade com a Ana: Caio (Indice: 0.40)
Recomendacao: Caio

```
