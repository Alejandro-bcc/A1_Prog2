# A1 Prog2 - Vinac

## Autoria
- GRR: 20242778
- Nome: Alejandro David Nava Nava

## Arquivos e Diretórios

- `vina.c` — Arquivo principal do programa, responsável pela passagem dos argumentos e chamada das operações.
- `archive.c` / `archive.h` — Implementação e interface das operações do archive, incluindo inserção, remoção, extração, movimentação e listagem de membros.
- `diretorio.c` / `diretorio.h` — Implementação e interface das estruturas de diretório e membro, além das funções de manipulação da lista duplamente encadeada de membros.
- `manipulador_arquivos.c` / `manipulador_arquivos.h` — Funções utilitárias para manipulação de arquivos, buffers e compressão/descompressão.
- `lz/` — Diretório contendo a implementação do algoritmo de compressão LZ77 (`lz.c`, `lz.h`).
- `makefile` — Script para compilação do projeto.
- `README.md` — Este arquivo.

## Algoritmos e Estruturas de Dados

- **Estruturas de Dados:**
  - Lista duplamente encadeada para o diretório de membros (`struct diretorio` e `struct membro`), permitindo inserção, remoção e movimentação eficiente dos membros.
  - Cada membro armazena metadados em uma struct (`struct membro_disco`), incluindo nome, UID, tamanhos, datas de modifição e offsets.
- **Algoritmos:**
  - **Inserção:** O membro é inserido ao final da lista (ou substitui um existente), e os offsets são recalculados. O arquivo archive é reescrito usando um arquivo temporário.
  - **Remoção:** O membro é removido da lista e o arquivo archive é reescrito sem o conteúdo do membro removido.
  - **Movimentação:** O membro é removido de sua posição e inserido após o membro target, com atualização das ordens e offsets.
  - **Extração:** O conteúdo do membro é lido do archive e escrito em disco, descomprimindo se necessário.
  - **Compressão:** Utiliza o algoritmo LZ77.
- **Alternativas de implemetação:**
    - Foi considerado realizar deslocamentos de metadados e conteúdos dos membros no archive original ao realizar a inserção, remoção e movimentação dos mesmos, porém, optou-se uso de arquivos temporários para implementar estas operações e não perder os metadados e conteúdo originais.
- **Dificuldades e Soluções:**
  - Garantir que os offsets e tamanhos fossem atualizados corretamente após cada operação. Isto foi garantido adicionando um atributo à estrutura do membro_disco para salvar o offset anterior à operação atual, permitindo recuperar os conteúdos do archive original.
  - Corrigir bugs relacionados à leitura e escrita de tamanhos incorretos, que causavam duplicação ou truncamento de dados extraídos. Para solucionar isto, decidiu-se adicinar um outro atributo ao membro_disc chamado tam_disc, que armezena o tamanho em disco real (independentemente de se o membro está comprimido ou não).

## Bugs Conhecidos

- Membros que são comprimidos ficam corrompidos ao ser extraidos.
- Membros inseridos a partir de um caminho (absoluto ou relativo) ficam com o nome do caminho e ao serem extraidos isso ocorre nesse caminho.
