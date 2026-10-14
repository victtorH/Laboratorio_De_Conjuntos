# 📚 Documentação Técnica: Robô Auxiliar de Matemática (Exercício 2)

**Autor:** Victtor Hugo Estevam  
**Instituição:** FATEC - Faculdade de Tecnologia  
**Curso:** Desenvolvimento de Software Multiplataformas (DSM)  
**Disciplina:** Matemática e Algoritmos  

---

## 🎯 Objetivo do Projeto
Este documento detalha a arquitetura, as funcionalidades e as decisões técnicas aplicadas no desenvolvimento do **Robô Auxiliar de Matemática (Parte 2)**. O objetivo principal do sistema é receber entradas numéricas diversas (inteiros, frações, decimais, dízimas periódicas e raízes) e classificá-las rigorosamente dentro da Teoria dos Conjuntos Matemáticos: Naturais ($\mathbb{N}$), Inteiros ($\mathbb{Z}$), Racionais ($\mathbb{Q}$), Irracionais ($\mathbb{I}$) e Reais ($\mathbb{R}$).

Busquei desenvolver este projeto não apenas para resolver o problema matemático, mas para aplicar boas práticas de engenharia de software, programação defensiva e algoritmos eficientes que venho estudando.

---

## 🏗️ Arquitetura e Modularização

Para evitar um código monolítico e difícil de manter, decidi separar o projeto em três camadas principais, inspirando-me em princípios de responsabilidade única (SOLID):

1. **`main.c` (Orquestrador / Controller):**
   - Gerencia o ciclo de vida da aplicação (REPL - *Read-Eval-Print Loop*).
   - Lida com o *input* bruto do usuário e delega o processamento lógico.
   - Faz a ponte (orquestração) entre o motor matemático e a interface gráfica.

2. **`familias.c` e `familias.h` (Motor Lógico / Model):**
   - Coração do sistema. Totalmente agnóstico de interface (não possui `printf`).
   - Responsável pelo *parsing* léxico, sanitização, cálculos de mitigação de *overflow* e classificação matemática usando máscaras de bits (*bitmasks*).

3. **`terminal_ui.c` e `terminal_ui.h` (Camada Visual / View):**
   - Encapsula toda a lógica de exibição, utilizando *ANSI escape codes* para colorização e formatação.
   - Traduz as classificações matemáticas (ex: `FAM_Q | FAM_Z`) em textos explicativos legíveis e amigáveis para o usuário.

---

## 🧠 Soluções Algorítmicas e Matemáticas

Para garantir a precisão e a performance do classificador, implementei alguns algoritmos clássicos adaptados para as necessidades do projeto:

### 1. Tratamento de Conjuntos Numéricos via *Bitmask*
Ao invés de usar múltiplos booleanos para definir se um número pertence a $\mathbb{N}, \mathbb{Z}, \mathbb{Q}, \mathbb{I}$ ou $\mathbb{R}$, utilizei uma Enumeração operando como uma **Máscara de Bits**:
```c
typedef enum {
    FAM_NONE = 0x00,
    FAM_N    = 0x01,
    FAM_Z    = 0x02,
    FAM_Q    = 0x04,
    // ...
} FamiliaNumerica;
```
Isso permite que a classificação seja feita com operações bit a bit (`res.familias |= FAM_R | FAM_Q | FAM_Z`), tornando o código extremamente elegante, performático e economizando memória.

### 2. Busca Binária para Raízes Quadradas
Para extrair a raiz inteira exata e classificar corretamente entradas como `sqrt(16)` ou `√2`, implementei o algoritmo `raiz_inteira_exata`. Utilizei **Busca Binária** (complexidade $O(\log n)$) em vez de uma iteração linear ou de depender da biblioteca `<math.h>`, demonstrando proficiência no controle do fluxo algorítmico sem onerar recursos com ponto flutuante desnecessário.

### 3. Máximo Divisor Comum (MDC)
Para encontrar a fração geratriz irredutível de decimais e dízimas, implementei o clássico **Algoritmo de Euclides**. Tive um cuidado especial aqui:
- Trabalhei com conversão interna para `unsigned long long` para evitar comportamento indefinido (UB) da linguagem C ao tentar extrair o valor absoluto do limite mínimo (`LLONG_MIN`).

---

## 🛡️ Programação Defensiva e Boas Práticas

Um dos meus maiores focos neste exercício foi construir um software **resiliente**. Como o usuário pode digitar qualquer coisa, implementei várias defesas:

### Sanitização de Buffer e Entrada (`main.c`)
- Utilizei `fgets()` no lugar do perigoso `scanf()`.
- Implementei a função `limpar_buffer_stdin()` para descartar caracteres remanescentes caso a entrada do usuário exceda a capacidade do buffer (evitando *loops* infinitos bizarros).
- A lógica de *trim* (`remover_espacos`) remove apenas espaços nas extremidades, rejeitando corretamente expressões malformadas internas (ex: `1   2` não vira `12`).

### Prevenção de *Overflow* Numérico (`familias.c`)
Estou ciente de que tipos numéricos têm limites físicos na memória. Para evitar que um usuário quebre o sistema digitando números astronômicos, criei funções *wrapper* rigorosas:
- `safe_strtoll`: Inspeciona a variável `errno == ERANGE` ao fazer o cast de strings.
- `safe_add` e `safe_mult`: Verificam os limites contra `LLONG_MAX` e `LLONG_MIN` *antes* de realizar a operação de fato, usando álgebra preventiva (ex: `se a > LLONG_MAX / b, então haverá overflow`).
- `safe_int_pow`: Previne *overflow* iterativo ao calcular denominadores baseados em potências de $10$ (usado para converter $0.333...$ em frações).

### Suporte a Multi-encoding
A rotina de validação de raízes compreende tanto notações ASCII normais (`sqrt()`, `v`, `V`) quanto o caractere UTF-8 nativo de raiz quadrada (`\xE2\x88\x9A` para `√`).

---

## 🌱 Conclusão e Aprendizados

Desenvolver o "Robô Auxiliar de Matemática" foi uma excelente oportunidade para aplicar teoria dos conjuntos de forma tangível. Mais do que entender quem pertence aos Racionais ($\mathbb{Q}$) ou Naturais ($\mathbb{N}$), este projeto me ensinou muito sobre os meandros da **Linguagem C**: manipulação de ponteiros genéricos (`char** endptr`), limites de arquitetura de processadores e o rigor necessário ao aceitar dados de entrada não confiáveis.

Embora o sistema resultante seja bastante robusto, sei que na engenharia de software sempre há espaço para evolução e otimização contínua. Fico muito feliz com o resultado acadêmico que alcancei aqui e com os padrões profissionais que consegui empregar no código-fonte.