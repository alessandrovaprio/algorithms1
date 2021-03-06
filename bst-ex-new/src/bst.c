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

#include "bst_private.h"
#include <stdio.h>
#include <stdlib.h>


/**** EXERCISE #1 - BEGIN of FUNDAMENTAL OPERATIONS ****/

upo_bst_t upo_bst_create(upo_bst_comparator_t key_cmp)
{
    upo_bst_t tree = malloc(sizeof(struct upo_bst_s));
    if (tree == NULL)
    {
        perror("Unable to create a binary search tree");
        abort();
    }

    tree->root = NULL;
    tree->key_cmp = key_cmp;

    return tree;
}

void upo_bst_destroy(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear(tree, destroy_data);
        free(tree);
    }
}

void upo_bst_clear_impl(upo_bst_node_t *node, int destroy_data)
{
    if (node != NULL)
    {
        upo_bst_clear_impl(node->left, destroy_data);
        upo_bst_clear_impl(node->right, destroy_data);

        if (destroy_data)
        {
            free(node->key);
            free(node->value);
        }

        free(node);
    }
}

void upo_bst_clear(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear_impl(tree->root, destroy_data);
        tree->root = NULL;
    }
}
upo_bst_node_t *upo_bst_node_create(void *key, void *value) {
    upo_bst_node_t *node = malloc(sizeof(upo_bst_node_t));
    if (node == NULL) {
        printf("Unable to create node \n");
        abort();
    }
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*upo_bst_node_t* upo_bst_put_impl (upo_bst_node_t* n, void *key, void* value, void** oldvalue, upo_bst_comparator_t cmp) {
	if (n == NULL) {
		return upo_bst_node_create(key, value);
	}
	else if (cmp(key, n->key) < 0) {
		n->left = upo_bst_put_impl(n->left,key, value,&oldvalue,cmp);
	} 
	else if (cmp(key, n->key) > 0) {
			n->right = upo_bst_put_impl(n->right,key,value,&oldvalue,cmp);
	}
	else {
		*oldvalue = n->value;
		n->value = value;
	}
	return n;
}*/
// java persona.nome = Daniele
// C persona->nome = Daniele
upo_bst_node_t* upo_bst_put_impl(upo_bst_node_t* node, void* key, void* value, void* oldValue, upo_bst_comparator_t cmp) {
	// se il nodo e' vuoto lo creo
	if (node == NULL) {
	   return upo_bst_node_create(key, value);
	}
	else if(cmp(key, node->key) > 0) { // controllo se la chiave e' maggiore della chiave del nodo
		node->right = upo_bst_put_impl(node->right, key, value, oldValue, cmp); 
	} 
	else if(cmp(key, node->key) < 0) { // controllo se la chiave e' minore della chiave del nodo
		node->left = upo_bst_put_impl(node->left, key, value, oldValue, cmp); 
	} 
	// se invece la chiave e' uguale alla chiave del nodo
	else if(cmp(key, node->key) == 0) {
		oldValue = node->value;
		node->value = value;
	}
	
	return node;
}
void* upo_bst_put(upo_bst_t tree, void *key, void *value)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    // fprintf(stderr, "To be implemented!\n");
    // abort();
    if (tree == NULL) {
    	return NULL;
    }
    void *vold = NULL;
    tree->root= upo_bst_put_impl( tree->root, key, value, &vold, tree->key_cmp);
    return vold;
}

upo_bst_node_t* upo_bst_insert_impl (upo_bst_node_t* n, void *key, void* value, upo_bst_comparator_t cmp) {
	if (n == NULL) {
		return upo_bst_node_create(key, value);
	}
	
	
	if (cmp(key, n->key) < 0) {
		n->left = upo_bst_insert_impl(n->left,key,value, cmp);
	} 
	else if (cmp(key, n->key) > 0) {
			n->right = upo_bst_insert_impl(n->right,key,value,cmp);
	}
	
	return n;
}

void upo_bst_insert(upo_bst_t tree, void *key, void *value)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    // fprintf(stderr, "To be implemented!\n");
    // abort();
    if( tree == NULL) {
	    return NULL;
    }
    tree->root= upo_bst_insert_impl( tree->root, key, value, tree->key_cmp);

}

upo_bst_node_t* upo_bst_get_impl(upo_bst_node_t* node, void *key, upo_bst_comparator_t cmp) {
	if (node == NULL ) {
        //printf("get impl is null \n" );
        return NULL;
    }
	if (cmp( node->key, key) == 0) {
		return node;
	} else if (cmp( key, node->key ) < 0) {
		return upo_bst_get_impl(node->left, key, cmp);
	}else if (cmp( key, node->key ) > 0) {
		return upo_bst_get_impl(node->right, key, cmp);
	}
	
}
void* upo_bst_get(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    // fprintf(stderr, "To be implemented!\n");
    // abort();
    upo_bst_node_t *node = NULL;
	if( tree != NULL ) {
		node = upo_bst_get_impl(tree->root, key,  tree->key_cmp);
	} 
	if (node != NULL) {
		return node->value;
	}
    return NULL;
}


int upo_bst_contains(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    upo_bst_node_t* node = NULL;
    if( tree != NULL && tree->root != NULL) {
        //printf("get \n");
        node = upo_bst_get_impl(tree->root, key, tree->key_cmp);

    }
	return node == NULL ? 0 : 1;
}
upo_bst_node_t* upo_bst_max_impl(upo_bst_node_t* node) {
   
    if (node->right != NULL) {
        return upo_bst_max_impl(node->right);

    }
    return node;
 }
upo_bst_node_t* upo_bst_delete_impl (upo_bst_node_t* node, void *key, int destroy_data, upo_bst_comparator_t cmp);

upo_bst_node_t* upo_bst_delete_impl_with_childs(upo_bst_node_t* node, void *key, int destroy_data, upo_bst_comparator_t cmp) {
	upo_bst_node_t* max = upo_bst_max_impl(node->left); //find the max precedessor
	node->key = max->key;
	node->value = max->value;
	node->left = upo_bst_delete_impl(max->left, max->key, destroy_data, cmp);
	
	return node;
	
}

upo_bst_node_t* upo_bst_delete_impl_no_childs(upo_bst_node_t* node, void *key, int destroy_data, upo_bst_comparator_t cmp) {
	upo_bst_node_t* tmp = node;
	if( node->left != NULL) {
		node = node->left;
	}
	else {
		node = node->right;
	}
	
	if (destroy_data)
    {
            free(tmp->key);
            free(tmp->value);
    }

    free(tmp);
	return node;
	
}
upo_bst_node_t* upo_bst_delete_impl (upo_bst_node_t* node, void *key, int destroy_data, upo_bst_comparator_t cmp) {
	if( node == NULL) {
		return NULL;
	}
	if( cmp(key, node->key) < 0 ) {
		node->left = upo_bst_delete_impl(node->left, key, destroy_data, cmp);
	}
	else if( cmp(key, node->key) > 0 ) {
		node->right = upo_bst_delete_impl(node->right, key, destroy_data, cmp);
	}
	else if(node->left != NULL && node->right != NULL) {
		node = upo_bst_delete_impl_with_childs(node, key, destroy_data, cmp);
	}
    else {
        node = upo_bst_delete_impl_no_childs(node, key, destroy_data, cmp);
    }
	
	return node;
}
void upo_bst_delete(upo_bst_t tree, const void *key, int destroy_data)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    if(tree == NULL) {
    	return;
    }
    tree->root = upo_bst_delete_impl(tree->root, key, destroy_data, tree->key_cmp);

}

size_t upo_bst_size_impl(upo_bst_node_t* node) {
	if (node == NULL) {
		return 0;
	}
	return 1+upo_bst_size_impl(node->left) + upo_bst_size_impl(node->right);
}

size_t upo_bst_size(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
     if(tree == NULL) {
	     return 0;
     }
     return upo_bst_size_impl(tree->root);

}


int upo_bst_is_leaf(upo_bst_node_t* node) {
	if (node == NULL || (node->left != NULL || node->right != NULL)) {
		return 0;
	} else {
		return 1;
	}
}

size_t upo_bst_height_impl(upo_bst_node_t* node){
	if(node == NULL || upo_bst_is_leaf(node)) {
		return 0;
	}
	return 1 + fmax(upo_bst_height_impl(node->left),upo_bst_height_impl(node->right));
}

size_t upo_bst_height(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
   	/*fprintf(stderr, "To be implemented!\n");
    abort(); */
    if( tree == NULL || tree->root == 0) {
    	return 0;
    }
    return upo_bst_height_impl(tree->root);

}


void upo_bst_traverse_in_order_impl(upo_bst_node_t* node, upo_bst_visitor_t visit, void *visit_arg) {
	if (node != NULL) {
          upo_bst_traverse_in_order_impl(node->left, visit, visit_arg);
          visit(node->key, node->value, visit_arg);
          upo_bst_traverse_in_order_impl(node->right, visit, visit_arg);
     }

}
void upo_bst_traverse_in_order(const upo_bst_t tree, upo_bst_visitor_t visit, void *visit_arg)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
     abort();*/
     upo_bst_traverse_in_order_impl(tree->root, visit, visit_arg);
     

}

int upo_bst_is_empty(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    // fprintf(stderr, "To be implemented!\n");
    // abort();
    if ( tree == NULL || tree->root == NULL) {
        return 1;
    }
    return 0;
}


/**** EXERCISE #1 - END of FUNDAMENTAL OPERATIONS ****/


/**** EXERCISE #2 - BEGIN of EXTRA OPERATIONS ****/

upo_bst_node_t * upo_bst_min_impl(upo_bst_node_t *node) {
    
    if(node == NULL){
		return NULL;
	}
    printf("check node %d \n", *(int *)node->key);
    if ( node->left != NULL ) {
        printf("nodeleft %d \n", *(int *)node->left->key);
        return upo_bst_min_impl(node->left);
    }
    printf("return node %d \n", *(int *)node->key);
    return node;

}
void* upo_bst_min(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    // fprintf(stderr, "To be implemented!\n");
    // abort();
    if ( tree == NULL || tree->root == NULL) {
        return NULL;
    }   
    upo_bst_node_t *node = malloc(sizeof(upo_bst_node_t));
    node = upo_bst_min_impl(tree->root);
    if ( node == NULL) {
        return 0;
    }
    return node->key;


    
}

 
void* upo_bst_max(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
     if ( tree == NULL || tree->root == NULL) {
        return NULL;
    }   
    upo_bst_node_t *node = malloc(sizeof(upo_bst_node_t));
    node = upo_bst_max_impl(tree->root);
    if ( node == NULL) {
        return 0;
    }
    return node->key;
}

void upo_bst_delete_min_impl(upo_bst_node_t *node, int destroy_data,upo_bst_t tree){
    if ( node->left != NULL) {
        upo_bst_delete_min_impl(node->left, destroy_data,tree);
    } else {
        printf("found delete min %d\n", *(int *)node->key); 
        upo_bst_delete(tree, node->key, destroy_data);
    }
  
}
void upo_bst_delete_min(upo_bst_t tree, int destroy_data)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    // fprintf(stderr, "To be implemented!\n");
    // abort();
    if( tree == NULL || tree->root == NULL) {
        return;
    }
    upo_bst_delete_min_impl(tree->root, destroy_data, tree);
}

void upo_bst_delete_max_impl(upo_bst_node_t *node, int destroy_data,upo_bst_t tree){
    if(node == NULL)
	{
		return;
	}
    if ( node->right != NULL) {
        upo_bst_delete_max_impl(node->right, destroy_data,tree);
    } else {
        printf("found delete max %d\n", *(int *)node->key); 
        upo_bst_delete(tree, node->key, destroy_data);
    }
}

void upo_bst_delete_max(upo_bst_t tree, int destroy_data)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    // fprintf(stderr, "To be implemented!\n");
    // abort();
    if( tree == NULL || tree->root == NULL) {
        return;
    }
    upo_bst_delete_max_impl(tree->root, destroy_data, tree);
}

void* upo_bst_floor(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

void *upo_bst_ceiling_impl(upo_bst_node_t *node, const void *key, upo_bst_comparator_t cmp)
{
	if(node == NULL){
		return NULL;
	}
	
	if(cmp(key, node->key) >= 0)
	{
		return upo_bst_ceiling_impl(node->right, key, cmp);
	}
	else
	{
		//return upo_bst_little_branch(node);
		return NULL;
	}
}

void* upo_bst_ceiling(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
     if(tree == NULL)
	{
		return NULL;
	}
	
	return upo_bst_ceiling_impl(tree->root, key, tree->key_cmp);
}

upo_bst_key_list_t upo_bst_keys_range(const upo_bst_t tree, const void *low_key, const void *high_key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

upo_bst_key_list_t upo_bst_keys(const upo_bst_t tree)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

int upo_bst_is_bst(const upo_bst_t tree, const void *min_key, const void *max_key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}


/**** EXERCISE #2 - END of EXTRA OPERATIONS ****/


upo_bst_comparator_t upo_bst_get_comparator(const upo_bst_t tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    return tree->key_cmp;
}
