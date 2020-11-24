#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <iostream>
#include "functions.h"
#include "telas.h"

int main(void){
	FILE *pacientes, *agenda;
	int op;
	do{
		pacientes = fopen("dados.dat","rb+");
		agenda = fopen("agenda.dat","rb+");
		TelaPrincipal();
		printf("================ %d Pacientes ====\n",Tamanho(pacientes, sizeof(SPaciente)));
		printf("--> ");
		scanf("%d",&op);
		switch (op){
			case 1:
				TelaCadastroPaciente();
				Cadastrar(pacientes);
				break;
			case 2:
				TelaAgendamento();
				if(Tamanho(pacientes, sizeof(SPaciente)) != 0){
					AgendamentoConsulta(pacientes, agenda);
				}else{
					printf("Nao ha pacientes cadastrados.\n\n");
					system("pause");
				}
				break;
			case 3:
				TelaAlteracaodePacientes();
				if(Tamanho(pacientes, sizeof(SPaciente)) != 0){
					//AlteracaoPaciente(pacientes);
				}else{
					printf("Nao ha pacientes cadastrados.\n\n");
					system("pause");
				}
				break;
			case 4:
				TelaVisualizarConsultas();
				if(Tamanho(pacientes, sizeof(SPaciente)) != 0){
					MostrarConsultas(pacientes, agenda);
				}else{
					printf("Nao ha pacientes cadastrados.\n\n");
					system("pause");
				}
				break;
			case 5:
				TelaGerarTxt();
				if(Tamanho(pacientes, sizeof(SPaciente)) != 0){
					GerarTxt(pacientes, agenda);
				}else{
					printf("Nao ha pacientes cadastrados.\n\n");
					system("pause");
				}
				break;
			case 6: printf("SAINDO\n"); break;
			default: printf("Opcao Invalida.\n"); system("pause"); break;
		}
	}while(op != 6);
	return 1;
}
