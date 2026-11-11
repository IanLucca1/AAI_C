/*
 * biblioteca.h
 *
 *  Created on: 26 de mai. de 2026
 *      Author: 1A291045134
 */

#ifndef BIBLIOTECA_H_
#define BIBLIOTECA_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct dadosData Data;
struct dadosData
{
	int dia;
	int mes;
	int ano;
};

typedef struct dadosLivro Livro;
struct dadosLivro
{
	// Dados
	int codigo;
	char titulo[51];
	char autor[51];
	int anoPublicacao;
	char status;

	// Ponteiros
	Livro *proximo;
};

typedef struct dadosSecao Secao;
struct dadosSecao
{
	// Dados
	int codigo;
	char nome[51];
	Data data;
	char status;
	int qtdLivros;

	// Ponteiros
	Secao *anterior;
	Secao *proximo;

	// Livros
	Livro *livros;
};

void menu();
void inserirNovaSecao(Secao **primeiro, Secao **ultimo);
Secao *buscarSecao(Secao *secao, int codigo);
void inserirNovoLivro(Secao *secao);
void excluirSecao(Secao **primeiro, Secao **ultimo, Secao *excluir);
void excluirLivro(Secao *secao, Livro **livro, int codLivro);
int solicitarCodigo(char *frase);
void validarString(char *string, int index, int tamanho);
void validarData(Data *data);
void listarSecoes(Secao *secao);
void listarLivros(Livro *livro);
void listarDadosSecao(Secao *secao);
void listarLivrosStatus(Secao *secao, Livro *livro, char status);

#endif /* BIBLIOTECA_H_ */
