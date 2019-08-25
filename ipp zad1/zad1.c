#include <string.h>
#include <stdlib.h>

typedef struct trie {
  int value;
  char* key;
  struct trie* kids;
  struct trie* next;
} trie;

/* Creates an empty trie.
 */
trie* trie_new () {
  trie* t = (trie*) malloc (sizeof (trie));
  t->value = 0;
 // t->length = 0;
  t->key = NULL;
  t->kids = NULL;
  t->next = NULL;
  return t;
}

/* Sets |t->key| to |key|.
 */
static void trie_set_key (trie* t, const char* key) {
  char* key_copy = (char*) malloc (sizeof (char) * (strlen (key) + 1));
  strcpy (key_copy, key);
  free (t->key);
  t->key = key_copy;
}

/* Creates a trie with |->key| set to |key| whose |->value| is on.
 */
static trie* trie_new_init (const char* key) {
  trie* t = trie_new ();
  t->value = 1;
  trie_set_key (t, key);
  return t;
}

/* Frees all memory used by the trie |t|.
 */
void trie_delete (trie* t) {
  if (t == NULL) {
    return;
  }
  trie_delete (t->kids);
  trie_delete (t->next);
  free (t->key);
  free (t);
}

typedef struct trie_str_pair {
  trie* trie;
  const char* str;
} trie_str_pair;

/* Creates a trie_str_pair with the values |->trie| and |->str| set to
 *  |t| and |str|, respectively.
 */
static trie_str_pair mk_trie_str_pair (trie* t, const char* str) {
  trie_str_pair pair;
  pair.trie = t;
  pair.str = str;
  return pair;
}

/* Tries to find a sibling of |t| or |t| itself that matches the input
 *  choice function |choiceFunc|. A match occurs if |choiceFunc| returns
 *  a string other than NULL. Upon a match, the matching trie and the string
 *  are returned. Otherwise NULL values are returned in the pair struct.
 */
static trie_str_pair lookup_by (
      const char* (*choiceFunc)(const char*, trie*)
    , const char* key, trie* t
  ) {
  while (t != NULL) {
    const char* str = choiceFunc (key, t);
    if (str != NULL) {
      return mk_trie_str_pair (t, str);
    }
    t = t->next;
  }
  return mk_trie_str_pair (NULL, NULL);
}

/* If |prefix| is a prefix of |str|, returns a pointer into |str| immediately
 *  after the prefix.
 */
static const char* strip_prefix (const char* prefix, const char* str) {
  int i;
  for (i = 0; prefix [i] != '\0'; ++i) {
    if (str [i] != prefix [i]) {
      return NULL;
    }
  }
  return str + i;
}

/* If |t->key| is a prefix of |str|, returns a pointer into |str| immediately
 *  after the prefix.
 */
static const char* strip_prefix_with_key (const char* str, trie* t) {
  return strip_prefix (t->key, str);
}

/* If |str| is a prefix of |t->key|, returns a pointer into |t->key|
 *  immediately after the prefix.
 */
static const char* strip_prefix_from_key (const char* str, trie* t) {
  return strip_prefix (str, t->key);
}

/* Returns a pointer into |str1| immediately after the longest common prefix
 *  between |str1| and |str2|.
 */
static const char* strip_common_prefix (const char* str1, const char* str2) {
  int i;
  for (i = 0; str1 [i] != '\0' && str2 [i] != '\0'; ++i) {
    if (str1 [i] != str2 [i]) {
      break;
    }
  }
  if (i == 0) {
    return NULL;
  }
  return str1 + i;
}

/* Returns a pointer into |str| past the longest common prefix between
 *  |str| and |t->str|.
 */
static const char* strip_common_prefix_on_key (const char* str, trie* t) {
  return strip_common_prefix (str, t->key);
}

/* Returns 1 if |key| is in the trie |t|. Returns 0 if not.
 */
int trie_lookup (trie* t, const char* key) {
  while (key != NULL && key [0] != '\0') {
    trie_str_pair pair = lookup_by (strip_prefix_with_key, key, t->kids);
    t = pair.trie;
    if (t == NULL) {
      return 0;
    }
    key = pair.str;
  }
  return t->value;
}

/* Adds |kid| to |t|'s list of kids.
 */
static void trie_add_kid (trie* t, trie* kid) {
  kid->next = t->kids;
  t->kids = kid;
}

/* Removes |kid| from |t|'s list of kids.
 * |kid| must be in |t|'s list of kids.
 */
static void trie_remove_kid (trie* t, trie* kid) {
  if (t->kids == kid) {
    t->kids = kid->next;
  }
  else {
    t = t->kids;
    while (t->next != kid) {
      t = t->next;
    }
    t->next = kid->next;
  }
}

/* Replaces |kid| from |t|'s list of kids with |new_kid|.
 * |kid| must be in |t|'s list of kids.
 */
static void trie_replace_kid (trie* t, trie* kid, trie* new_kid) {
  trie_remove_kid (t, kid);
  trie_add_kid (t, new_kid);
}

/* If |t| has exactly one kid and no grandkids, |t| and its kid are merged
 *  into one trie node. In other words, |t|'s kid's |->key| is appended to
 *  |t->key| and |t->value| becomes that of its kid's |->value|.
 */
static void trie_try_merge_with_kids (trie* t) {
  if (t->key != NULL) {
    trie* kid = t->kids;
    if (kid != NULL && kid->next == NULL) {
      t->value = kid->value;
      t->kids = kid->kids;
      int new_len = strlen (t->key) + strlen (kid->key);
      t->key = realloc (t->key, sizeof (char) * (new_len + 1));
      strcat (t->key, kid->key);
      free (kid->key);
      free (kid);
    }
  }
}

/* Helper for trie_insert.
 */
static void trie_insert_split_key (
      trie* t
    , const char* key_prefix, const char* key_suffix
  ) {
  trie* kid = trie_new_init (key_suffix);
  trie_add_kid (t, kid);
  trie_set_key (t, key_prefix);
}

/* Helper for trie_insert.
 */
static void trie_insert_simple (trie* t, const char* key) {
  trie* kid = trie_new_init (key);
  trie_add_kid (t, kid);
}

/* Helper for trie_insert.
 */
static void trie_insert_fork (
      trie* t
    , trie* kid
    , char* key_prefix  /* Caller loses ownership of this string */
    , const char* key_suffix
    , const char* kid_key_suffix
  ) {
  trie* fork_kid = trie_new ();
  fork_kid->key = key_prefix;
  fork_kid->kids = trie_new_init (key_suffix);
  fork_kid->kids->next = kid;
  trie_replace_kid (t, kid, fork_kid);
  fork_kid->next = kid->next;
  kid->next = NULL;
  trie_set_key (kid, kid_key_suffix);
}

/* Inserts |key| into the trie |t|.
 */
void trie_insert (trie* t, const char* key) {
  if (key [0] == '\0') {
    return;
  }
  while (1) {
    trie_str_pair pair = lookup_by (strip_prefix_with_key, key, t->kids);
    trie* kid = pair.trie;
    const char* stripped = pair.str;
    if (kid != NULL) {
      if (stripped [0] == '\0') {
        kid->value = 1;
        return;
      }
      t = kid;
      key = stripped;
      continue;
    }
    pair = lookup_by (strip_prefix_from_key, key, t->kids);
    kid = pair.trie;
    stripped = pair.str;
    if (kid != NULL) {
      trie_insert_split_key (kid, key, stripped);
      return;
    }
    pair = lookup_by (strip_common_prefix_on_key, key, t->kids);
    kid = pair.trie;
    stripped = pair.str;
    if (kid == NULL) {
      trie_insert_simple (t, key);
      return;
    }
    int prefix_len = stripped - key;
    char* common_prefix = (char*) malloc (sizeof (char) * (prefix_len + 1));
    strncpy (common_prefix, key, prefix_len);
    common_prefix [prefix_len] = '\0';
    trie_insert_fork (t, kid, common_prefix, stripped, kid->key + prefix_len);
    return;
  }
}

/* Helper for trie_remove.
 */
static void trie_remove_simple (trie* t, trie* kid) {
  trie_remove_kid (t, kid);
  free (kid->key);
  free (kid);
}

/* Helper for trie_remove.
 */
static void trie_remove_merge (trie* t) {
  t->value = 0;
  trie_try_merge_with_kids (t);
}

/* Removes |key| from the trie |t|.
 */
void trie_remove (trie* t, const char* key) {
  trie_str_pair pair = lookup_by (strip_prefix_with_key, key, t->kids);
  trie* kid = pair.trie;
  const char* stripped = pair.str;
  if (kid == NULL) {
    return;
  }
  if (stripped [0] == '\0') {
    if (kid->kids == NULL) {
      trie_remove_simple (t, kid);
    }
    else {
      trie_remove_merge (kid);
    }
  }
  else {
    trie_remove (kid, stripped);
  }
  trie_try_merge_with_kids (t);
}
int main(){
	trie* drzewo;
	drzewo = trie_new();
	trie_insert(drzewo, "ala");
	printf("%d",trie_lookup(drzewo, "ala"));
	trie_remove(drzewo, "ala");
	printf("%d",trie_lookup(drzewo, "ala"));
	trie_insert(drzewo, "ala");
	trie_insert(drzewo, "ala2");
	trie_remove(drzewo, "ala");
	printf("%d",trie_lookup(drzewo, "ala2"));
	trie_delete(drzewo);
	return 0;
}
