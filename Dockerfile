FROM alpine:latest

# Instala ferramentas essenciais de compilação C e CMake
RUN apk add --no-cache build-base cmake

# Define o diretório de trabalho interno do container
WORKDIR /app

# Cria a pasta onde os arquivos temporários de compilação vão residir
RUN mkdir -p build

# Mantém o container em execução aguardando comandos
CMD ["sh"]
