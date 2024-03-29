/** @name Elementy interfejsu
 *    @{
 *    */
#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>

trie* trie_new (){
	trie* t = (trie*) malloc(sizeof(trie));
	t->key = L'\0';
	t->next = NULL;
	t->children = NULL;
	t->parent = NULL;
	t->prev = NULL;
	return t;
}

void trie_done(trie* t){
	trie *tPtr = t;
	trie *tmp = t;
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
	}
}

void licz(const wchar_t* key, int k,int licznik[200]){
	int wint;
	int i=0;
	while(key[i] != '\0'){
		wint = (int)key[i];
                if(k==1) licznik[wint-97]++;
	        else licznik[wint-97]--;
	        i++;
	}
}

int trie_save(const trie* t, FILE* stream){
	if(t == NULL)
		return 0;
	if(t->key != '\0'){	
		if(fprintf(stream, "%lc\n", t->key) < 0)
			return -1;
	}
	else{
	       if(fprintf(stream, "!\n") < 0)
	                return -1;
	}
	trie *k = NULL;
	trie *n = NULL;
	trie *n2 = NULL;
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
	trie_save(k, stream);
	while(n2 != NULL){
		trie_save(n2, stream);
		n2 = n2->next;
	}
	return 0;	
}

trie *Trie_create_node(wchar_t key){
	trie *node = trie_new();
	node->key = key;
	return node;
}

trie* trie_find2(trie* t, const wchar_t* key){
		trie *level = t;
		trie *pPtr = NULL;
		while(1){
			trie *found = NULL;
			trie *curr;
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
}

int trie_insert(trie* t, const wchar_t* key, int licznik[200]){
	if(key != NULL) licz(key,1,licznik);
	trie *pTra = NULL;
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
	if(trie_find2(pTra, key)) return 0; //duplikat
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
				return trie_insert(pTra->next, key,licznik);
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
		return 1;
}
int trie_delete(trie* t, const wchar_t* key,int licznik[200]){
	trie *tPtr = NULL;
	trie *tmp = NULL;
	if(t == NULL || key == NULL) return 0;
	if(key != NULL) licz(key, -1,licznik);
	tPtr = trie_find2(t->children, key);
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
	return 1;
}
bool trie_find(const trie* t, const wchar_t* key){
	trie *ans = trie_find2(t->children, key);
	if(ans == NULL) return false;
	return true;
}
