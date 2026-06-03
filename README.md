# Sistema de Gerenciamento

Sistema de gerenciamento de atrizes em C++ desenvolvido para um trabalho academico.

O programa carrega registros de um arquivo CSV, permite consultar, listar, ordenar, adicionar e remover registros logicamente, e pode salvar os dados em CSV ou arquivo binario.

## Estrutura

```text
.
|-- CMakeLists.txt
|-- data/
|   `-- atrizes.csv
|-- include/
|   `-- atriz.hpp
`-- src/
    |-- atriz_repository.cpp
    `-- main.cpp
```

## Requisitos

- Compilador C++ com suporte a C++17
- CMake 3.16 ou superior

## Build no Linux

```bash
cmake -S . -B build
cmake --build build
./build/sistema-gerenciamento
```

## Build no Windows PowerShell

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\sistema-gerenciamento.exe
```

Dependendo do gerador usado pelo CMake, o executavel tambem pode ficar em:

```powershell
.\build\sistema-gerenciamento.exe
```

## Uso

Por padrao, o programa usa `data/atrizes.csv` como base inicial e `atrizes.bin` como arquivo binario.

```bash
./build/sistema-gerenciamento --csv data/atrizes.csv --bin atrizes.bin
```

No PowerShell:

```powershell
.\build\Release\sistema-gerenciamento.exe --csv data\atrizes.csv --bin atrizes.bin
```

Opcoes:

- `--csv caminho`: define o arquivo CSV inicial.
- `--bin caminho`: define o arquivo binario usado para salvar/carregar.
- `--help`: mostra a ajuda.

## Funcionalidades

- Listar todos os registros ativos.
- Listar trecho por indice.
- Adicionar novo registro.
- Remover registro por identificador com remocao logica.
- Ordenar por nome, idade ou identificador.
- Buscar por identificador.
- Buscar por nome.
- Salvar registros ativos em CSV.
- Salvar e carregar registros em arquivo binario.

## Melhorias da reestruturacao

- Codigo separado por responsabilidade.
- Uso de `std::vector` no lugar de arrays dinamicos manuais.
- Uso de `std::sort` no lugar de quicksort manual duplicado.
- Tratamento de erros com excecoes e mensagens claras.
- Validacao basica de entrada do usuario.
- Leitura de argumentos de linha de comando.
- Arquivos de build ignorados pelo Git.
- Base de dados movida para `data/`.
