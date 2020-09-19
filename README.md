# steel-data-ingestion

## Descrição:

Software multithread responsável pela leitura de dados deste sistema de inspeção de defeitos, apresentando-os adequadamente em dois terminais de vídeo, sendo um na sala de controle da laminação e outro no centro de pesquisas. O primeiro destes terminais apresentará dados do processo de laminação, de interesse apenas para fins de controle e supervisão do processo, ao passo que o segundo terminal é dedicado para análise dos defeitos superficiais nas tiras de aço de forma a identificar suas causas e a execução de medidas para eliminar  os mesmos.

## Linguagem:
* C/C++

## Bibliotecas:
* API Win 32

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
 
