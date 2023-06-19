/** @file
 * Interfejs struktury vector przechowywującej znaki
 *
 * @authors Michał Orżanowski <mo439981@students.mimuw.edu.pl>
 * 
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef VECTOR_CHAR_H
#define VECTOR_CHAR_H

#include <stddef.h>
#include <stdbool.h>

/** 
 * To jest struktura przechowująca wierchołek vectora.
 */
typedef struct vector_char {
    /** Napis */
    char *items;
    /** Długość vektora */
    size_t length;
    /** Pojemność vektora */
    size_t capacity;
} vector_char;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnego znaku.
 * @return Wartość @p true, jeśli struktura została stworzona.
 *         Wartość @p false, w przeciwnym wypadku
 */
bool vector_init(vector_char *);

/** @brief Dodaje element na strukture.
 * Alokuje pamięć na nowy element dodany do struktury.
 * @return Wartość @p true, jeśli nowy element został dodany do struktury.
 *         Wartość @p false, w przeciwnym wypadku
 */
bool vector_add(vector_char *, char);

/** @brief Usuwa strukture.
 * Zwalnia pamięć zaalokowaną na podaną strukture.
 * @return Wartość @p true, jeśli struktura została zwolniona.
 *         Wartość @p false, w przeciwnym wypadku
 */
void vector_free(vector_char *);

/** @brief Sprawdza czy struktura jest pusta.
 * Sprawdza czy struktura jest pusta.
 * @return Wartość @p true, jeśli struktura jest pusta.
 *         Wartość @p false, w przeciwnym wypadku
 */
bool vector_is_empty(vector_char *);

#endif