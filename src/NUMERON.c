#include <stdio.h>
#include <ctype.h> 
#include <string.h>

/* INTEGRANTES:
- ANDRE DOERNER DUARTE - 10427938
- MATHEUS LEONARDO CARDOSO KROEFF - 10426434
- NAOTO USHIZAKI - 10437445
*/
// ---------------------------------------------------------------- //
// Iniciando a Maquina:
char input[1024];  // entrada do user
int le_posicao = 0; // posicao atual da leitura

// Recebendo simbolos:
char lendo(void){
    // retorna o caracter lido na posição atual do analisador
    return input[le_posicao]; 
}   

// Checa se simbolo do input do usuário == simbolos da gramática dada
int check_symbol(char simbolo){
    if(input[le_posicao] == simbolo){
        le_posicao++; // incrementa se caracter existe na gramatica
        return 1; // Sucesso
    }
    return 0; //  Devolve/return 0 se não pertencer
}

// Inicializando funcões para cada símbolo não-terminal(não recebem parâmetros):
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
    if(I() && input[le_posicao] == '\0'){ // '\0' -> sinaliza o fim 
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------- //
// Funções seguindo as regras:
int I(void){
    int pos_I = le_posicao;
    if(S()){ // Checa se o próximo símbolo é reconhecível
        return 1;
    }
    le_posicao = pos_I; // se falhar o reconhecimento de simbolo -> restaura posição inicial (pos_I)
    return 0; // falha
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
    // Aceita se escolher ignorar (conjunto vazio)    
    return 1;
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
int F(){
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
    // Testar do 1 a 9:
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
    // Checa se char atual analisado -> é dígito: assim incrementa posição.
    while(isdigit((unsigned char)input[le_posicao])){
        le_posicao++;
    }
    // Ignora -> conjunto vazio:
    return 1;
}

// ---------------------------------------------------------------- //
// MAIN CONTROLADOR
int main(){
    
    printf("\n-----------------------------\n");
    printf("NUMERON AUTOMATON\n");
    printf("\n-----------------------------\n");

    printf("DIGITE A CADEIA: ");
    scanf(" %1023s", input);

    if(parse()){
        printf("CADEIA = ACEITA\n");
    } else {
        printf("CADEIA = REJEITADA\n");
    }

    return 0;
}
