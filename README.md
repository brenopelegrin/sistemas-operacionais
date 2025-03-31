[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XlMfsDo5)

# Projeto - Sistemas Operacionais 1

Na tabela abaixo, são expostos os entregáveis e o link para documentação de cada um. Na documentação de cada entregável,
existem instruções de como rodar o programa, bem como informações sobre a arquitetura, construção e considerações de cada entregável.

| Entregável    | Diretório      | Documentação                         |
|---------------|----------------|--------------------------------------|
| Checkpoint 1  | checkpoint1/   | [README.md](checkpoint1/README.md)   |
| Checkpoint 2  | checkpoint2/   | [README.md](checkpoint2/README.md)   |
| Checkpoint 3  | checkpoint3/   | [README.md](checkpoint3/README.md)   |
| Projeto final | projeto-final/ | [README.md](projeto-final/README.md) |

A documentação sobre o login na VM, escrita pelos professores da disciplina, pode ser encontrada no arquivo [INSTRUCTIONS.md](INSTRUCTIONS.md).

## Padrões gerais

Todos os entregáveis foram criados utilizando ``Docker`` para garantir a reproducibilidade e portabilidade do código.

O versionamento no Git foi feito utilizando o workflow conhecido como **Git Flow**.

A compilação dos códigos em ``C`` foi feita utilizando ``make``.


### Padrões de versionamento
Sobre as branches do Git, o seguinte padrão é seguido:
- branch ``main``: a branch mais estável, onde está o código de "produção", o qual será feito o deploy na VM.
- branch ``develop``: a branch utilizada para centralizar o desenvolvimento, pode conter código instável.
- outras branches: branches de feature, fix, enhancements, utilizadas temporariamente e depois 'mergeadas' na develop

Padrão de commits:
- os commits devem conter poucas mudanças (cerca de 3 arquivos no máximo), que reflita a mensagem do commit
- a mensagem do commit deve ser curta e escrita em inglês para facilitar a leitura do projeto
- as mensagens dos commits devem seguir o padrão ``TAG: short description of the changes``, onde a TAG pode ser ADD, DEL, FIX, ENH, que significam respectivamente: adição, deleção, conserto, melhoria.
- os commits devem ser feitos apenas na branch ``develop``. Commits diretamente na ``main`` não são recomendados pois afetam a estabilidade do deploy.

## Deploy na VM

Dentro da VM, foi criado um script ``~/deploy.sh`` na pasta home. Esse script irá baixar o repositório e mudar para a branch
``main``, utilizando uma chave SSH pré-configurada na máquina.

Para rodá-lo, basta logar na VM:

```bash
ssh gsograd06@andromeda.lasdpc.icmc.usp.br -p 2176
```

E então, rodar o script:

```bash
bash deploy.sh
```

O repositório do projeto estará disponível então no diretório ``~/projeto-ssc0541-grupo-06``:

```bash
cd ~/projeto-ssc0541-grupo-06
```

Para instruções de como rodar cada entregável, por favor, siga a documentação (arquivo ``README.md``) dentro dos respectivos diretórios.
