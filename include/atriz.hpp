#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

struct Atriz {
    int identificador{};
    std::string nomeSobrenome;
    std::string estadoOrigem;
    int idade{};
    int quantidadeFilmes{};
    bool ativo{true};
};

enum class CampoOrdenacao {
    Identificador,
    Nome,
    Idade,
};

std::vector<Atriz> carregarCSV(const std::filesystem::path& caminho);
void salvarCSV(const std::filesystem::path& caminho, const std::vector<Atriz>& atrizes);
void salvarBinario(const std::filesystem::path& caminho, const std::vector<Atriz>& atrizes);
std::vector<Atriz> carregarBinario(const std::filesystem::path& caminho);

void adicionarAtriz(std::vector<Atriz>& atrizes, const Atriz& novaAtriz);
bool removerAtrizPorId(std::vector<Atriz>& atrizes, int identificador);
void ordenarAtrizes(std::vector<Atriz>& atrizes, CampoOrdenacao campo);

std::optional<std::size_t> buscarPorId(const std::vector<Atriz>& atrizes, int identificador);
std::optional<std::size_t> buscarPorNome(const std::vector<Atriz>& atrizes, const std::string& nome);

void imprimirAtriz(std::ostream& saida, const Atriz& atriz);
void listarAtrizes(std::ostream& saida, const std::vector<Atriz>& atrizes);
void listarTrecho(std::ostream& saida, const std::vector<Atriz>& atrizes, std::size_t inicio, std::size_t fim);
