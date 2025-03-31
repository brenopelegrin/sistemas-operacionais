# checkpoint1

Nesse diretório, estão disponíveis os códigos relacionados a entrega do **Checkpoint 1**. Nessa entrega parcial, foram desenvolvidos
três pequenos programas para demonstrar 3 chamadas de sistema para cada uma das três categorias: Memória (``memory``), Processos (``processes``)
e I/O e Arquivos (``io-and-files``), totalizando 9 chamadas de sistema. 

Além disso, foram desenvolvidos dois programas demonstrativos, ``cpu-bound``  e ``io-bound``, para demosntrar respectivamente, o comportamento de
processos CPU bound e I/O bound.

## Estrutura

Todos os códigos dos programas foram desenvolvidos em linguagem ``C`` e alguns utilizam bibliotecas padrão como a GNU C Library para interagir
com as chamadas de sistema.

O build dos programas é feito automaticamente através de um ``Makefile``, e foi criada uma imagem Docker para garantir a portabilidade e reproducibilidade
do projeto.

A estrutura do projeto é a seguinte:

```bash
checkpoint1
├── Dockerfile          # Arquivo da imagem Docker
├── Makefile            # Arquivo com instruções para compilação automática
├── README.md           # Documentação do projeto
├── bin                 # Pasta para output dos binários (apenas dentro do container)
├── bin                 # Pasta temporária para output da compilação (apenas dentro do container)
├── entrypoint.sh       # Script executado ao iniciar o container, responsável por executar os programas e gerar métricas
└── src                 # Pasta onde estão os arquivos dos programas
    ├── cpu-bound
    │   └── main.c      # Source do programa cpu-bound
    ├── io-and-files
    │   └── main.c      # Source do programa io-and-files
    ├── io-bound
    │   └── main.c      # Source do programa io-bound
    ├── memory
    │   └── main.c      # Source do programa memory
    └── processes
        └── main.c      # Source do programa processes
```

## Como compilar e rodar

Para executar esse projeto da forma recomendada, deve-se utilizar o Docker.

Primeiro, certifique-se de que você está no diretório ``checkpoint1``:

```bash
cd projeto-ssc0541-grupo-06/checkpoint1/
```

### Compilando e executando o projeto com Docker

---

1. Faça o build da imagem Docker, salvando ela com a tag ``so-icmc/checkpoint1:latest``:

```bash
docker build -t so-icmc/checkpoint1:latest .
```

2. Após fazer o build, execute a imagem ``so-icmc/checkpoint1:latest`` para rodar os programas e obter as métricas:

```bash
docker rm -f checkpoint1 && docker run --name checkpoint1 so-icmc/checkpoint1:latest
```

Se quiser executar tudo em um comando só (assumindo que você já está no diretório ``checkpoint1``), faça:

```bash
docker build -t so-icmc/checkpoint1:latest . && \
docker rm -f checkpoint1 && docker run --name checkpoint1 so-icmc/checkpoint1:latest
```

### Compilando e executando SEM docker (não recomendado)

---

Primeiro, certifique-se de que você tem ``make``, ``gcc``, ``strace``, ``time`` instalados:

```bash
sudo apt install -yq make gcc strace time
```

1. Crie os diretórios locais para armazenar arquivos de build e os binários:

```bash
mkdir -p ./bin && \
mkdir -p ./build
```

2. Rode o ``Makefile`` para cada programa e copie os binários:

```bash
make io-and-files && cp ./build/io-and-files.o ./bin/io-and-files && \
make memory && cp ./build/memory.o ./bin/memory && \
make processes && cp ./build/processes.o ./bin/processes && \
make cpu-bound && cp ./build/cpu-bound.o ./bin/cpu-bound && \
make io-bound && cp ./build/io-bound.o ./bin/io-bound && \
make clean
```

3. Agora, os programas estão disponíveis em ``./bin/<nome_do_programa>``. Execute-os utilizando ``strace`` ou ``time`` para obter as métricas a sua escolha:

```bash
strace -c ./bin/io-and-files && \
/usr/bin/time -v ./bin/io-and-files
```

## Sobre a imagem Docker

A imagem Docker criada possui dois estágios:

- 1º estágio (build): utiliza a base ``gcc:14.2.0-bookworm`` para compilar os programas e mover os binários para a pasta /usr/app/bin
- 2º estágio (runtime): utiliza a base ``ubuntu:24.04`` para rodar o ``entrypoint.sh`` que irá executar os programas obtendo métricas com auxílio dos utilitários ``strace`` e ``time``.

Separamos o estágio de build e o de execução para melhorar o desempenho da imagem e otimizá-la, além de garantir que as ferramentas de build estão sempre disponíveis (na imagem do GCC) sem ter que instalá-las no Ubuntu.