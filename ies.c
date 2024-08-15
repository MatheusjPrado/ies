#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_CHAMADOS 10
#define MAX_SENHAS 1000
#define MAX_PACIENTES 50

typedef struct {
    char senha[5];  
    int guiche;
    char tipo[15]; 

} Chamado;

Chamado painelChamados[MAX_CHAMADOS];
int totalChamados = 0;

Chamado filaPreferencial[MAX_PACIENTES];
int totalPreferencial = 0;

Chamado filaComum[MAX_PACIENTES];
int totalComum = 0;

int proximaSenha = 100;             // inicializar com a senha numero 100 e depois vai aumentando


void gerarSenha(char* senha) {                  
    sprintf(senha, "G%d", proximaSenha);                 // encadeia "G" com a senha
    proximaSenha++;
}

// funcao para obter a data e hora atual
void obterDataHora(char* dataHora) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(dataHora, 20, "%d/%m/%Y %H:%M", tm_info);
}

void registrarPaciente(int tipo) {
    Chamado chamado;

    // gera um número de guichê aleatório entre 1 e 3 para simular um guiche disponivel
    chamado.guiche = 1 + rand() % 3;

   
    gerarSenha(chamado.senha);

    if (tipo == 1) {
        strcpy(chamado.tipo, "Preferencial");
        filaPreferencial[totalPreferencial++] = chamado;
    } else {
        strcpy(chamado.tipo, "Comum");
        filaComum[totalComum++] = chamado;
    }
}

// funcao para exibir o painel de chamados com data, hora e cidade
void apresentarPainel() {
    if (totalChamados == 0) {
        printf("Nenhum chamado registrado no painel.\n");
        return;
    }

    char dataHora[20];
    obterDataHora(dataHora);

    // Limpa a tela do terminal
    #ifdef _WIN32
        system("cls");  // Comando para limpar a tela no Windows
    #else
        system("clear");  // Comando para limpar a tela no Linux
    #endif

  
    printf("\nPainel de Chamados - Foz do Iguacu - Parana | Data e Hora: %s\n", 
           dataHora);
    printf("Chamado Mais Recente: Guiche %d - Senha: %s - Tipo: %s\n",
           painelChamados[totalChamados - 1].guiche,
           painelChamados[totalChamados - 1].senha,
           painelChamados[totalChamados - 1].tipo);


    printf("\nUltimos Chamados:\n");
    for (int i = totalChamados - 2; i >= 0; i--) {
        printf("Guiche %d - Senha: %s - Tipo: %s\n",
               painelChamados[i].guiche,
               painelChamados[i].senha,
               painelChamados[i].tipo);
    }
}

void chamarProximoPaciente() {
    Chamado chamado;

    if (totalPreferencial > 0) {
        chamado = filaPreferencial[0];
        for (int i = 0; i < totalPreferencial - 1; i++) {
            filaPreferencial[i] = filaPreferencial[i + 1];
        }
        totalPreferencial--;
    } else if (totalComum > 0) {
        chamado = filaComum[0];
        for (int i = 0; i < totalComum - 1; i++) {
            filaComum[i] = filaComum[i + 1];
        }
        totalComum--;
    } else {
        printf("Nenhum paciente na fila.\n");
        return;
    }

    if (totalChamados >= MAX_CHAMADOS) {
        // se o painel estiver cheio, remove o chamado mais antigo
        for (int i = 0; i < MAX_CHAMADOS - 1; i++) {
            painelChamados[i] = painelChamados[i + 1];
        }
        totalChamados--;
    }

    painelChamados[totalChamados++] = chamado;
    printf("Chamado registrado: Guiche %d - Senha: %s - Tipo: %s\n",
           chamado.guiche, chamado.senha, chamado.tipo);


    apresentarPainel();
}


int main() {
    srand(time(NULL));

    int opcao, tipo;

    do {
        printf("\nSistema de Gerenciamento de Atendimento\n");
        printf("1. Registrar Paciente\n");
        printf("2. Chamar Proximo Paciente\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Tipo de Paciente (1: Preferencial, 2: Comum): ");
                scanf("%d", &tipo);
                registrarPaciente(tipo);
                printf("Paciente registrado!\n");
                break;

            case 2:
                chamarProximoPaciente();
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 0);

    return 0;
}
