/** @file
 * Interfejs struktury vector przechowywującej parę: liczby i wskaźnika
 *
 * @authors Michał Orżanowski <mo439981@students.mimuw.edu.pl>
 * 
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdbool.h>

/** 
 * To jest struktura przechowująca pare: liczby i wskaźnika.
 */
typedef struct entry {
    /** Indeks syna do sprawdzenia */
    int idx;
    /** Wskaźnik na wierzchołek ojca */
    const void *node;
} entry;

/** 
 * To jest struktura przechowująca wierchołek vectora.
 */
typedef struct stack {
    /** Tablica par entry */
    entry *items;
    /** Długość vektora */
    size_t length;
    /** Pojemność vektora */
    size_t capacity;
} stack;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę niezawierającą żadnych par.
 * @return Wartość @p true, jeśli struktura została stworzona.
 *         Wartość @p false, w przeciwnym wypadku
 */
bool stack_init(stack *);

/** @brief Dodaje element na strukture.
 * Alokuje pamięć na nowy element dodany do struktury.
 * @return Wartość @p true, jeśli nowy element został dodany do struktury.
 *         Wartość @p false, w przeciwnym wypadku
 */
bool stack_add(stack *, entry);

/** @brief Usuwa strukture.
 * Zwalnia pamięć zaalokowaną na podaną strukture.
 * @return Wartość @p true, jeśli struktura została zwolniona.
 *         Wartość @p false, w przeciwnym wypadku
 */
void stack_free(stack *);

/** @brief Sprawdza czy struktura jest pusta.
 * Sprawdza czy struktura jest pusta.
 * @return Wartość @p true, jeśli struktura jest pusta.
 *         Wartość @p false, w przeciwnym wypadku
 */
bool stack_is_empty(stack *);

#endif