/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 *
 * @author Michał Orżanowski <mo439981@students.mimuw.edu.pl>
 * @copyright Michał Orżanowski
 * @date 2022
 */

#include "phone_forward.h"
#include "vector_char.h"
#include "vector_str.h"
#include "stack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** To jest struktura oparta na drzewie trie, w której przechowywane są: 
przekierowanie, wskaźniki na synów, wskaźnik na ojca.
Sama struktura reprezentuje wierchołek, który jest cyfrą. */
struct PhoneForward {
    /** Wskaźnik na ojca */
    PhoneForward * parent;
    /** Przekierowanie */
    char * replacement;
    /** Wskażniki na dzieci */
    PhoneForward * child[12];
};

/** To jest struktura zawierająca tablice numerów i długość tej tablicy */
struct PhoneNumbers {
    /** Tablica numerów */
    char ** numbers;
    /** Długość tablicy numerów */
    size_t length;
};

PhoneForward * phfwdNew(void) {
    PhoneForward * result = malloc(sizeof(PhoneForward));
    if(result == NULL)
        return NULL;
    else {
        result->replacement = NULL;
        result->parent = NULL;
        for(int i = 0; i < 12; i++) {
            result->child[i] = NULL;
        }
        return result;
    }
}

/** Funkcja tworząca strukture PhoneNumbers która nie ma w sobie rzadnego numeru */
PhoneNumbers * phnumNew() {
    PhoneNumbers *result = malloc(sizeof(PhoneNumbers));
    if (result == NULL)
        return NULL;
    result->numbers = NULL;
    result->length = 0;
    return result;
}

/** Funkcja tworząca strukture PhoneNumbers z jednym numerem */
PhoneNumbers * phnumNewOne(char *s) {
    PhoneNumbers *result = malloc(sizeof(PhoneNumbers));
    if (result == NULL) {
        free(s);
        return NULL;
    }
    result->numbers = malloc(sizeof (char *) * 1);
    if (result->numbers == NULL) {
        free(s);
        free(result);
        return NULL;
    }
    result->length = 1;
    result->numbers[0] = s;
    return result;
}

/** Funkcja tworząca strukture PhoneNumbers z wieloma numerami numerem */
PhoneNumbers * phnumNewMulti(vector_str v) {
    PhoneNumbers *result = malloc(sizeof(PhoneNumbers));
    if (result == NULL) {
        vector_str_free(&v);
        return NULL;
    }
    result->numbers = v.items;
    result->length = v.length;
    return result;
}

char const * phnumGet(PhoneNumbers const *pnum, size_t idx) { 
    if (pnum == NULL)
        return NULL;
    if (idx >= pnum->length)
        return NULL;
    return pnum->numbers[idx];
}

void phnumDelete(PhoneNumbers *pnum) {
    if(pnum == NULL)
        return;
    /* Zwalniam kolejne numery */
    for(size_t i = 0; i < pnum->length; i++) {
        free(pnum->numbers[i]);
    }
    free(pnum->numbers);
    free(pnum);
}

/** @brief Funkcja porównująca numery.
 * Porównuje znak po znaku czy @p num1 i @p num2 są równe.
 * @param[in] num1   – wskaźnik na napis reprezentujący pierwszy
 *                     numer;
 * @param[in] num2   – wskaźnik na napis reprezentujący drugi
 *                     numer.
 * @return Wartość @p true, jeśli numery są równe.
 *         Wartość @p false, w przeciwnym przypadku.
 */
static bool numCompare(char const *num1, char const *num2) {
    size_t length1 = strlen(num1);
    size_t length2 = strlen(num2);
    if (length1 != length2)
        return false;
    for(size_t i = 0; i < length1; i++) {
        if (num1[i] != num2[i])
            return false;
    }
    return true;
}

/** @brief Funkcja kopiująca numery.
 * Przepisuje znak po znaku @p num.
 * @param[in] num    – wskaźnik na napis reprezentujący numer
 * @return Wskażnik na numer. Wartość NULL jeżeli nie udało się zaalokować
           pamieci.
 */
static char * numCopy(char const *num) {
    size_t length = strlen(num);
    char *result = malloc(length + 1);
    if (result == NULL)
        return NULL;
    for (size_t i = 0; i < length; i++) {
        result[i] = num[i];
    }
    result[length] = '\0';
    return result;
}

/** @brief Funkcja sprawdzająca czy podany napis jest numerem.
 * Porównuje znak po znaku czy @p num składa się wyłącznie z cyfr.
 * @param[in] num    – wskaźnik na napis reprezentujący numer
 * @return Wartość @p true, jeśli napis jest numerem.
 *         Wartość @p false, w przeciwnym przypadku.
 */
static bool numCheck(char const *num) {
    if (num == NULL)
        return false;
    if (num[0] == '\0')
        return false;
    size_t i = 0;
    while((num[i] >= '0' && num[i] <= '9') || num[i] == '#' || num[i] == '*') {
        i++;
    }
    return num[i] == '\0';
}

/** @brief Funkcja zamieniająca znak na liczbe.
 * Zamienia znak @p c na odpowiadający mu numer.
 * @param[in] c    – znak.
 * @return Wartość liczby reprezenującej dany znak.
 */
static size_t charToNum(char const c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c == '*')
        return 10;
    else if (c == '#')
        return 11;
    else
        /* Nie zdaży się */
        return -1;
}

/** @brief Funkcja zamieniająca znak na liczbe.
 * Zamienia liczbe @p n na odpowiadający mu znak.
 * @param[in] n    – liczbe.
 * @return Znak reprezenujący daną liczbe.
 */
static char numToChar(size_t n) {
    if (n <= 9)
        return n + '0';
    else if (n == 10)
        return '*';
    else if (n == 11)
        return '#';
    else
        /* Nie zdaży się */
        return '0';
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
    /* Sprawdzam czy podane wartości są poprawne */
    if (pf == NULL) 
        return false;
    if ((!numCheck(num1) || !numCheck(num2)) || numCompare(num1, num2)) 
        return false;
    size_t length1 = strlen(num1);
    /* Szukam najdłuższego prefixu */
    for(size_t i = 0; i < length1; i++) {
        size_t dig = charToNum(num1[i]);
        /* Jeżeli dany wierzchołek należący do prefixu nie istnieje tworze go */
        if (pf->child[dig] == NULL) {
            PhoneForward *newChild = phfwdNew();
            if (newChild == NULL)
                return false;
            newChild->parent = pf;
            pf->child[dig] = newChild;
            pf = newChild;
        }
        /* Przechodze do wierchołka z prefixu jeżeli istnieje */
        else {
            pf = pf->child[dig];
        }
    }
    /* Ustawiam przekierowanie na podany numer */
    free(pf->replacement);
    pf->replacement = numCopy(num2);
    return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
    /* Sprawdzam czy podane wartości są poprawne */
    PhoneForward *sub = pf;
    if(pf == NULL) 
        return;
    if(!numCheck(num))
        return;
    /* Szukam najdłuższy pasujący prefix numeru */
    size_t length = strlen(num);
    for(size_t i = 0; i < length; i++) {
        if (sub != NULL) {
            size_t dig = charToNum(num[i]);
            sub = sub->child[dig];
        }
    }
    if (sub != NULL) {
        /* Przypadek 1: Usuwam znalezione poddrzewo jeżeli nie jest całym drzewem*/
        if(sub->parent != NULL) {
            for(size_t i = 0; i < 12; i++) {
                if (sub->parent->child[i] == sub) {
                    sub->parent->child[i] = NULL;
                }
            }
            sub->parent = NULL;
            phfwdDelete(sub);
        }
        /* Przypadek 2: Usuwam całe drzewo zostawiając pusty korzeń */
        else {
            free(sub->replacement);
            sub->replacement = NULL;
            for(size_t i = 0; i < 12; i++) {
                PhoneForward *child = sub->child[i];
                sub->child[i] = NULL;
                if(child != NULL)
                    child->parent = NULL;
                phfwdDelete(child);
            }
        }
    }
}

void phfwdDelete(PhoneForward *pf) {
    while(pf != NULL) {
        /* Czyszcze połączenia miedzy wierzchołkami */
        con:
        for(size_t i = 0; i < 12; i++) {
            if (pf->child[i] != NULL) {
                PhoneForward *parent = pf;
                pf = pf->child[i];
                parent->child[i] = NULL;
                goto con;
            }
        }
        /* Czyszcze zawartości wierzchołków */
        PhoneForward *parent = pf->parent;
        free(pf->replacement);
        free(pf);
        pf = parent;
    }
}

PhoneNumbers * phfwdGet(PhoneForward const *pf, char const *num) {
    /* Sprawdzam czy podane wartości są poprawne */
    if (pf == NULL)
        return NULL;
    if (num == NULL)
        return phnumNew();
    if (!numCheck(num))
        return phnumNew();
    /* Szukam najdłuższy prefiks pasujący do num */
    size_t length = strlen(num);
    size_t pref_length = length;
    const PhoneForward *node = pf;
    for(size_t i = 0; i < length; i++) {
        size_t dig = charToNum(num[i]);
        if (node->child[dig] == NULL) {
            pref_length = i;
            break;
        }
        node = node->child[dig];
    }
    /* Cofam się do najdłuższego prefiksu zawierającego przekierowanie */
    while(node != NULL && node->replacement == NULL) {
        node = node->parent;
        pref_length--;
    }
    /* Sprawdzam czy dany numer został przekierowany */
    if (node == NULL)
        return phnumNewOne(numCopy(num));
    /* Wyznaczam przekierowanie łącząc przekierowanie najdłuższego prefiksu i reszte podanego numeru */
    size_t replacement_length = strlen(node->replacement);
    char *result = malloc(length - pref_length + replacement_length + 1);
    if(result == NULL)
        return NULL;
    for(size_t i = 0; i < replacement_length; i++) {
        result[i] = node->replacement[i];
    }
    size_t k = replacement_length;
    for(size_t i = pref_length; i < length; i++) {
        result[k] = num[i];
        k++;
    }
    result[length - pref_length + replacement_length] = '\0';
    return phnumNewOne(result);
}

/** @brief Funkcja porównująca numery leksykograficznie.
 * Porównuje znak po znaku czy @p num1 i @p num2 są równe.
 * @param[in] p1   – wskaźnik na napis reprezentujący pierwszy
 *                     numer;
 * @param[in] p2   – wskaźnik na napis reprezentujący drugi
 *                     numer.
 * @return Wartość @p >=1, jeśli drugi numer jest prefixem pierwszego.
 *         Wartość @p <=-1, jeśli pierwszy numer jest prefixem drugiego.
 *         Wartość @p 0, jeśli numery są równe.
 */
static int compar(const void *p1, const void *p2) {
    const char *s1 = *(const char **) p1;
    const char *s2 = *(const char **) p2;
    size_t i = 0;
    while(s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return charToNum(s1[i]) - charToNum(s2[i]);
        }
        else
            i++;
    }
    if (s1[i] == '\0')
        return -1;
    else if (s2[i] == '\0')
        return 1;
    else
        return 0;
    
}

PhoneNumbers * phfwdReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL)
        return NULL;
    if (num == NULL)
        return phnumNew();
    if (!numCheck(num))
        return phnumNew();
    stack s;
    vector_char pref;
    vector_str ans;
    if (!stack_init(&s))
        return NULL;
    if (!vector_init(&pref)) {
        stack_free(&s);
        return NULL;
    }
    if (!vector_str_init(&ans)) {
        vector_free(&pref);
        stack_free(&s);
        return NULL;
    }
    char *num2 =  malloc(strlen(num) + 1);
    if (num2 == NULL) {
        vector_str_free(&ans);
        vector_free(&pref);
        stack_free(&s);
        free(num2);
        return NULL;
    }
    strcpy(num2, num);
    if (!vector_str_add(&ans, num2)) {
        vector_str_free(&ans);
        vector_free(&pref);
        stack_free(&s);
        return NULL;
    }
    if (!stack_add(&s, (entry){0, pf})) {
        vector_str_free(&ans);
        vector_free(&pref);
        stack_free(&s);
        return NULL;
    }
    while(!stack_is_empty(&s)) {
        if (s.items[s.length - 1].idx == 12) {
            if(pref.length > 0)
                pref.length--;
            s.length--;
            if (!stack_is_empty(&s))
                s.items[s.length - 1].idx++;
            continue;
        }
        pf = s.items[s.length - 1].node;
        pf = pf->child[s.items[s.length - 1].idx];
        if (pf != NULL) {
            if (!vector_add(&pref, numToChar(s.items[s.length - 1].idx))) {
                vector_str_free(&ans);
                vector_free(&pref);
                stack_free(&s);
                return NULL;
            }
            if (pf->replacement != NULL) { 
                size_t len = strlen(pf->replacement);
                if (strncmp(pf->replacement, num, len) == 0) {
                    size_t len_num = strlen(num);
                    char *result = malloc(pref.length + len_num - len + 1);
                    if (result == NULL) 
                        return NULL;
                    memcpy(result, pref.items, pref.length);
                    memcpy(result + pref.length, num + len, len_num - len + 1);
                    if(!vector_str_add(&ans, result)) {
                        free(result);
                        vector_str_free(&ans);
                        vector_free(&pref);
                        stack_free(&s);
                        return NULL;
                    }
                }
            }
            if(!stack_add(&s, (entry){0, pf})) {
                vector_str_free(&ans);
                vector_free(&pref);
                stack_free(&s);
                return NULL;
            }
        }
        else
            s.items[s.length - 1].idx++;
    }

    qsort(ans.items, ans.length, sizeof(char *), compar);

    size_t left = 0;
    if (ans.length != 0) {
        for(size_t right = 1; right < ans.length; right++) {
            if (strcmp(ans.items[left], ans.items[right]) == 0) {
                free(ans.items[right]);
            }
            else {
                left++;
                ans.items[left] = ans.items[right];
            }
        }
        left++;
    }
    ans.length = left;
    stack_free(&s);
    vector_free(&pref);
    return phnumNewMulti(ans);
}

PhoneNumbers * phfwdGetReverse(PhoneForward const *pf, char const *num) {
    if (pf == NULL)
        return NULL;
    if (num == NULL)
        return phnumNew();
    if (!numCheck(num))
        return phnumNew();
    stack s;
    vector_char pref;
    vector_str ans;
    if (!stack_init(&s))
        return NULL;
    if (!vector_init(&pref)) {
        stack_free(&s);
        return NULL;
    }
    if (!vector_str_init(&ans)) {
        vector_free(&pref);
        stack_free(&s);
        return NULL;
    }
    char *num2 =  malloc(strlen(num) + 1);
    if (num2 == NULL) {
        vector_str_free(&ans);
        vector_free(&pref);
        stack_free(&s);
        return NULL;
    }
    strcpy(num2, num);
    if (!vector_str_add(&ans, num2)) {
        vector_str_free(&ans);
        vector_free(&pref);
        stack_free(&s);
        free(num2);
        return NULL;
    }
    if (!stack_add(&s, (entry){0, pf})) {
        vector_str_free(&ans);
        vector_free(&pref);
        stack_free(&s);
        return NULL;
    }
    while(!stack_is_empty(&s)) {
        if (s.items[s.length - 1].idx == 12) {
            if(pref.length > 0)
                pref.length--;
            s.length--;
            if (!stack_is_empty(&s))
                s.items[s.length - 1].idx++;
            continue;
        }
        pf = s.items[s.length - 1].node;
        pf = pf->child[s.items[s.length - 1].idx];
        if (pf != NULL) {
            if (!vector_add(&pref, numToChar(s.items[s.length - 1].idx))) {
                vector_str_free(&ans);
                vector_free(&pref);
                stack_free(&s);
                return NULL;
            }
            if (pf->replacement != NULL) { 
                size_t len = strlen(pf->replacement);
                if (strncmp(pf->replacement, num, len) == 0) {
                    size_t len_num = strlen(num);
                    char *result = malloc(pref.length + len_num - len + 1);
                    if (result == NULL) 
                        return NULL;
                    memcpy(result, pref.items, pref.length);
                    memcpy(result + pref.length, num + len, len_num - len + 1);
                    if(!vector_str_add(&ans, result)) {
                        free(result);
                        vector_str_free(&ans);
                        vector_free(&pref);
                        stack_free(&s);
                        return NULL;
                    }
                }
            }
            if(!stack_add(&s, (entry){0, pf})) {
                vector_str_free(&ans);
                vector_free(&pref);
                stack_free(&s);
                return NULL;
            }
        }
        else
            s.items[s.length - 1].idx++;
    }

    qsort(ans.items, ans.length, sizeof(char *), compar);

    size_t left = 0;
    if (ans.length != 0) {
        for(size_t right = 1; right < ans.length; right++) {
            if (strcmp(ans.items[left], ans.items[right]) == 0) {
                free(ans.items[right]);
            }
            else {
                left++;
                ans.items[left] = ans.items[right];
            }
        }
        left++;
    }
    ans.length = left;

    size_t left2 = 0;
    
    for(size_t right2 = 0; right2 < ans.length; right2++) {
        PhoneNumbers *a = phfwdGet(pf, ans.items[right2]);
        if (a == NULL) {
            stack_free(&s);
            vector_free(&pref);
            vector_str_free(&ans);
            return NULL;
        }
        else if (strcmp(phnumGet(a, 0), num) != 0) {
            free(ans.items[right2]);
        }
        else {
            ans.items[left2] = ans.items[right2];
            left2++;
        }
    }
    
    ans.length = left2;
    
    stack_free(&s);
    vector_free(&pref);
    return phnumNewMulti(ans);
}