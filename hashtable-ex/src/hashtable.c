/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2015 University of Piemonte Orientale, Computer Science Institute
 *
 * This file is part of UPOalglib.
 *
 * UPOalglib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UPOalglib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UPOalglib.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <assert.h>
#include "hashtable_private.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <upo/error.h>
#include <upo/utility.h>


/*** EXERCISE #1 - BEGIN of HASH TABLE with SEPARATE CHAINING ***/

// vado in creazione ho bisogno della size della stuct hasher_t e comparator
upo_ht_sepchain_t upo_ht_sepchain_create(size_t m, upo_ht_hasher_t key_hash, upo_ht_comparator_t key_cmp)
{
    // dichiaro una struttura di tipo hashtablo sepchain
    upo_ht_sepchain_t ht = NULL;
    size_t i = 0;

    /* precondizioni non devo averli a null*/
    assert( key_hash != NULL );
    assert( key_cmp != NULL );

    /* Alloco memoria per il tipo hashtable */
    ht = malloc(sizeof(struct upo_ht_sepchain_s));
    if (ht == NULL)
    {
        perror("Unable to allocate memory for Hash Table with Separate Chaining");
        abort();
    }

    /* Alloco memoria per gli slots di hashtable */
    ht->slots = malloc(m*sizeof(upo_ht_sepchain_slot_t));
    if (ht->slots == NULL)
    {
        perror("Unable to allocate memory for slots of the Hash Table with Separate Chaining");
        abort();
    }

    /* Initialize fields */
    for (i = 0; i < m; ++i)
    {
        ht->slots[i].head = NULL;
    }
    ht->capacity = m;
    ht->size = 0;
    ht->key_hash = key_hash;
    ht->key_cmp = key_cmp;

    return ht;
}

void upo_ht_sepchain_destroy(upo_ht_sepchain_t ht, int destroy_data)
{
    if (ht != NULL)
    {
        upo_ht_sepchain_clear(ht, destroy_data);
        free(ht->slots);
        free(ht);
    }
}

void upo_ht_sepchain_clear(upo_ht_sepchain_t ht, int destroy_data)
{
    if (ht != NULL && ht->slots != NULL)
    {
        size_t i = 0;

        /* For each slot, clear the associated list of collisions */
        for (i = 0; i < ht->capacity; ++i)
        {
            upo_ht_sepchain_list_node_t *list = NULL;

            list = ht->slots[i].head;
            while (list != NULL)
            {
                upo_ht_sepchain_list_node_t *node = list;

                list = list->next;

                if (destroy_data)
                {
                    free(node->key);
                    free(node->value);
                }

                free(node);
            }
            ht->slots[i].head = NULL;
        }
        ht->size = 0;
    }
}
upo_ht_sepchain_list_node_t* createNode(){

    upo_ht_sepchain_list_node_t* n = malloc(sizeof(upo_ht_sepchain_list_node_t));
    
    if (n == NULL)
    {
        perror("Unable to allocate memory for Node");
        //abort();
    }

    n->value=NULL;
    n->key=NULL;
    n->next=NULL;

    return n;
}

// void* upo_ht_sepchain_put(upo_ht_sepchain_t ht, void *key, void *value)
// {
//     void *old_value = NULL;

//     if(ht==NULL)
//         return old_value;
//     /* TO STUDENTS:
//      *  Remove the following two lines and put here your implementation. */
//     // fprintf(stderr, "To be implemented!\n");
//     // abort();
//     upo_ht_hasher_t hash = ht->key_hash;
//     upo_ht_comparator_t cmp = ht->key_cmp;

//     size_t h = hash(key,ht->capacity);
//     upo_ht_sepchain_list_node_t* n = ht->slots[h].head;

//     while(n->key!=NULL && cmp(key, n->key) != 0) { // 
//         n = n->next;
//     }
//     if(n == NULL ) { // the key is not in ht i create
//         (ht->size)++;
//         n = createNode();
//         n->key = key;
//         n->value = value;
//         n->next = ht->slots[h].head; // set the head to be the next of the new node
//         ht->slots[h].head = n; // set the head as the new node
//     } else {
//         old_value = n->value;
//         n->value = value;
//     }
        

//     return old_value;
// }

// faccio una put, quindi se trovo la chiave vado in sostituzione altrimenti inserisco nello slot la nuova.
// restituisco il vecchio valore se sostituisco
void* upo_ht_sepchain_put(upo_ht_sepchain_t ht, void *key, void *value){
    //inizializzo il vecchio valore a null
    void* old_value = NULL;
    if (ht == NULL) {
        return NULL;
    }
    // recupero il numero della lista da cui partire
    size_t h = ht->key_hash(key, ht->capacity);
    // salvo nel nodo la testa dello slot selezionato
    upo_ht_sepchain_list_node_t* node = ht->slots[h].head;
    
    // scorro gli elementi dello slot finche' la chiave e'diversa da null e la chiave e' diversa da quella che sto scorrendo
    while(node->key != NULL && ht->key_cmp(key, node->key) != 0) {
        // dichiaro che il nodo e' uguale al suo next per scorrere
        node = node->next;
    }
    if(node->key != NULL) { //dopo il ciclo, se la chiave del nodo e' diversa da null vuol dire che l'ho trovata
        //old_value = (upo_ht_key_list_node_t*)malloc(sizeof(upo_ht_key_list_node));
        old_value = node->value; //salvo nel valore di ritorno il valore corrente
        node->value = value; //salvo il nuovo valore

    } else { // se non ho trovato la chiave creo un nuovo nodo
        node = (upo_ht_sepchain_list_node_t*)malloc(sizeof(upo_ht_sepchain_list_node_t));
        node->key = key;
        node->value = value;
        node->next = ht->slots[h].head; //lo metto in testa dicendo che il suo successore e' la testa
        (ht->size)++; //aumento la dimensione
    }

    return old_value;    
}











// uguale alla put ma se trovo la chiave non faccio nulla, creo solo se non esiste
void upo_ht_sepchain_insert(upo_ht_sepchain_t ht, void *key, void *value)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    // fprintf(stderr, "To be implemented!\n");
    // abort();
    upo_ht_hasher_t hash = ht->key_hash;
    upo_ht_comparator_t cmp = ht->key_cmp;

    size_t h = hash(key,ht->capacity);
    upo_ht_sepchain_list_node_t* n = ht->slots[h].head;

    while(n->key!=NULL && cmp(key, n->key)!= 0) { // 
        n = n->next;
    }
    if(n == NULL ) { // the key is not in ht i create
        n = createNode();
        n->key = key;
        n->value = value;
        n->next = ht->slots[h].head; // set the head to be the next of the new node
        ht->slots[h].head = n; // set the head as the new node
    }
}

//lo scopo e' di recuperare il valore data una chiave
void* upo_ht_sepchain_get(const upo_ht_sepchain_t ht, const void *key){
    //TODO
    if(ht == NULL) {
        return NULL;
    }
    size_t h = ht->key_hash(key, ht->capacity);
    upo_ht_sepchain_list_node_t* n = ht->slots[h].head;

    // scorro gli elementi dello slot finche' la chiave e'diversa da null e la chiave e' diversa da quella che sto scorrendo
    while((n->key != NULL && ht->key_cmp(n->key, key) != 0)) {
        n = n->next;
    }
    //se la chiave e' diversa da null vuol dire che l'ho trovata e ritorno il valore
    if(n->key != NULL) {
        return n->value;
    }
    else {
        return NULL;
    }
}

//uguale alla get ma ritorno 1 se trovo e 0 se non trovo
int upo_ht_sepchain_contains(const upo_ht_sepchain_t ht, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    if( ht == NULL) {
        return 0;
    }
    upo_ht_hasher_t h = ht->key_hash;
    upo_ht_comparator_t cmp = ht->key_cmp;

    int hash = h(key, ht->capacity);
    upo_ht_sepchain_list_node_t* n = ht->slots[hash].head;

    while(n!= NULL && cmp(key,n->key)!= 0)
        n = n->next;

    if (n != NULL) {
        return 1;
    }
    return 0;
}
//distruggo il nodo, se ho destroydata dealloco anche la chiave e il valore del nodo
void destroy_node (upo_ht_sepchain_list_node_t* node, int destroydata) {
    if(destroydata) {
        free(node->key);
        free(node->value);
    }
    free(node);
}
// cancella una chiave
void upo_ht_sepchain_delete(upo_ht_sepchain_t ht, const void *key, int destroy_data)
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
    }


    
}
// ritorno la dimensione semplicemente con ht->size
size_t upo_ht_sepchain_size(const upo_ht_sepchain_t ht)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
     if(ht!=NULL)
        return ht->size;
    else
        return 0;
}

//controllo se e' vuota
int upo_ht_sepchain_is_empty(const upo_ht_sepchain_t ht)
{
    // se upo_ht_sepchain_size ritorna zero allora ritorno 1 (true), altrimenti 0
    return upo_ht_sepchain_size(ht) == 0 ? 1 : 0;
}

size_t upo_ht_sepchain_capacity(const upo_ht_sepchain_t ht)
{
    return (ht != NULL) ? ht->capacity : 0;
}

double upo_ht_sepchain_load_factor(const upo_ht_sepchain_t ht)
{
    return upo_ht_sepchain_size(ht) / (double) upo_ht_sepchain_capacity(ht);
}

upo_ht_comparator_t upo_ht_sepchain_get_comparator(const upo_ht_sepchain_t ht)
{
    return ht->key_cmp;
}

upo_ht_hasher_t upo_ht_sepchain_get_hasher(const upo_ht_sepchain_t ht)
{
    return ht->key_hash;
}


/*** EXERCISE #1 - END of HASH TABLE with SEPARATE CHAINING ***/


/*** EXERCISE #2 - BEGIN of HASH TABLE with LINEAR PROBING ***/


upo_ht_linprob_t upo_ht_linprob_create(size_t m, upo_ht_hasher_t key_hash, upo_ht_comparator_t key_cmp)
{
    upo_ht_linprob_t ht = NULL;
    size_t i = 0;

    /* preconditions */
    assert( key_hash != NULL );
    assert( key_cmp != NULL );

    /* Allocate memory for the hash table type */
    ht = malloc(sizeof(struct upo_ht_linprob_s));
    if (ht == NULL)
    {
        perror("Unable to allocate memory for Hash Table with Linear Probing");
        abort();
    }

    /* Allocate memory for the array of slots */
    if (m > 0)
    {
        ht->slots = malloc(m*sizeof(upo_ht_linprob_slot_t));
        if (ht->slots == NULL)
        {
            perror("Unable to allocate memory for slots of the Hash Table with Separate Chaining");
            abort();
        }

        /* Initialize the slots */
        for (i = 0; i < m; ++i)
        {
            ht->slots[i].key = NULL;
            ht->slots[i].value = NULL;
            ht->slots[i].tombstone = 0;
        }
    }

    ht->capacity = m;
    ht->size = 0;
    ht->key_hash = key_hash;
    ht->key_cmp = key_cmp;

    return ht;
}

void upo_ht_linprob_destroy(upo_ht_linprob_t ht, int destroy_data)
{
    if (ht != NULL)
    {
        upo_ht_linprob_clear(ht, destroy_data);
        free(ht->slots);
        free(ht);
    }
}

void upo_ht_linprob_clear(upo_ht_linprob_t ht, int destroy_data)
{
    if (ht != NULL && ht->slots != NULL)
    {
        size_t i = 0;

        /* For each slot, clear the associated list of collisions */
        for (i = 0; i < ht->capacity; ++i)
        {
            if (ht->slots[i].key != NULL)
            {
                if (destroy_data)
                {
                    free(ht->slots[i].key);
                    free(ht->slots[i].value);
                }
                ht->slots[i].key = NULL;
                ht->slots[i].value = NULL;
                ht->slots[i].tombstone = 0;
            }
        }
        ht->size = 0;
    }
}
//stessa cosa, se trovo la chiave sostituisco valore, altrimenti creo nuovo
void* upo_ht_linprob_put(upo_ht_linprob_t ht, void *key, void *value){
    void *old_value = NULL;

    if (ht == NULL) {
        return NULL;
    }
    // bisogna considerare il fattore di carico, se >= 0.5 bisogna ricalcolarlo
    if( upo_ht_linprob_load_factor(ht) >= 0.5) {
        upo_ht_linprob_resize(ht, ht->capacity * 2);
    }

    //solita maniera di recuperare l'hash
    size_t h = ht->key_hash(key, ht->capacity);
    int t_found = 0; //inizializzo la variabile che mi indica se ho trovato una tomba
    size_t tomb_h = 0; 
    //scorro finche' la chiave non e' null, finche' non trovo la chiave oppure se questo slot e' marcato come tomba
    while((ht->slots[h].key != NULL && ht->key_cmp(key, ht->slots[h].key) != 0) || ht->slots[h].tombstone ==1) {
        //se ho trovato che lo slot e' tomba e non ne ho trovati altre
        if (ht->slots[h].tombstone && t_found == 0) {
            t_found = 1; //dico che ho trovato tomba
            tomb_h = h; //metto in tomb il valore dell'hash
        }
        //uso questo per passare allo slot successivo (vedere teoria)
        h = (h +1) % ht->capacity;
    }
    if(ht->slots[h].key == NULL) { //non ho trovato la chiave, la inserisco
        if (t_found == 1) {
            h = tomb_h;
        }
        ht->slots[h].key = key;
        ht->slots[h].value = value;
        ht->slots[h].tombstone = 0; //metto che non e' tomba
        (ht->size)++; //aumento la dimensione
    } else { // ho trovato la chiave e faccio l'update
        old_value = ht->slots[h].value; //salvo il vecchio valore da restituire
        ht->slots[h].value = value; //metto nuovo valore
    }
    return old_value;
}

// void* upo_ht_linprob_put(upo_ht_linprob_t ht, void *key, void *value)
// {
//     void *old_value = NULL;

//     /* TO STUDENTS:
//      *  Remove the following two lines and put here your implementation. */
//     if (upo_ht_linprob_load_factor(ht) >= 0.5) {
//         upo_ht_linprob_resize(ht, (ht->capacity)*2);
//     }
//     size_t hash = ht->key_hash(key, ht->capacity);

//     int found_t = 0;
// 	size_t tomb_h = -1;

//     while( (ht->slots[hash].key != NULL && ht->key_cmp(ht->slots[hash].key, key) != 0) || ht->slots[hash].tombstone == 1) {
//         if (ht->slots[hash].tombstone && found_t == 0) {
//             found_t = 1;
//             tomb_h = hash;
//         }
//         hash = (hash +1) % ht->capacity;
//     }
//     if( ht->slots[hash].key == NULL) {
//         if(found_t == 1) {
//             hash = tomb_h;
//         }
//         ht->slots[hash].key = key;
//         ht->slots[hash].value = value;
//         ht->slots[hash].tombstone = 0;
//         (ht->size)++;

//     } else {
//         old_value = ht->slots[hash].value;
//         ht->slots[hash].value = value;
//     }
//     return old_value;
// }
//uguale alla put ma inserisco solo se non c'e' la chiave
void upo_ht_linprob_insert(upo_ht_linprob_t ht, void *key, void *value)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    if (upo_ht_linprob_load_factor(ht) >= 0.5) {
        upo_ht_linprob_resize(ht, ht->capacity *2);
    }
    size_t h = ht->key_hash(key, ht->capacity);

    int found_t = 0;
    size_t tomb_h;

    while((ht->slots[h].key != NULL && ht->key_cmp(ht->slots[h].key, key) != 0) || ht->slots[h].tombstone == 1) {
        if( (ht->slots)[h].tombstone && found_t == 0) {
            found_t = 1;
            tomb_h = h;
        }
        h = (h + 1) % ht->capacity;
    }
    if ( ht->slots[h].key == NULL ) {
        if (found_t == 1) {
            h = tomb_h;
        }
        ht->slots[h].key = key;
        ht->slots[h].value = value;
        ht->slots[h].tombstone = 0;
        (ht->size)++;
    }
}
//recupero il valore data una chiava, se non c'e' torno null. (i costrutti di scorrimento sono uguali alla put)
void* upo_ht_linprob_get(const upo_ht_linprob_t ht, const void *key) {
    size_t h = ht->key_hash(key, ht->capacity);

    while((ht->slots[h].key != NULL && ht->key_cmp(key,ht->slots[h].key) != 0) || ht->slots[h].tombstone == 1 ) {
        h = (h+1) % ht->capacity;
    }
    if(ht->slots[h].key != NULL) {
        return ht->slots[h].value;
    }
    return NULL;
}

// void* upo_ht_linprob_get(const upo_ht_linprob_t ht, const void *key)
// {
//     /* TO STUDENTS:
//      *  Remove the following two lines and put here your implementation. */
//     size_t hash = ht->key_hash(key, ht->capacity);
//     while((ht->slots[hash].key != NULL && ht->key_cmp(ht->slots[hash].key, key) != 0) || ht->slots[hash].tombstone == 1) {
//         hash = (hash + 1) % (ht->capacity);
//     }
//     if (ht->slots[hash].key != NULL) {
//         return (ht->slots)[hash].value;
//     } else {
//         return 0;
//     }
// }
// stessa cosa della get ma resituisce 1 se trova la chiave (invece del valore) e zero se non trova (invece di null)
int upo_ht_linprob_contains(const upo_ht_linprob_t ht, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    size_t hash = ht->key_hash(key, ht->capacity);
    while((ht->slots[hash].key != NULL && ht->key_cmp(ht->slots[hash].key, key)!= 0) || ht->slots[hash].tombstone == 1) {
        hash = (hash + 1) % (ht->capacity);
    }
    if (ht->slots[hash].key != NULL) {
        return 1;
    } else {
        return 0;
    }
}

//cancella la chiave
void upo_ht_linprob_delete(upo_ht_linprob_t ht, const void *key, int destroy_data) {
    size_t h = ht->key_hash(key, ht->capacity);
    //scorrimento sempre uguglae
    while( (ht->slots[h].key != NULL && ht->key_cmp(ht->slots[h].key, key) != 0) || (ht->slots)[h].tombstone == 1) {
        h = (h+1) % ht->capacity;
    }
    if (ht->slots[h].key != NULL) { //se trovo la chiave
        ht->slots[h].key = NULL; //metto a null chiave e valore
        ht->slots[h].value = NULL;
        ht->slots[h].tombstone = 1; //metto che questo slot e' una tomba
        (ht->size)--; //diminuisco dimensione
    }
    //ricalcolo fattore di carico (vedere teoria e pseudocodice)
    if(upo_ht_linprob_load_factor(ht)<=0.125)
        upo_ht_linprob_resize(ht,(ht->capacity)/2);
}




// void upo_ht_linprob_delete(upo_ht_linprob_t ht, const void *key, int destroy_data)
// {
//     /* TO STUDENTS:
//      *  Remove the following two lines and put here your implementation. */
//     size_t h = ht->key_hash(key, ht->capacity);
//     while( (ht->slots[h].key != NULL && ht->key_cmp(ht->slots[h].key, key) != 0) || (ht->slots)[h].tombstone == 1) {
//         h = (h +1) % ht->capacity;
//     }
//     if ( ht->slots[h].key != NULL ) {
//         ht->slots[h].key = NULL;
//         ht->slots[h].value = NULL;
//         ht->slots[h].tombstone = 1;
//         (ht->size)--;
//         if (destroy_data) {
//             free(ht->slots[h].value);
//             free(ht->slots[h].key);
//         }
//     }
//     if( upo_ht_linprob_load_factor(ht) <= 0.125) {
//         upo_ht_linprob_resize(ht, ht->capacity / 2);
//     }
// }
//recupero la size
size_t upo_ht_linprob_size(const upo_ht_linprob_t ht)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    if (ht == NULL ) {
        return 0;
    } else {
        return ht->size;
    }
}

//ritorna 1 se vuota, altrimenti 0
int upo_ht_linprob_is_empty(const upo_ht_linprob_t ht)
{
    return upo_ht_linprob_size(ht) == 0 ? 1 : 0;
}

size_t upo_ht_linprob_capacity(const upo_ht_linprob_t ht)
{
    return (ht != NULL) ? ht->capacity : 0;
}

double upo_ht_linprob_load_factor(const upo_ht_linprob_t ht)
{
    return upo_ht_linprob_size(ht) / (double) upo_ht_linprob_capacity(ht);
}

void upo_ht_linprob_resize(upo_ht_linprob_t ht, size_t n)
{
    /* preconditions */
    assert( n > 0 );

    if (ht != NULL)
    {
        /* The hash table must be rebuilt from scratch since the hash value of
         * keys will be in general different (due to the change in the
         * capacity). */

        size_t i = 0;
        upo_ht_linprob_t new_ht = NULL;

        /* Create a new temporary hash table */
        new_ht = upo_ht_linprob_create(n, ht->key_hash, ht->key_cmp);
        if (new_ht == NULL)
        {
            perror("Unable to allocate memory for slots of the Hash Table with Separate Chaining");
            abort();
        }

        /* Put in the temporary hash table the key-value pairs stored in the
         * hash table to resize.
         * Note: by calling function 'put' we are also rehashing the keys
         * according to the new capacity. */
        for (i = 0; i < ht->capacity; ++i)
        {
            if (ht->slots[i].key != NULL)
            {
                upo_ht_linprob_put(new_ht, ht->slots[i].key, ht->slots[i].value);
            }
        }

        /* Copy the new slots in the old hash table.
         * To do so we use a trick that avoids to loop for each key-value pair:
         * swap the array of slots, the size and the capacity between new and
         * old hash tables. */
        upo_swap(&ht->slots, &new_ht->slots, sizeof ht->slots);
        upo_swap(&ht->capacity, &new_ht->capacity, sizeof ht->capacity);
        upo_swap(&ht->size, &new_ht->size, sizeof ht->size);

        /* Destroy temporary hash table */
        upo_ht_linprob_destroy(new_ht, 0);
    }
}


/*** EXERCISE #2 - END of HASH TABLE with LINEAR PROBING ***/


/*** EXERCISE #3 - BEGIN of HASH TABLE - EXTRA OPERATIONS ***/

// recupera tutte le chiavi
upo_ht_key_list_t upo_ht_sepchain_keys(const upo_ht_sepchain_t ht)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    upo_ht_key_list_t head = NULL;
    upo_ht_key_list_t tail = NULL;

    size_t i;
    for(i=0; i< upo_ht_sepchain_capacity(ht); i++) {

        upo_ht_sepchain_list_node_t* node = ht->slots[i].head;
        //giro finche il nodo e' diverso da null
        while(node != NULL) {
            if(head == NULL ) { //se non c'e' la testa la creo
                head = (upo_ht_key_list_node_t *) malloc(sizeof(upo_ht_key_list_node_t));
                head->key = node->key;
                head->next = NULL;
                tail = head;
            }
            else {
                //metto tutto in coda e aggiungo la 
                tail->next = (upo_ht_key_list_node_t *) malloc(sizeof(upo_ht_key_list_node_t));
				tail = tail->next;
				tail->key = node->key;
				tail->next = NULL; //non metto il next perche' non so se e' l'ultimo, al giro dopo mettero' il next
            }
            node = node->next;
        }
        return head;
    }
    //size_t h = ht->key_hash(key, ht->capacity);
}

// devo visitare tutti i gli slot
void upo_ht_sepchain_traverse(const upo_ht_sepchain_t ht, upo_ht_visitor_t visit, void *visit_arg)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    if(ht == NULL) {
        return;
    }
    size_t i;
    //giro la lista ma poi, per ogni elemento dovro' scorrere gli slot
    for(i=0; i< upo_ht_sepchain_capacity(ht);i++) {
        upo_ht_sepchain_list_node_t* node = ht->slots[i].head;
        // scrorro gli slot dell'elemento node preso dallo slot in posizione i
        while(node!=NULL) {
            visit(node->key, node->value, visit_arg);
            node = node->next;
        }
    }
}
// scorro e recupero tutte le chiavi
upo_ht_key_list_t upo_ht_linprob_keys(const upo_ht_linprob_t ht)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    upo_ht_key_list_t head = NULL, tail= NULL;

    //uso un for perche' non abbiamo liste concatenate e dobbiamo prenderli tutti
    for(size_t i =0; i< upo_ht_linprob_capacity(ht); i++) {
        if (head == NULL && ht->slots[i].key != NULL) {
            head = (upo_ht_key_list_node_t *)malloc(sizeof(upo_ht_key_list_node_t));
            head->key = ht->slots[i].key;
            head->next = NULL;
            tail = head;
        } 
        else if( ht->slots[i].key != NULL) {
            tail->next = (upo_ht_key_list_node_t *) malloc(sizeof(upo_ht_key_list_node_t));
            tail = tail->next;
            tail->key = ht->slots[i].key;
            tail->next = NULL;
        }
    }
    return head;
}
//deve attraversare tutti gli slots
void upo_ht_linprob_traverse(const upo_ht_linprob_t ht, upo_ht_visitor_t visit, void *visit_arg)
{
    /* TO STUDENTS:
     *  Remove the following two lies and put here your implementation. */

   //uso un for perche' non abbiamo liste concatenate e dobbiamo prenderli tutti
   for(size_t i=0; i < upo_ht_linprob_capacity(ht); i++) {
       visit(ht->slots[i].key,ht->slots[i].value, visit_arg);
   }
}


/*** EXERCISE #3 - END of HASH TABLE - EXTRA OPERATIONS ***/


/*** BEGIN of HASH FUNCTIONS ***/


size_t upo_ht_hash_int_div(const void *x, size_t m)
{
    /* preconditions */
    assert( x != NULL );
    assert( m > 0 );

    return *((int*) x) % m;
}

size_t upo_ht_hash_int_mult(const void *x, double a, size_t m)
{
    /* preconditions */
    assert( x != NULL );
    assert( a > 0 && a < 1 );
    assert( m > 0 );

    return floor( m * fmod(a * *((int*) x), 1.0) );
}

size_t upo_ht_hash_int_mult_knuth(const void *x, size_t m)
{
    return upo_ht_hash_int_mult(x, 0.5*(sqrt(5)-1), m);
}

size_t upo_ht_hash_str(const void *x, size_t h0, size_t a, size_t m)
{
    const char *s = NULL;
    size_t h = h0; 

    /* preconditions */
    assert( x != NULL );
    assert( m > 0 );
/*
    assert( a < m );
*/
    assert( h0 < m );

    s = *((const char**) x);
    for (; *s; ++s)
    {
        h = (a*h + *s) % m;
    }

    return h;
}

size_t upo_ht_hash_str_djb2(const void *x, size_t m)
{
    return upo_ht_hash_str(x, 5381U, 33U, m);
}

size_t upo_ht_hash_str_djb2a(const void *x, size_t m)
{
    const char *s = NULL;
    size_t h = 5381U; 

    /* preconditions */
    assert( x != NULL );
    assert( m > 0 );

    s = *((const char**) x);
    for (; *s; ++s)
    {
        h = (33U*h ^ *s) % m;
    }

    return h;
}

size_t upo_ht_hash_str_java(const void *x, size_t m)
{
    return upo_ht_hash_str(x, 0U, 31U, m);
}

size_t upo_ht_hash_str_kr2e(const void *x, size_t m)
{
    return upo_ht_hash_str(x, 0U, 31U, m);
}

size_t upo_ht_hash_str_sgistl(const void *x, size_t m)
{
    return upo_ht_hash_str(x, 0U, 5U, m);
}

size_t upo_ht_hash_str_stlport(const void *x, size_t m)
{
    return upo_ht_hash_str(x, 0U, 33U, m);
}

/*** END of HASH FUNCTIONS ***/
