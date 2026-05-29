#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <regex>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

struct Valores {
    std::string nome;
    int ram;
};

bool comparar_ram(const Valores &v1, const Valores &v2) {
    return v1.ram > v2.ram;
}

int main() {
    std::string path = "/proc";
    std::vector<Valores> valores;

    for (const auto &entry: fs::directory_iterator(path)) {
        if (!entry.is_directory()) continue;

        fs::path caminho = entry.path() / "status";
        std::ifstream arquivo(caminho);

        if (arquivo.is_open()) {
            std::string linha;
            std::string nomeProcesso = "";
            int ramValor = -1;
            bool achouVmRSS = false;

            while (std::getline(arquivo, linha)) {
                if (linha.find("Name:") == 0) {
                    nomeProcesso = linha.substr(5);
                    nomeProcesso.erase(0, nomeProcesso.find_first_not_of(" \t"));
                }

                if (linha.find("VmRSS:") == 0) {
                    std::regex regexVmRSS("VmRSS:\\s*(\\d+)");
                    std::smatch resultado;

                    if (std::regex_search(linha, resultado, regexVmRSS)) {
                        ramValor = std::stoi(resultado[1].str());
                        achouVmRSS = true;
                    }
                }
            }

            if (achouVmRSS) {
                valores.push_back({nomeProcesso, ramValor});
            }
        }
    }

    std::sort(valores.begin(), valores.end(), comparar_ram);

    std::cout << "\n=== RANKING DE USO DE RAM ===\n";
    for (const auto &v: valores) {
        std::cout << "Processo: " << v.nome << " | RAM: " << (v.ram / 1024.0) << " MB" << std::endl;
    }

    return 0;
}
