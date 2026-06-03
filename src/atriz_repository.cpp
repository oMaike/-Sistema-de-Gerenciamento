#include "atriz.hpp"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace {

std::string trim(std::string valor) {
    const auto inicio = valor.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) {
        return "";
    }

    const auto fim = valor.find_last_not_of(" \t\r\n");
    return valor.substr(inicio, fim - inicio + 1);
}

std::vector<std::string> dividirCSV(const std::string& linha) {
    std::vector<std::string> campos;
    std::string campo;
    bool dentroDeAspas = false;

    for (std::size_t i = 0; i < linha.size(); ++i) {
        const char caractere = linha[i];

        if (caractere == '"') {
            if (dentroDeAspas && i + 1 < linha.size() && linha[i + 1] == '"') {
                campo += '"';
                ++i;
            } else {
                dentroDeAspas = !dentroDeAspas;
            }
            continue;
        }

        if (caractere == ',' && !dentroDeAspas) {
            campos.push_back(trim(campo));
            campo.clear();
            continue;
        }

        campo += caractere;
    }

    if (dentroDeAspas) {
        throw std::runtime_error("Linha CSV possui aspas sem fechamento");
    }

    campos.push_back(trim(campo));
    return campos;
}

int converterInteiro(const std::string& valor, const std::string& campo, int linha) {
    try {
        std::size_t consumidos = 0;
        const int numero = std::stoi(valor, &consumidos);

        if (consumidos != valor.size()) {
            throw std::invalid_argument("valor parcialmente numerico");
        }

        return numero;
    } catch (const std::exception&) {
        throw std::runtime_error("Campo '" + campo + "' invalido na linha " + std::to_string(linha) + ": " + valor);
    }
}

void escreverString(std::ofstream& arquivo, const std::string& valor) {
    if (valor.size() > std::numeric_limits<std::uint32_t>::max()) {
        throw std::runtime_error("String grande demais para gravacao binaria");
    }

    const auto tamanho = static_cast<std::uint32_t>(valor.size());
    arquivo.write(reinterpret_cast<const char*>(&tamanho), sizeof(tamanho));
    arquivo.write(valor.data(), static_cast<std::streamsize>(valor.size()));
}

std::string lerString(std::ifstream& arquivo) {
    std::uint32_t tamanho = 0;
    arquivo.read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));

    if (arquivo.fail()) {
        throw std::runtime_error("Falha ao ler tamanho da string no arquivo binario");
    }

    std::string valor(tamanho, '\0');
    if (tamanho > 0) {
        arquivo.read(valor.data(), static_cast<std::streamsize>(tamanho));
    }

    if (arquivo.fail()) {
        throw std::runtime_error("Falha ao ler string no arquivo binario");
    }

    return valor;
}

std::string escaparCSV(const std::string& valor) {
    if (valor.find_first_of(",\"\n\r") == std::string::npos) {
        return valor;
    }

    std::string escapado = "\"";
    for (const char caractere : valor) {
        if (caractere == '"') {
            escapado += "\"\"";
        } else {
            escapado += caractere;
        }
    }
    escapado += '"';
    return escapado;
}

bool pareceCabecalho(const std::vector<std::string>& campos) {
    return !campos.empty() && campos[0] == "identificador";
}

}  // namespace

std::vector<Atriz> carregarCSV(const std::filesystem::path& caminho) {
    std::ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o CSV: " + caminho.string());
    }

    std::vector<Atriz> atrizes;
    std::string linha;
    int numeroLinha = 0;

    while (std::getline(arquivo, linha)) {
        ++numeroLinha;
        linha = trim(linha);

        if (linha.empty() || linha[0] == '#') {
            continue;
        }

        const auto campos = dividirCSV(linha);
        if (numeroLinha == 1 && pareceCabecalho(campos)) {
            continue;
        }

        if (campos.size() != 5) {
            throw std::runtime_error("Linha " + std::to_string(numeroLinha) + " deve ter 5 campos CSV");
        }

        Atriz atriz;
        atriz.identificador = converterInteiro(campos[0], "identificador", numeroLinha);
        atriz.nomeSobrenome = campos[1];
        atriz.estadoOrigem = campos[2];
        atriz.idade = converterInteiro(campos[3], "idade", numeroLinha);
        atriz.quantidadeFilmes = converterInteiro(campos[4], "quantidadeFilmes", numeroLinha);
        atriz.ativo = true;

        atrizes.push_back(atriz);
    }

    return atrizes;
}

void salvarCSV(const std::filesystem::path& caminho, const std::vector<Atriz>& atrizes) {
    std::ofstream arquivo(caminho);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel gravar o CSV: " + caminho.string());
    }

    for (const auto& atriz : atrizes) {
        if (!atriz.ativo) {
            continue;
        }

        arquivo << atriz.identificador << ','
                << escaparCSV(atriz.nomeSobrenome) << ','
                << escaparCSV(atriz.estadoOrigem) << ','
                << atriz.idade << ','
                << atriz.quantidadeFilmes << '\n';
    }
}

void salvarBinario(const std::filesystem::path& caminho, const std::vector<Atriz>& atrizes) {
    std::ofstream arquivo(caminho, std::ios::binary);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel gravar o binario: " + caminho.string());
    }

    for (const auto& atriz : atrizes) {
        arquivo.write(reinterpret_cast<const char*>(&atriz.identificador), sizeof(atriz.identificador));
        escreverString(arquivo, atriz.nomeSobrenome);
        escreverString(arquivo, atriz.estadoOrigem);
        arquivo.write(reinterpret_cast<const char*>(&atriz.idade), sizeof(atriz.idade));
        arquivo.write(reinterpret_cast<const char*>(&atriz.quantidadeFilmes), sizeof(atriz.quantidadeFilmes));
        arquivo.write(reinterpret_cast<const char*>(&atriz.ativo), sizeof(atriz.ativo));
    }
}

std::vector<Atriz> carregarBinario(const std::filesystem::path& caminho) {
    std::ifstream arquivo(caminho, std::ios::binary);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Nao foi possivel abrir o binario: " + caminho.string());
    }

    std::vector<Atriz> atrizes;

    while (true) {
        Atriz atriz;
        arquivo.read(reinterpret_cast<char*>(&atriz.identificador), sizeof(atriz.identificador));

        if (arquivo.eof()) {
            break;
        }

        if (arquivo.fail()) {
            throw std::runtime_error("Falha ao ler identificador no arquivo binario");
        }

        atriz.nomeSobrenome = lerString(arquivo);
        atriz.estadoOrigem = lerString(arquivo);
        arquivo.read(reinterpret_cast<char*>(&atriz.idade), sizeof(atriz.idade));
        arquivo.read(reinterpret_cast<char*>(&atriz.quantidadeFilmes), sizeof(atriz.quantidadeFilmes));
        arquivo.read(reinterpret_cast<char*>(&atriz.ativo), sizeof(atriz.ativo));

        if (arquivo.fail()) {
            throw std::runtime_error("Arquivo binario corrompido ou incompleto: " + caminho.string());
        }

        atrizes.push_back(atriz);
    }

    return atrizes;
}

void adicionarAtriz(std::vector<Atriz>& atrizes, const Atriz& novaAtriz) {
    if (buscarPorId(atrizes, novaAtriz.identificador).has_value()) {
        throw std::runtime_error("Ja existe um registro ativo com esse identificador");
    }

    atrizes.push_back(novaAtriz);
}

bool removerAtrizPorId(std::vector<Atriz>& atrizes, int identificador) {
    for (auto& atriz : atrizes) {
        if (atriz.identificador == identificador && atriz.ativo) {
            atriz.ativo = false;
            return true;
        }
    }

    return false;
}

void ordenarAtrizes(std::vector<Atriz>& atrizes, CampoOrdenacao campo) {
    std::sort(atrizes.begin(), atrizes.end(), [campo](const Atriz& esquerda, const Atriz& direita) {
        if (campo == CampoOrdenacao::Nome) {
            return esquerda.nomeSobrenome < direita.nomeSobrenome;
        }

        if (campo == CampoOrdenacao::Idade) {
            return esquerda.idade < direita.idade;
        }

        return esquerda.identificador < direita.identificador;
    });
}

std::optional<std::size_t> buscarPorId(const std::vector<Atriz>& atrizes, int identificador) {
    for (std::size_t indice = 0; indice < atrizes.size(); ++indice) {
        if (atrizes[indice].identificador == identificador && atrizes[indice].ativo) {
            return indice;
        }
    }

    return std::nullopt;
}

std::optional<std::size_t> buscarPorNome(const std::vector<Atriz>& atrizes, const std::string& nome) {
    for (std::size_t indice = 0; indice < atrizes.size(); ++indice) {
        if (atrizes[indice].nomeSobrenome == nome && atrizes[indice].ativo) {
            return indice;
        }
    }

    return std::nullopt;
}

void imprimirAtriz(std::ostream& saida, const Atriz& atriz) {
    saida << std::right << std::setw(4) << atriz.identificador << " | "
          << std::left << std::setw(28) << atriz.nomeSobrenome << " | "
          << std::setw(18) << atriz.estadoOrigem << " | "
          << std::right << std::setw(5) << atriz.idade << " | "
          << std::setw(6) << atriz.quantidadeFilmes << '\n';
}

void listarAtrizes(std::ostream& saida, const std::vector<Atriz>& atrizes) {
    for (const auto& atriz : atrizes) {
        if (atriz.ativo) {
            imprimirAtriz(saida, atriz);
        }
    }
}

void listarTrecho(std::ostream& saida, const std::vector<Atriz>& atrizes, std::size_t inicio, std::size_t fim) {
    if (atrizes.empty() || inicio >= atrizes.size() || inicio > fim) {
        return;
    }

    fim = std::min(fim, atrizes.size() - 1);

    for (std::size_t indice = inicio; indice <= fim; ++indice) {
        if (atrizes[indice].ativo) {
            imprimirAtriz(saida, atrizes[indice]);
        }
    }
}
