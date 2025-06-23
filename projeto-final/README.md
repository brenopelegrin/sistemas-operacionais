# Simulador Web Interativo – Projeto Final de Sistemas Operacionais 1

Esse repositório reúne a aplicação web que desenvolvemos como projeto final da disciplina *Sistemas Operacionais 1 (SSC0541)*, oferecida pelo ICMC-USP em 2025.

A ideia foi pegar os três checkpoints que fizemos ao longo do semestre — sobre chamadas de sistema, Produtor-Consumidor e gerência de memória com paginação — e integrá-los em um único sistema interativo acessível via navegador. O objetivo principal era tornar o conteúdo mais visual e acessível, sem depender do terminal ou de compilar manualmente os programas.


| Parte    | Diretório      | Documentação                         |
|---------------|----------------|--------------------------------------|
| Backend  | backend/   | [README.md](backend/README.md)   |
| Frontend | frontend/ | [README.md](frontend/README.md) |

## Instruções para rodar

É necessário ter docker compose com versão maior ou igual a 2.36.2.
É necessário ter docker com versão maior ou igual a 28.2.2.

Para rodar, execute os comandos, estando no diretório `projeto-final`:

```bash
docker compose down && \
docker compose build --no-cache && \
docker compose up -d 
```

## Como funciona

A aplicação é dividida em três partes principais:

- *Frontend* (React + Vite): responsável pela interface web. O usuário faz login, escolhe qual módulo quer simular (CP1, CP2 ou CP3), executa a tarefa e visualiza os resultados na tela.
- *Backend* (Flask + Celery): recebe as requisições da interface, autentica o usuário e encaminha a tarefa para a fila de execução.
- *Handler* (Celery Worker): consome as tarefas da fila, executa os binários dos checkpoints e salva o resultado no banco.

Cada módulo da disciplina foi empacotado separadamente como um taskpack (com Dockerfile próprio) e acoplado ao handler usando additional_contexts no docker-compose. As tarefas são processadas de forma assíncrona, e os resultados são exibidos na interface assim que ficam prontos.

### Módulos disponíveis

- *Checkpoint 1: simula chamadas de sistema, incluindo criação de processos e operações de entrada/saída. Compara o comportamento de processos **CPU-bound* (uso intensivo de processador) e *I/O-bound* (dependentes de entrada/saída).
- *Checkpoint 2: simula o problema clássico do **Produtor-Consumidor*, com controle de concorrência usando semáforos e threads.
- *Checkpoint 3: implementa um simulador de **gerência de memória com paginação*, permitindo escolher algoritmos de substituição (FIFO, LRU).

## Tecnologias usadas

- *React* e *Vite* no frontend
- *Flask, **Celery* e *RabbitMQ* no backend
- *PostgreSQL* para armazenar os dados das tarefas
- *Docker* e *Docker Compose* para orquestrar todos os serviços
- *JWT* para autenticação

## Sobre a autenticação

O login é feito via POST /token passando user e password. Se o usuário for válido, a API retorna um token JWT que precisa ser usado nas próximas requisições (como no envio de uma tarefa). Os endpoints protegidos rejeitam chamadas sem token válido.

## Por que fizemos isso

Ao longo do semestre a gente percebeu que vários conceitos de SO, por mais importantes que fossem, acabavam ficando presos no terminal. Compilar código C e analisar saída de strace não é algo tão acessível. Criando esse simulador com interface web, conseguimos deixar tudo mais visual, modular e fácil de testar — tanto pra gente quanto pra quem quiser usar depois.