# 🚀 Projeto C Portável com Docker

Este projeto foi configurado para rodar de forma idêntica em qualquer sistema operacional (**Windows, macOS ou Linux**), sem a necessidade de instalar compiladores de C (como GCC ou Clang) localmente na sua máquina física. Tudo roda isolado dentro de um container Docker.

---

## 🛠️ Pré-requisitos Obrigatórios

Antes de começar, você precisa ter apenas duas ferramentas instaladas na sua máquina física:
1. **Git** (Para clonar e gerenciar o código).
2. **Docker Desktop** (Essencial para rodar o ambiente de desenvolvimento). 
   * *Certifique-se de abrir o Docker Desktop antes de executar os comandos abaixo.*

---

## 💻 Como Iniciar o Projeto na sua Máquina

Se esta é a sua primeira vez mexendo no projeto, siga estes passos no seu terminal:

1. **Clone o repositório:**
   ```bash
   git clone <URL_DO_SEU_REPOSITORIO>
   cd Laboratorio_De_Conjuntos
   ```

2. **Construa o ambiente do Docker:**
   ```bash
   docker compose build
   ```
   *(Este comando pode demorar alguns minutos na primeira vez, pois ele vai baixar as ferramentas de C corretas estruturadas pelo time).*

---

## 🔄 Como Desenvolver e Compilar o Código

O fluxo de desenvolvimento foi desenhado para atualizar suas modificações instantaneamente:

### Passo 1: Edite o Código
Abra a pasta do projeto no seu editor favorito (VS Code, CLion, Notepad++, etc.) e altere ou adicione novos arquivos dentro da pasta do exercício em que você está trabalhando (ex: `exercicio1/`). Salve os arquivos normalmente na sua máquina física.

⚠️ **IMPORTANTE (Se criar novos arquivos .c):** Toda vez que você criar um novo arquivo de código (ex: `funcoes.c`), você precisa avisar o sistema de compilação. Abra o arquivo **`CMakeLists.txt`** na raiz do projeto e adicione o caminho do novo arquivo dentro do bloco correspondente ao seu executável. Exemplo:

```cmake
add_executable(exe1 
    exercicio1/main.c 
    exercicio1/exercicio1.c
    exercicio1/funcoes.c  # <-- Adicione o novo arquivo aqui!
)
```

---

### Passo 2: Compile e Execute no Docker

Como o projeto está dividido em exercícios independentes, você pode compilar e rodar cada um individualmente mudando o nome do executável no final do comando:

* **Para rodar o Exercício 1:**
  ```bash
  docker compose run --rm compiler sh -c "mkdir -p build && cd build && cmake .. && make && ./exe1"
  ```

* **Para rodar o Exercício 2:**
  ```bash
  docker compose run --rm compiler sh -c "mkdir -p build && cd build && cmake .. && make && ./exe2"
  ```

* **Para rodar o Exercício 3:**
  ```bash
  docker compose run --rm compiler sh -c "mkdir -p build && cd build && cmake .. && make && ./exe3"
  ```

**O que este comando faz automaticamente?**
* Entra no ambiente isolado do Docker.
* Cria a pasta de compilação (`build`) caso ela não exista.
* Lê as alterações que você acabou de fazer nas pastas dos exercícios.
* Compila o código C atualizado utilizando o CMake (versão padrão C11).
* Executa o exercício específico escolhido e imprime o resultado direto na tela do seu terminal.
* Apaga o container temporário após o uso (`--rm`), não acumulando lixo na sua máquina.

---

## 🚀 Fluxo de Trabalho com Git (Push e Merge)

O Docker **não** interfere em nada no fluxo tradicional do Git. Como os arquivos de código ficam na sua máquina real, nós usamos o padrão profissional de branches por funcionalidade/aluno (`feat/nome/tarefa`):

1. Crie uma branch para o seu exercício: `git checkout -b feat/seu-nome/exercicio-X`
2. Faça as alterações no código e teste com o comando de compilação do Docker acima.
3. Quando tudo estiver funcionando sem erros:
   ```bash
   git add .
   git commit -m "feat: adiciona nova funcionalidade em C"
   git push origin feat/seu-nome/exercicio-X
   ```
4. Abra o **Pull Request** no GitHub para que o seu parceiro de grupo revise e aprove o **Merge** na branch `main`.
