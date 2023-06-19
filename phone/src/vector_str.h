/** @file
 * Interfejs struktury vector przechowywującej napisy
 *
 * @authors Michał Orżanowski <mo439981@students.mimuw.edu.pl>
 * 
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef VECTOR_STR_H
#define VECTOR_STR_H

#include <stddef.h>
#include <stdbool.h>

/** 
 * To jest struktura przechowująca wierchołek vectora.
 */
typedef struct vector_str {
    /** Tablica napisów */
    char **items;
    /** Długość vektora */
    size_t length;
    /** Pojemność vektora */
    size_t capacity;
} vector_str;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych napisów.
 * @return Wartość @p true, jeśli struktura została stworzona.
 *         Wartość @p false, w przeciwnym wypadku
 */
bool vector_str_init(vector_str *);

/** @brief Dodaje element na strukture.
 * Alokuje pamięć na nowy element dodany do struktury.
 * @return Wartość @p true, jeśli nowy element został dodany do struktury.
 *         Wartość @p false, w przeciwnym wypadku
 */
bool vector_str_add(vector_str *, char *);

/** @brief Usuwa strukture.
 * Zwalnia pamięć zaalokowaną na podaną strukture.
 * @return Wartość @p true, jeśli struktura została zwolniona.
 *         Wartość @p false, w przeciwnym wypadku
 */
void vector_str_free(vector_str *);

#endif