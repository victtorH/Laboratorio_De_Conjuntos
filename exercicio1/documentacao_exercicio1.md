# Documentação Técnica: Projeto de Teoria dos Conjuntos (Exercício 1)

**Autor:** Victtor Hugo Estevam  
**Curso:** Desenvolvimento de Software Multiplataforma - FATEC  
**Disciplina:** Matemática para computação

---

## 1. Visão Geral

Este projeto é a minha implementação do Exercício 1 sobre Teoria dos Conjuntos. O objetivo principal foi criar uma aplicação interativa em C para manipulação de conjuntos matemáticos, garantindo que as operações (União, Interseção, etc.) funcionassem corretamente. 

Como estudante, encarei este desafio como uma excelente oportunidade para tentar aplicar conceitos reais de engenharia de software na prática. Busquei focar bastante na estruturação dos dados, no gerenciamento responsável da memória (alocação dinâmica) e em criar uma interface de terminal (TUI) mais amigável. Tentei elevar o nível técnico do projeto, mas sempre com a consciência de que é um ambiente de aprendizado e que ainda tenho muito a evoluir.

---

## 2. Estrutura e Modularização (Arquitetura)

Para manter o código limpo e organizado, decidi dividir a aplicação em três módulos principais. Acredito que essa separação de responsabilidades (*Separation of Concerns*) ajuda muito na manutenção e facilita a leitura do código pelos professores e colegas:

1. **`conjuntos.c` e `conjuntos.h`:** O "coração" lógico do projeto. Aqui abstraí a entidade `Set` e implementei toda a lógica matemática das operações de conjuntos e do gerenciamento dinâmico do vetor de strings.
2. **`terminal_ui.c` e `terminal_ui.h`:** O módulo visual. Dediquei este espaço para isolar as chamadas de impressão no console, usando sequências ANSI para deixar a interface interativa e colorida, separando totalmente a lógica de exibição da lógica de negócios.
3. **`main.c`:** O orquestrador da aplicação. Gerencia o fluxo do programa, lida com as entradas do usuário (de forma validada) e conecta a interface visual com a lógica dos conjuntos.

---

## 3. Funcionalidades Implementadas

O sistema permite a criação dinâmica de dois conjuntos (Caixa A e Caixa B) e realiza as seguintes operações matemáticas clássicas:

* **União ($A \cup B$):** Cria um novo conjunto com todos os elementos de A e B, sem duplicatas.
* **Interseção ($A \cap B$):** Isola os elementos que existem simultaneamente em A e em B.
* **Diferença ($A - B$):** Retorna os elementos que são exclusivos da caixa A.
* **Pertinência ($x \in A$):** Verifica se um valor específico (informado via teclado) está contido na Caixa A.
* **Inclusão ($A \subseteq B$):** Valida se todos os elementos da Caixa A também estão na Caixa B.
* **Cardinalidade ($\vert{}A\vert{}$):** Informa a quantidade total de elementos armazenados no conjunto.

---

## 4. Destaques Técnicos e Aprendizados (Boas Práticas)

Durante o desenvolvimento, tentei ir além do "fazer funcionar", buscando aplicar as boas práticas que venho estudando. Alguns pontos que considerei importantes de destacar:

### 4.1. Gerenciamento Seguro de Memória
A implementação não usa vetores estáticos de tamanho fixo. Optei por uma estrutura totalmente dinâmica (`char **elements`) que cresce sob demanda usando `malloc` e `realloc`.
* **Evitando dependências externas:** Para manter o código estritamente aderente ao padrão ISO C puro e evitar problemas de compatibilidade (como depender do POSIX `strdup`), criei a função auxiliar `duplicate_string` em `conjuntos.c`.
* **Limpeza rigorosa (`set_free`):** Tive um cuidado especial para iterar sobre cada string alocada para dar `free` antes de liberar a estrutura principal do conjunto. A ideia foi garantir que o programa rode sem deixar *memory leaks* (vazamento de memória), algo fundamental em C.

### 4.2. Programação Defensiva
O tratamento de *input* no C é sempre um grande desafio. No arquivo `main.c`, criei funções como `read_string_secure` para tentar blindar o sistema contra erros comuns:
* Substituí o uso do `scanf` pelo `fgets`, limpando o buffer do teclado adequadamente.
* Tratei entradas longas demais do usuário, evitando vulnerabilidades de *buffer overflow*.
* Tentei fazer o programa lidar graciosamente com interrupções inesperadas (como EOF, `Ctrl+D` ou `Ctrl+Z`), garantindo que a memória seja liberada e o sistema aborte com segurança.

### 4.3. Interface Gráfica no Terminal (TUI)
* Isolar a interface no módulo `terminal_ui` deixou meu `main.c` muito mais limpo.
* Tive o cuidado de implementar um sistema básico de quebra de linha dinâmica (na função `tui_print_set`) para garantir que os conjuntos desenhados não "quebrassem" o layout do terminal caso o usuário adicionasse muitos elementos.

---

## 5. Considerações Finais

Desenvolver este exercício foi uma ótima experiência. Pude consolidar a teoria matemática vista na Fatec de uma forma prática, enquanto treinava conceitos de baixo nível em C que exigem bastante disciplina (como manipular ponteiros duplos e alocação dinâmica).

Embora o código atenda aos requisitos, sei que há sempre espaço para melhorar. Por exemplo, se esse sistema precisasse lidar com milhões de dados no futuro, a busca sequencial $O(n)$ precisaria ser substituída por uma Tabela Hash para buscas em $O(1)$. De qualquer forma, foi um esforço honesto para entregar o melhor código possível no meu momento atual de aprendizado.