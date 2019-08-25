#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#define Alphabet 27
typedef int bool;
#define true 1
#define false 0
typedef struct trie_node{
	bool NotLeaf;
	trie_node *pChildren[Alphabet];
	char * word;
}node;

trie_node *NewLeaf(char* key){
	trie_node *t_node;
	int cnt;
	t_node = (trie_node *)malloc(sizeof(trie_node));
	for(cnt = 0; cnt < 27; cnt++)
			t_node->pChildren[cnt] = NULL;
	t_node->NotLeaf = false;
	strcpy(t_node->word,key);
	return t_node;
}
trie_node *NewIntern(){
	trie_node *t_node;
	int cnt;
	t_node = (trie_node *)malloc(sizeof(trie_node));
	for(cnt = 0; cnt < 27; cnt++) t_node->pChildren[cnt] = NULL;
	t_node->NotLeaf = true;
	t_node-> word[0] = 0;
	return t_node;
}
void Find(trie_node *trie, char* key){
	trie_node *next, *index, *data;
	int cnt;
	next = trie;
	if(next == NULL){
		printf("NO\n");
		return;
	}
	else index = next;
	cnt = 0;
	while((index->NotLeaf == true) && (cnt < strlen(key)) && (index->pChildren[key[cnt]-'a'] != NULL)){
						  next = index->pChildren[key[cnt]-'a'];
						  index = next;
						  cnt++;	
	} 
	if(next == NULL) printf("NO\n");
	else{
		data = next;
		if(!strcmp(data->word,key)) printf("YES\n");
		else{
			if((data->pChildren[26]) && !strcmp(data->pChildren[26]->word,key))
									 printf("YES\n");
		    else printf("NO\n");
		}
	}
}
trie_node *Insert(trie_node *trie, char* key){
	trie_node *next, *index, *parent;
	trie_node *new_leaf, *data, *new_index;
	trie_node *oldChildren, *newWord, *intern;
	int inWordIndex, prefixLenght, lenght = strlen(key);
	next = trie;
	if(next == NULL){
		trie = NewIntern();
		new_leaf = NewLeaf(key);
		trie->pChildren[key[0]-'a'] = new_leaf;
		return trie;
	}
	else index = next;
	inWordIndex = 0;
	while((inWordIndex < lenght) &&(index->NotLeaf == true)&&(index->pChildren[key[inWordIndex]-'a'] != NULL)){
		parent = next;
		next = index->pChildren[key[inWordIndex]-'a'];
		index = next;
		inWordIndex++;
	}
	if((inWordIndex < lenght) && (index->pChildren[key[inWordIndex]-'a'] == NULL) && (index->NotLeaf == true)){
		new_index = NewLeaf(key);
		index->pChildren[key[inWordIndex]-'a'] = new_index;
		return trie;
	} 
	else data = next; 
	if(!strcmp(data->word,key)) printf("ignored\n");
	else{
		oldChildren = parent->pChildren[key[inWordIndex-1]-'a'];
		newWord = NewLeaf(key);
		prefixLenght = strlen(key);
		if(data->word[0] != '\0')
						 if(strlen(data->word) < prefixLenght)
						 					   prefixLenght = strlen(data->word);
	}
	bool createIntern = false;
	while((inWordIndex <= prefixLenght)&&(((data->word[0] != '\0' )&& (data->word[inWordIndex-1] == key[inWordIndex-1])) || (data->word[0] == '\0'))){
		intern = NewIntern();
		parent->pChildren[key[inWordIndex-1]-'a'] = intern;
		parent->NotLeaf = true;
		parent = intern;
		inWordIndex++;
		createIntern = true;
	} 
	if(createIntern) inWordIndex--;
	if((inWordIndex != prefixLenght) || ((inWordIndex == prefixLenght)&&(strlen(key) == strlen(data->word)))){
		parent->pChildren[26] = oldChildren;
		parent->pChildren[key[prefixLenght]-'a'] = newWord;
	}
	else if(data->word[0] != '\0'){
		if(strlen(data->word) <= prefixLenght){
  		     parent->pChildren[26] = oldChildren;
			 parent->pChildren[key[prefixLenght]-'a'] = newWord;
        }
 		else{
			 parent->pChildren[26] = newWord;
 			 parent->pChildren[data->word[prefixLenght]-'a'] = oldChildren;
		 } 
	}
	else{
		for(int cnt = 0; cnt < 27; cnt++)
				parent->pChildren[cnt] = oldChildren->pChildren[cnt];
		parent->pChildren[26] = newWord;
	}
	return trie;
}
void DisplayTrie(trie_node *trie, int nivel)
{
int count;

if(trie)
 {
 if (trie->NotLeaf != true) // if trie_node is a leaf(a word is stored in)
 { // display the string at his level
 for (count = 0; count <= nivel; count++) 
 cout << " ";
 cout << trie->word << endl;
 }
 // display all the words stored through trie children
 for (count = 26; count >= 0; count--)
 DisplayTrie(trie->pChildren[count], nivel + 4);
 }
}
int main()
{
 trie_node *trie;
char UserInputWord[20], cont_insert=' ';
int option = 0; //stores the user's input(the chosen option)
 trie = NULL;
label_menu:
while( option != 5)
{
 //display menu
 cout << endl << " Menu: " << endl;
 cout << "___________________________________________" << endl;
 cout << " 1. Create tree\n 2. Insert node\n 3. Search for node\n 4. Display tree\n 5. Exit\n";

 //get user input
 cout << "\n\n\nInput choice: ";
 cin >> option;
 switch (option)
 {
 case 1: //Create tree
 while(cont_insert != 'n')
 {
 // get user input string
 cout << endl << "Insert word :";
 cin >> UserInputWord;

 trie = Insert(trie,UserInputWord);
 cout << "\n Continue ? <y/n>";
 cont_insert = getch();
 }

 break;
 case 2: //Insert node
 cout << endl << "Insert word :";
 cin >> UserInputWord;
 Insert(trie,UserInputWord);

 break;
 case 3: //Search for node
 cout << endl << "Searched word :";  
 cin >> UserInputWord;
 Find(trie,UserInputWord);

 break;
 case 4: //Display tree
 DisplayTrie(trie,0);

 break;
 case 5: //Exit
 break;
 default:
 cout << "Choose from the displayed options (1-5) !!!";
 goto label_menu;
 }//end switch
 }//end while
 system("PAUSE");
return 0;
} 

