Funcionalidades (Mínimo Viável)

    Iteração pelo sistema de arquivos /proc.

    Leitura de metadados do arquivo status de cada processo.

    Extração de valores de memória usando Regex.

    Armazenamento em std::vector utilizando structs personalizadas.

🛠️ Tecnologias

    Linguagem: C++17 (ou superior).

    Bibliotecas Principais: <filesystem>, <regex>, <fstream>.

📂 Como Compilar e Executar

    Certifique-se de estar no Linux (o /proc é exclusivo de sistemas Unix-like).

    Compile via terminal:
    Bash

    g++ -std=c++17 main.cpp -o ProcHunter

    Execute:
    Bash

    ./ProcHunter
