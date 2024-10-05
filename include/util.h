// SPDX-License-Identifier: ISC

/**
 * @file util.h
 * @brief Declarações de utilidades miscelâneas.
 *
 * @details Este arquivo contém declarações de utilidades diversas que podem
 * ser usadas em várias partes do projeto. Atualmente, ele define uma macro
 * para verificação de erros (`ERRNOCHECK`), mas outras utilidades miscelâneas
 * poderão ser adicionadas futuramente.
 */

#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <errno.h>
#include <stdio.h>

#include "quicksort.h"
#include "vec.h"

/**
 * @brief Macro para verificação de erros e redirecionamento de fluxo.
 *
 * @details Esta macro verifica uma condição (`condition`) e, se verdadeira,
 * exibe uma mensagem de erro (`errmsg`) junto com detalhes do erro do sistema.
 * Em seguida, redireciona a execução para o rótulo especificado (`label`).
 *
 * @param condition Condição a ser verificada.
 * @param errmsg Mensagem de erro a ser exibida se a condição for verdadeira.
 * @param label Rótulo para onde o fluxo de execução será redirecionado se a
 * condição for verdadeira.
 */
#define ERRNOCHECK(condition, errmsg, label)                           \
	do {                                                           \
		if (condition) {                                       \
			int errsv = errno;                             \
			fprintf(stderr, "%s:%d:%s: Error: ", __FILE__, \
				__LINE__, __func__);                   \
			errno = errsv;                                 \
			perror((errmsg));                              \
			goto label;                                    \
		}                                                      \
	} while (0)

#define ARRLEN(arr) (sizeof(arr) / sizeof(typeof(*arr)))

void swap(int *arr, size_t a, size_t b);
const char *strat_to_str(PivotStrategy s);
const char *type_to_str(VecType t);

#endif // !UTIL_H