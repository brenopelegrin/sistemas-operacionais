# checkpoint3

Nesse diretório, estão disponíveis os códigos relacionados à entrega do **Checkpoint 3**. Essa entrega consiste em um simulador de gerenciamento de memória virtual que implementa o sistema de paginação. O objetivo principal é executar uma sequência de operações de memória (leitura, escrita) e instruções de CPU ou E/S, lidas de um arquivo de entrada. O simulador gerencia múltiplos processos, cada um com seu próprio espaço de endereçamento virtual e tabela de páginas.

O sistema lida com page faults e usa algoritmos de substituição de páginas para decidir qual página remover da memória física quando um novo quadro é necessário e não há outros quadros livres. Foram implementados dois algoritmos clássicos para essa finalidade: LRU (Least Recently Used), que substitui a página menos recentemente usada, e Clock, que utiliza um bit de referência para dar uma segunda chance às páginas antes de substituí-las. Durante a simulação, são apresentadas informações sobre o estado atual do sistema e, ao final, é mostrado um resumo geral do desempenho com algumas estatísticas, como número de page faults e taxa de acerto (hit rate).

## Estrutura

Todos os códigos dos programas foram desenvolvidos em linguagem ``C++``.

O build dos programas é feito automaticamente através de um ``Makefile``, e foi criada uma imagem Docker para garantir a portabilidade e reproducibilidade
do projeto.

A estrutura do projeto é a seguinte:

```bash
checkpoint3
├── Dockerfile                # Arquivo da imagem Docker
├── Makefile                  # Arquivo com instruções para compilação automática
├── README.md                 # Documentação do projeto
├── bin                       # Pasta para output dos binários (apenas dentro do container)
├── buiild                    # Pasta temporária para output da compilação (apenas dentro do container)
├── data                      # Pasta para inserir os inputs a serem lidos pelo programa
├── entrypoint.sh             # Script executado ao iniciar o container, responsável por executar os programas e gerar métricas
└── src                       # Pasta onde estão os arquivos dos programas
    └── main.cpp              # Source do programa principal
```

## Como compilar e rodar

Para executar esse projeto da forma recomendada, deve-se utilizar o Docker.

Primeiro, certifique-se de que você está no diretório ``checkpoint3``:

```bash
cd projeto-ssc0541-grupo-06/checkpoint3/
```

### Compilando e executando o projeto com Docker

---

1. Faça o build da imagem Docker, salvando ela com a tag ``so-icmc/checkpoint3:latest``:

```bash
docker build -t so-icmc/checkpoint3:latest .
```

2. Após fazer o build, execute a imagem ``so-icmc/checkpoint3:latest`` para rodar os programas e obter as métricas:

```bash
docker rm -f checkpoint3 && docker run --name checkpoint3 so-icmc/checkpoint3:latest
```

Se quiser executar tudo em um comando só (assumindo que você já está no diretório ``checkpoint3``), faça:

```bash
docker build -t so-icmc/checkpoint3:latest . && \
docker rm -f checkpoint3 && docker run --name checkpoint3 so-icmc/checkpoint3:latest
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

2. Rode o ``Makefile`` e copie os binários:

```bash
make && cp ./build/main.o ./bin/main && \
make clean
```

3. Agora, o programa está disponível em ``./bin/main``. Execute-o usando o comando:

```bash
./bin/main --page_size 4096 --frame_size 4096 --logic_address_size 16 --num_frames 8 data/input.txt
```

Para ver informações sobre como usar o CLI, execute:

```bash
./bin/main -h
```

## Sobre a imagem Docker

A imagem Docker criada possui dois estágios:

- 1º estágio (build): utiliza a base ``gcc:14.2.0-bookworm`` para compilar os programas e mover os binários para a pasta /usr/app/bin
- 2º estágio (runtime): utiliza a base ``ubuntu:24.04`` para rodar o ``entrypoint.sh`` que irá executar o programa

Separamos o estágio de build e o de execução para melhorar o desempenho da imagem e otimizá-la, além de garantir que as ferramentas de build
estão sempre disponíveis (na imagem do GCC) sem ter que instalá-las no Ubuntu.