#include "atriz.hpp"

#include <exception>
#include <filesystem>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct Config {
    std::filesystem::path csv{"data/atrizes.csv"};
    std::filesystem::path binario{"atrizes.bin"};
    bool mostrarAjuda{false};
};

void imprimirAjuda(const char* programa) {
    std::cout << "Uso: " << programa << " [--csv caminho] [--bin caminho]\n\n"
              << "Opcoes:\n"
              << "  --csv caminho   Arquivo CSV inicial. Padrao: data/atrizes.csv\n"
              << "  --bin caminho   Arquivo binario para salvar/carregar. Padrao: atrizes.bin\n"
              << "  --help          Mostra esta ajuda\n";
}

Config lerConfig(int argc, char* argv[]) {
    Config config;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            config.mostrarAjuda = true;
            return config;
        }

        if (arg == "--csv") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--csv precisa de um caminho");
            }
            config.csv = argv[++i];
            continue;
        }

        if (arg == "--bin") {
            if (i + 1 >= argc) {
                throw std::runtime_error("--bin precisa de um caminho");
            }
            config.binario = argv[++i];
            continue;
        }

        throw std::runtime_error("Argumento invalido: " + arg);
    }

    return config;
}

void limparEntrada() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int lerInteiro(const std::string& mensagem) {
    int valor = 0;

    while (true) {
        std::cout << mensagem;
        if (std::cin >> valor) {
            limparEntrada();
            return valor;
        }

        if (std::cin.eof()) {
            throw std::runtime_error("Entrada encerrada");
        }

        std::cout << "Entrada invalida. Digite um numero.\n";
        limparEntrada();
    }
}

std::string lerTexto(const std::string& mensagem) {
    std::string valor;
    std::cout << mensagem;

    if (!std::getline(std::cin, valor)) {
        throw std::runtime_error("Entrada encerrada");
    }

    return valor;
}

void imprimirCabecalho() {
    std::cout << "  ID | Nome                         | Estado             | Idade | Filmes\n"
              << "-----+------------------------------+--------------------+-------+--------\n";
}

void imprimirMenu() {
    std::cout << "\nSistema de Gerenciamento\n"
              << "1. Listar todos os registros\n"
              << "2. Listar trecho de registros\n"
              << "3. Adicionar novo registro\n"
              << "4. Remover registro por identificador\n"
              << "5. Ordenar por nome\n"
              << "6. Ordenar por idade\n"
              << "7. Ordenar por identificador\n"
              << "8. Buscar por identificador\n"
              << "9. Buscar por nome\n"
              << "10. Salvar em arquivo binario\n"
              << "11. Salvar em arquivo CSV\n"
              << "12. Carregar de arquivo binario\n"
              << "13. Sair\n";
}

Atriz lerNovaAtriz() {
    Atriz atriz;
    atriz.identificador = lerInteiro("Identificador: ");
    atriz.nomeSobrenome = lerTexto("Nome e sobrenome: ");
    atriz.estadoOrigem = lerTexto("Estado de origem: ");
    atriz.idade = lerInteiro("Idade: ");
    atriz.quantidadeFilmes = lerInteiro("Quantidade de filmes: ");
    atriz.ativo = true;
    return atriz;
}

void mostrarRegistroEncontrado(const std::vector<Atriz>& atrizes, std::size_t indice) {
    std::cout << "Registro encontrado:\n";
    imprimirCabecalho();
    imprimirAtriz(std::cout, atrizes[indice]);
}

}  // namespace

int main(int argc, char* argv[]) {
    try {
        const Config config = lerConfig(argc, argv);
        if (config.mostrarAjuda) {
            imprimirAjuda(argv[0]);
            return 0;
        }

        std::vector<Atriz> atrizes = carregarCSV(config.csv);
        std::cout << "Registros carregados: " << atrizes.size() << "\n";

        while (true) {
            imprimirMenu();
            const int escolha = lerInteiro("Opcao: ");

            switch (escolha) {
                case 1:
                    imprimirCabecalho();
                    listarAtrizes(std::cout, atrizes);
                    break;

                case 2: {
                    const int inicio = lerInteiro("Indice inicial: ");
                    const int fim = lerInteiro("Indice final: ");

                    if (inicio < 0 || fim < 0) {
                        std::cout << "Os indices precisam ser positivos.\n";
                        break;
                    }

                    imprimirCabecalho();
                    listarTrecho(std::cout, atrizes, static_cast<std::size_t>(inicio), static_cast<std::size_t>(fim));
                    break;
                }

                case 3:
                    adicionarAtriz(atrizes, lerNovaAtriz());
                    std::cout << "Registro adicionado.\n";
                    break;

                case 4: {
                    const int identificador = lerInteiro("Identificador para remover: ");
                    if (removerAtrizPorId(atrizes, identificador)) {
                        std::cout << "Registro removido logicamente.\n";
                    } else {
                        std::cout << "Registro nao encontrado.\n";
                    }
                    break;
                }

                case 5:
                    ordenarAtrizes(atrizes, CampoOrdenacao::Nome);
                    std::cout << "Registros ordenados por nome.\n";
                    break;

                case 6:
                    ordenarAtrizes(atrizes, CampoOrdenacao::Idade);
                    std::cout << "Registros ordenados por idade.\n";
                    break;

                case 7:
                    ordenarAtrizes(atrizes, CampoOrdenacao::Identificador);
                    std::cout << "Registros ordenados por identificador.\n";
                    break;

                case 8: {
                    const int identificador = lerInteiro("Identificador para buscar: ");
                    const auto indice = buscarPorId(atrizes, identificador);
                    if (indice.has_value()) {
                        mostrarRegistroEncontrado(atrizes, *indice);
                    } else {
                        std::cout << "Registro nao encontrado.\n";
                    }
                    break;
                }

                case 9: {
                    const std::string nome = lerTexto("Nome para buscar: ");
                    const auto indice = buscarPorNome(atrizes, nome);
                    if (indice.has_value()) {
                        mostrarRegistroEncontrado(atrizes, *indice);
                    } else {
                        std::cout << "Registro nao encontrado.\n";
                    }
                    break;
                }

                case 10:
                    salvarBinario(config.binario, atrizes);
                    std::cout << "Arquivo binario salvo em: " << config.binario << "\n";
                    break;

                case 11:
                    salvarCSV(config.csv, atrizes);
                    std::cout << "CSV salvo em: " << config.csv << "\n";
                    break;

                case 12:
                    atrizes = carregarBinario(config.binario);
                    std::cout << "Registros carregados do binario: " << atrizes.size() << "\n";
                    break;

                case 13:
                    return 0;

                default:
                    std::cout << "Opcao invalida.\n";
                    break;
            }
        }
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << "\n";
        return 1;
    }
}
