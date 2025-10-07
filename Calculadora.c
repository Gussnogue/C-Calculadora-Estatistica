#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR "cls"
#else
    #include <unistd.h>
    #define CLEAR "clear"
#endif

#define MAX_DADOS 1000
#define MAX_CONJUNTOS 10

typedef struct {
    double dados[MAX_DADOS];
    int tamanho;
    char nome[50];
} ConjuntoDados;

ConjuntoDados conjuntos[MAX_CONJUNTOS];
int totalConjuntos = 0;

void limparTela() {
    system(CLEAR);
}

void pausar() {
    printf("\nPressione Enter para continuar...");
    getchar();
    getchar();
}

int lerInteiro(const char* mensagem) {
    int valor;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1) {
            break;
        } else {
            printf("Erro: Digite um número inteiro válido.\n");
            while (getchar() != '\n');
        }
    }
    return valor;
}

double lerDouble(const char* mensagem) {
    double valor;
    while (1) {
        printf("%s", mensagem);
        if (scanf("%lf", &valor) == 1) {
            break;
        } else {
            printf("Erro: Digite um número válido.\n");
            while (getchar() != '\n');
        }
    }
    return valor;
}

void inserirDadosManual() {
    if (totalConjuntos >= MAX_CONJUNTOS) {
        printf("Limite máximo de conjuntos atingido!\n");
        pausar();
        return;
    }

    limparTela();
    printf("=== INSERIR DADOS MANUALMENTE ===\n\n");
    
    printf("Nome do conjunto: ");
    scanf("%s", conjuntos[totalConjuntos].nome);
    
    int n = lerInteiro("Quantidade de dados: ");
    
    if (n <= 0 || n > MAX_DADOS) {
        printf("Quantidade inválida! Máximo: %d dados\n", MAX_DADOS);
        pausar();
        return;
    }
    
    printf("\nDigite os dados:\n");
    for (int i = 0; i < n; i++) {
        printf("Dado %d: ", i + 1);
        conjuntos[totalConjuntos].dados[i] = lerDouble("");
    }
    
    conjuntos[totalConjuntos].tamanho = n;
    totalConjuntos++;
    
    printf("\nConjunto '%s' adicionado com sucesso!\n", conjuntos[totalConjuntos-1].nome);
    pausar();
}

void gerarDadosAleatorios() {
    if (totalConjuntos >= MAX_CONJUNTOS) {
        printf("Limite máximo de conjuntos atingido!\n");
        pausar();
        return;
    }

    limparTela();
    printf("=== GERAR DADOS ALEATÓRIOS ===\n\n");
    
    printf("Nome do conjunto: ");
    scanf("%s", conjuntos[totalConjuntos].nome);
    
    int n = lerInteiro("Quantidade de dados: ");
    double minimo = lerDouble("Valor mínimo: ");
    double maximo = lerDouble("Valor máximo: ");
    
    if (n <= 0 || n > MAX_DADOS) {
        printf("Quantidade inválida!\n");
        pausar();
        return;
    }
    
    printf("\nGerando dados...\n");
    for (int i = 0; i < n; i++) {
        double random = (double)rand() / RAND_MAX;
        conjuntos[totalConjuntos].dados[i] = minimo + random * (maximo - minimo);
    }
    
    conjuntos[totalConjuntos].tamanho = n;
    totalConjuntos++;
    
    printf("Dados aleatórios gerados com sucesso!\n");
    pausar();
}

void ordenarDados(double dados[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (dados[j] > dados[j+1]) {
                double temp = dados[j];
                dados[j] = dados[j+1];
                dados[j+1] = temp;
            }
        }
    }
}

double calcularMedia(double dados[], int n) {
    double soma = 0;
    for (int i = 0; i < n; i++) {
        soma += dados[i];
    }
    return soma / n;
}

double calcularMediana(double dados[], int n) {
    double copia[MAX_DADOS];
    memcpy(copia, dados, n * sizeof(double));
    ordenarDados(copia, n);
    
    if (n % 2 == 0) {
        return (copia[n/2 - 1] + copia[n/2]) / 2.0;
    } else {
        return copia[n/2];
    }
}

double calcularModa(double dados[], int n) {
    double moda = dados[0];
    int maxFreq = 1;
    
    for (int i = 0; i < n; i++) {
        int freq = 1;
        for (int j = i + 1; j < n; j++) {
            if (fabs(dados[j] - dados[i]) < 1e-10) {
                freq++;
            }
        }
        if (freq > maxFreq) {
            maxFreq = freq;
            moda = dados[i];
        }
    }
    
    return (maxFreq > 1) ? moda : NAN;
}

double calcularVariancia(double dados[], int n) {
    if (n <= 1) return 0;
    
    double media = calcularMedia(dados, n);
    double soma = 0;
    
    for (int i = 0; i < n; i++) {
        soma += (dados[i] - media) * (dados[i] - media);
    }
    
    return soma / (n - 1);
}

double calcularDesvioPadrao(double dados[], int n) {
    return sqrt(calcularVariancia(dados, n));
}

double calcularAmplitude(double dados[], int n) {
    double min = dados[0];
    double max = dados[0];
    
    for (int i = 1; i < n; i++) {
        if (dados[i] < min) min = dados[i];
        if (dados[i] > max) max = dados[i];
    }
    
    return max - min;
}

double calcularCoeficienteVariacao(double dados[], int n) {
    double media = calcularMedia(dados, n);
    double desvio = calcularDesvioPadrao(dados, n);
    
    if (fabs(media) < 1e-10) return NAN;
    return (desvio / media) * 100;
}

void calcularMedidasResumo() {
    if (totalConjuntos == 0) {
        printf("Nenhum conjunto de dados disponível!\n");
        pausar();
        return;
    }
    
    limparTela();
    printf("=== MEDIDAS DE RESUMO ===\n\n");
    
    for (int i = 0; i < totalConjuntos; i++) {
        printf("%d. %s (%d dados)\n", i + 1, conjuntos[i].nome, conjuntos[i].tamanho);
    }
    
    int escolha = lerInteiro("\nEscolha o conjunto: ") - 1;
    
    if (escolha < 0 || escolha >= totalConjuntos) {
        printf("Conjunto inválido!\n");
        pausar();
        return;
    }
    
    ConjuntoDados* conjunto = &conjuntos[escolha];
    
    limparTela();
    printf("=== MEDIDAS DE RESUMO - %s ===\n\n", conjunto->nome);
    
    printf("Dados: ");
    for (int i = 0; i < conjunto->tamanho && i < 10; i++) {
        printf("%.2f ", conjunto->dados[i]);
    }
    if (conjunto->tamanho > 10) printf("...");
    printf("\n\n");
    
    double media = calcularMedia(conjunto->dados, conjunto->tamanho);
    double mediana = calcularMediana(conjunto->dados, conjunto->tamanho);
    double moda = calcularModa(conjunto->dados, conjunto->tamanho);
    double variancia = calcularVariancia(conjunto->dados, conjunto->tamanho);
    double desvio = calcularDesvioPadrao(conjunto->dados, conjunto->tamanho);
    double amplitude = calcularAmplitude(conjunto->dados, conjunto->tamanho);
    double cv = calcularCoeficienteVariacao(conjunto->dados, conjunto->tamanho);
    
    printf("Tamanho da amostra (n): %d\n", conjunto->tamanho);
    printf("Média: %.4f\n", media);
    printf("Mediana: %.4f\n", mediana);
    
    if (!isnan(moda)) {
        printf("Moda: %.4f\n", moda);
    } else {
        printf("Moda: Não há moda única\n");
    }
    
    printf("Variância amostral: %.4f\n", variancia);
    printf("Desvio padrão: %.4f\n", desvio);
    printf("Amplitude: %.4f\n", amplitude);
    
    if (!isnan(cv)) {
        printf("Coeficiente de Variação: %.2f%%\n", cv);
    } else {
        printf("Coeficiente de Variação: Indefinido (média zero)\n");
    }
    
    pausar();
}

void analiseDistribuicaoFrequencia() {
    if (totalConjuntos == 0) {
        printf("Nenhum conjunto de dados disponível!\n");
        pausar();
        return;
    }
    
    limparTela();
    printf("=== DISTRIBUIÇÃO DE FREQUÊNCIA ===\n\n");
    
    for (int i = 0; i < totalConjuntos; i++) {
        printf("%d. %s (%d dados)\n", i + 1, conjuntos[i].nome, conjuntos[i].tamanho);
    }
    
    int escolha = lerInteiro("\nEscolha o conjunto: ") - 1;
    
    if (escolha < 0 || escolha >= totalConjuntos) {
        printf("Conjunto inválido!\n");
        pausar();
        return;
    }
    
    ConjuntoDados* conjunto = &conjuntos[escolha];
    
    int numClasses = lerInteiro("\nNúmero de classes: ");
    
    if (numClasses <= 0 || numClasses > conjunto->tamanho) {
        printf("Número de classes inválido!\n");
        pausar();
        return;
    }
    
    double min = conjunto->dados[0];
    double max = conjunto->dados[0];
    
    for (int i = 1; i < conjunto->tamanho; i++) {
        if (conjunto->dados[i] < min) min = conjunto->dados[i];
        if (conjunto->dados[i] > max) max = conjunto->dados[i];
    }
    
    double amplitude = max - min;
    double larguraClasse = amplitude / numClasses;
    
    limparTela();
    printf("=== DISTRIBUIÇÃO DE FREQUÊNCIA - %s ===\n\n", conjunto->nome);
    printf("Classes: %d | Amplitude total: %.4f\n\n", numClasses, amplitude);
    
    printf("Classe       Frequência  Frequência%%\n");
    printf("------------------------------------\n");
    
    for (int i = 0; i < numClasses; i++) {
        double inicio = min + i * larguraClasse;
        double fim = inicio + larguraClasse;
        int freq = 0;
        
        for (int j = 0; j < conjunto->tamanho; j++) {
            if (conjunto->dados[j] >= inicio && conjunto->dados[j] < fim) {
                freq++;
            }
        }
        if (i == numClasses - 1) {
            for (int j = 0; j < conjunto->tamanho; j++) {
                if (conjunto->dados[j] >= inicio && conjunto->dados[j] <= fim) {
                    freq++;
                }
            }
        }
        
        double freqPercent = (double)freq / conjunto->tamanho * 100;
        printf("%.2f-%.2f   %8d   %10.1f%%\n", inicio, fim, freq, freqPercent);
    }
    
    pausar();
}

void testeHipoteseMedia() {
    if (totalConjuntos == 0) {
        printf("Nenhum conjunto de dados disponível!\n");
        pausar();
        return;
    }
    
    limparTela();
    printf("=== TESTE DE HIPÓTESE PARA MÉDIA ===\n\n");
    
    for (int i = 0; i < totalConjuntos; i++) {
        printf("%d. %s (%d dados)\n", i + 1, conjuntos[i].nome, conjuntos[i].tamanho);
    }
    
    int escolha = lerInteiro("\nEscolha o conjunto: ") - 1;
    
    if (escolha < 0 || escolha >= totalConjuntos) {
        printf("Conjunto inválido!\n");
        pausar();
        return;
    }
    
    ConjuntoDados* conjunto = &conjuntos[escolha];
    double mediaAmostral = calcularMedia(conjunto->dados, conjunto->tamanho);
    double desvioPadrao = calcularDesvioPadrao(conjunto->dados, conjunto->tamanho);
    
    printf("\nMédia amostral: %.4f\n", mediaAmostral);
    printf("Desvio padrão: %.4f\n", desvioPadrao);
    printf("Tamanho da amostra: %d\n", conjunto->tamanho);
    
    double mediaH0 = lerDouble("\nMédia sob H0 (μ₀): ");
    double alpha = lerDouble("Nível de significância (α): ");
    
    double erroPadrao = desvioPadrao / sqrt(conjunto->tamanho);
    double estatisticaT = (mediaAmostral - mediaH0) / erroPadrao;
    
    printf("\n--- RESULTADOS DO TESTE ---\n");
    printf("Estatística t: %.4f\n", estatisticaT);
    printf("Erro padrão: %.4f\n", erroPadrao);
    
    double valorCritico = 2.0;
    if (conjunto->tamanho > 30) {
        valorCritico = 1.96;
    }
    
    printf("Valor crítico aproximado (95%%): ±%.3f\n", valorCritico);
    
    if (fabs(estatisticaT) > valorCritico) {
        printf("Decisão: REJEITAR H₀\n");
        printf("Conclusão: Há evidências de que a média difere de %.4f\n", mediaH0);
    } else {
        printf("Decisão: NÃO REJEITAR H₀\n");
        printf("Conclusão: Não há evidências de que a média difere de %.4f\n", mediaH0);
    }
    
    pausar();
}

void correlacaoEntreConjuntos() {
    if (totalConjuntos < 2) {
        printf("É necessário pelo menos 2 conjuntos de dados!\n");
        pausar();
        return;
    }
    
    limparTela();
    printf("=== CORRELAÇÃO ENTRE CONJUNTOS ===\n\n");
    
    for (int i = 0; i < totalConjuntos; i++) {
        printf("%d. %s (%d dados)\n", i + 1, conjuntos[i].nome, conjuntos[i].tamanho);
    }
    
    int conj1 = lerInteiro("\nEscolha o primeiro conjunto: ") - 1;
    int conj2 = lerInteiro("Escolha o segundo conjunto: ") - 1;
    
    if (conj1 < 0 || conj1 >= totalConjuntos || 
        conj2 < 0 || conj2 >= totalConjuntos) {
        printf("Conjunto(s) inválido(s)!\n");
        pausar();
        return;
    }
    
    ConjuntoDados* c1 = &conjuntos[conj1];
    ConjuntoDados* c2 = &conjuntos[conj2];
    
    if (c1->tamanho != c2->tamanho) {
        printf("Os conjuntos devem ter o mesmo tamanho!\n");
        pausar();
        return;
    }
    
    double media1 = calcularMedia(c1->dados, c1->tamanho);
    double media2 = calcularMedia(c2->dados, c2->tamanho);
    
    double somaProdutos = 0;
    double somaQuadrados1 = 0;
    double somaQuadrados2 = 0;
    
    for (int i = 0; i < c1->tamanho; i++) {
        double diff1 = c1->dados[i] - media1;
        double diff2 = c2->dados[i] - media2;
        
        somaProdutos += diff1 * diff2;
        somaQuadrados1 += diff1 * diff1;
        somaQuadrados2 += diff2 * diff2;
    }
    
    double correlacao = somaProdutos / sqrt(somaQuadrados1 * somaQuadrados2);
    
    printf("\n--- CORRELAÇÃO ENTRE %s E %s ---\n", c1->nome, c2->nome);
    printf("Coeficiente de Correlação (r): %.4f\n", correlacao);
    
    printf("\nInterpretação:\n");
    if (correlacao > 0.7) {
        printf("Correlação positiva forte\n");
    } else if (correlacao > 0.3) {
        printf("Correlação positiva moderada\n");
    } else if (correlacao > -0.3) {
        printf("Correlação fraca\n");
    } else if (correlacao > -0.7) {
        printf("Correlação negativa moderada\n");
    } else {
        printf("Correlação negativa forte\n");
    }
    
    pausar();
}

void listarConjuntos() {
    limparTela();
    printf("=== CONJUNTOS DE DADOS ===\n\n");
    
    if (totalConjuntos == 0) {
        printf("Nenhum conjunto de dados cadastrado.\n");
    } else {
        for (int i = 0; i < totalConjuntos; i++) {
            printf("%d. %s - %d dados\n", i + 1, conjuntos[i].nome, conjuntos[i].tamanho);
            printf("   Dados: ");
            for (int j = 0; j < conjuntos[i].tamanho && j < 5; j++) {
                printf("%.2f ", conjuntos[i].dados[j]);
            }
            if (conjuntos[i].tamanho > 5) printf("...");
            printf("\n\n");
        }
    }
    
    pausar();
}

void menuPrincipal() {
    int opcao;
    
    do {
        limparTela();
        printf("=== CALCULADORA ESTATÍSTICA ===\n\n");
        printf("1. Inserir dados manualmente\n");
        printf("2. Gerar dados aleatórios\n");
        printf("3. Listar conjuntos de dados\n");
        printf("4. Medidas de resumo\n");
        printf("5. Distribuição de frequência\n");
        printf("6. Teste de hipótese para média\n");
        printf("7. Correlação entre conjuntos\n");
        printf("0. Sair\n");
        printf("\nEscolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Opção inválida!\n");
            while (getchar() != '\n');
            pausar();
            continue;
        }
        
        switch (opcao) {
            case 0:
                printf("\nSaindo... Obrigado por usar a calculadora!\n");
                break;
            case 1:
                inserirDadosManual();
                break;
            case 2:
                gerarDadosAleatorios();
                break;
            case 3:
                listarConjuntos();
                break;
            case 4:
                calcularMedidasResumo();
                break;
            case 5:
                analiseDistribuicaoFrequencia();
                break;
            case 6:
                testeHipoteseMedia();
                break;
            case 7:
                correlacaoEntreConjuntos();
                break;
            default:
                printf("Opção inválida!\n");
                pausar();
                break;
        }
        
    } while (opcao != 0);
}

int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    
    printf("Iniciando Calculadora Estatística...\n");
    sleep(1);
    menuPrincipal();
    
    return 0;
}