/*
 * biblioteca.c
 *
 *  Created on: 26 de mai. de 2026
 *      Author: 1A291045134
 */

#include "biblioteca.h"

void menu()
{
	char opcao;
	Secao *primeiro = NULL;
	Secao *ultimo = NULL;
	Secao *secao;
	int codSecao;
	int codLivro;

	do
	{
		printf("----------------"
			   "\nA - Inserir"
			   "\nB - Excluir"
			   "\nC - Relatórios"
			   "\nD - Finalizar"
			   "\nDigite uma opção: ");
		scanf(" %c", &opcao);
		opcao = toupper(opcao);

		switch (opcao)
		{
		case 'A':
			do
			{
				printf("----------------"
					   "\n1 - Inserir nova seção"
					   "\n2 - Inserir livro em uma seção existente"
					   "\n3 - Voltar"
					   "\nDigite uma opção: ");
				scanf(" %c", &opcao);
				opcao = toupper(opcao);

				switch (opcao)
				{
				case '1':
					inserirNovaSecao(&primeiro, &ultimo);
					break;

				case '2':
					printf("----------------");
					codSecao = solicitarCodigo("\nDigite o código da seção: ");
					secao = buscarSecao(primeiro, codSecao);
					if (secao != NULL)
					{
						if (secao->status == 'I')
						{
							printf("Esta sessão está Inativa...\n");
						}
						else
						{
							inserirNovoLivro(secao);
						}
					}
					break;

				case '3':
					break;

				default:
					printf("Opção inválida...\n");
					break;
				}
			} while (opcao != '3');
			break;

		case 'B':
			if (primeiro == NULL)
			{
				printf("Nenhuma sessão criada...\n");
				break;
			}

			do
			{
				printf("----------------"
					   "\n1 - Excluir seção"
					   "\n2 - Excluir livro de uma seção"
					   "\n3 - Voltar"
					   "\nDigite uma opção: ");
				scanf(" %c", &opcao);
				opcao = toupper(opcao);

				switch (opcao)
				{
				case '1':
					printf("----------------");
					do
					{
						codSecao = solicitarCodigo("\nDigite o código da seção: ");
						secao = buscarSecao(primeiro, codSecao);

					} while (secao == NULL);

					excluirSecao(&primeiro, &ultimo, secao);
					break;

				case '2':
					printf("----------------");
					codSecao = solicitarCodigo("\nDigite o código da seção: ");
					secao = buscarSecao(primeiro, codSecao);
					codLivro = solicitarCodigo("Digite o código do livro: ");
					excluirLivro(secao, &secao->livros, codLivro);
					break;

				case '3':
					break;

				default:
					printf("Opção inválida...");
					break;
				}
			} while (opcao != '3');
			break;

		case 'C':
			if (primeiro == NULL)
			{
				printf("Nenhuma sessão criada...\n");
				break;
			}

			do
			{
				printf("----------------"
					   "\n1 - Listar todas as seções e seus livros"
					   "\n2 - Listar dados de uma seção por código"
					   "\n3 - Listar livros por status"
					   "\n4 - Voltar"
					   "\nDigite uma opção: ");
				scanf(" %c", &opcao);
				opcao = toupper(opcao);

				switch (opcao)
				{
				case '1':
					listarSecoes(primeiro);
					break;

				case '2':

					printf("----------------");
					do
					{
						codSecao = solicitarCodigo("\nDigite o código da seção: ");
						secao = buscarSecao(primeiro, codSecao);
					} while (secao == NULL);

					listarDadosSecao(secao);
					break;

				case '3':
					printf("----------------"
						   "\nLivros Disponíveis\n");
					listarLivrosStatus(primeiro, primeiro->livros, 'D');
					printf("----------------"
						   "\nLivros Emprestados\n");
					listarLivrosStatus(primeiro, primeiro->livros, 'E');
					break;

				case '4':
					break;

				default:
					printf("Opção inválida...\n");
					break;
				}
			} while (opcao != '4');

			break;

		case 'D':
			printf("\nPrograma finalizado...");

			break;

		default:
			printf("Opção inválida...\n");
			break;
		}
	} while (opcao != 'D');
}

void inserirNovaSecao(Secao **primeiro, Secao **ultimo)
{
	static int codSecao = 1;
	Secao *novo = (Secao *)malloc(sizeof(Secao));
	char string[51] = "\0";

	novo->codigo = codSecao++;

	do
	{
		printf("----------------"
			   "\nDigite o nome da seção: ");
		scanf(" %[^\n]", string);
		if (string[0] == '\0')
		{
			printf("Este campo não pode ser vazio...\n");
		}
	} while (string[0] == '\0');
	validarString(string, 0, strlen(string));
	strncpy(novo->nome, string, sizeof(novo->nome) - 1);

	validarData(&novo->data);

	novo->qtdLivros = 0;

	do
	{
		printf("Seção Ativa ou Inativa?(A/I): ");
		scanf(" %c", &novo->status);
		novo->status = toupper(novo->status);
		if (novo->status != 'A' && novo->status != 'I')
		{
			printf("Opção inválida...\n");
		}
	} while (novo->status != 'A' && novo->status != 'I');

	novo->livros = NULL;

	novo->proximo = NULL;
	if ((*ultimo) != NULL)
	{
		(*ultimo)->proximo = novo;
		novo->anterior = (*ultimo);
	}
	else
	{
		novo->anterior = NULL;
		(*primeiro) = novo;
	}
	(*ultimo) = novo;

	printf("Seção inserida com sucesso!\n");
	printf("É necessário inserir um livro para esta seção.\n");
	inserirNovoLivro(novo);
}

Secao *buscarSecao(Secao *secao, int codigo)
{

	if (secao == NULL)
	{
		printf("Seção não encontrada...\n");
		return NULL;
	}

	if (secao->codigo != codigo)
	{
		return buscarSecao(secao->proximo, codigo);
	}
	return secao;
}

void inserirNovoLivro(Secao *secao)
{
	int validaCorretamente;
	Livro *novo = (Livro *)malloc(sizeof(Livro));
	char string[51] = "\0";

	if (secao->livros == NULL)
	{
		novo->codigo = (secao->codigo * 1000) + 1;
	}
	else
	{
		novo->codigo = (secao->livros)->codigo + 1;
	}

	do
	{
		printf("----------------"
			   "\nDigite o título do livro: ");
		scanf(" %[^\n]", string);
		if (string[0] == '\0')
		{
			printf("Este campo não pode ser vazio...\n");
		}
	} while (string[0] == '\0');
	validarString(string, 0, strlen(string));
	strncpy(novo->titulo, string, sizeof(novo->titulo) - 1);

	do
	{
		printf("Digite o nome do autor: ");
		scanf(" %[^\n]", string);
		if (string[0] == '\0')
		{
			printf("Este campo não pode ser vazio...\n");
		}
	} while (string[0] == '\0');
	validarString(string, 0, strlen(string));
	strncpy(novo->autor, string, sizeof(novo->autor) - 1);

	do
	{
		printf("Digite o ano de publicação: ");
		validaCorretamente = scanf("%i", &novo->anoPublicacao);

		if (validaCorretamente == 0)
		{
			printf("Ano de publicação inválido...\n");
			scanf("%*c");

			while (getchar() != '\n')
				;
			;

			novo->anoPublicacao = 9999;
			continue;
		}

		if (novo->anoPublicacao > 2026)
		{
			printf("O ano de publicação não pode ser maior que o atual...\n");
		}
	} while (novo->anoPublicacao > 2026);

	do
	{
		printf("Livro Disponível ou Emprestado?(D/E): ");
		scanf(" %c", &novo->status);
		novo->status = toupper(novo->status);
		if (novo->status != 'D' && novo->status != 'E')
		{
			printf("Opção inválida...\n");
		}
	} while (novo->status != 'D' && novo->status != 'E');

	novo->proximo = secao->livros;
	secao->livros = novo;
	secao->qtdLivros++;
}

void excluirSecao(Secao **primeiro, Secao **ultimo, Secao *excluir)
{
	if (excluir == NULL)
	{
		return;
	}

	Livro *proxExclusao;
	while (excluir->livros != NULL)
	{
		proxExclusao = (excluir->livros)->proximo;
		free(excluir->livros);
		excluir->livros = proxExclusao;
	}

	// É o primeiro da lista
	if (excluir->anterior == NULL)
	{
		*primeiro = excluir->proximo;
		if (*primeiro != NULL)
		{
			(*primeiro)->anterior = NULL;
		}
		else
		{
			*ultimo = NULL;
		}
	}
	// É o último da lista
	else if (excluir->proximo == NULL)
	{
		*ultimo = excluir->anterior;
		if (*ultimo != NULL)
		{
			(*ultimo)->proximo = NULL;
		}
	}
	// Está no meio da lista
	else
	{
		(excluir->proximo)->anterior = excluir->anterior;
		(excluir->anterior)->proximo = excluir->proximo;
	}

	printf("Seção excluida com sucesso!\n");
	free(excluir);
}

void excluirLivro(Secao *secao, Livro **livro, int codLivro)
{
	if (secao == NULL || livro == NULL || *livro == NULL)
	{
		printf("Livro com código %d não encontrado.\n", codLivro);
		return;
	}

	if (secao->qtdLivros <= 1)
	{
		printf("A seção só possui um livro, que não pode ser excluído...\n");
		return;
	}

	if ((*livro)->codigo == codLivro)
	{
		Livro *excluir = *livro;

		*livro = (*livro)->proximo;

		free(excluir);
		secao->qtdLivros--;
		printf("Livro %d excluído com sucesso!\n", codLivro);
		return;
	}

	excluirLivro(secao, &(*livro)->proximo, codLivro);
}

int solicitarCodigo(char *frase)
{
	int codigo;
	int validarCodigo;
	do
	{
		printf("%s", frase);
		validarCodigo = scanf("%d", &codigo);

		if (validarCodigo == 0)
		{
			printf("Código inválido...\n");
			scanf("%*c");

			while (getchar() != '\n')
				;

			codigo = -1;
			continue;
		}

		if (codigo < 0)
		{
			printf("Código inválido...\n");
		}
	} while (codigo < 0);
	return codigo;
}

void validarString(char *string, int index, int tamanho)
{
	if (index == tamanho)
	{
		return;
	}

	if (index == 0 || string[index - 1] == ' ')
	{
		if (islower(string[index]))
		{
			string[index] = toupper(string[index]);
		}
	}
	else
	{
		if (isupper(string[index]))
		{
			string[index] = tolower(string[index]);
		}
	}

	validarString(string, index + 1, tamanho);
}

void validarData(Data *data)
{
	char dataString[12];
	int valido = 0;

	while (!valido)
	{
		printf("Digite a data de criação (DD/MM/YYYY): ");
		scanf(" %11s", dataString);

		// Se não ler os 3 números, repete
		if (sscanf(dataString, "%d/%d/%d", &data->dia, &data->mes, &data->ano) != 3)
		{
			printf("Formato invalido...\n");
			continue;
		}

		// se o ano é bissexto (1 pra sim, 0 pra nao)
		int bissexto = (data->ano % 4 == 0 && data->ano % 100 != 0) || (data->ano % 400 == 0);

		// Validação
		if (data->ano < 1000 || data->ano < 1 || data->ano > 2026 || data->mes < 1 || data->mes > 12 || data->dia < 1 || (data->dia > 31) || ((data->mes == 4 || data->mes == 6 || data->mes == 9 || data->mes == 11) && data->dia > 30) || (data->mes == 2 && bissexto && data->dia > 29) || (data->mes == 2 && !bissexto && data->dia > 28))
		{

			printf("Data deve ter o formato de dd/mm/yyy! Tente novamente...\n");
		}
		else
		{
			valido = 1;
		}
	}
}

void listarSecoes(Secao *secao)
{
	printf("----------------"
		   "\nSeção Nº%i - %s\n",
		   secao->codigo, secao->nome);
	listarLivros(secao->livros);

	if (secao->proximo != NULL)
	{
		listarSecoes(secao->proximo);
	}
}

void listarLivros(Livro *livro)
{
	printf("%i: \"%s\"(%i) - %s\n", livro->codigo, livro->titulo,
		   livro->anoPublicacao, livro->autor);

	if (livro->proximo != NULL)
	{
		listarLivros(livro->proximo);
	}
}

void listarDadosSecao(Secao *secao)
{
	printf("----------------"
		   "\nSeção Nº%i - %s"
		   "\nData de Criação: %i/%i/%i"
		   "\nStatus: %c"
		   "\nQtd de Livros: %i\n",
		   secao->codigo, secao->nome,
		   (secao->data).dia, (secao->data).mes, (secao->data).ano,
		   secao->status, secao->qtdLivros);
}

void listarLivrosStatus(Secao *secao, Livro *livro, char status)
{
	if (livro->status == status)
	{
		printf("%i: \"%s\"(%i) - %s\n", livro->codigo, livro->titulo,
			   livro->anoPublicacao, livro->autor);
	}

	if (livro->proximo != NULL)
	{
		listarLivrosStatus(secao, livro->proximo, status);
		return;
	}

	if (secao->proximo != NULL)
	{
		listarLivrosStatus(secao->proximo, (secao->proximo)->livros, status);
	}
}