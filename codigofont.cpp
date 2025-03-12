#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Atriz {
    int identificador;
    string nomeSobrenome;
    string estadoOrigem;
    int idade;
    int quantidadeFilmes;
    bool ativo; // Para remoção lógica
};

// Funções auxiliares para manipulação de strings
string lerString(ifstream& arquivo) {
    size_t tamanho;
    arquivo.read(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));
    string resultado(tamanho, '\0');
    arquivo.read(&resultado[0], tamanho);
    return resultado;
}

void escreverString(ofstream& arquivo, const string& str) {
    size_t tamanho = str.size();
    arquivo.write(reinterpret_cast<const char*>(&tamanho), sizeof(tamanho));
    arquivo.write(str.c_str(), tamanho);
}

Atriz* lerCSV(int& tamanho) {
    ifstream arquivo("atrizes.csv");
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo CSV!" << endl;
        return nullptr;
    }

    string linha;
    int capacidade = 40;
    Atriz* atrizes = new Atriz[capacidade];
    tamanho = 0;

    while (getline(arquivo, linha)) {
        if (tamanho >= capacidade) {
            // Redimensionar array
            capacidade += 10;
            Atriz* novoArray = new Atriz[capacidade];
            for (int i = 0; i < tamanho; ++i) {
                novoArray[i] = atrizes[i];
            }
            delete[] atrizes;
            atrizes = novoArray;
        }

        stringstream ss(linha);
        Atriz atriz;
        string idadeStr, filmesStr;

        getline(ss, idadeStr, ',');
        atriz.identificador = stoi(idadeStr);

        getline(ss, atriz.nomeSobrenome, ',');
        getline(ss, atriz.estadoOrigem, ',');
        getline(ss, idadeStr, ',');
        atriz.idade = stoi(idadeStr);
        getline(ss, filmesStr, ',');
        atriz.quantidadeFilmes = stoi(filmesStr);

        atriz.ativo = true;
        atrizes[tamanho++] = atriz;
    }

    arquivo.close();
    return atrizes;
}

void salvarComoBinario(const Atriz* atrizes, int tamanho) {
    ofstream arquivo("atrizes.bin", ios::binary);
    for (int i = 0; i < tamanho; ++i) {
        const auto& atriz = atrizes[i];
        arquivo.write(reinterpret_cast<const char*>(&atriz.identificador), sizeof(atriz.identificador));
        escreverString(arquivo, atriz.nomeSobrenome);
        escreverString(arquivo, atriz.estadoOrigem);
        arquivo.write(reinterpret_cast<const char*>(&atriz.idade), sizeof(atriz.idade));
        arquivo.write(reinterpret_cast<const char*>(&atriz.quantidadeFilmes), sizeof(atriz.quantidadeFilmes));
        arquivo.write(reinterpret_cast<const char*>(&atriz.ativo), sizeof(atriz.ativo));
    }
    arquivo.close();
}

Atriz* lerBinario(int& tamanho) {
    ifstream arquivo("atrizes.bin", ios::binary);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo binário!" << endl;
        return nullptr;
    }

    int capacidade = 40;
    Atriz* atrizes = new Atriz[capacidade];
    tamanho = 0;

    while (true) {
        if (tamanho >= capacidade) {
            // Redimensionar
            capacidade += 10;
            Atriz* novoArray = new Atriz[capacidade];
            for (int i = 0; i < tamanho; ++i) {
                novoArray[i] = atrizes[i];
            }
            delete[] atrizes;
            atrizes = novoArray;
        }

        Atriz atriz;
        arquivo.read(reinterpret_cast<char*>(&atriz.identificador), sizeof(atriz.identificador));
        if (arquivo.eof()) break; // Verifique o fim do arquivo

        atriz.nomeSobrenome = lerString(arquivo);
        atriz.estadoOrigem = lerString(arquivo);
        arquivo.read(reinterpret_cast<char*>(&atriz.idade), sizeof(atriz.idade));
        arquivo.read(reinterpret_cast<char*>(&atriz.quantidadeFilmes), sizeof(atriz.quantidadeFilmes));
        arquivo.read(reinterpret_cast<char*>(&atriz.ativo), sizeof(atriz.ativo));

        if (arquivo.fail()) {
            cout << "Erro ao ler dados do arquivo binário." << endl;
            delete[] atrizes;
            return nullptr;
        }

        atrizes[tamanho++] = atriz;
    }

    arquivo.close();
    return atrizes;
}

void adicionarAtriz(Atriz*& atrizes, int& tamanho, int& capacidade, const Atriz& novaAtriz) {
    if (tamanho >= capacidade) {
        // Redimensionar array
        capacidade += 10;
        Atriz* novoArray = new Atriz[capacidade];
        for (int i = 0; i < tamanho; ++i) {
            novoArray[i] = atrizes[i];
        }
        delete[] atrizes;
        atrizes = novoArray;
    }
    atrizes[tamanho++] = novaAtriz;
}

void removerAtriz(Atriz* atrizes, int& tamanho, int identificador) {
    for (int i = 0; i < tamanho; ++i) {
        if (atrizes[i].identificador == identificador) {
            atrizes[i].ativo = false; // inativa
        }
    }
}

int particionarPorNome(Atriz* atrizes, int baixo, int alto) {
    string pivo = atrizes[alto].nomeSobrenome; // Escolhe o último elemento como pivô
    int i = baixo - 1; // Índice do menor elemento

    for (int j = baixo; j < alto; ++j) {
        if (atrizes[j].nomeSobrenome < pivo) {
            ++i;
            swap(atrizes[i], atrizes[j]);
        }
    }
    swap(atrizes[i + 1], atrizes[alto]);
    return i + 1;
}

int particionarPorIdade(Atriz* atrizes, int baixo, int alto) {
    int pivo = atrizes[alto].idade; // Escolhe o último elemento como pivô
    int i = baixo - 1; // Índice do menor elemento

    for (int j = baixo; j < alto; ++j) {
        if (atrizes[j].idade < pivo) {
            ++i;
            swap(atrizes[i], atrizes[j]);
        }
    }
    swap(atrizes[i + 1], atrizes[alto]);
    return i + 1;
}

int particionarPorId(Atriz* atrizes, int baixo, int alto) {
    int pivo = atrizes[alto].identificador; // Escolhe o último elemento como pivô
    int i = baixo - 1; // Índice do menor elemento

    for (int j = baixo; j < alto; ++j) {
        if (atrizes[j].identificador < pivo) {
            ++i;
            swap(atrizes[i], atrizes[j]);
        }
    }
    swap(atrizes[i + 1], atrizes[alto]);
    return i + 1;
}

// Função quicksort para ordenacao: nome
void quicksortPorNome(Atriz* atrizes, int baixo, int alto) {
    if (baixo < alto) {
        int pos_novo_pivo = particionarPorNome(atrizes, baixo, alto);
        quicksortPorNome(atrizes, baixo, pos_novo_pivo - 1);
        quicksortPorNome(atrizes, pos_novo_pivo + 1, alto);
    }
}

// Função quicksort para ordenacao: idade
void quicksortPorIdade(Atriz* atrizes, int baixo, int alto) {
    if (baixo < alto) {
        int pos_novo_pivo = particionarPorIdade(atrizes, baixo, alto);
        quicksortPorIdade(atrizes, baixo, pos_novo_pivo - 1);
        quicksortPorIdade(atrizes, pos_novo_pivo + 1, alto);
    }
}

// Função quicksort para ordenar por identificador
void quicksortPorId(Atriz* atrizes, int baixo, int alto) {
    if (baixo < alto) {
        int pos_novo_pivo = particionarPorId(atrizes, baixo, alto);
        quicksortPorId(atrizes, baixo, pos_novo_pivo - 1);
        quicksortPorId(atrizes, pos_novo_pivo + 1, alto);
    }
}

void ordenarPorNome(Atriz* atrizes, int tamanho) {
    quicksortPorNome(atrizes, 0, tamanho - 1);
}

void ordenarPorIdade(Atriz* atrizes, int tamanho) {
    quicksortPorIdade(atrizes, 0, tamanho - 1);
}

void ordenarPorId(Atriz* atrizes, int tamanho) {
    quicksortPorId(atrizes, 0, tamanho - 1);
}

int buscaBinariaPorId(const Atriz* atrizes, int tamanho, int identificador) {
    int baixo = 0, alto = tamanho - 1;

    while (baixo <= alto) {
        int meio = baixo + (alto - baixo) / 2;

        if (atrizes[meio].identificador == identificador && atrizes[meio].ativo) {
            return meio;
        }
        if (atrizes[meio].identificador < identificador) {
            baixo = meio + 1;
        } else {
            alto = meio - 1;
        }
    }
    return -1; // Não encontrado
}

int buscaLinearPorNome(const Atriz* atrizes, int tamanho, const string& nome) {
    for (int i = 0; i < tamanho; ++i) {
        if (atrizes[i].nomeSobrenome == nome && atrizes[i].ativo) {
            return i;
        }
    }
    return -1; // Não encontrado
}

void listarRegistros(const Atriz* atrizes, int tamanho) {
    for (int i = 0; i < tamanho; ++i) {
        if (atrizes[i].ativo) {
            cout << atrizes[i].identificador << " | " << atrizes[i].nomeSobrenome << " | " << atrizes[i].estadoOrigem << " | "
                 << atrizes[i].idade << " | " << atrizes[i].quantidadeFilmes << endl;
        }
    }
}

void listarTrecho(const Atriz* atrizes, int tamanho, int inicio, int fim) {
    for (int i = inicio; i <= fim && i < tamanho; ++i) {
        if (atrizes[i].ativo) {
            cout << atrizes[i].identificador << " | " << atrizes[i].nomeSobrenome << " | " << atrizes[i].estadoOrigem << " | "
                 << atrizes[i].idade << " | " << atrizes[i].quantidadeFilmes << endl;
        }
    }
}

void salvarComoCSV(const Atriz* atrizes, int tamanho) {
    ofstream arquivo("atrizes.csv");
    for (int i = 0; i < tamanho; ++i) {
        const auto& atriz = atrizes[i];
        if (atriz.ativo) {
            arquivo << atriz.identificador << "," << atriz.nomeSobrenome << "," << atriz.estadoOrigem << ","
                    << atriz.idade << "," << atriz.quantidadeFilmes << endl;
        }
    }
    arquivo.close();
}

void menu() {
    cout << "1. Listar todos os registros" << endl;
    cout << "2. Listar um trecho de registros" << endl;
    cout << "3. Adicionar novo registro" << endl;
    cout << "4. Remover registro" << endl;
    cout << "5. Ordenar por nome" << endl;
    cout << "6. Ordenar por idade" << endl;
    cout << "7. Ordenar por identificador" << endl;
    cout << "8. Buscar por identificador" << endl;
    cout << "9. Buscar por nome" << endl;
    cout << "10. Gravar alteracoes em arquivo binario" << endl;
    cout << "11. Gravar alteracoes em arquivo CSV" << endl;
    cout << "12. Sair" << endl;
}

int main() {
    int tamanho = 0, capacidade = 40;
    Atriz* atrizes = lerCSV(tamanho);

    if (!atrizes) {
        cout << "Não foi possível carregar os registros." << endl;
        return 1;
    }

    salvarComoBinario(atrizes, tamanho);

    while (true) {
        menu();
        int escolha;
        cin >> escolha;
        cin.ignore(); // Para limpar o buffer de entrada

        switch (escolha) {
        case 1:
            listarRegistros(atrizes, tamanho);
            break;
        case 2: {
            int inicio, fim;
            cout << "Informe o índice inicial: ";
            cin >> inicio;
            cout << "Informe o índice final: ";
            cin >> fim;
            listarTrecho(atrizes, tamanho, inicio, fim);
            break;
        }
        case 3: {
            Atriz novaAtriz;
            cout << "Informe o identificador: ";
            cin >> novaAtriz.identificador;
            cin.ignore();
            cout << "Informe o nome e sobrenome: ";
            getline(cin, novaAtriz.nomeSobrenome);
            cout << "Informe o estado de origem: ";
            getline(cin, novaAtriz.estadoOrigem);
            cout << "Informe a idade: ";
            cin >> novaAtriz.idade;
            cout << "Informe a quantidade de filmes: ";
            cin >> novaAtriz.quantidadeFilmes;
            novaAtriz.ativo = true;
            adicionarAtriz(atrizes, tamanho, capacidade, novaAtriz);
            break;
        }
        case 4: {
            int identificador;
            cout << "Informe o identificador do registro a ser removido: ";
            cin >> identificador;
            removerAtriz(atrizes, tamanho, identificador);
            break;
        }
        case 5:
            ordenarPorNome(atrizes, tamanho);
            break;
        case 6:
            ordenarPorIdade(atrizes, tamanho);
            break;
        case 7:
            ordenarPorId(atrizes, tamanho);
            break;
        case 8: {
            int identificador;
            cout << "Informe o identificador a ser buscado: ";
            cin >> identificador;
            ordenarPorId(atrizes, tamanho); // Ordene antes de buscar
            int index = buscaBinariaPorId(atrizes, tamanho, identificador);
            if (index != -1) {
                cout << "Registro encontrado: " << endl;
                cout << atrizes[index].identificador << " | " << atrizes[index].nomeSobrenome << " | "
                     << atrizes[index].estadoOrigem << " | " << atrizes[index].idade << " | "
                     << atrizes[index].quantidadeFilmes << endl;
            } else {
                cout << "Registro não encontrado." << endl;
            }
            break;
        }
        case 9: {
            string nome;
            cout << "Informe o nome a ser buscado: ";
            getline(cin, nome);
            int index = buscaLinearPorNome(atrizes, tamanho, nome);
            if (index != -1) {
                cout << "Registro encontrado: " << endl;
                cout << atrizes[index].identificador << " | " << atrizes[index].nomeSobrenome << " | "
                     << atrizes[index].estadoOrigem << " | " << atrizes[index].idade << " | "
                     << atrizes[index].quantidadeFilmes << endl;
            } else {
                cout << "Registro não encontrado." << endl;
            }
            break;
        }
        case 10:
            salvarComoBinario(atrizes, tamanho);
            break;
        case 11:
            salvarComoCSV(atrizes, tamanho);
            break;
        case 12:
            delete[] atrizes;
            return 0;
        default:
            cout << "Opção inválida!" << endl;
            break;
        }
    }

    return 0;
}