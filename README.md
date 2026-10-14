Analisando a lista oficial de **Entregáveis** e os **Exemplos de casos de borda** da folha:

1. **Parte 1 (Obrigatório nos entregáveis):** exige especificamente prints/logs dos casos de teste de **conjunto vazio**, **conjuntos iguais**, **conjuntos disjuntos** e **elementos repetidos na entrada**, além da demonstração do caso geral com operações e cardinalidade.


2. **Parte 2 (Obrigatório nos entregáveis):** exige no mínimo 8 números de entrada, cobrindo **inteiro negativo**, **fração**, **decimal exato**, **dízima periódica** (com cálculo da fração geratriz) e **raiz irracional**.


3. **Parte 3:** exige a demonstração dos 3 pares analisados e a recomendação final de afinidade.


4. **Relatório de 1 página:** a folha pede no README a explicação de onde a teoria apareceu e a operação mais difícil.



---

Aqui está o seu `README.md` completo, estruturado e com os marcadores exatos `<!-- [COLOQUE AQUI ...] -->` indicando cada print e caso de teste que vocês precisam anexar:

# Laboratório de Conjuntos e Conjuntos Numéricos
**Curso:** Desenvolvimento de Software Multiplataforma - FATEC Osasco  
**Disciplina:** Matemática para Computação  
**Integrantes:** Matheus Santos, Victor Hugo

---

## 🛠️ Tecnologias e Ambiente
- **Linguagem:** C (Padrão C11)
- **Compilação e Execução:** Docker e CMake

### Como Preparar e Executar os Exercícios:
```bash
# -- Preparando o Ambiente --
git clone https://github.com/victtorH/Laboratorio_De_Conjuntos.git
cd Laboratorio_De_Conjuntos

docker compose build

# -- Executando os Exercicios --

# Exercício 1
docker compose run --rm compiler sh -c "mkdir -p build && cd build && cmake .. && make && ./exe1"

# Exercício 2
docker compose run --rm compiler sh -c "mkdir -p build && cd build && cmake .. && make && ./exe2"

# Exercício 3
docker compose run --rm compiler sh -c "mkdir -p build && cd build && cmake .. && make && ./exe3"
```

---

## 📌 Parte 1: Teoria dos Conjuntos (Básico)

*Responsável: Victor Hugo*

### Decisões de Implementação

### Exemplos de Execução e Casos de Borda Obrigatórios

#### 1. Caso Geral (Menu Principal e Operações)

Demonstração das operações matemáticas ($A \cup B$, $A \cap B$, $A - B$, pertinência, inclusão e cardinalidade).

#### 2. Caso de Borda: Conjunto Vazio

Entrada: $A = \emptyset$, $B = \{1, 2\}$

Resultado esperado: $A \cup B = \{1, 2\}$; $A \cap B = \emptyset$; $A \subseteq B$ é verdadeiro.

#### 3. Caso de Borda: Conjuntos Iguais

Entrada: $A = B = \{1, 2, 3\}$

Resultado esperado: $A - B = \emptyset$; $A \subseteq B$ é verdadeiro; $A \subset B$ é falso.

#### 4. Caso de Borda: Conjuntos Disjuntos

Entrada: $A = \{1, 2\}$, $B = \{3, 4\}$

Resultado esperado: $A \cap B = \emptyset$; $A \cup B = \{1, 2, 3, 4\}$.

#### 5. Caso de Borda: Elementos Repetidos na Entrada

Entrada: $A = \{1, 1, 1, 2\}$

Resultado esperado: Descarte de duplicatas resultando em $A = \{1, 2\}$ com cardinalidade $\vert{}A\vert{} = 2$.

---

## 📌 Parte 2: Conjuntos Numéricos (Intermediário)

*Responsável: Victor Hugo*

### Decisões de Implementação

### Exemplos de Execução (Classificação de Números)

#### 1. Dízima Periódica (Identificação e Fração Geratriz)

Entrada: `1,2(5)`

Resultado esperado: Identificação de dízima composta, fração geratriz `113/90`, pertence a $\mathbb{Q}$ e $\mathbb{R}$.

#### 2. Fração e Números Negativos

Entradas testadas: `-3`, `3/4`, `-5/2`, `6/3` (simplifica para inteiro 2).

#### 3. Números Irracionais e Raízes

Entradas testadas: `√9` (raiz exata = 3) e `√2` (raiz não exata, irracional $\mathbb{I}, \mathbb{R}$).

---

## 📌 Parte 3: Sistema de Recomendação (Índice de Jaccard)

*Responsável: Matheus da Silva Santos*

### Decisões de Implementação

* **Modelagem Matemática:** Perfis de interesse modelados como conjuntos de strings. A unicidade formal dos elementos foi assegurada verificando a pertinência antes de cada inserção[cite: 1, 4].
* **Estrutura de Dados:** Utilização de vetores estáticos em memória local (`struct Conjunto`) para evitar vazamentos de memória e manter o algoritmo direto e legível.
* **Métrica:** Aplicação estrita da fórmula do Índice de Jaccard:

$$J(A, B) = \frac{\vert{}A \cap B\vert{}}{\vert{}A \cup B\vert{}}$$



[cite: 3]

### Demonstração dos Casos de Teste do Enunciado

### Saída da Execução (Log)

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


---

