#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
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
    std::string pathMemo = "/proc/meminfo";
    int ramTotal;
    int ramFree;

std::ifstream arquivoRam("/proc/meminfo");
std::string linha;
if(arquivoRam.is_open()){
    if(std::getline(arquivoRam, linha)){
        std::string total;
        size_t inicio = std::string::npos;
        size_t fim = std::string::npos;
        total = linha.substr(9);
        std::cout << total << std::endl;

        for (size_t i = 0; i < total.length(); i++) {
        if(std::isdigit(total[i])){
            inicio = i;
            break;
            }
        }
        for (size_t y = 0; y < total.length(); y++) {
            if(!std::isdigit(total[y])){
              fim = y;
              break;
            }
        }
        std::string valor = (total.substr(inicio, fim - inicio));
        int valorRam = std::atoi(total.c_str());
        std::cout <<"total!:" << valor << std::endl;
        std::cout << "VALOR RAM:" << valorRam << std::endl;
        ramTotal = valorRam;
    }
    std::string total2;

    if(linha.find("MemFree:")){
        total2 = linha.substr(9);
        // std::cout << total2 << std::endl;
        size_t primerioEspaco = total2.find_first_not_of(' ');
        size_t UltimoEspaco = total2.find_last_not_of(' ');
        total2.erase(0, primerioEspaco);
        std::cout << "--------"<< total2 << std::endl;
    }  
}


    for (const auto &entry: fs::directory_iterator(path)) {
        if (!entry.is_directory()) continue;



        fs::path caminho = entry.path() / "status";
        
        // fs::path caminhoMemo = entry.path() / "meminfo";
        // std::ifstream arquivoMemo(caminhoMemo);
        
        // if(arquivoMemo.is_open()){
            
            // std::string linha;

            // while(std::getline(arquivoMemo, linha)){

                // if(linha.find("MemTotal:") == 0){
                     // std::string valor = linha.substr(9);
                    // valor.erase(0, valor.find_first_not_of(" \t"));
                    // ramTotal = valor;
                // }
            // }
        // }
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
    std::cout << "RamTotal:" << ramTotal << std::endl;
    return 0;
}
