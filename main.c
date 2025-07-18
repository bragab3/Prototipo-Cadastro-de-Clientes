#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_NOME 101
#define MAX_EMAIL 51
#define MAX_CLIENTES 1000
#define MAX_CPF 15
#define MAX_TELEFONE 15
#define MAX_NASC 11


struct Cadastro { //struct com dados de clientes
    char nome[MAX_NOME];
    char cpf[MAX_CPF];
    char tel[MAX_TELEFONE];
    char nasc[MAX_NASC];
};

struct Cadastro clientes[MAX_CLIENTES]; //para usar nas funções 
    
bool validar_nome (char nome[MAX_NOME]) { //função para verificar se nome do cliente segue os critérios definidos

    int tam = strlen(nome);
    if (tam < 4 || tam > 100) return false; //verificando caracteres, tiver mais ou menos do que o solicitado, retorna falso e não prossegue e função
    bool tem_espaço = false;
    for (int i = 0; i < strlen(nome); i++) {
        if (!(isalpha(nome[i]) || nome[i] == ' ' || nome[i] == '-' || nome[i] == '\'')) return false; //para a função em false para qualquer caracter diferente de letras, espaço e hífen
    }
    for (int i = 0; i < tam; i++) {
        if (nome[i] == ' ') { 
            tem_espaço = true; //o laço passa em cada caracter de nome e verifica se tem espaço
        }
    }
    if(tem_espaço == false) return false;
    return true;
}

bool validar_cpf(char cpf[MAX_CPF]) {
    
    if (strlen(cpf) != 14 || cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-'){ //verifica se está no formato solicitado
        return false;
    }

    int digitos_cpf[11];
    int j = 0;
    
    for (int i = 0; i < strlen(cpf); i++) { //removendo caracteres que não são números pra fazer conta com eles depois
        if (cpf[i] != ' ' && cpf[i] != '.' && cpf[i] != '-') {
        digitos_cpf[j] =  cpf[i] - '0';
        j++;
        }
    }
    
    int soma1 = 0;
    int peso1 = 10;
    
    for(int i = 0; i < 9; i++) {
        soma1 = soma1 + digitos_cpf[i] * peso1; //fazendo a soma com pesos para encontrar digito verificador
        peso1--;
        }
        
    int dgv1 = 11 - (soma1 % 11); //primeiro digito verificador 
    
    if (dgv1 >= 10) dgv1 = 0; //regra da verificação
    if (digitos_cpf[9] != dgv1) return false; //verificação
    
    int soma2 = 0;
    int peso2 = 11;
    
    for(int i = 0; i < 10; i++) {
        soma2 = soma2 + digitos_cpf[i] * peso2; //fazendo a soma com pesos para encontrar digito verificador
        peso2--;
        }
        
    int dgv2 = 11 - (soma2 % 11); //segundo digito verificador 
    
    if (dgv2 >= 10) dgv2 = 0; //regra da verificação
    
    if (digitos_cpf[10] != dgv2) return false; //verificação
    return true; //se chegou até aqui é válido 
}


int main() {
    struct Cadastro cadastros[MAX_CLIENTES];
    printf("====Cadastro de clientes====\n");
    printf("Digite o CPF em seu respectivo formato:");
    int num_cliente = 0;
    scanf("%s", cadastros[num_cliente].cpf);
    if (! validar_cpf(cadastros[num_cliente].cpf)) {
        printf("Cpf inválido!");
    } else { printf("Cpf válido!");
    num_cliente ++;
    }
    
return 0;
}
