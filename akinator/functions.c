#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include "functions.h"


string* initstring() {
	string* str;
	
	str = (string*)malloc(sizeof(string));

	str->content = (char*)malloc(sizeof(char));
	str->len = 0;
	
	return str;
}


Tree* inittree(int32_t key, string* content) {
	Tree* tree;

	tree = (Tree*)malloc(sizeof(Tree));

	tree->value = content;
	tree->key = key;
	tree->right = NULL;
	tree->left = NULL;

	return tree;
}


void freetree(Tree* tree) {
	if (tree->right)
		freetree(tree->right);

	if (tree->left)
		freetree(tree->left);

	free(tree->value);
	free(tree);
}


void addtotree(Tree* tree, int32_t key, string* content) {
	Tree* new;

	if (tree) {
		if (key == tree->key * 2) {
			new = inittree(key, content);
			tree->left = new;
		}
		else if (key == tree->key * 2 + 1) {
			new = inittree(key, content);
			tree->right = new;
		}
		else {
			addtotree(tree->left, key, content);
			addtotree(tree->right, key, content);
		}
	}
}


void printstr(string* str) {
	for (int32_t i = 0; i < str->len; i++)
		printf("%c", *(str->content + i));
	
	printf("\n");
}


void printtree(Tree* tree) {
	if (tree) {
		printtree(tree->right);
		printtree(tree->left);
		printf("Key: %d \n", tree->key);
		printf("Value: ");
		printstr(tree->value);
	}
}


Tree* buildfromfile(FILE* file) {
	Tree* tree;
	int32_t newline, key;
	string* str;
	char c;

	newline = 1;
	key = 0;
	str = NULL;
	tree = NULL;

	c = fgetc(file);
	while (c != EOF) {

		if (newline) {
			if (c >= '0' && c <= '9') {
				key *= 10;
				key += (c - '0');
			}
			else
				newline = 0;

		}
		else {
			if (!str)
				str = initstring();

			if (c != '\n') {
				str->content = (char*)realloc(str->content, sizeof(char) * (str->len + 1));
				*(str->content + str->len) = c;
				str->len++;
			}
			else {
				*(str->content + str->len) = '\0';

				if (!tree)
					tree = inittree(key, str);
					
				else
					addtotree(tree, key, str);
					
				key = 0;
				str = NULL;
				newline = 1;
			}
			
		}
		c = fgetc(file);

	}

	return tree;
}


string* inttostr(int32_t a) {
	string* str;
	char tmp;

	str = initstring();

	for (int32_t i = 0; a != 0; i++) {
		str->content = (char*)realloc(str->content, sizeof(char) * (i + 2));
		*(str->content + i) = a % 10 + '0';
		a /= 10;
		str->len = i + 1;
	}

	*(str->content + str->len) = '\0';

	for (int32_t i = 0; i < str->len / 2; i++) {
		tmp = *(str->content + i);
		*(str->content + i) = *(str->content + str->len - 1 - i);
		*(str->content + str->len - 1 - i) = tmp;
	}
		
	return str;
}


void dumptofile(Tree* tree, FILE* file) {
	string* keytmp;

	if (tree) {
		keytmp = initstring();

		keytmp = inttostr(tree->key);

		fputs(keytmp->content, file);

		fputc(' ', file);

		fputs(tree->value->content, file);

		fputc('\n', file);

		dumptofile(tree->right, file);
		dumptofile(tree->left, file);

		free(keytmp);
	}
}


void scanstring(string* str) {
	char c;
	int32_t i;

	i = 0;
	c = getchar();

	while (c != '\n') {
		str->content = (char*)realloc(str->content, sizeof(char) * (i + 1));

		*((str->content) + i++) = c;
		str->len = i;
		c = getchar();
	}

	*((str->content) + i) = '\0';

}


int32_t mstrs(string* str, char* str2) {

	for (int32_t i = 0; i < str->len; i++)
		if (*(str->content + i) != *(str2 + i))
			return 0;

	return 1;
}


int32_t isvalidans(string* str) {
	return mstrs(str, "Yes\0") || mstrs(str, "yes\0") || mstrs(str, "No\0") || mstrs(str, "no\0");
}


void playgame(Tree* tree) {
	string *ans, *tmp1, *tmp2;

	ans = initstring();
	
	if (!tree->left && !tree->right) {
		printstr(tree->value);
		printf("Is that a correct answer? (Yes/No)\n");

		scanstring(ans);

		while (!isvalidans(ans)) {
			printf("Please enter a valid answer Yes/No\n");
			free(ans);
			ans = initstring();
			scanstring(ans);
		}

		if (mstrs(ans, "No\0") || mstrs(ans, "no\0")) {

			tmp1 = initstring();
			tmp2 = initstring();

			printf("What is the right answer?\n");

			scanstring(tmp1);

			addtotree(tree, tree->key * 2, tmp1);

			printf("Than with what question you can distinguish my guess from the right answer?\n");

			free(ans);
			ans = initstring();
			scanstring(ans);

			tmp2 = tree->value;
			tree->value = ans;

			addtotree(tree, tree->key * 2 + 1, tmp2);
		}

	}	
	else {
		printstr(tree->value);

		scanstring(ans);

		while (!isvalidans(ans)) {
			printf("Please enter a valid answer Yes/No\n");
			free(ans);
			ans = initstring();
			scanstring(ans);
		}
			

		if (mstrs(ans, "Yes\0") || mstrs(ans, "yes\0")) {
			free(ans);
			playgame(tree->right);
		}
		else {
			free(ans);
			playgame(tree->left);
		}
	}
}
