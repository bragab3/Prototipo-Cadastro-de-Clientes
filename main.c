#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_NOME 101
#define MAX_EMAIL 51
#define MAX_CLIENTES 1000
#define MAX_CPF 15
#define MAX_TELEFONE 16
#define MAX_NASC 11
#define MAX_DDD 67
#define ANO_ATUAL 2025


// struct com dados de clientes
struct Cadastro { 
    char nome[MAX_NOME];
    char cpf[MAX_CPF];
    char tel[MAX_TELEFONE];
    char email[MAX_EMAIL];
    char nasc[MAX_NASC];
    int data[3];
};


// protótipos para uso nas funções
struct Cadastro clientes[MAX_CLIENTES];  
void menu(); 
   
    
// função para verificar se nome do cliente segue os critérios definidos
int validar_nome (char nome[MAX_NOME]) { 
    nome[strcspn(nome, "\n")] = '\0'; 
    int tam = strlen(nome);
    
    // verificando o tamanho do nome
    if (tam < 4 || tam >= MAX_NOME) return 1;   // código 1 - Erro: Nome curto ou longo demais
    
    // verificando se contém espaço
    bool tem_espaço = false;
    for (int i = 0; i < tam; i++) {
        if (nome[i] == ' ') { 
            tem_espaço = true; 
        }
    }
    
    if (nome[tam-1] == ' ') return 2;           // código 2 - Erro: Nome terminado em espaço
    if (! tem_espaço) return 3;                 // código 3 - Erro: Nome sem sobrenome
    
    
    // verificando se existe caracter diferente de letra, espaço, hífen ou apóstrofo
    for (int i = 0; i < tam; i++) {
        if (!(isalpha(nome[i]) || nome[i] == ' ' || nome[i] == '-' || nome[i] == '\'')) {
            return 4;                           // código 4 - Erro: Nome contém caracter inválido
        }
    }

    return 0;                                   // código 0 - Padrão para "Válido"
}

// função para verificar se e-mail do cliente segue os critérios definidos
int validar_email(char email[MAX_EMAIL]) { 
    email[strcspn(email, "\n")] = '\0';         // Remover '\n' do fgets, se houver
    int tam = strlen(email); 
    
    // verificando o tamanho do e-mail
    if (tam >= MAX_EMAIL) return 1;             // código 1 - Erro: E-mail longo demais

    // criando strings separadas para domínio e username
    char username[MAX_EMAIL]; 
    char dominio[MAX_EMAIL];
    int arroba = -1;                            // iniciando variavel para achar onde esta o @
    int qnts_arrobas = 0;                       // quantos arrobas. Ao final, se for diferente de 1, é inválido


    //laço para verificar se caracteres atendem condições básicas - Incluimos a necessidade de ter um ponto (definição do grupo)
    for (int i = 0; i < tam; i++) { 
        if (!(islower(email[i]) || isdigit(email[i]) || email[i] == '.' || email[i] == '-' || email[i] == '@')) {
            return 2;                           // código 2 - Erro: E-mail contém caracter inválido
        }

        if (email[i] == '@') {                  // se achar o @, guarda a posição e aumenta o contador de @
            arroba = i;
            qnts_arrobas++;
        }
    }

    if (qnts_arrobas != 1 || arroba == 0 || arroba == tam - 1) {
        return 3;                               // código 3 - Erro: mais de um arroba, ou em posição errada
    }
    
    int t_user = arroba;                        // o tamanho do username vai até o @
    int t_dom = tam - arroba - 1;               // o tamanho do dominio vai do @ pra frente

    strncpy(username, email, t_user);           // separando username em string própria
    username[t_user] = '\0';
    strncpy(dominio, email + arroba + 1, t_dom);    //s eparando domínio em string própria
    dominio[t_dom] = '\0';

    if (username[0] == '-' || username[t_user - 1] == '-' || dominio[0] == '-' || dominio[t_dom - 1] == '-') {
        return 4;                               // código 4 - Erro: hífen em posição inválida
    }
    
    if (dominio[0] == '.' || dominio[t_dom - 1] == '.') {
        return 5;                               // código 5 - Erro: ponto em posição inválida
    }
    if (strchr(dominio, '.') == NULL) {
        return 6;                               // código 6 - Erro: domínio de e-mail não contém ponto 
    }                                           // embora não esteja nas condições do trabalho, decidimos condicionar a existência de um '.' no domínio, por padrão

    // evitar dois pontos seguidos no domínio
    for (int i = 1; i < t_dom; i++) {
        if (dominio[i] == '.' && dominio[i - 1] == '.') {
            return 7;                           // código 7 - Erro: domínio de e-mail contém dois pontos seguidos             
        }
    }

    return 0;; 
}

// função para verificar se CPF do cliente segue os critérios definidos. Sem problemas com a receita federal na nossa loja :D
int validar_cpf(char cpf[MAX_CPF]) { 
    int tam_cpf = strlen(cpf); 
    
    // verificando o formato de entrada
    if (tam_cpf != 14 || cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-'){ 
        return 1;                           // código 1 - Erro: CPF digitado em formato inválido
    }

    int digitos_cpf[11];
    int j = 0;
    
    
    // removendo caracteres que não são números pra fazer conta com eles depois
    for (int i = 0; i < tam_cpf; i++) { 
        if (cpf[i] >= '0' && cpf[i] <= '9') {
            digitos_cpf[j++] = cpf[i] - '0';
        }
    }
    
    
    // verificando se todos os dígitos são iguais
    bool todos_iguais = true;
    for (int i = 1; i < 11; i++) {
        if (digitos_cpf[i] != digitos_cpf[0]) {
            todos_iguais = false;
            break;
        }
    }
    if (todos_iguais) return 2;             // código 2 - Erro: todos os dígitos iguais
    
    
    // encontrando o primeiro digito verificador 
    int soma1 = 0;
    int peso1 = 10;
    for(int i = 0; i < 9; i++) {
        soma1 += digitos_cpf[i] * peso1;    // fazendo a soma com pesos para encontrar digito verificador
        peso1--;
    }
        
    int dgt_vrf1 = 11 - (soma1 % 11); 
    if (dgt_vrf1 >= 10) dgt_vrf1 = 0;       //regra da verificação
    if (digitos_cpf[9] != dgt_vrf1) {
        return 3;                           // código 3 - Erro: 1o dígito verificador está errado
    }
    
    
    // encontrando o segundo digito verificador 
    int soma2 = 0;
    int peso2 = 11;
    for(int i = 0; i < 10; i++) {
        soma2 += digitos_cpf[i] * peso2;    // fazendo a soma com pesos para encontrar digito verificador
        peso2--;
    }
        
    int dgt_vrf2 = 11 - (soma2 % 11); 
    if (dgt_vrf2 >= 10) dgt_vrf2 = 0;       //regra da verificação
    if (digitos_cpf[10] != dgt_vrf2) {
        return 4;                           // código 4 - Erro: 2o dígito verificador está errado
    }
    
    return 0;                               // código 0 - Padrão para "Válido"
}

// função para verificar se e-mail do cliente segue os critérios definidos
int validar_telefone(char tel[MAX_TELEFONE]){
    
    int dval[MAX_DDD] = {                   // adiciona todos os DDDs do Brasil no array (ddd valido abreviado)
        11, 12, 13, 14, 15, 16, 17, 18, 19,
        21, 22, 24, 27, 28,
        31, 32, 33, 34, 35, 37, 38,
        41, 42, 43, 44, 45, 46, 47, 48, 49,
        51, 53, 54, 55,
        61, 62, 63, 64, 65, 66, 67, 68, 69,
        71, 73, 74, 75, 77, 79,
        81, 82, 83, 84, 85, 86, 87, 88, 89,
        91, 92, 93, 94, 95, 96, 97, 98, 99
    };
    
    // Adiciona DDD (21) se não tiver
    if (tel[0] != '(') {
        char novo_tel[32];
        snprintf(novo_tel, sizeof(novo_tel), "(21) %s", tel);
        strncpy(tel, novo_tel, MAX_TELEFONE);
    }
    
    int tam = strlen(tel);
    
    // verificações já com DDD
    if (tel[0] == '(' && tel[3] == ')' && tel[4] == ' '){
        if (!isdigit(tel[1]) || !isdigit(tel[2])) {
            return 1;                           // código 1 - Erro: DDD contém um campo não-numérico
        }
        
        char ddd[3] = {tel[1], tel[2], '\0'};
        int n_ddd = atoi(ddd);                  //transforma o ddd para int 
        
        // vVerificando se o DDD é válido
        bool ddd_valido = false;
        for (int i = 0; i < MAX_DDD; i++){      //percorre o array de dval para verificar
            if (n_ddd == dval[i]){              //se for igual adiciona valor true ao ddd_valido
                ddd_valido = true;
                break;
            }
        }
        
        if (!ddd_valido) return 2;              // código 2 - Erro: DDD inválido
        
        if (tel[tam - 5] != '-') return 3;      // código 3 - Erro: número digitado sem o hífen antes dos 4 últimos dígitos
        
        if (tam == 15 && tel[5] != '9') {
            return 4;                           // código 4 - Erro: número de 9 dígitos que começa sem o '9'
        }

        // verificando se todas as posições são dígitos, menos o hífen
        for (int i = 5; i < tam; i++) { 
            if (i == tam - 5) continue; 
            if (!isdigit(tel[i])) {
                return 5;                       // código 5 - Erro: número digitado com caracter não-numérico
            }
        }
        return 0;                               // código 0 - Padrão para "Válido"
    }
    return 6;                                   // código 6 - Erro: número não atende validações
}

// função para verificar se data do cliente segue os critérios definidos
int validar_data(int Dia, int Mes, int Ano) {

    // verificando input de dados
    if (Dia < 1 || Dia > 31 || Mes < 1 || Mes > 12 || Ano > ANO_ATUAL || Ano < 1) {
        return 1;                               // código 1 - Erro: Dia, mês ou ano inválido(s)
    }
    
    if ((Mes == 4 || Mes == 6 || Mes ==  9 || Mes ==  11)  && Dia > 30) {
        return 2;                               // código 2 - Erro: Esse mês só possui 30 dias
    }
    
    // verificando se é bissexto
    bool bissexto = false;
    if ((Ano % 4 == 0 && Ano % 100 != 0) || (Ano % 400 == 0)) bissexto = true;
    
    // se é bissexto, 29 de fevereido é uma data possível
    int max_fevereiro = 28;
    if (bissexto) max_fevereiro = 29;
    if (Mes == 2 && Dia > max_fevereiro) {
        return 3;                               // código 3 - Erro: Dia do mês cadastrado não existe
    }
    return 0;                                   // código 0 - Padrão para "Válido"
}


// função para cadastrar novos clientes, ou alterar existentes
void cadastrar_clientes(){ 
    static int total_clientes = 0;              // com cumulativa para auxiliar na contagem iterativa
    int qtd_novos_clientes;
    printf("\n\n=======================================================\n");
    printf("                  Cadastro de clientes                 \n");
    printf("=======================================================\n");
    printf("\nQuantos novos clientes gostaria de cadastrar?\n");
    scanf("%d", &qtd_novos_clientes);
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // limpar buffer após scanf
    
    if (qtd_novos_clientes==0) menu();
    
    if (total_clientes + qtd_novos_clientes > MAX_CLIENTES) {
        printf("\nLimite de clientes ultrapassado! Só é possivel cadastrar %d clientes.\n", MAX_CLIENTES - total_clientes);
        return;
    }
    
    for (int i = 0; i < qtd_novos_clientes; i++) {
        int indice = total_clientes + i;
        
        printf("\n=======================================================\n");
        printf("\nDigite as informacoes do cliente %d\n\n", indice + 1);
        
        

        // Lê CPF e valida
        printf("CPF, no formato XXX.XXX.XXX-XX: ");
        fgets(clientes[indice].cpf, MAX_CPF, stdin);
        clientes[indice].cpf[strcspn(clientes[indice].cpf, "\n")] = '\0';

        while (validar_cpf(clientes[indice].cpf) != 0) {
            switch (validar_cpf(clientes[indice].cpf)) {
                case 1:
                    printf("\nCPF invalido - formato errado. Tente novamente: ");
                    fgets(clientes[indice].cpf, MAX_CPF, stdin);
                    clientes[indice].cpf[strcspn(clientes[indice].cpf, "\n")] = '\0';
                    break;
                case 2:
                    printf("\nCPF invalido - todos os digitos sao iguais. Tente novamente: ");
                    fgets(clientes[indice].cpf, MAX_CPF, stdin);
                    clientes[indice].cpf[strcspn(clientes[indice].cpf, "\n")] = '\0';
                    break;
                case 3:
                    printf("\nCPF invalido - primeiro digito verificador esta errado. Tente novamente: ");
                    fgets(clientes[indice].cpf, MAX_CPF, stdin);
                    clientes[indice].cpf[strcspn(clientes[indice].cpf, "\n")] = '\0';
                    break;
                case 4:
                    printf("\nCPF invalido - segundo digito verificador esta errado. Tente novamente: ");
                    fgets(clientes[indice].cpf, MAX_CPF, stdin);
                    clientes[indice].cpf[strcspn(clientes[indice].cpf, "\n")] = '\0';
                    break;
            }
        }

        // Verificando se o CPF já existe
        int pos_existe = -1;
        for (int j = 0; j < total_clientes; j++) {
            if (strcmp(clientes[indice].cpf, clientes[j].cpf) == 0) {
                pos_existe = j;
                break;
            }
        }

        if (pos_existe != -1) {
            char confirma[4];                           // 4 para evitar overflow e guardar '\n' + '\0'
            printf("\n\nCPF ja cadastrado para o cliente: %s\n", clientes[pos_existe].nome);
            while ((c = getchar()) != '\n' && c != EOF); // limpa buffer
            printf("\nDeseja atualizar os dados do cliente vinculado a esse CPF? (s/n): ");
            fgets(confirma, sizeof(confirma), stdin);
            confirma[strcspn(confirma, "\n")] = '\0';

            if (confirma[0] == 's' || confirma[0] == 'S') {
                indice = pos_existe;                    // Usar a posição já existente para atualização
                printf("\nAtualize os dados do cliente:\n");
            } else {
                printf("Alteração nao realizada. Favor seguir com novo preenchimento.\n\n");
                i--; // repete essa iteração para tentar um novo cadastro
                continue;
            }
            
        }   

        // Agora cadastra os dados (nome, data, telefone, email) 
        
        while ((c = getchar()) != '\n' && c != EOF); // limpa buffer
        printf("Nome completo: ");
        fgets(clientes[indice].nome, MAX_NOME, stdin);
        clientes[indice].nome[strcspn(clientes[indice].nome, "\n")] = '\0';
    
        while (validar_nome(clientes[indice].nome) != 0) {
            switch (validar_nome(clientes[indice].nome)) {
                case 1:
                    printf("\nNome invalido - curto ou longo demais. Tente novamente: ");
                    fgets(clientes[indice].nome, MAX_NOME, stdin);
                    clientes[indice].nome[strcspn(clientes[indice].nome, "\n")] = '\0';
                    break;
                case 2:
                    printf("\nNome invalido - terminado em espaço. Tente novamente: ");
                    fgets(clientes[indice].nome, MAX_NOME, stdin);
                    clientes[indice].nome[strcspn(clientes[indice].nome, "\n")] = '\0';
                    break;
                case 3:
                    printf("\nNome invalido - sem sobrenomes. Tente novamente: ");
                    fgets(clientes[indice].nome, MAX_NOME, stdin);
                    clientes[indice].nome[strcspn(clientes[indice].nome, "\n")] = '\0';
                    break;
                case 4:
                    printf("\nNome invalido - contem caracter invalido. Tente novamente: ");
                    fgets(clientes[indice].nome, MAX_NOME, stdin);
                    clientes[indice].nome[strcspn(clientes[indice].nome, "\n")] = '\0';
                    break;                
            }
        }

        
        printf("Data de nascimento, no formato DD/MM/AAAA: ");
        scanf("%02d/%02d/%04d", &clientes[indice].data[0], &clientes[indice].data[1], &clientes[indice].data[2]);
        while ((c = getchar()) != '\n' && c != EOF);                // limpar buffer após scanf
        
        while (validar_data(clientes[indice].data[0], clientes[indice].data[1], clientes[indice].data[2]) !=0) {
            switch (validar_data(clientes[indice].data[0], clientes[indice].data[1], clientes[indice].data[2])) {
                case 1:
                    printf("\nData invalida - dia, mes ou ano invalido(s). Tente novamente: "); 
                    scanf("%02d/%02d/%04d", &clientes[indice].data[0], &clientes[indice].data[1], &clientes[indice].data[2]);
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                case 2:
                    printf("\nData invalida - esse mes so possui 30 dias. Tente novamente: "); 
                    scanf("%02d/%02d/%04d", &clientes[indice].data[0], &clientes[indice].data[1], &clientes[indice].data[2]);
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                case 3:
                    printf("\nData invalida - dia cadastrado nao existe nesse mes. Tente novamente: "); 
                    scanf("%02d/%02d/%04d", &clientes[indice].data[0], &clientes[indice].data[1], &clientes[indice].data[2]);
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
            }
        }

        printf("Telefone, no formato (XX) XXXXX-XXXX: ");
        fgets(clientes[indice].tel,MAX_TELEFONE, stdin);
        clientes[indice].tel[strcspn(clientes[indice].tel, "\n")] = '\0'; //troca o \n do fgets por \0
        
        while (validar_telefone(clientes[indice].tel) != 0) { 
            switch (validar_telefone(clientes[indice].tel)) {
                case 1:
                    printf("Telefone inválido - DDD contem um campo nao-numerico. Tente novamente: \n"); 
                    fgets(clientes[indice].tel,MAX_TELEFONE, stdin);
                    clientes[indice].tel[strcspn(clientes[indice].tel, "\n")] = '\0';
                    break;
                case 2:
                    printf("Telefone inválido - DDD invalido. Tente novamente: \n"); 
                    fgets(clientes[indice].tel,MAX_TELEFONE, stdin);
                    clientes[indice].tel[strcspn(clientes[indice].tel, "\n")] = '\0';
                    break;
                case 3:
                    printf("Telefone inválido - numero digitado sem o hifen antes dos 4 ultimos digitos. Tente novamente: \n"); 
                    fgets(clientes[indice].tel,MAX_TELEFONE, stdin);
                    clientes[indice].tel[strcspn(clientes[indice].tel, "\n")] = '\0';
                    break;
                case 4:
                    printf("Telefone inválido - numero de 9 digitos que comeca sem o 9. Tente novamente: \n"); 
                    fgets(clientes[indice].tel,MAX_TELEFONE, stdin);
                    clientes[indice].tel[strcspn(clientes[indice].tel, "\n")] = '\0';
                    break; 
                case 5:
                    printf("Telefone inválido - numero digitado com caracter nao-numerico. Tente novamente: \n"); 
                    fgets(clientes[indice].tel,MAX_TELEFONE, stdin);
                    clientes[indice].tel[strcspn(clientes[indice].tel, "\n")] = '\0';
                    break;  
                case 6:
                    printf("Telefone inválido! Tente novamente: \n"); 
                    fgets(clientes[indice].tel,MAX_TELEFONE, stdin);
                    clientes[indice].tel[strcspn(clientes[indice].tel, "\n")] = '\0';
                    break;  
            }
        }

        printf("E-mail: ");
        fgets(clientes[indice].email,MAX_EMAIL, stdin);
        clientes[indice].email[strcspn(clientes[indice].email, "\n")] = '\0';
        
        while (validar_email(clientes[indice].email) !=0) {
            switch (validar_email(clientes[indice].email)) {
                case 1:
                    printf("\nE-mail invalido - longo demais. Tente novamente: ");
                    fgets(clientes[indice].email, MAX_EMAIL, stdin);
                    clientes[indice].email[strcspn(clientes[indice].email, "\n")] = '\0';
                    break;
                case 2: 
                    printf("\nE-mail invalido - foi digitado um caracter invalido. Tente novamente: ");
                    fgets(clientes[indice].email, MAX_EMAIL, stdin);
                    clientes[indice].email[strcspn(clientes[indice].email, "\n")] = '\0';
                    break;
                case 3:
                    printf("\nE-mail invalido - mais de um @. Tente novamente: ");
                    fgets(clientes[indice].email, MAX_EMAIL, stdin);
                    clientes[indice].email[strcspn(clientes[indice].email, "\n")] = '\0';
                    break;
                case 4:
                    printf("\nE-mail invalido - hífen em posição inválida. Tente novamente: ");
                    fgets(clientes[indice].email, MAX_EMAIL, stdin);
                    clientes[indice].email[strcspn(clientes[indice].email, "\n")] = '\0';
                    break;
                case 5:
                    printf("\nE-mail invalido - ponto em posição inválida. Tente novamente: ");
                    fgets(clientes[indice].email, MAX_EMAIL, stdin);
                    clientes[indice].email[strcspn(clientes[indice].email, "\n")] = '\0';
                    break;
                case 6:
                    printf("\nE-mail invalido - dominio de email nao contem ponto. Tente novamente: ");
                    fgets(clientes[indice].email, MAX_EMAIL, stdin);
                    clientes[indice].email[strcspn(clientes[indice].email, "\n")] = '\0';
                    break;
                case 7:
                    printf("\nE-mail invalido - domínio de e-mail contém dois pontos seguidos. Tente novamente: ");
                    fgets(clientes[indice].email, MAX_EMAIL, stdin);
                    clientes[indice].email[strcspn(clientes[indice].email, "\n")] = '\0';
                    break;
            }
        }

    }
    
    total_clientes += qtd_novos_clientes;
    printf("\n=======================================================\n");
    printf("\nCadastro(s) realizado(s) com sucesso!\n\n");
    printf("Voltando para o menu...\n\n");
    menu();
 } 
 
 // função de busca de clientes
void buscar_clientes() {
    
    int escolha; 
    char cpf[MAX_CPF];                  // cria variavel para ler o cpf que deseja buscar


    while(1) {                          //cria um while para se repetir até que o usuário deseje sair
        bool encontrado = false; 

        printf("\n\n=======================================================\n");
        printf("                   Busca de clientes                   \n");
        printf("=======================================================\n");
    
        printf("1 - Buscar cliente\n2 - Cancelar\n");
        scanf("%d", &escolha);
        getchar(); 
    
        if (escolha != 1 && escolha != 2) {
            printf("Escolha invalida. Tente novamente: "); 
        }
        
        if (escolha == 1){ 
            printf("\nDigite o CPF do cliente que deseja buscar informacoes: \n");
            fgets(cpf, MAX_CPF, stdin); 
            cpf[strcspn(cpf, "\n")] = '\0'; 
            
            for (int i = 0; i < MAX_CLIENTES; i++) { 
                if (strcmp(cpf, clientes[i].cpf) == 0){
                    // imprimindo as informacoes
                    printf("\n================= Cliente encontrado ==================\n");
                    printf("\nNome completo: %s", clientes[i].nome);
                    printf("\nCPF: %s", clientes[i].cpf);
                    printf("\nData de nascimento: %02d/%02d/%04d", clientes[i].data[0], clientes[i].data[1], clientes[i].data[2]);
                    printf("\nTelefone: %s", clientes[i].tel);
                    printf("\nE-mail: %s", clientes[i].email);
                    encontrado = true; 
                    break; 
                }
            }
                
            if (!encontrado){ 

                printf("\nCliente não encontrado :/\n"); 
                printf("Deseja cadastrar esse cliente?\n"); 
                printf("1 - Sim\n2 - Nao\n"); 
                
                int deseja_cad; 
                scanf("%d", &deseja_cad); 
                getchar(); 
                
                if (deseja_cad == 1) { 
                    cadastrar_clientes();
                }

                while (deseja_cad != 1 && deseja_cad != 2) { 
                    printf("\nEscolha invalida! Tente novamente: \n");
                    scanf("%d", &deseja_cad);
                    getchar(); 
                }
            }
        }
    
        if (escolha == 2 ) {
            printf("\nVoltando para o menu...\n\n");
            menu();
            return;
        }
    }
}


// função para ordenar aniversariantes do mês
 void listar_aniversariantes() {
    struct Cadastro aniversariantes[MAX_CLIENTES];  
    int count = 0;
    int mes_desejado, c;
    int clientes_mes = 0;
    
    printf("\n\n=======================================================\n");
    printf("                    Aniversariantes                    \n");
    printf("=======================================================\n");
    printf("\n\nVoce gostaria de ver os aniversariantes de qual mes?\n");
    scanf("%d", &mes_desejado);
    while ((c = getchar()) != '\n' && c != EOF); // limpa buffer do scanf
    
    while (mes_desejado<1 || mes_desejado>12) {
        printf("\nMes invalido. Tente outro: ");
        scanf("%d", &mes_desejado);
        while ((c = getchar()) != '\n' && c != EOF); // limpa buffer do scanf
    }

    // definir a quantidade de aniversariantes naquele mês
    for (int i = 0; i< MAX_CLIENTES; i++) {
        if (clientes[i].data[1] == mes_desejado) clientes_mes++;
    }

    // filtrar aniversários do mês desejado
    for (int i = 0; i < MAX_CLIENTES; i++) {
        if (clientes[i].nome[0] != '\0' && clientes[i].data[1] == mes_desejado) {
            aniversariantes[count++] = clientes[i];
        }
    }

    // ordenar aniversariantes por dia - via bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (aniversariantes[j].data[0] > aniversariantes[j + 1].data[0]) {
                struct Cadastro temporario = aniversariantes[j];        // utilização de recurso temporário para o bubble sort
                aniversariantes[j] = aniversariantes[j + 1];
                aniversariantes[j + 1] = temporario;
            }
        }
    }

     // exibir aniversariantes
    if (count == 0) {
        printf("\nNenhum aniversariante encontrado no mês %02d. ://\n", mes_desejado);
    } else {
        printf("\nExiste(m) %d aniversariante(s) nesse mes u.u\n\n", clientes_mes);
        printf("====================================================\n");
        printf("Aniversariantes do mês %02d:\n", mes_desejado);
        for (int i = 0; i < count; i++) {
            printf("- %02d/%02d: %s\n", aniversariantes[i].data[0], aniversariantes[i].data[1], aniversariantes[i].nome);
        }
        printf("\n====================================================\n");
    }
    int escolha_volta_menu;
    printf("\nGostaria de voltar ao menu?\n1 - Sim\n2 - Nao\n");
    scanf("%d", &escolha_volta_menu);
    while (escolha_volta_menu < 1 || escolha_volta_menu > 2) {
        printf("Opcao invalida. Tente novamente: ");
        scanf("%d", &escolha_volta_menu);
    }
    if (escolha_volta_menu == 1) menu();
    else {
    listar_aniversariantes();
    }
}
 
 
// menu principal 
 void menu() {
    int opcao_menu;
    printf("\n=================== Menu ===================\n");
    printf("\nO que voce gostaria de fazer agora?\n");
    printf("\n1 - Cadastrar novos clientes\n");
    printf("2 - Buscar dados de clientes\n");
    printf("3 - Aniversários do mês\n");
    printf("4 - Encerrar programa\n\n");
    scanf("\n%d", &opcao_menu);
    while (opcao_menu<1 || opcao_menu>4) {
        printf("Opcao invalida. Tente novamente:\n");
        scanf("%d", &opcao_menu);
    }
    
    if (opcao_menu == 4) {
        int erro = 5;
        if (erro) {
            printf("\nEncerrando o programa...\n");
            exit(5);                            // Sai do programa com código 5 (erro)
        }
    }

    if (opcao_menu == 1) cadastrar_clientes();
    if (opcao_menu == 2) buscar_clientes();
    if (opcao_menu == 3) listar_aniversariantes();
}


int main() {
    struct Cadastro cadastros[MAX_CLIENTES];
    printf("===========================================\n");
    printf("====== Ola! Somos a ChoCãoLatudos :D ======\n");
    printf("====== Eh um prazer ter você conosco ======\n");
    printf("===========================================\n\n");
    printf("Parece que eh sua primeira vez usando nosso sistema. Para começar, sera preciso realizar ao menos 1 cadastro de novo cliente. Vamos começar?\n");
    cadastrar_clientes();
    
    return 0;
}
