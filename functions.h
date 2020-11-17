#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#define TAM 100

typedef char string[TAM];

struct SData{
	int dia, mes, ano;
};

struct SHora{
	int hora, minutos;
};

struct SPaciente{
	string cod;
	string nome;
	string convenio;
	string telefone;
	string celular;
};

struct SConsulta{
	SData data_consulta;
	SHora hora_consulta;
	char codigo[TAM];
	char tipo_consulta[TAM];
};

int Tamanho(FILE *arq, int estrutura){ // funcao para verificar quantos pacientes estao cadastrados
	fseek(arq, 0, SEEK_END);
	return ftell(arq) / estrutura;
}

int CodigoInvalido(FILE *arq, char *cod){ // funcao que valida o codigo do paciente
	int i, tam_arq;
	SPaciente p;
	tam_arq = Tamanho(arq, sizeof(SPaciente));
	for(i=0;i<tam_arq;i++){
		fseek(arq,i*sizeof(SPaciente), SEEK_SET);
		fread(&p, sizeof(SPaciente), 1, arq);
		if(strcmp(cod, p.cod) == 0){
			return 1;
		}
	}return 0;
}

int DataeHorarioInvalido(FILE *arq, SConsulta c){ // funcao que valida a DATA e HORA da consulta
	int i=0;
	int termina_validacao = 1;
	SConsulta con;
	do{
		if(((c.data_consulta.dia >= 1) && (c.data_consulta.dia <= 31)) && ((c.data_consulta.mes >= 1) && (c.data_consulta.mes <= 12))){
			if((c.hora_consulta.hora >= 7) && (c.hora_consulta.hora <= 20)){				
				while(termina_validacao){
					fseek(arq, i * sizeof(SConsulta), SEEK_SET);
					fread(&con, sizeof(SConsulta), 1, arq);
					i++;
					if((con.data_consulta.dia == c.data_consulta.dia) && (con.data_consulta.mes == c.data_consulta.mes) && (con.data_consulta.ano == c.data_consulta.ano)){
						if((con.hora_consulta.hora == c.hora_consulta.hora) && (con.hora_consulta.minutos == c.hora_consulta.minutos)){
							printf("Ja ha consultas agendadas nesta data.\n");						
							return 1;
						}else{			
							termina_validacao = 0;							
							return 0;
						}						
					}else{
						termina_validacao = 0;						
						return 0;
					}					
				}
			}else{
				printf("Consultas so podem ser marcadas entre 07 e 19:55\n");
				return 1;
			}								
		}else{
			printf("Dia ou mes invalido.\n");
			return 1;
		}		
	}while(termina_validacao);
}

void Cadastrar(FILE *arq){  // funcao de cadastro de pacientes
	arq = fopen("dados.dat","ab+");
	SPaciente paciente;
	char confirma;
	do{
		printf("Codigo do paciente: ");
		scanf("%s",&paciente.cod);
		if(CodigoInvalido(arq, paciente.cod)){
			printf("O codigo inserido ja existe.\n");
		}
	}while(CodigoInvalido(arq, paciente.cod));
	printf("Nome do Paciente..: ");
	scanf("%s",&paciente.nome);
	printf("Nome do convenio..: ");
	scanf("%s",&paciente.convenio);
    printf("Telefone Fixo.....: ");
	scanf("%s",&paciente.telefone);
    printf("Celular...........: ");
	scanf("%s",&paciente.celular);
	fflush(stdin);
	printf("\nConfirma <S/N>: ");
	scanf(" %c", &confirma);
	if (toupper(confirma) == 'S'){
		fseek(arq, 0, SEEK_END);
		fwrite(&paciente, sizeof(SPaciente), 1, arq);
		printf("Paciente cadastrado!\n\n");
		system("pause");
	}
	fclose(arq);
}

void AgendamentoConsulta(FILE *arq, FILE *con){  // funcao de agedamento de consultas
	con = fopen("agenda.dat","ab+");
	SConsulta agendamento;
	char confirma;
	do{ // validacao codigo paciente
        printf("Informe o codigo do paciente: ");
        scanf("%s",&agendamento.codigo);
        if(!CodigoInvalido(arq,agendamento.codigo)){
            printf("Codigo inexistente.\n");
        }
	}while(!CodigoInvalido(arq, agendamento.codigo));
	do{ // validacao hora e data
		printf("Data da consulta...: ");
		scanf("%d %*c %d %*c %d",&agendamento.data_consulta.dia, &agendamento.data_consulta.mes, &agendamento.data_consulta.ano);
		printf("Horario da consulta: ");
		scanf("%d %*c %d",&agendamento.hora_consulta.hora, &agendamento.hora_consulta.minutos);
	}while(DataeHorarioInvalido(con, agendamento));
	printf("Tipo da Consulta[Retorno ou Consulta]: ");
	scanf("%s",&agendamento.tipo_consulta);
	fflush(stdin);
	printf("\nConfirma<S/N>: ");
	scanf(" %c",&confirma);
	if(tolower(confirma) == 's'){
		fseek(con,0,SEEK_END);
		fwrite(&agendamento, sizeof(SConsulta),1,con);
		printf("Horario agendado.\n\n");
		system("pause");
	}
	fclose(con);
}

void MostrarConsultas(FILE *arq, FILE *con){  // funcao que exibe os horarios das consultas e o nome do paciente
	con = fopen("agenda.dat","rb+");
	int i=0, tam_arq_pac;
	long tam_total;
	SPaciente p;
	SConsulta c;
	tam_arq_pac = Tamanho(arq, sizeof(SPaciente));
	for(i=0;i<tam_arq_pac;i++){
		fseek(arq, i * sizeof(SPaciente), SEEK_SET);
		fread(&p, sizeof(SPaciente), 1, arq);
		printf("Paciente: %s\n",p.nome);
		printf("-------------------------------------\n");
		fseek(con, i * sizeof(SConsulta), SEEK_SET);
		fread(&c, sizeof(SConsulta), 1, con);
		if(strcmp(p.cod, c.codigo) == 0){
			printf("Data da consulta: %d/%d/%d\n",c.data_consulta.dia, c.data_consulta.mes, c.data_consulta.ano);
			printf("Hora da consulta: %d:%d\n",c.hora_consulta.hora, c.hora_consulta.minutos);
			printf("Tipo da consulta: %s\n",c.tipo_consulta);
		}else{
			printf("O paciente nao tem consulta marcada.\n");
		}
		printf("-------------------------------------\n\n");								
	}
	fclose(con);
	system("pause");
}

void GerarTxt(FILE *dad, FILE *con){// funcao que gera o arquivo txt
	FILE *arquivo_txt;
	int i, tam_arq;
	char nome_arq[20];
	SPaciente p;
	SConsulta c;
	tam_arq = Tamanho(dad, sizeof(SPaciente));
	printf("Informe o nome do arquivo: ");
	scanf("%s",&nome_arq);
	strcat(nome_arq, ".txt");
	arquivo_txt = fopen(nome_arq,"w+");
	if(!arquivo_txt){
		printf("Falha ao criar o arquivo.\n");
		exit(1);
	}	
	for(i=0;i<tam_arq;i++){
		fseek(dad, i * sizeof(SPaciente), SEEK_SET);
		fread(&p, sizeof(SPaciente), 1, dad);
		fprintf(arquivo_txt,"Paciente: %s\n",p.nome);
		fprintf(arquivo_txt,"-------------------------------------\n");
		fseek(con, i*sizeof(SConsulta), SEEK_SET);
		fread(&c, sizeof(SConsulta), 1, con);
		if(strcmp(p.cod, c.codigo) == 0){
			fprintf(arquivo_txt,"Data da consulta: %d/%d/%d\n",c.data_consulta.dia,c.data_consulta.mes, c.data_consulta.ano);
			fprintf(arquivo_txt,"Hora da consulta: %d:%d\n",c.hora_consulta.hora, c.hora_consulta.minutos);
			fprintf(arquivo_txt,"Tipo da consulta: %s\n",c.tipo_consulta);
		}else{
			fprintf(arquivo_txt,"O paciente nao tem consulta marcada.\n");
		}
		fprintf(arquivo_txt,"-------------------------------------\n\n");								
	}
	fclose(arquivo_txt);
	printf("\nArquivo gerado!\n\n");
	system("pause");	
}

void AlteracaoPaciente(FILE *arq){  // funcao de alteracao dos pacientes
	FILE *arq_aux = fopen("aux.dat","ab+");
	int i, tam_arq;
	char cod[TAM], confirma;
	SPaciente p;
	tam_arq = Tamanho(arq, sizeof(SPaciente));
	do{
        printf("Informe o codigo do paciente: ");
        scanf("%s",&cod);
        if(!CodigoInvalido(arq,cod)){
            printf("Codigo inexistente.\n");
        }
	}while(!CodigoInvalido(arq, cod));
	arq = fopen("dados.dat","wb"); // apaga o arquivo anterior e cria um novo arquivo
	for(i=0;i<tam_arq;i++){
		fseek(arq, i * sizeof(SPaciente), SEEK_SET);
		fread(&p, sizeof(SPaciente),1,arq);
		printf("%s\n",p.cod);
		if(strcmp(cod, p.cod) == 0){
			printf("Nome do paciente: ");
			scanf("%s",&p.nome);
			printf("Convenvio.......: ");
			scanf("%s",&p.convenio);
			printf("Telefone fixo...: ");
			scanf("%s",&p.telefone);
			printf("Celular.........: ");
			scanf("%s",&p.celular);			
			printf("\nConfirma <S/N>: ");
			scanf(" %c",&confirma);
			if(tolower(confirma) == 's'){
				fwrite(&p, sizeof(SPaciente),1,arq);
				printf("\nAlteracao concluida.\n\n");
				system("pause");							
			}
		}else{
			fseek(arq, i * sizeof(SPaciente), SEEK_SET);
			fwrite(&p, sizeof(SPaciente),1,arq);
		}		
	}
	fclose(arq);
	fclose(arq_aux);
}
