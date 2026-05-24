# Simulação em C++ — Gisele Bündchen VS As Forças do Mal

Este diretório (`cpp/`) contém uma reescrita da lógica do jogo original em Python.
A lógica do jogo é simulada via linha de comando, com uma execução fixa como exemplo.

## O que foi reescrito

- Personagem (`Gisele`) com física simples de pulo/gravidade (sem sprites).
- Coletáveis (`Banana`, `Camera`, `Rosa`) com polimorfismo e efeitos.
- Contadores e condições de vitória/derrota.
- “Aceleração” periódica (equivalente ao evento `ACELERAR` do Python) e trecho final de chegada.

## Como compilar e executar

Requisitos:

- CMake >= 3.16
- Compilador com suporte a C++

Exemplo (Linux):

```bash
cd cpp
cmake -S . -B build
cmake --build build -j
./build/gisele_sim
```

## Estrutura

- `include/game/*`: headers
- `src/*`: implementação
- `src/main.cpp`: simulação fixa que demonstra a lógica do jogo

## Notas

- Não há interação por teclado. A simulação chama `pular()` em ticks pré-definidos só pra demonstrar a dinâmica do jogo.
