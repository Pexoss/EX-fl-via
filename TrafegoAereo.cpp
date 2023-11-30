#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

void exibirDesenho() {
    printf("=============================================\n");
    printf("||            Controle de Tráfego Aéreo     ||\n");
    printf("=============================================\n");
}

struct reg {
    char companhia[20], aeronave[8];
    int horarios;
    char status;
};

void imprimirTracos(int n) {
    for (int i = 0; i < n; i++) {
        printf("=");
    }
    printf("\n");
}

int tamanho(FILE *);

void cadastrar(FILE *);

void consultar(FILE *);

void excluir(FILE *);

int main() {
    setlocale(LC_ALL, "Portuguese");
    int escolha;

    FILE *arq;

    if ((arq = fopen("F:\\registro\\reg.dat", "rb+")) == NULL) {
        if ((arq = fopen("F:\\registro\\reg.dat", "wb+")) == NULL) {
            printf("Erro ao abrir o arquivo.\n");
            return 1;
        }
    }

    do {
        exibirDesenho();
        printf("\n\n");
        imprimirTracos(60);
        printf("Controle de Tráfego Aéreo\n");
        imprimirTracos(60);

        imprimirTracos(30);
        printf("MENU DE OPÇÕES\n");
        imprimirTracos(30);
        printf("Escolha uma das opções abaixo:\n");
        printf("1- Cadastrar aeronave\n");
        printf("2- Consultar aeronave\n");
        printf("3- Excluir aeronave\n");
        printf("4- Sair\n");

        printf("Escolha uma opção (1-4): ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                imprimirTracos(30);
                printf("Opção escolhida: Cadastrar aeronave\n");
                cadastrar(arq);
                break;
            case 2:
                imprimirTracos(30);
                printf("Opção escolhida: Consultar aeronave\n");
                consultar(arq);
                break;
            case 3:
                imprimirTracos(30);
                printf("Opção escolhida: Excluir aeronave\n");
                excluir(arq);
                break;
            case 4:
                imprimirTracos(30);
                printf("Opção escolhida: Sair\n");
                break;
            default:
                imprimirTracos(30);
                printf("Opção inválida.\n");
                getchar();
                system("pause");
                system("cls");
        }

    } while (escolha != 4);

    printf("Sistema encerrado.\n");

    return 0;
}

void cadastrar(FILE *arq) {
    reg voo;
    char confirma;
    voo.status = ' ';

    printf("Cadastrando novo registro:\n");
    printf("\nNúmero do registro:%d\n", tamanho(arq) + 1);
    printf("Companhia..................:");
    getchar();
    fgets(voo.companhia, sizeof(voo.companhia), stdin);
    voo.companhia[strcspn(voo.companhia, "\n")] = '\0';
    printf("Aeronave...................:");
    fgets(voo.aeronave, sizeof(voo.aeronave), stdin);
    voo.aeronave[strcspn(voo.aeronave, "\n")] = '\0';
    printf("Horários de operação......:");
    scanf("%d", &voo.horarios);
    printf("\nConfirma <s/n>:");
    getchar();
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S') {
        printf("\nGravando...\n");
        fseek(arq, 0, SEEK_END);
        fwrite(&voo, sizeof(reg), 1, arq);
        fflush(arq); // Adicionando esta linha para garantir a escrita imediata no arquivo
        printf("Registro gravado com sucesso.\n");
    }

    system("pause");
    system("cls");
}

void consultar(FILE *arq) {
    reg voo;
    char nomeConsulta[20];

    printf("Digite a companhia para consultar voos cadastrados: ");
    getchar();
    fgets(nomeConsulta, sizeof(nomeConsulta), stdin);
    nomeConsulta[strcspn(nomeConsulta, "\n")] = '\0';

    fseek(arq, 0, SEEK_SET);

    int encontrado = 0;

    while (fread(&voo, sizeof(reg), 1, arq) == 1) {
        if (strcmp(nomeConsulta, voo.companhia) == 0 && voo.status == ' ') {
            encontrado = 1;
            printf("\nSituação atual da aeronave:\n");
            printf("Companhia: %s\n", voo.companhia);
            printf("Aeronave: %s\n", voo.aeronave);
            printf("Horários de operação: %d\n", voo.horarios);
            printf("Status: %c\n", voo.status);
            break;
        }
    }

    if (!encontrado) {
        printf("\nAeronave não encontrada.\n");
    }

    system("pause");
    system("cls");
}

int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(reg);
}

void excluir(FILE *arq) {
    reg voo;
    char nomeExcluir[20];
    char confirma;

    printf("Informe a companhia do voo para excluir: ");
    getchar();
    fgets(nomeExcluir, sizeof(nomeExcluir), stdin);
    nomeExcluir[strcspn(nomeExcluir, "\n")] = '\0';

    fseek(arq, 0, SEEK_SET);
    int encontrado = 0;
    int posicao = 0;

    while (fread(&voo, sizeof(reg), 1, arq) == 1) {
        posicao++;

        if (strcmp(nomeExcluir, voo.companhia) == 0 && voo.status == ' ') {
            encontrado = 1;
            printf("\nRegistro encontrado:\n");
            printf("Companhia......:%s\n", voo.companhia);
            printf("Aeronave...:%s\n", voo.aeronave);
            printf("Horários de operação:%d\n", voo.horarios);
            printf("Confirma a exclusão: <s/n>\n");
            scanf(" %c", &confirma);

            if (toupper(confirma) == 'S') {
                printf("\nExcluindo...\n\n");
                fseek(arq, (posicao - 1) * sizeof(reg), SEEK_SET);
                voo.status = '*';
                fwrite(&voo, sizeof(reg), 1, arq);
                fflush(arq); // Adicionando esta linha para garantir a escrita imediata no arquivo
                printf("Registro excluído com sucesso.\n");
                system("pause");
                system("cls");
            } else {
                printf("Exclusão cancelada.\n");
                system("pause");
                system("cls");
            }

            break;
        }
    }

    if (!encontrado) {
        printf("\nRegistro não encontrado ou já excluído.\n");
    }
    system("pause");
    system("cls");
}

