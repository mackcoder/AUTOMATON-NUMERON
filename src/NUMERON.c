#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/* INTEGRANTES:
- ANDRE DOERNER DUARTE - 10427938
- MATHEUS LEONARDO CARDOSO KROEFF - 10426434
- NAOTO USHIZAKI - 10437445
*/

// ---------------------------------------------------------------- //
// Iniciando a Máquina:
char input[1024];   // entrada do user
int le_posicao = 0; // posição atual da leitura

// ---------------------------------------------------------------- //
// Ignora espaços em branco
void pular_espacos(void){
    while (isspace((unsigned char)input[le_posicao])) {
        le_posicao++;
    }
}

// Recebendo símbolos:
char lendo(void){
    pular_espacos();
    return input[le_posicao];
}

// Checa se símbolo do input do usuário == símbolo esperado da gramática
int check_symbol(char simbolo){
    pular_espacos();
    if(input[le_posicao] == simbolo){ 
        le_posicao++; // incrementa se caracter existe na gramatica
        return 1; // Sucesso
    }
    return 0;  //  Devolve/return 0 se não pertencer
}

// ---------------------------------------------------------------- //
void download(int passo, int total){
    int width = 30;
    int preenche = (passo * width) / total;

    printf("\r[");
    for(int a = 0; a < preenche; a++)
        printf("█");

    for(int b = preenche; b < width; b++)
        printf(" ");

    printf("] %d%%", (passo * 100) / total);
    fflush(stdout);
}

// ---------------------------------------------------------------- //
// Inicializando funções para cada símbolo não-terminal(não recebem parâmetros):
// Necessário iniciar assim porque elas vão se chamar depois.
int I(void);
int S(void);
int K(void);
int T(void);
int Z(void);
int F(void);
int N(void);
int D(void);

// ---------------------------------------------------------------- //
// Função do parse -> decide se, depois de analisar a expressão inteira, se a cadeia
// pertence ou não à gramática:
int parse(void){
    le_posicao = 0;
    pular_espacos();

    if(I()){
        pular_espacos();
        if(input[le_posicao] == '\0'){
            return 1;
        }
    }
    return 0;
}

// ---------------------------------------------------------------- //
// Regras da gramática:
int I(void){
    int pos_I = le_posicao;
    if(S()){   // Checa se o próximo símbolo é reconhecível
        return 1;
    }
    le_posicao = pos_I; // se falhar o reconhecimento de simbolo -> restaura posição inicial (pos_I)
    return 0;  // falha
}

// ---------------------------------------------------------------- //
int S(void){
    int pos_S = le_posicao;
    if(T() && K()){ // Checando se ambos os símbolo são reconhecíveis
        return 1;
    }
    le_posicao = pos_S;
    return 0;
}

// ---------------------------------------------------------------- //
int K(void){
    int pos_K = le_posicao;

    // Tratar o operador '+' que K le:
    if(check_symbol('+')){
        if(T() && K()){
            return 1;
        }
        le_posicao = pos_K;
    }

    pos_K = le_posicao;

    // Tratar o operador '-' que K le:
    if(check_symbol('-')){
        if(T() && K()){
            return 1;
        }
        le_posicao = pos_K;
    }

    return 1; // Aceita se escolher ignorar (conjunto vazio)    
}

// ---------------------------------------------------------------- //
int T(void){
    int pos_T = le_posicao;

    if(F() && Z()){
        return 1;
    }
    le_posicao = pos_T;
    return 0;
}

// ---------------------------------------------------------------- //
int Z(void){
    int pos_Z = le_posicao;

    // Verifica se o símbolo lido é "*" e se os símbolo F e Z são lidos também:
    if(check_symbol('*')){
        if(F() && Z()){
            return 1;
        }
        // Volta para posição "inicial" para testar outra "rota" -> caso falha = true
        le_posicao = pos_Z;
    }

    pos_Z = le_posicao;

    // Verifica se o símbolo lido é "/" e se os símbolo F e Z são lidos também:
    if(check_symbol('/')){
        if(F() && Z()){
            return 1;
        }
        le_posicao = pos_Z;
    }

    return 1; // pode ignorar
}

// ---------------------------------------------------------------- //
int F(void){
    int pos_F = le_posicao;

    // Checa se F tem o simbolo -> parenteses e se S é algo que ele está lendo:
    if(check_symbol('(')){
        if(S() && check_symbol(')')){
            return 1;
        }
        le_posicao = pos_F;
    }

    // Caso não for o caso anterior -> lê o símbolo não-terminal N:
    le_posicao = pos_F;
    if(N()){
        return 1;
    }

    le_posicao = pos_F;
    if(check_symbol('-')){
        if(N()){
            return 1;
        }
        le_posicao = pos_F;
    }

    return 0;
}

// ---------------------------------------------------------------- //
int N(void){
    pular_espacos();

    int pos_N = le_posicao;

    if(input[le_posicao] >= '1' && input[le_posicao] <= '9'){
        le_posicao++;

        if(D()){
            return 1;
        }
    }

    le_posicao = pos_N;
    return 0;
}

// ---------------------------------------------------------------- //
int D(void){
    pular_espacos();

    // Checa se char atual analisado -> é dígito: assim incrementa posição.
    while(isdigit((unsigned char)input[le_posicao])){
        le_posicao++;
    }
    // Ignora -> conjunto vazio:
    return 1; 
}

// ---------------------------------------------------------------- //
// Limpa o buffer do teclado após scanf
void limpar_buffer(void){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

// ---------------------------------------------------------------- //
// MAIN CONTROLADOR
int main(){

    for(int i = 0; i <= 100; i++){
        download(i, 100);
        usleep(10000);
    }

    printf("\n");
    printf("====================================================================================================\n");
    printf("||                                                                                        ||\n");
    printf("||                                      [ MACHINE SPIRIT ONLINE ]                         ||\n");
    printf("||                                                                                        ||\n");
    printf("||        █████╗ ██╗   ██╗████████╗ ██████╗ ███╗   ███╗ █████╗ ████████╗ ██████╗          ||\n");
    printf("||       ██╔══██╗██║   ██║╚══██╔══╝██╔═══██╗████╗ ████║██╔══██╗╚══██╔══╝██╔═══██╗         ||\n");
    printf("||       ███████║██║   ██║   ██║   ██║   ██║██╔████╔██║███████║   ██║   ██║   ██║         ||\n");
    printf("||       ██╔══██║██║   ██║   ██║   ██║   ██║██║╚██╔╝██║██╔══██║   ██║   ██║   ██║         ||\n");
    printf("||       ██║  ██║╚██████╔╝   ██║   ╚██████╔╝██║ ╚═╝ ██║██║  ██║   ██║   ╚██████╔╝         ||\n");
    printf("||       ╚═╝  ╚═╝ ╚═════╝    ╚═╝    ╚═════╝ ╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝    ╚═════╝          ||\n");
    printf("||                                                                                        ||\n");
    printf("||                              >> TERMINAL DE CADEIAS <<                                ||\n");
    printf("||                                                                                        ||\n");
    printf("||          > AGUARDANDO INSTRUÇÕES DO OPERADOR...                                       ||\n");
    printf("||          ╔══════════════════════════════════════════════════════╗                      ||\n");
    printf("||          ║   [1] INICIAR INSERÇÃO DE CADEIAS                    ║                      ||\n");
    printf("||          ║   [2] TERMINATE CONNECTION                           ║                      ||\n");
    printf("||          ╚══════════════════════════════════════════════════════╝                      ||\n");
    printf("||                                                                                        ||\n");
    printf("\n====================================================================================================\n\n");

    int opcao_user;

    while(1){
        printf("\n|| > ESCOLHA UMA DAS OPÇÕES: ");

        if(scanf("%d", &opcao_user) != 1){
            printf("> INVALID COMMAND.\n");
            limpar_buffer();
            continue;
        }

        limpar_buffer();

        switch(opcao_user){
            case 1:
                printf("\n╔════════════════════════════════════════════════════╗\n");
                printf("║                INPUT DAS CADEIAS                  ║\n");
                printf("╚════════════════════════════════════════════════════╝\n");
                printf("\n> INSIRA A CADEIA: ");

                if(fgets(input, sizeof(input), stdin) == NULL){
                    printf("\n[STATUS]: ERRO AO LER ENTRADA.\n");
                    break;
                }

                input[strcspn(input, "\n")] = '\0';

                if(parse()){
                    printf("\n[STATUS]: \033[4mCADEIA ACEITA\033[0m\n");
                    printf(" [RESULTADO]: ESTADO FINAL ALCANÇADO.\n");
                } else {
                    printf("\n[STATUS]: \033[4mCADEIA REJEITADA\033[0m\n");
                    printf(" [RESULTADO]: TRANSICAO INVALIDA DETECTADA.\n");
                }

                break;

            case 2:
                printf("\n");
                printf("███╗   ██╗██╗   ██╗███╗   ███╗███████╗██████╗  ██████╗ ███╗   ██╗\n");
                printf("████╗  ██║██║   ██║████╗ ████║██╔════╝██╔══██╗██╔═══██╗████╗  ██║\n");
                printf("██╔██╗ ██║██║   ██║██╔████╔██║█████╗  ██████╔╝██║   ██║██╔██╗ ██║\n");
                printf("██║╚██╗██║██║   ██║██║╚██╔╝██║██╔══╝  ██╔══██╗██║   ██║██║╚██╗██║\n");
                printf("██║ ╚████║╚██████╔╝██║ ╚═╝ ██║███████╗██║  ██║╚██████╔╝██║ ╚████║\n");
                printf("╚═╝  ╚═══╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═══╝\n");
                printf("\n");
                printf("                  > OPERACAO FINALIZADA <\n");
                printf("             > MACHINE SPIRIT SHUTTING DOWN <\n\n");
                return 0;

            default:
                printf("> INVALID COMMAND.\n");
        }
    }
}
