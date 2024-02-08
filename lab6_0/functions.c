#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include "functions.h"


TREE* inittree(int32_t key) {
	TREE* tmp = (TREE*)malloc(sizeof(TREE));

	tmp->key = key;
	tmp->height = 1;
	tmp->left = NULL;
	tmp->right = NULL;

	return tmp;
}


int32_t getheight(TREE* tr) {
	if (tr == NULL)
		return 0;

	return tr->height;
}


void recalcheight(TREE* tr) {
	tr->height = max(getheight(tr->left), getheight(tr->right)) + 1;
}


int8_t bf(TREE* tr) {
	return getheight(tr->right) - getheight(tr->left);
}


TREE* leftrot(TREE* tr) {
	TREE* nrt;

	nrt = tr->right;
	tr->right = nrt->left;
	nrt->left = tr;

	recalcheight(tr);
	recalcheight(nrt);

	return nrt;
}


TREE* rightrot(TREE* tr) {
	TREE* nrt;

	nrt = tr->left;
	tr->left = nrt->right;
	nrt->right = tr;

	recalcheight(tr);
	recalcheight(nrt);

	return nrt;
}


TREE* balance(TREE* tr) {
	int8_t tmp;
	recalcheight(tr);

	tmp = bf(tr);

	if (tmp == 2) {
		if (bf(tr->right) < 0)
			tr->right = rightrot(tr->right);
		
		return leftrot(tr);
	}
	else if(tmp == -2) {
		if (bf(tr->left) > 0)
			tr->left = leftrot(tr->left);

		return rightrot(tr);
	}

	return tr;
}


TREE* insert(TREE* tr, int32_t key) {
	if (tr == NULL)
		return inittree(key);

	if (tr->key > key)
		tr->left = insert(tr->left,key);

	else
		tr->right = insert(tr->right, key);

	return balance(tr);
}


void freetree(TREE* tr) {
	if (tr) {
		freetree(tr->right);
		freetree(tr->left);
		free(tr);
	}
}