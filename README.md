# -Sistema-de-Gerenciamento
Sistema de gerenciamento de livros em C++ 

Projeto desenvolvido para um trabalho da faculdade

# -Introdução 

Trata-se do trabalho que visa manter os registros de atrizes e suas características, com 
inserção, exclusão, gravação e consulta de registros. O software conta com o usuário através da 
interação para realizar inserções, remoção e listagem de atrizes e a opção de disponibilizar ao 
usuário a possibilidade de colocar os registros em ordem de acordo com variáveis específicas. Os 
dados foram armazenados adequadamente, cabendo então no binário com arquivos de suporte 
para a persistência de tais dados, de forma que possam ser aplicados rapidamente. 

# -Descrição Detalhada:

A estrutura Atriz é responsável por armazenar os dados de cada atriz no sistema. Ela contém os 
seguintes atributos: 

identificador:
Um número único para identificar cada atriz. Esse identificador é fundamental para 
realizar operações como remoção ou busca de uma atriz específica. 

nomeSobrenome:
O nome completo da atriz. Esse campo é utilizado tanto para exibir o nome nas 
listagens quanto para ordenar as atrizes por nome. 

estadoOrigem:
Indica o estado de origem da atriz, oferecendo um pouco mais de contexto sobre a 
atriz, que pode ser útil para análises regionais. 

idade:
A idade da atriz. Esse valor é utilizado em operações de ordenação e também pode ser útil 
para filtrar atrizes de diferentes faixas etárias. 

quantidadeFilmes:
O número de filmes em que a atriz participou. Esse dado também pode ser 
utilizado em diferentes critérios de ordenação ou análise. 

ativo: Um valor booleano (verdadeiro ou falso) que indica se a atriz está ativa ou foi removida 
logicamente do sistema. Esse campo é útil para operações de "remoção lógica", onde os dados da 
atriz ainda permanecem no sistema, mas ela é considerada removida. 

A estrutura Atriz serve para armazenar todas as informações relacionadas a cada atriz, e essa 
estrutura é utilizada como elemento para o vetor dinâmico que armazena todas as atrizes. 

# 2. Vetor Dinâmico de Atriz 
Para armazenar as atrizes, o programa utiliza um vetor dinâmico (ou array de tamanho variável), 
o que permite aumentar o espaço de armazenamento conforme novas atrizes são adicionadas. O 
vetor começa com uma capacidade inicial (por exemplo, 40 atrizes) e, conforme o número de 
atrizes cresce, o vetor é redimensionado automaticamente. Este processo é feito pela função 
redimensionarArray, que cria um novo vetor com mais espaço e copia os dados do vetor antigo 
para o novo. 

# 3. Arquivos CSV e Binários 
O programa utiliza dois tipos de arquivos para armazenar os dados: 
Arquivo CSV: O arquivo CSV é usado como a fonte inicial dos dados. Ele contém as 
informações sobre as atrizes em formato de texto simples, onde os campos de cada atriz são 
separados por vírgulas. O programa faz a leitura desse arquivo, extrai os dados e os armazena no 
vetor dinâmico. 
 
Arquivo Binário: Após as alterações (como adicionar ou remover atrizes), os dados são gravados 
em um arquivo binário. Este tipo de arquivo é mais eficiente para leitura e escrita, pois ocupa 
menos espaço e é mais rápido de processar em comparação com arquivos de texto. 

# 4. Funções de Ordenação: Quick Sort 
O algoritmo de ordenação utilizado no programa é o Quick Sort. Este algoritmo divide o vetor de 
atrizes em duas partes com base em um critério de ordenação escolhido pelo usuário, 
recursivamente ordenando as sublistas. O critério de ordenação pode ser baseado em: 

Identificador: Ordena as atrizes pelo identificador, que é um número único. 
Nome: Ordena as atrizes pelo nome completo (alfabeticamente). 
Idade: Ordena as atrizes pela idade. 

O algoritmo de Quick Sort é eficiente, com complexidade média de tempo de O(n log n), o que 
garante uma boa performance mesmo com um número considerável de atrizes. 

# 5. Funcionalidades Principais do Programa 
Adicionar Atrizes: O programa permite ao usuário adicionar novas atrizes ao vetor. Quando uma 
nova atriz é adicionada, o vetor é redimensionado se necessário. 

Remover Atrizes: A remoção de atrizes não é feita fisicamente (deletando o registro), mas sim 
logicamente, desmarcando o campo ativo como falso. 

Listar Atrizes: O programa exibe as atrizes ativas (onde ativo é verdadeiro) na tela, mostrando 
todas as informações relevantes. 

Salvar Alterações: As modificações feitas nas atrizes (como remoções ou adições) podem ser 
salvas de volta no arquivo binário. 

# 6. Interação com o Usuário 
O programa possui um menu interativo que permite ao usuário realizar diferentes ações. As 
opções incluem listar as atrizes, adicionar novas atrizes, remover atrizes, ordenar os dados, ou 
salvar as alterações em um arquivo binário. A interação é feita por meio de uma interface de 
texto simples, onde o usuário escolhe as opções fornecidas. 

# Resumo da Lógica do Programa 
O programa começa lendo dados de um arquivo CSV contendo informações sobre atrizes e 
armazena esses dados em um vetor dinâmico de Atriz. 
O usuário pode realizar várias operações, como listar, adicionar, remover ou ordenar as atrizes. 
O arquivo binário é utilizado para salvar as alterações feitas no vetor, garantindo persistência dos 
dados. 

O programa também permite ordenar as atrizes utilizando o algoritmo Quick Sort, com diferentes 
critérios de ordenação. 
O software continua em execução até que o usuário opte por sair. 

A estrutura do programa é projetada para ser modular, facilitando a manutenção e a adição de 
novos recursos, além de garantir eficiência no processamento dos dados. 

# Acertos e Erros Durante o Desenvolvimento: 

Acertos:
Durante o desenvolvimento do trabalho, tive a oportunidade de aprimorar várias 
habilidades, principalmente no que diz respeito à utilização de criatividade para resolver 
problemas. A implementação do algoritmo de ordenação Quick Sort foi uma escolha acertada, 
proporcionando uma ordenação eficiente e rápida das atrizes. 

Erros:
Por outro lado, tive algumas dificuldades significativas, especialmente ao tentar 
implementar a funcionalidade de inserção de novas atrizes. A lógica de adicionar novas atrizes 
ao sistema não funcionou corretamente nas primeiras tentativas, o que resultou em um gasto 
considerável de tempo para identificar e corrigir o problema. Esse processo envolveu a revisão 
de diversas partes do código, o que atrasou o progresso da implementação. Apesar dessas falhas, 
o erro nos proporcionou aprendizado valioso sobre a importância de testar minuciosamente cada 
funcionalidade antes de seguir para a próxima fase do desenvolvimento. 


