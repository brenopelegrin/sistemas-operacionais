# checkpoint2

Nesse diretório, estão disponíveis os códigos relacionados a entrega do **Checkpoint 2**.

## Estrutura

Todos os códigos dos programas foram desenvolvidos em linguagem ``C``.
- Inserir aqui informações sobre as bibliotecas utilizadas

O build dos programas é feito automaticamente através de um ``Makefile``, e foi criada uma imagem Docker para garantir a portabilidade e reproducibilidade
do projeto.

A estrutura do projeto é a seguinte:

```bash
checkpoint2
├── Dockerfile                # Arquivo da imagem Docker
├── Makefile                  # Arquivo com instruções para compilação automática
├── README.md                 # Documentação do projeto
├── bin                       # Pasta para output dos binários (apenas dentro do container)
├── buiild                    # Pasta temporária para output da compilação (apenas dentro do container)
├── entrypoint.sh             # Script executado ao iniciar o container, responsável por executar os programas e gerar métricas
└── src                       # Pasta onde estão os arquivos dos programas
    ├── main.c                # Source do programa principal
    └── lib-queuelkdlist
        └── queuelkdlist.c    # Source da biblioteca lib-queuelkdlist
        └── queuelkdlist.h    # Header da biblioteca lib-queuelkdlist
```

## Como compilar e rodar

Para executar esse projeto da forma recomendada, deve-se utilizar o Docker.

Primeiro, certifique-se de que você está no diretório ``checkpoint2``:

```bash
cd projeto-ssc0541-grupo-06/checkpoint2/
```

### Compilando e executando o projeto com Docker

---

1. Faça o build da imagem Docker, salvando ela com a tag ``so-icmc/checkpoint2:latest``:

```bash
docker build -t so-icmc/checkpoint2:latest .
```

2. Após fazer o build, execute a imagem ``so-icmc/checkpoint2:latest`` para rodar os programas e obter as métricas:

```bash
docker rm -f checkpoint2 && docker run --name checkpoint2 so-icmc/checkpoint2:latest
```

Se quiser executar tudo em um comando só (assumindo que você já está no diretório ``checkpoint2``), faça:

```bash
docker build -t so-icmc/checkpoint2:latest . && \
docker rm -f checkpoint2 && docker run --name checkpoint2 so-icmc/checkpoint2:latest
```

### Compilando e executando SEM docker (não recomendado)

---

Primeiro, certifique-se de que você tem ``make``, ``gcc`` instalados:

```bash
sudo apt install -yq make gcc
```

1. Crie os diretórios locais para armazenar arquivos de build e os binários:

```bash
mkdir -p ./bin && \
mkdir -p ./build
```

2. Rode o ``Makefile`` para cada programa e copie os binários:

```bash
make && cp ./build/main.o ./bin/main && \
make clean
```

3. Agora, o programa está disponíveis em ``./bin/main``. Execute-os usando o comando:

```bash
./bin/main
```

## Sobre a imagem Docker

A imagem Docker criada possui dois estágios:

- 1º estágio (build): utiliza a base ``gcc:14.2.0-bookworm`` para compilar os programas e mover os binários para a pasta /usr/app/bin
- 2º estágio (runtime): utiliza a base ``ubuntu:24.04`` para rodar o ``entrypoint.sh`` que irá executar o programa

Separamos o estágio de build e o de execução para melhorar o desempenho da imagem e otimizá-la, além de garantir que as ferramentas de build estão sempre disponíveis (na imagem do GCC) sem ter que instalá-las no Ubuntu.