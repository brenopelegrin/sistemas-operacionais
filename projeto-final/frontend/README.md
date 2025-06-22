# Frontend

Este projeto é uma aplicação React que permite aos usuários interagir com diferentes "checkpoints" (1, 2 e 3).

## Visão Geral

A aplicação apresenta uma interface com cards para cada checkpoint.

## Arquitetura

A arquitetura do projeto é baseada em componentes React. Os principais componentes são:

*   `src/routes/Models/index.tsx`: Este componente renderiza a página principal com os cards para cada checkpoint.
*   `src/components/TaskFetch/index.tsx`: Este componente contém funções para realizar requisições POST para a API de backend, utilizando a biblioteca `axios`. As requisições são feitas para o endpoint `/task` com um payload contendo o tipo da tarefa (`soicmc.main.run_checkpoint1`, `soicmc.main.run_checkpoint2`, `soicmc.main.run_checkpoint3`) e argumentos específicos.

## Como Executar

1.  Certifique-se de ter o Node.js e o yarn instalados.
2.  Execute `yarn install` para instalar as dependências.
3.  Execute `yarn dev` para iniciar o servidor de desenvolvimento com Vite.
4.  Acesse a aplicação no seu navegador através do endereço `http://localhost:5173`.

Este projeto utiliza as seguintes tecnologias:

*   React
*   Vite
*   Yarn

## Endpoints da API

As seguintes requisições POST são feitas para a API de backend:

*   `soicmc.main.run_checkpoint1`: Executa a tarefa relacionada ao checkpoint 1.
*   `soicmc.main.run_checkpoint2`: Executa a tarefa relacionada ao checkpoint 2.
*   `soicmc.main.run_checkpoint3`: Executa a tarefa relacionada ao checkpoint 3.

Um endpoint GET também está disponível:

*   `/task/{id}`: Busca informações sobre uma tarefa específica, utilizando o ID da tarefa.

## Observações

*   Os checkpoints 2 e 3 podem não ter funcionalidades implementadas no momento.
*   A API de backend deve estar em execução no endereço `http://localhost:8080` para que as requisições funcionem corretamente.
