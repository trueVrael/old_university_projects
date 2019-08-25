#ifndef __TRIE_H__
#define __TRIE_H__

#include <stdbool.h>
#include <wchar.h>

/**
 *   Struktura przechowująca słownik.
 *     */

typedef struct trie{
        wchar_t key;
        struct trie *next;
        struct trie *prev;
        struct trie *children;
        struct trie *parent;
}trie;


/**
 *   Inicjalizacja trie.
 *     Trie ten należy zniszczyć za pomocą dictionary_done().
 *       @return Nowe trie
 *         */
struct trie * trie_new(void);

/**
 *   Destrukcja trie.
 *     @param[in,out] t Trie.
 *       */
void trie_done(struct trie *t);


/**
 *   Wstawia podane słowo do słownika.
 *     @param[in,out] t Trie.
 *       @param[in] word Słowo, które należy wstawić do trie.
 *         @return 0 jeśli słowo było już w trie, 1 jeśli udało się wstawić.
 *           */
int trie_insert(struct trie *t, const wchar_t* word, int licznik[200]);


/**
 *   Usuwa podane słowo z trie, jeśli istnieje.
 *     @param[in,out] t Trie.
 *       @param[in] word Słowo, które należy usunąć z trie.
 *         @return 1 jeśli udało się usunąć, zero jeśli nie.
 *           */
int trie_delete(struct trie *t, const wchar_t* word, int licznik[200]);


/**
 *   Sprawdza, czy dane słowo znajduje się w trie.
 *     @param[in] t Trie.
 *       @param[in] word Słowo, które należy usunąć z trie
 *         @return Wartość logiczna czy `word` jest w słowniku.
 *           */
bool trie_find(const struct trie *t, const wchar_t* word);

/**
 *   Zapisuje słownik.
 *     @param[in] dict Słownik.
 *       @param[in,out] stream Strumień, gdzie ma być zapisany słownik.
 *         @return <0 jeśli operacja się nie powiedzie, 0 w p.p.
 *           */
int trie_save(const struct trie *dict, FILE* stream);
#endif /* __TRIE_H__ */
