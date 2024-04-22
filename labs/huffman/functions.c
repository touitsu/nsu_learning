#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <io.h>
#include "functions.h"


pq* initpq() {
	pq* queue;

	queue = (pq*)malloc(sizeof(pq));

	queue->val = NULL;
	queue->next = NULL;

	return queue;
}


void insertpq(pq* root, pq* node) {
	tree* tmp;

	if (root->val == NULL) {
		root->val = node->val;
		free(node);
	}

	else {
		if (root->val->fq > node->val->fq) {
			tmp = root->val;
			root->val = node->val;
			node->val = tmp;
			node->next = root->next;
			root->next = node;
		}

		else {
			if (root->next) {
				if (root->next->val->fq < node->val->fq)
					insertpq(root->next, node);

				else {
					node->next = root->next;
					root->next = node;
				}
			}

			else
				root->next = node;

		}
	}
}


pq* pop(pq** root) {
	pq* res;

	if (*root == NULL)
		res = NULL;

	else if (!(*root)->next)
		res = *root;

	else {
		res = *root;
		*root = (*root)->next;
	}

	return res;
}


tree* inittree() {
	tree* tr;

	tr = (tree*)malloc(sizeof(tree));

	tr->val = UINT32_MAX;
	tr->fq = 0;
	tr->left = NULL;
	tr->right = NULL;

	return tr;
}


void freetree(tree* tr) {
	if (tr) {
		freetree(tr->right);
		freetree(tr->left);
		free(tr);
	}
}


int32_t unicodetoint(uint8_t c) {
	int32_t res;
	int8_t octc;

	if ((c & 0x80) == 0)
		res = c;

	else if ((c & 0xE0) == 0xC0) {
		res = (c & 0x1F) << 6;
		c = fgetc(stdin);
		res += (c & 0x3F);
	}

	else if ((c & 0xF0) == 0xE0) {
		res = (c & 0x0F) << 12;
		c = fgetc(stdin);
		res += ((c & 0x3F) << 6);
		c = fgetc(stdin);
		res += (c & 0x3F);
	}
	else {
		res = (c & 0x0F) << 18;
		c = fgetc(stdin);
		res += ((c & 0x3F) << 12);
		c = fgetc(stdin);
		res += ((c & 0x3F) << 6);
		c = fgetc(stdin);
		res += (c & 0x3F);
	}

	return res;
}


char* inttounicode(uint32_t n) {
	char* res;

	if (n <= 0x8F) {
		res = (char*)malloc(sizeof(char) * 2);
		*(res) = n;
		*(res + 1) = '\0';
	}

	else if (n > 0x8F && n <= 0x8FF) {
		res = (char*)malloc(sizeof(char) * 3);
		*(res) = (n >> 6) + 0xC0;
		*(res + 1) = (n & 0x03F) + 0x80;
		*(res + 2) = '\0';
	}

	else if (n > 0x8FF && n <= 0xFFFF) {
		res = (char*)malloc(sizeof(char) * 4);
		*(res) = (n >> 12) + 0xE0;
		for (int32_t i = 1; i < 3; i++)
			*(res + i) = ((n >> (12 - 6 * i)) & 0x03F) + 0x80;

		*(res + 3) = '\0';
	}

	else {
		res = (char*)malloc(sizeof(char) * 5);
		*(res) = n >> 18 + 0xF0;
		for (int32_t i = 1; i < 4; i++)
			*(res + i) = (n >> (18 - 6 * i)) & (0x03F << (8 * (i - 1))) + 0x80;

		*(res + 4) = '\0';
	}

	return res;
}


tree** getleafes(int32_t* n) {
	int64_t* occurances;
	int32_t unic, upperl;
	char c;
	tree** leafes;
	tree* tr;

	occurances = (int64_t*)calloc(0xFFFF - 1, sizeof(int64_t));

	c = fgetc(stdin);
	unic = 0;

	if (c == -17)
		for (int32_t i = 0; i < 3; i++)
			c = fgetc(stdin);

	while (c != EOF) {
		unic = unicodetoint(c);

		*(occurances + unic) += 1;
		c = fgetc(stdin);
	}

	upperl = 8;
	leafes = (tree**)malloc(sizeof(tree*) * upperl);

	for (int32_t i = 0; i < 0xFFFF - 1; i++) {
		if (*(occurances + i) != 0) {
			if ((*n) + 1 == upperl) {
				upperl += 16;
				leafes = (tree**)realloc(leafes, sizeof(tree*) * upperl);
			}

			tr = inittree();
			tr->val = i;
			tr->fq = *(occurances + i);
			*(leafes + (*n)) = tr;
			(*n)++;
		}
	}

	leafes = (tree**)realloc(leafes, sizeof(tree*) * (*n));

	free(occurances);

	return leafes;
}


tree* huffmantree(tree** leafes, int32_t n) {
	pq* queue;
	pq* tmpqueue;
	pq* tmpqueue2;
	tree* tr;

	queue = initpq();

	for (int32_t i = 0; i < n; i++) {
		tmpqueue = initpq();
		tmpqueue->val = *(leafes + i);
		insertpq(queue, tmpqueue);
	}

	tr = NULL;

	while (queue->next) {
		tmpqueue = pop(&queue);
		tmpqueue2 = pop(&queue);



		tr = inittree();
		tr->fq = tmpqueue->val->fq + tmpqueue2->val->fq;
		tr->right = tmpqueue->val;
		tr->left = tmpqueue2->val;

		if (tmpqueue2 == queue) {
			free(tmpqueue);
			free(tmpqueue2);
			return tr;
		}

		free(tmpqueue);
		free(tmpqueue2);

		tmpqueue = initpq();
		tmpqueue->val = tr;
		insertpq(queue, tmpqueue);
	}
}


int32_t strlen(char* str) {
	int32_t res;

	for (res = 0; *(str + res) != '\0'; res++);

	return res;
}


char* concat(char* a, char* b) {
	char* res;
	int32_t alen, blen;

	alen = strlen(a);
	blen = strlen(b);

	res = (char*)malloc(sizeof(char) * (alen + blen + 1));

	for (int32_t i = 0; i < alen; i++)
		*(res + i) = *(a + i);

	for (int32_t i = 0; i < blen; i++)
		*(res + alen + i) = *(b + i);

	*(res + alen + blen) = '\0';

	return res;
}


char* copystr(char* str) {
	char* res;

	res = (char*)malloc(sizeof(char) * (strlen(str) + 1));

	for (int32_t i = 0; i <= strlen(str); i++)
		*(res + i) = *(str + i);

	return res;
}


void getpaths(kv* res, tree* tr, char* val) {
	char* newval1;
	char* newval2;
	static int32_t i = 0;

	if (tr) {
		if (tr->val != UINT32_MAX) {
			(*(res + i)).key = tr->val;
			(*(res + i)).val = copystr(val);
			i++;
		}
		else {
			newval1 = concat(val, "1\0");
			newval2 = concat(val, "0\0");

			getpaths(res, tr->right, newval1);
			getpaths(res, tr->left, newval2);

			free(newval1);
			free(newval2);
		}
	}
}


void reversestring(char* str) {
	char tmp;
	for (int32_t i = 0; i < strlen(str) / 2; i++) {
		tmp = *(str + i);
		*(str + i) = *(str + (strlen(str) - i) - 1);
		*(str + (strlen(str) - i) - 1) = tmp;
	}
}


string* huffcodetobytes(char* str) {
	string* res;
	int64_t bytes;

	res = (string*)malloc(sizeof(string));
	res->content = (char*)malloc(sizeof(char) * 9);

	bytes = 1;

	for (int32_t i = 0; i < strlen(str); i++)
		bytes += (*(str + i) - '0') << i;

	for (int32_t i = 0; bytes != 0; i++) {
		*(res->content + i) = bytes % 256;
		bytes /= 256;
		*(res->content + i + 1) = '\0';
		res->len = i + 1;
	}

	return res;
}


void dumpkv(FILE* kvout, kv* res, int32_t n) {
	int32_t tmp;
	char* tmpstr;

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; (*(res + i)).key != 0; j++) {
			fputc((*(res + i)).key % 256, kvout);
			(*(res + i)).key /= 256;
		}

		fputc(0, kvout);

		for (int32_t j = 0; j < strlen((*(res + i)).val); j++)
			fputc(*((*(res + i)).val + j), kvout);

		fputc('\n', kvout);
	}
}


void encode(FILE* in, FILE* out, FILE* kvout) {
	int32_t n, tmp;
	tree* tr;
	tree** leafes;
	kv* res;
	char* tmp2;
	string* tmp1;
	char c;

	n = 0;

	leafes = getleafes(&n);

	tr = huffmantree(leafes, n);

	res = (kv*)malloc(sizeof(kv) * n);

	getpaths(res, tr, "\0");

	free(leafes);
	freetree(tr);

	fputc(0xEF, out);
	fputc(0xBB, out);
	fputc(0xBF, out);

	rewind(in);

	c = fgetc(in);

	if (c == -17)
		for (int32_t i = 0; i < 3; c = fgetc(in), i++);

	while (c != EOF) {
		tmp = unicodetoint(c);

		for (int32_t i = 0; i < n; i++)
			if (tmp == (*(res + i)).key) {
				tmp1 = huffcodetobytes((*(res + i)).val);
				fputs(tmp1->content, out);
				free(tmp1->content);
				free(tmp1);
			}
		c = fgetc(in);
	}

	dumpkv(kvout, res, n);

	for (int32_t i = 0; i < n; i++)
		free((*(res + i)).val);

	free(res);
}


void inserttree(tree* tr, tree* node, int32_t n, int32_t curn) {
	if (tr && curn <= n) {
		if (n == curn * 2)
			tr->left = node;

		else if (n == curn * 2 + 1)
			tr->right = node;

		else {
			inserttree(tr->left, node, n, curn * 2);
			inserttree(tr->right, node, n, curn * 2 + 1);
		}
	}
}


kv* readkv(FILE* in, int32_t* leafescount) {
	kv* res;
	uint8_t c;
	uint32_t n, key;
	char* code;

	res = (kv*)malloc(sizeof(kv) * ((*leafescount) + 1));

	c = fgetc(in);

	key = 0;
	code = (char*)malloc(sizeof(char) * 2);

	while (c != 255) {
		res = (kv*)realloc(res, sizeof(kv) * ((*leafescount) + 1));

		for (int32_t i = 0; c != 0; i++) {
			key += (c << (8 * i));
			c = fgetc(in);
		}
		c = fgetc(in);

		for (int32_t i = 0; c != '\n'; i++) {
			code = (char*)realloc(code, sizeof(char) * (i + 2));
			*(code + i) = c;
			*(code + i + 1) = '\0';
			c = fgetc(in);
		}
		c = fgetc(in);

		(*(res + *leafescount)).key = key;
		(*(res + *leafescount)).val = code;
		(*leafescount)++;

		key = 0;
		n = 0;
		code = (char*)malloc(sizeof(char) * 2);
	}

	return res;
}


int8_t strcmp(string* str1, string* str2) {
	for (int32_t i = 0; i < str1->len && i < str2->len; i++)
		if (*(str1->content + i) != *(str2->content + i))
			return 0;

	return 1;
}


int8_t charcmp(char* str1, char* str2) {
	if (strlen(str1) != strlen(str2))
		return 0;

	for (int32_t i = 0; i < strlen(str1); i++)
		if (*(str1 + i) != *(str2 + i))
			return 0;

	return 1;
}


void moveleft(string* str, FILE* in, int32_t n) {
	char c;

	for (int32_t i = 0; i < n; i++) {
		c = fgetc(in);
		for (int32_t j = 0; j < str->len - 1; j++) {
			*(str->content + j) = *(str->content + j + 1);
		}
		*(str->content + str->len - 1) = c;
	}
}


void decode(FILE* in, FILE* out, FILE* kvin) {
	errno_t err;
	kv* res;
	int32_t n, maxcodelen, idx;
	char c;
	string* tmp1, * tmp2;

	n = 0;

	res = readkv(kvin, &n);

	maxcodelen = 0;

	for (int32_t i = 0; i < n; i++)
		maxcodelen = max(maxcodelen, strlen((*(res + i)).val) / 8 + 1);

	tmp1 = (string*)malloc(sizeof(string));
	tmp1->len = maxcodelen;
	tmp1->content = (char*)malloc(sizeof(char) * tmp1->len);

	c = fgetc(in);
	if (c == -17)
		for (int32_t i = 0; i < 2; c = fgetc(in), i++);

	moveleft(tmp1, in, maxcodelen);

	fputc(0xEF, out);
	fputc(0xBB, out);
	fputc(0xBF, out);

	idx = -1;
	tmp2 = NULL;

	while (*(tmp1->content) != EOF) {
		for (int32_t i = 0; i < n && idx == -1; i++) {
			free(tmp2);
			tmp2 = huffcodetobytes((*(res + i)).val);

			if (strcmp(tmp1, tmp2))
				idx = i;

			else
				free(tmp2->content);
		}

		moveleft(tmp1, in, tmp2->len);
		free(tmp2->content);
		free(tmp2);


		tmp2 = inttounicode((*(res + idx)).key);

		fputs(tmp2, out);

		idx = -1;
	}

	free(tmp2);
	free(tmp1);
}


char* tobin(char* path) {
	char* res;
	int32_t i;

	res = (char*)malloc(sizeof(char) * 2);

	i = 0;

	for (i = 0; *(path + i); i++) {
		res = (char*)realloc(res, sizeof(char) * (2 + i));
		*(res + i) = *(path + i);
		*(res + i + 1) = '\0';
	}

	*(res + i - 3) = 'b';
	*(res + i - 2) = 'i';
	*(res + i - 1) = 'n';

	return res;
}