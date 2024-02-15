/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */


/******************************************************************************/
/*** NOME:                                                                  ***/
/*** COGNOME:                                                               ***/
/*** MATRICOLA:                                                             ***/
/******************************************************************************/


#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <upo/bst.h>
#include <upo/hashtable.h>


/**** BEGIN of EXERCISE #1 ****/

size_t upo_bst_subtree_count_even_helper(const upo_bst_node_t *node, int depth)
{
    
    if (node == NULL) {
        return 0;
    }
    //se depth e' pari allora devo aggiungere 1 altrimenti NO.
    if (depth % 2 == 0) {
        
        return 1 + upo_bst_subtree_count_even_helper(node->left, depth + 1) +
               upo_bst_subtree_count_even_helper(node->right, depth + 1);
    } else {
        
        return upo_bst_subtree_count_even_helper(node->left, depth + 1) +
               upo_bst_subtree_count_even_helper(node->right, depth + 1);
    }
}

//algoritmo semplice di ricerca di un nuodo dentro un albero
upo_bst_node_t* upo_bst_get_impl(upo_bst_node_t* node, void *key, upo_bst_comparator_t cmp, int* depth) {
	if (node == NULL ) {
        //printf("get impl is null \n" );
        return NULL;
    }
     
	if (cmp( node->key, key) == 0) {
		return node;
	} else if (cmp( key, node->key ) < 0) {
        // ad ogni giro aumento il depth
        *depth = *depth+1;
		return upo_bst_get_impl(node->left, key, cmp, depth);
	}else if (cmp( key, node->key ) > 0) {
         // ad ogni giro aumento il depth
        *depth = *depth+1;
		return upo_bst_get_impl(node->right, key, cmp, depth);
	}
	
}


size_t upo_bst_subtree_count_even(const upo_bst_t bst, const void *key)
{   
    //salvo variabile per riempirla con il depth del nodo che devo cercare. Lo passo tramite referenza.
    int my_depth = 0;
    const upo_bst_node_t *root = upo_bst_get_impl(bst->root, key, bst->key_cmp, &my_depth);
    
    return upo_bst_subtree_count_even_helper(root, my_depth);
}


/**** BEGIN of EXERCISE #2 ****/

void destroy_node (upo_ht_sepchain_list_node_t* node, int destroydata) {
    if(destroydata) {
        free(node->key);
        free(node->value);
    }
    free(node);
}


// cancella una chiave
int upo_ht_sepchain_delete(upo_ht_sepchain_t ht, const void *key, int destroy_data)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    upo_ht_hasher_t hasher = ht->key_hash;
    upo_ht_comparator_t cmp = ht->key_cmp;

    int hash = hasher(key, ht->capacity);
    //inizializzo il nodo come al solito e il prevoius a null.
    upo_ht_sepchain_list_node_t* n = ht->slots[hash].head;
    upo_ht_sepchain_list_node_t* previous = NULL;

// scorro gli elementi dello slot finche' la chiave e'diversa da null e la chiave e' diversa da quella che sto scorrendo
    while(n!= NULL && cmp(key, n->key)!= 0) {
        //salvo il precedente prima di avanzare
        previous = n;
        n = n->next;
    }
    //se trovo la chiave
    if (n!= NULL) {
        if(previous == NULL) { //se e' il primo elemento della lista 
            ht->slots[hash].head = n->next; //dico che la testa e' il successivo
        } else {
            previous->next = n->next; //metto come next del precedente il successivo del nodo, cosi' slego il nodo corrente
        }
        destroy_node(n, destroy_data); //distruggo il nodo
        (ht->size)--; //diminuisco la dimensione
        return ht->size;
    }


    
}


void upo_ht_sepchain_odelete(upo_ht_sepchain_t ht, const void *key, int destroy_data)
{
    return upo_ht_sepchain_delete(ht, key, destroy_data);
}


