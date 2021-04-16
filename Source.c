#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct node{
	struct node* left;
	struct node* right;
	struct node* parent;
	int *false;
	int *true;
	int lvl;

}NODE;

NODE* buildBDD(NODE** parent, char* bool, int lvl, NODE*** hashtable, int* one, int* zero);
int getResult(NODE** parent, char* bool);
int getHash(void* ptr);
NODE** init(char* bool);
void hashInsert(NODE* node, NODE*** hashtable, int h, int lvl);
int findHashIndex(NODE* node, NODE** hashtable, int lvl, int h_size);

int main() {

	int i, one=1,zero=0;

	

	NODE** hashtable = init("10110010");

	NODE* start = malloc(sizeof(NODE));

	start = buildBDD(start, "10110010", 0, &hashtable, &one, &zero);


	printf("ds");
	return 0;
}

NODE** init(char* bool) {
	int i, o, p, log = log2(strlen(bool));
	
	NODE** hashtable = malloc(sizeof(NODE*) * log);
	for (i = 0; i < log; i++) {
		p = pow(2, i);
		hashtable[i] = malloc(sizeof(NODE) * p);

		for (o = 0; o < p; o++) {
			hashtable[i][o].lvl = -1;
		}
	}
	return hashtable;
}

int getResult(NODE** parent, char* bool) {
	

	/*if (strlen(bool) == 1) {
		if (bool[0] == '0') {
			return parent->false;
		}
		else {
			return parent->true;
		}
	}

	char s[100000];
	strncpy(s, bool + 1, strlen(bool));
	s[strlen(bool) - 1] = '\0';

	if (bool[0]=='0') {
		return getResult(parent->left, s);
	}
	else {
		return getResult(parent->right, s);
	}*/

}

int getHash(void* ptr){
	if (ptr != NULL) {

		int value = (int)ptr;
		value = ~value + (value << 11);
		value = value ^ (value >> 7);
		value = value + (value << 2);
		value = value ^ (value >> 5);
		value = value * 33;
		value = value ^ (value >> 17);
		return value;
	}
	return -1;
}

int findHashIndex(NODE* node, NODE** hashtable, int lvl, int h_size) {
	int h = getHash(node) % h_size;

	printf("%d %d\n", lvl, h);
	while (hashtable[lvl][h].lvl == -1) {
		if (hashtable[lvl][h].left == node->left && hashtable[lvl][h].right == node->right)
			return -1;

		h = h + 1 / h_size;
	}
	return h;
}

void hashInsert(NODE* node, NODE*** hashtable, int h, int lvl) {
	
	(*hashtable)[lvl][h].parent = node->parent;
	(*hashtable)[lvl][h].right = node->right;
	(*hashtable)[lvl][h].left = node->left;
	(*hashtable)[lvl][h].true = node->true;
	(*hashtable)[lvl][h].false = node->false;
	(*hashtable)[lvl][h].lvl = node->lvl;

}

NODE* buildBDD(NODE *parent, char* bool, int lvl, NODE***hashtable, int *one, int *zero) {
	int h, h_size = pow(2, lvl);
	
	/*if ((*hashtable)[lvl]== NULL) {
		
		(*hashtable)[lvl] = malloc(sizeof(NODE)*h_size);
		for (int i = 0; i < h_size; i++) {
			(*hashtable)[lvl][i].lvl = -1;
		}
	}*/


	if (strlen(bool) > 2) {
		NODE* kid1 = malloc(sizeof(NODE));
		NODE* kid2 = malloc(sizeof(NODE));
		kid1->lvl = lvl;
		kid2->lvl = lvl;
		kid1->parent = parent;
		kid2->parent = parent;
		/*kid1->false = NULL;
		kid1->true = NULL;
		kid2->false = NULL;
		kid2->true = NULL;*/

		char s1[100000], s2[100000];
		strncpy(s1, bool, strlen(bool) / 2);
		s1[strlen(bool) / 2] = '\0';
		strncpy(s2, bool + strlen(bool) / 2, strlen(bool) - strlen(bool) / 2);
		s2[strlen(bool) / 2] = '\0';
		
		parent->right = buildBDD(kid1, s1, lvl+1, &hashtable, one, zero);
		parent->left = buildBDD(kid2, s2, lvl+1, &hashtable, one, zero);
		
		if (parent->right == parent->left) {
			//zmaze a rodic parenta bude ukazovat na dieta 
			parent->right->parent = parent->parent;
			if (parent->parent->right == parent)
				parent->parent->right = parent->right;
			else
				parent->parent->left = parent->right;
		}
		else {
			
			h = findHashIndex(parent, hashtable, lvl, h_size);
			if (h != -1)
				hashInsert(parent, &hashtable,h, lvl);
			else {
				parent->right->parent = parent->parent;
				if (parent->parent->right == parent)
					parent->parent->right = parent->right;
				else
					parent->parent->left = parent->right;
			}
		}

		
		return parent;
	}
	else {
		//TODO
		NODE* kid = malloc(sizeof(NODE));
		kid->lvl = lvl;
		kid->parent = parent;
		/*kid->left = NULL;
		kid->right = NULL;*/
		
		//[0] je nalavo cize false
		if (bool[0] == '0') {
			kid->false = zero;
		}
		else {
			kid->false = one;
		}

		//[1] je napravo cize true
		if (bool[1] == '0') { 
			kid->true = zero;
		}
		else {
			kid->true = one;
		}

		return kid;
	}

}

