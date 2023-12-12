#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>


typedef struct Tree {
	int32_t val;
	struct Tree* left;
	struct Tree* right;
}Tree;


Tree* inittree(Tree* tree, int32_t value) {
	tree = (Tree*)malloc(sizeof(Tree));

	tree->val = value;
	tree->right = NULL;
	tree->left = NULL;
}


void freetree(Tree* tree) {
	if (tree->right)
		freetree(tree->right);

	if (tree->left)
		freetree(tree->left);

	free(tree);
}


void addelement(Tree* tree, int32_t value) {
	Tree* newtree;

	newtree = NULL;

	if (value > tree->val) {
		if (tree->right)
			addelement(tree->right, value);

		else {
			newtree = inittree(newtree, value);
			tree->right = newtree;
		}
	}
	else if (value < tree->val) {
		if (tree->left)
			addelement(tree->left, value);

		else {
			newtree = inittree(newtree, value);
			tree->left = newtree;
		}
	}
}


int32_t maximum(int32_t a, int32_t b) {
	if (a >= b)
		return a;

	return b;
}


int32_t getdepth(Tree* tree) {
	int32_t leftdepth, rightdepth;

	leftdepth = 0;
	rightdepth = 0;

	if (!tree)
		return 0;

	leftdepth += getdepth(tree->left);
	rightdepth += getdepth(tree->right);

	return maximum(rightdepth, leftdepth) + 1;
}


int32_t isbalanced(Tree* tree) {
	int32_t leftdepth, rightdepth;

	leftdepth = 0;
	rightdepth = 0;

	leftdepth = getdepth(tree->left);
	rightdepth = getdepth(tree->right);

	printf("%d %d\n", leftdepth, rightdepth);

	if (leftdepth < rightdepth && leftdepth + 1 >= rightdepth)
		return 1;

	else if (rightdepth < leftdepth && rightdepth + 1 >= leftdepth)
		return 1;

	return 0;
}


void search(Tree* tree) {
	printf("%d ", tree->val);
	if (tree->left)
		search(tree->left);

	if (tree->right)
		search(tree->right);
}


int32_t main() {
	Tree* root;
	int32_t n, tmp;

	root = NULL;
	n = 0;
	tmp = 0;

	scanf_s("%d", &n);

	scanf_s("%d", &tmp);

	root = inittree(root, tmp);

	for (int32_t i = 1; i < n; i++) {
		scanf_s("%d", &tmp);
		addelement(root, tmp);
	}

	search(root);

	freetree(root);

	return 0;
}