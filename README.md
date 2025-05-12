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
- **Alternativas Consideradas:**
  - Considerou-se o uso de arrays dinâmicos para o diretório, mas a lista duplamente encadeada foi escolhida pela facilidade de remoção e movimentação de membros.
  - Para compressão, optou-se por uma implementação simples de LZ77, já que era requisito do trabalho.
- **Dificuldades e Soluções:**
  - Gerenciamento correto de ponteiros e memória ao manipular a lista duplamente encadeada, especialmente ao substituir ou remover membros.
  - Garantir que os offsets e tamanhos fossem atualizados corretamente após cada operação.
  - Evitar vazamentos de memória e erros de leitura/gravação, especialmente ao manipular arquivos binários.
  - Corrigir bugs relacionados à leitura e escrita de tamanhos incorretos, que causavam duplicação ou truncamento de dados extraídos.

## Bugs Conhecidos

- Em casos de falha de energia ou interrupção durante a escrita do arquivo temporário, o archive pode ser corrompido.
- Não há verificação de integridade dos dados comprimidos/descomprimidos.
- O programa não trata nomes de arquivos com mais de 1023 caracteres.
- O uso de arquivos temporários pode causar problemas se múltiplas instâncias do programa forem executadas simultaneamente no mesmo diretório.
- Mensagens de erro podem ser pouco detalhadas em alguns casos de falha de I/O.

