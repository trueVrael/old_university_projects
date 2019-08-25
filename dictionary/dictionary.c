/** @file
  Implementacja słownika na  trie.
  Tomasz Kaszlewicz
 */

#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

#define _GNU_SOURCE

/**
  Struktura przechowująca słownik.
  Zwykłe trie
 */
/** @name Elementy interfejsu
   @{
*/
typedef struct dictionary{
	trie* dic;
}dictionary;
int licznik[200]={0};
//licznik liter w słowniku

dictionary* dictionary_new (){
	dictionary* t = (dictionary*) malloc(sizeof(dictionary));
        t->dic = trie_new();	
	return t;
}
/**
  funkcja pomocna w kilku funkcjach słownika
  @param[in, out] t, słownik, na którym operuje
  @param[in] key, szukane słowo
  @return wskaźnik do słowa szukanego
*/
/*
dictionary* dictionary_find2(dictionary* t, const wchar_t* key){
	dictionary *level = t;
	dictionary *pPtr = NULL;
	while(1){
		dictionary *found = NULL;
		dictionary *curr;
		for(curr = level; curr != NULL; curr = curr->next){
			if(curr->key == *key){
				found = curr;
				break;
			}
		}
		if(found == NULL) return NULL;
		if(*key == '\0'){
			pPtr = curr;
			return pPtr;
		}
		level = found->children;
		key++;
	}
}*/
/**
  Usuwa słownik z pamięci
  @param[in] t Słownik do usuniecia
*/
void dictionary_done(dictionary* t){
/*	dictionary *tPtr = t;
	dictionary *tmp = t;
	while(tPtr){
		while(tPtr->children) tPtr = tPtr->children;
		if(tPtr->prev && tPtr->next){
			tmp = tPtr;
			tPtr->next->prev = tPtr->prev;
			tPtr->prev->next = tPtr->next;
			free(tmp);
		}
		else if(!(tPtr->prev) && tPtr->next){
			tmp = tPtr;
			tPtr->parent->children = tPtr->next;
			tPtr->next->prev = NULL;
			tPtr = tPtr-> next;
			free(tmp);
		}
		else{
			tmp = tPtr;
			if(tPtr->parent == NULL){
				free(tmp);
				return;
			}
			tPtr = tPtr->parent;
			tPtr->children = NULL;
			free(tmp);
		}
	}*/
	trie_done(t->dic);
}
/**
  Zapisuje słownik do pliku
  @param[in,out] t słownik, który zapisuje
  @param[out] stream, plik do którego zapisuje
  @return 1, jeśli zapis się udał, 0 w.p.p
*/
int dictionary_save(const dictionary* t, FILE* stream){
/*	if(t == NULL)
		return 0;
	if(t->key != '\0'){	
		if(fprintf(stream, "%lc\n", t->key) < 0)
			return -1;
	}
	else{
	        if(fprintf(stream, "!\n") < 0)
	                return -1;
	}
	dictionary *k = NULL;
	dictionary *n = NULL;
	dictionary *n2 = NULL;
	int licz = 0;
	if (t->children != NULL){
		k = t->children;
		licz++;
	}
	if(licz > 0){
		n = k->next;
		n2 = k->next;
		while(n != NULL){
			licz++;
			n = n->next;
		}
	}
	
	if(fprintf(stream, "%d\n", licz) < 0)
		return -1;	
	dictionary_save(k, stream);
	
	while(n2 != NULL){
		dictionary_save(n2, stream);
		n2 = n2->next;
	}
	return 0;	*/
	trie_save(t->dic, stream);
}

/**
  zllicza liczbe wystapień każdej litery słowa
  @param[in] key słowo, którego litery zliczam
  @pram[in] k, jeśli k=1 to dodaje litery, a w.p.p odejmuje litery
*/
/*
void licz(const wchar_t* key, int k){
	int wint;
	int i=0;
	while(key[i] != '\0'){
		wint = (int)key[i];
		if(k==1) licznik[wint-97]++;
		else licznik[wint-97]--;
		i++;
	}
}*/

/**
  stwórz wierzchołek
  @param[in] key, wartość key dla wierzchołka
*//*
dictionary *Trie_create_node(wchar_t key){
	dictionary *node = dictionary_new();
	node->key = key;
	return node;
}*/

/**
  Włóż słowo do słownika słownika
  @param[in,out] t słownik, do którego wstawiam słowo
  @param[in] key, wstawiane slowo
  @return 1 jeśli sie udało, 0 w.p.p
*/
int dictionary_insert(dictionary* t, const wchar_t* key){
/*	if(key != NULL) licz(key,1);
	dictionary *pTra = NULL;
	if(t == NULL) return 0;
	pTra = t->children;
	if(pTra == NULL){
		for(pTra = t; *key; pTra = pTra->children){
			pTra->children = Trie_create_node(*key);
			pTra->children->parent = pTra;
			key++;
		}
		pTra->children = Trie_create_node(L'\0');
		pTra->children->parent = pTra;
		return 1;
	}
	if(dictionary_find2(pTra, key)) return 0; //duplikat
	while(*key != '\0'){
		if(*key == pTra->key){
			key++;
			pTra = pTra->children;
		}
		else break;
	}
	while(pTra->next){
		if(*key == pTra->next->key){
			key++;
			return dictionary_insert(pTra->next, key);
		}
		pTra = pTra->next;
	}
	pTra->next = Trie_create_node(*key);
	pTra->next->parent = pTra->parent;
	pTra->next->prev = pTra;
	if(!(*key)) return 1;
	key++;
	for(pTra = pTra->next; *key; pTra= pTra->children){
		pTra->children = Trie_create_node(*key);
		pTra->children->parent = pTra;
		key++;
	}
	pTra->children = Trie_create_node(L'\0');
	pTra->children->parent = pTra;
	return 1;*/
	trie_insert(t->dic,key,licznik);
}

/**
  Usuń |key| ze słownika |t|.
  @param[in,out] t Słownik, z którego usuwam słowo
  @param[in] key, słowo do usuniecia
  return 1, jeśli się powiodło, 0 w.p.p
*/
int dictionary_delete(dictionary* t, const wchar_t* key){
/*	dictionary *tPtr = NULL;
	dictionary *tmp = NULL;
	if(t == NULL || key == NULL) return 0;
	if(key != NULL) licz(key, -1);
	tPtr = dictionary_find2(t->children, key);
	if(tPtr == NULL) return 0;
	while(1){
		if(tPtr->prev && tPtr->next){
			tmp = tPtr;
			tPtr->next->prev = tPtr->prev;
			tPtr->prev->next = tPtr->next;
			free(tmp);
			break;
		}
		else if(tPtr->prev && !(tPtr->next)){
			tmp = tPtr;
			tPtr->prev->next = NULL;
			free(tmp);
			break;
		}
		else if(!(tPtr->prev) && tPtr->next){
			tmp = tPtr;
			tPtr->parent->children = tPtr->next;
			free(tmp);
			break;
		}
		else{
			tmp = tPtr;
			tPtr = tPtr->parent;
			tPtr->children = NULL;
			free(tmp);
		}
	}
	return 1;*/
	trie_delete(t->dic, key,licznik);
}
/**
  Szuka słowa w słowniku
  @param[in.out] t, Słownik, w którym szuka
  @param[in] key, poszukiwane słowo
  @return true, jesli słowo jest w słowniku, false w.p.p
*/
bool dictionary_find(const dictionary* t, const wchar_t* key){
/*	dictionary *ans = dictionary_find2(t->children, key);
	if(ans == NULL) return false;
	return true;*/
	trie_find(t->dic,key);
}
/**
  Funkcja pomocnicza do load
*/
void next(wchar_t* buf, FILE* stream, dictionary* dict,int cnt){
	wchar_t w[128];
	wchar_t buf2[128];
	wchar_t* tmp;
	int s;
	wcscpy(buf2,buf);
	fscanf(stream, "%ls %d", w, &s);
	if(w[0] != L'!'){
		if(buf != L"") tmp = wcscat(buf, w);
		else tmp = w;
	}
	else{
		tmp = buf;
	}
	if(s == 0)
		dictionary_insert(dict,tmp);
	else
		next(tmp, stream, dict,s);
	cnt--;
	if(cnt>0){
		next(buf2, stream, dict, cnt);
	}
	if(ferror(stream)){
		dictionary_done(dict);
		dict = NULL;
	}
}
/**
  Wczytuje słownik
  @param[in] stream, plik, z którego pobieram słownik
  @return Słownik
*/
struct dictionary * dictionary_load(FILE* stream){
	struct dictionary *dict = dictionary_new();
	wchar_t buf[128]=L"";
	wchar_t hax[] =L"";
	wchar_t* tmp;
	int l;
	fscanf(stream, "%ls %d",buf, &l);	
	while(fscanf(stream, "%ls %d", buf, &l) != EOF){
		tmp = buf;
		if(l == 0){
			tmp = wcscat(buf, hax);
			dictionary_insert(dict, tmp);
		}
		else
			next(tmp, stream,dict,l);
		
	}
	if(ferror(stream)){
		dictionary_done(dict);
		dict = NULL;
	}
	return dict;
}
/**
  Funkcja sortująca
*/
int compare_elements(const void *v1, const void *v2){
	wchar_t * const *p1 = v1;
	wchar_t * const *p2 = v2;
	return wcscoll(*p1,*p2);
}

/**
  Sprawdza czy w słowniku |dict| istnieją słowa
  podobne do słowa |word|. Jeśli tak to zapisuje
  je do listy |list|
  @param[in,out] dict Słownik, który przeszukuje
  @param[in,out] list Lista, do której dodaje słowa podobne
  @param[in] word, słowo, do którego szukam podobnych
*/
void dictionary_hints(const struct dictionary *dict, const wchar_t* word,
        struct word_list *list){
    	word_list_init(list);
    	size_t wlen = wcslen(word);
	wchar_t tmp[wlen];
//dlugosc -1
	for(size_t i=0; i<wlen; i++){
		int x=0;
		for(size_t j=0; j <wlen; j++){
			if(i != j){
				tmp[x]=word[j];
				x++;
			}
		}
		tmp[wlen-1] = '\0';
		if(dictionary_find(dict, tmp) == true){
			if(word_list_unique(list, tmp))
				word_list_add(list,tmp);
		}
	}
//dlugosc
wchar_t tmp2[wlen+1];
	for(size_t i=0; i<wlen; i++){
		for(int l=0; l<200; l++){
			if((licznik[l]) && ((wchar_t)(l+97) != word[i])){
				for(int j=0; j<wlen; j++){
					if(i!=j)tmp2[j]=word[j];
					else tmp2[j] = (wchar_t)(l+97);
				}
				tmp2[wlen] = '\0';
				//tmps[i] = (wchar_t)(l+97);
				if(dictionary_find(dict, tmp2) == true){
					if(word_list_unique(list, tmp2))
						word_list_add(list,tmp2);
				}
			}
		}
	}
//dlugosc +1
wchar_t tmp3[wlen+2];
	for(size_t i=0; i<=wlen; i++){
		for(int l=0; l<200; l++){
			if(licznik[l]){
				int x=0;
				tmp3[i]=(wchar_t)(l+97);
				for(int j=0; j<wlen; j++){
					if(i==j) x++;
					tmp3[x]=word[j];
					x++;
				}
				tmp3[wlen+1] = '\0';
				if(dictionary_find(dict, tmp3) == true){
					if(word_list_unique(list,tmp3))
						word_list_add(list,tmp3);
				}
			}
		}
	}
	qsort(list->array,list->size, sizeof(wchar_t*), compare_elements);
}
/**@}*/
