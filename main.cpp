#include <cctype>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <fstream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <set>
#include <string>
#include <filesystem>
#include <regex>
#include <thread>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

struct Valores {
    std::string nome;
    double ram;
};

bool comparar_ram(const Valores &v1, const Valores &v2) {
    return v1.ram > v2.ram;
}

int main() {
    while(true){
    
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
        // std::cout <<"total!:" << valor << std::endl;
        // std::cout << "VALOR RAM:" << valorRam << std::endl;
        ramTotal = valorRam;
    }
}
    std::ifstream arquivoRam2("/proc/meminfo");
    if(arquivoRam2.is_open()){
        
    std::string alvo = "MemFree:";
    std::string linha2;

    while (std::getline(arquivoRam, linha2)) {
    if(linha2.find(alvo) != std::string::npos){
        std::string total = linha2;
        std::cout << "VALOR 2" << total << std::endl;
        size_t inicio = std::string::npos;
        size_t fim = std::string::npos;

        total = linha2.substr(9);

        for(size_t i = 0; i < total.length(); i++){
            if(std::isdigit(total[i])){
                inicio = i;
                break;
            }
            }
            for(size_t y = 0; y<total.length(); y++){
                if(!std::isdigit(total[y])){
                    fim = y;
                 break;
                }
            }
                std::string valor2 = (total.substr(inicio, fim - inicio));
                std::cout << "---------" << valor2 << std::endl;
                ramFree = std::atoi(valor2.c_str());
                }
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
                valores.push_back({nomeProcesso, (ramValor / 1024.0)});
            }
        }
    }

    std::sort(valores.begin(), valores.end(), comparar_ram);

    std::cout << "\n=== RANKING DE USO DE RAM ===\n";
    for (const auto &v: valores) {
        std::cout << "               " << std::endl;
        std::cout << "Processo: " << v.nome  << std::endl;
        std::cout << "Ram: " << std::fixed << std::setprecision(2) << v.ram << std::endl;
        std::cout << "               " << std::endl;
    }

    int usado = ramTotal - ramFree;
    std::cout << "USADO :"<<std::fixed << std::setprecision(2) << usado << ((double)usado / ramTotal) * 100 << "%" << std::endl;
    std::cout << "RAM LIVRE: " << ramFree << std::endl;
    std::cout << "TOTAL DE RAM: " << ramTotal << std::endl;
     std::this_thread::sleep_for(std::chrono::seconds(5));
    std::system("clear");
    // clearTerminal();
    }
}
