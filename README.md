# steel-data-ingestion

## Programadores:

 - Lucas Neves Lima
 - Beatriz Inácio de Oliveira

## Descrição:

Software multithread responsável pela leitura de dados deste sistema de inspeção de defeitos, apresentando-os adequadamente em dois terminais de vídeo, sendo um na sala de controle da laminação e outro no centro de pesquisas. O primeiro destes terminais apresentará dados do processo de laminação, de interesse apenas para fins de controle e supervisão do processo, ao passo que o segundo terminal é dedicado para análise dos defeitos superficiais nas tiras de aço de forma a identificar suas causas e a execução de medidas para eliminar  os mesmos.

## Linguagem:

* C/C++

## Bibliotecas:

* API Win 32
* MessageGenerate(biblioteca criada por nós para gerar as mensagens do sistema)

## Tarefas:

1. Tarefa de leitura do sistema de inspeção de defeitos. Realiza a leitura das mensagens provenientes do
sistema de inspeção de defeitos e as deposita em uma tabela circular em memória.

2. Tarefa de captura de defeitos das tiras. Retira, da lista circular em memória, as mensagens que defeitos
superficiais das tiras e as repassa diretamente para a tarefa de exibição de defeitos.

3. Tarefa de captura de dados de processo. Retira, da lista circular em memória, as mensagens de dados
referentes ao processo de laminação e as escreve em um arquivo circular em disco.

4. Tarefa de exibição de defeitos de tiras. Esta tarefa recebe mensagens de defeitos superficiais nas tiras de
aço da tarefa de captura de defeitos das tiras e as exibe no terminal de vídeo do centro de pesquisas.

5. Tarefa de exibição de dados de processo. Retira, do arquivo circular em disco, mensagens de dados
referentes ao processo de laminação e as exibe no terminal de vídeo na sala de controle da laminação.

6. Tarefa de leitura do teclado. Esta tarefa dá tratamento aos comandos digitados pelo operador. 

## Threads:

 A aplicação(processo) principal conta com 4 threads. 

 *main: A thread principal que incia as outra e faz a leitura do teclado
 para os eventos de interrupção.

 *h_Thread1: A thread que produz os dados, ela chama a função que gera as mensagens e as insere na fila com período 1000ms.
 
 *h_Thread2: A thread 2 é a thread responsavel por consumir mensagens do tipo 11.

 *h_Thread3: A thread 3 é responsael por consumir mensagens do tipo 22.

## Processos:

Temos 3 processos que executam na nossa solução, o principal que inicia as threads criadas e funcionais até agora.
e 2 processos secundários para a futura inplementação do display de informações.

## Arquiteturas:

A arquitetura escolhida é a de uma solução com 3 projetos que contém arquivos de código escritos de forma sequêncial.

[Thread principal] -----> { [Thread Leitura de erro], [Thread Leitura de mensagens]  }

## Console:

O console imprime as mensagens de consumo e depósito de mensagens, além de um contador que indica quantas posições estao livres na lista.

## Entradas do teclado:

- "1" : pausa consumo das mensagens do tipo 11. Mandando o comando "1" novamente, o consumo de mensagens do tipo 11 é retomado.
- "2" : pausa consumo das mensagens do tipo 22. Mandando o comando "2" novamente, o consumo de mensagens do tipo 22 é retomado.

## WIP
 
