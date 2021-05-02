#include <stdio.h>
#include <string.h>
#include <math.h>
#define BUFF_STRINGSIZE 100000

typedef struct node{
	void* left;
	void* right;
	struct node* parent;
	/*int *false;
	int *true;*/
	int lvl;

	//na testy
	int visit;

}NODE;

NODE* buildBDD(NODE** parent, char* bool, int lvl, NODE*** hashtable, int* one, int* zero, char side);
NODE* buildROBDD(NODE** parent, char* bool, int lvl, NODE*** hashtable, int* one, int* zero, char side);
int getResult(NODE** parent, char* bool);
int getResult_(NODE** parent, char* bool);//tmp
int getHash(void* ptr);
NODE** init(char* bool);
void hashInsert(NODE* node, NODE*** hashtable, int h, int lvl);
int findHashIndex(NODE* node, NODE*** hashtable, int lvl, int h_size, int *flag);
int getNodesCount(NODE* start);
void testResults(NODE* reduced, NODE* full, char* s);
char* incBinaryString(char* string);

int main() {

	int i, one=1,zero=0, *buf, *bas;
	bas = &one;

	char s[] = "1101000111010001101000111010001101000111010001110100011101000110100011101000110100011110100011101000110100011110100011110100011101000110100011111010001110100011101011010001110100011010001110100011010001110100011101000111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111110100011101000111010001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001110011111010001110100011010001110100011010001110100011101000111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111110100011101000111010110100011101000110100011101000110100011101000111010001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001111101000111010001110100011101000110100011101000110100011110100011101000110100011110100011110100011101000110100011100111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111010001101001101000111010001101000111010001101000111010001110100011101000110100011101000110100011110100011101000110100011110100011110100011101000110100011111010001110100011101011010001110100011010001110100011010001110100011101000111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111110100011101000111010001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001110011111010001110100011010001110100011010001110100011101000111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111110100011101000111010110100011101000110100011101000110100011101000111010001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001111101000111010001110100011101000110100011101000110100011110100011101000110100011110100011110100011101000110100011100111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111010001101000111010001101000111101000111010011010001110100011010001110100011010001110100011101000111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111110100011101000111010110100011101000110100011101000110100011101000111010001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001111101000111010001110100011101000110100011101000110100011110100011101000110100011110100011110100011101000110100011100111110100011101000110100011101000110100011101000111010001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001111101000111010001110101101000111010001101000111010001101000111010001110100011101000110100011101000110100011110100011101000110100011110100011110100011101000110100011111010001110100011101000111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001110100011010011010001110100011010001110100011010001110100011101000111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111110100011101000111010110100011101000110100011101000110100011101000111010001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001111101000111010001110100011101000110100011101000110100011110100011101000110100011110100011110100011101000110100011100111110100011101000110100011101000110100011101000111010001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001111101000111010001110101101000111010001101000111010001101000111010001110100011101000110100011101000110100011110100011101000110100011110100011110100011101000110100011111010001110100011101000111010001101000111010001101000111101000111010001101000111101000111101000111010001101000111001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001110100011010001110100011010001111010001110100011010001111010001111010001110100011010001110111010001101000111101000111010001101000111101000111101000111010001101000111011010001111010001111010001110100011010001110111010001101000111101000111010001101000111101000111101000111010001101000111";
	
	//11010001101011010001101011010001101000111101000110101101000110101101000110100011001111010001101011010001101011010001101000110011110100011010001100111101000110100011001100111101101000110101101000110101101000110100011110100011010110100011010110100011010001100111101000110101101000110101101000110100011001111010001101000110011110100011010001100110011110100011010110100011010110100011010001100111101000110100011001111010001101000110011100011010110100011010110100011010001100111101000110100011001111010001101000110011
	//1101000110100011

	NODE** hashtable= init(s);
	NODE* a, * b, * c, * d;
	
	printf("********%f\n", log2(strlen(s)) );
	NODE* start = malloc(sizeof(NODE));
	NODE* BDDstart = malloc(sizeof(NODE));
	BDDstart = buildBDD(BDDstart, s, 0, &hashtable, &one, &zero, NULL);
	
	start = buildROBDD(start, s, 0, &hashtable, &one, &zero, NULL);
	a = hashtable[0][0].left;
	b = hashtable[0][0].right;

	printf("pocet buniek: %d\n",  getNodesCount(hashtable[0]));
	printf("pocet buniek: %d\n",  getNodesCount(BDDstart));

	
	testResults(hashtable[0], BDDstart, s);

	//printf("%d %d\n",getResult(hashtable[0], "1101111") , getResult(BDDstart, "1101111"));
	//getResult(BDDstart, "1101100");
	printf("end");
	return 0;
}

void testResults(NODE * reduced, NODE * full, char* s) {
	int i,log = log2(strlen(s));
	char* key = malloc(sizeof(char) * log);
	
	for (i = 0; i < log; i++)
		key[i] = '0';
	key[i] = '\0';

	for (i = 0; i < pow(2, log); i++) {
		strcpy(key, incBinaryString(key));
		//printf("%s\n", key );
		if (getResult(reduced, key) != getResult_(full, key)) {
			printf("%s reduced: %d - full: %d\n",key, getResult(reduced, key), getResult_(full, key));

		}
	}

	
}

char* incBinaryString(char* string) {
	int i, len = strlen(string);
	char* buff = malloc(len * sizeof(char));
	strcpy(buff, string);
	
	for (i = len - 1; i >= 0; i--) {
		if (buff[i] == '0') {
			buff[i] = '1';
			return buff;
		}

		buff[i] = '0';
	}
	return buff;
}

int getNodesCount(NODE* start) {

	if (start->visit == 1)
		return 0;
	
	int* p = start;
	if (*p == 1 || *p == 0)
		return 0;

	start->visit = 1;
	return getNodesCount(start->left) + getNodesCount(start->right) + 1;
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

//asi zle
int getResult(NODE* parent, char* bool) {
	int* x = parent, sub;
	NODE* a;
	if (*x == 1 || *x == 0)
		return *x;




	if (bool[parent->lvl] == '1') {
		a = parent->right;
		x = parent->right;
		if ((*x == 1 || *x == 0) && parent->lvl < (strlen(bool) - 2)) {

			if (bool[strlen(bool) - 2] == '1')
				x = parent->right;
			else
				x = parent->left;

			return getResult(x, bool);
		}
		
		sub = a->lvl - parent->lvl;

		
	}
	else {
		a = parent->left;
		x = parent->left;

		if ((*x == 1 || *x == 0) && parent->lvl < (strlen(bool) - 2)) {

			if (bool[strlen(bool) - 2] == '1')
				x = parent->right;
			else
				x = parent->left;

			return getResult(x, bool);
		}

		sub = a->lvl - parent->lvl;

	}

	return getResult(x, bool);
}

int getResult_(NODE* parent, char* bool) {
	int* x;


	if (bool[parent->lvl] == '1') {
		x = parent->right;
		
		if (*x == 1 || *x == 0)
			return *x;
	}
	else {
		x = parent->left;
		
		if (*x == 1 || *x == 0)
			return *x;
	}
	
	getResult_(x, bool);
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

int findHashIndex(NODE* node, NODE*** hashtable, int lvl, int h_size, int *flag) {
	
	int h = (getHash(node->left)>>1 + getHash(node->right)) % h_size;
	if (h == -1) {
		printf("NULL pointer in hash\n");
		return -2;
	}
	NODE a = (*hashtable)[lvl][h];

	//printf("%d %d\n", (*hashtable)[lvl][h].lvl, a.lvl);
	while ((*hashtable)[lvl][h].lvl != -1) {
		if ((*hashtable)[lvl][h].left == node->left && (*hashtable)[lvl][h].right == node->right) {
			*flag = h;
			return -1;
		}

		h = (h + 1) % h_size;
	}
	return h;
}

void hashInsert(NODE* node, NODE*** hashtable, int h, int lvl) {
	
	(*hashtable)[lvl][h].parent = node->parent;
	(*hashtable)[lvl][h].right = node->right;
	(*hashtable)[lvl][h].left = node->left;
	//(*hashtable)[lvl][h].true = node->true;
	//(*hashtable)[lvl][h].false = node->false;
	(*hashtable)[lvl][h].lvl = node->lvl;

}


NODE* buildROBDD(NODE* parent, char* bool, int lvl, NODE*** hashtable, int* one, int* zero, char side) {
	int h, h_size = pow(2, lvl), flag ;
	parent->lvl = lvl;
	NODE* buf;


	if (strlen(bool) > 2) {
		NODE* kid1 = malloc(sizeof(NODE));
		NODE* kid2 = malloc(sizeof(NODE));
	
		kid1->parent = parent;
		kid2->parent = parent;
		kid1->visit = 0;
		kid2->visit = 0;

		//char s1[BUFF_STRINGSIZE], s2[BUFF_STRINGSIZE];
		char* s1 = malloc(sizeof(char) * strlen(bool)), * s2 = malloc(sizeof(char) * strlen(bool));
		strncpy(s1, bool, strlen(bool) / 2);
		s1[strlen(bool) / 2] = '\0';
		strncpy(s2, bool + strlen(bool) / 2, strlen(bool) - strlen(bool) / 2);
		s2[strlen(bool) / 2] = '\0';

		//test switched right - left
		parent->left = buildROBDD(kid1, s1, lvl + 1, hashtable, one, zero, 'R');
		parent->right = buildROBDD(kid2, s2, lvl + 1, hashtable, one, zero, 'L');

		if (parent->right == parent->left) {
			//zmaze a rodic parenta bude ukazovat na dieta 
			if (parent->right != one && parent->right != zero) {
				//buf = parent->right;
				////buf->parent = parent->parent;
				//parent->right = buf->right;
				//parent->left = buf->left;
				return parent->right;
			}
			//ak je to uplne prvy pointer, tak tam musi nieco vlozit, aby to nebolo prazde aj ked je to redukovane na 0
			if (lvl == 0) {
				hashInsert(parent, hashtable, 0, lvl);
			}

			//return parent->right;
			return parent;
		}
		else {
			flag = 1;
			h = findHashIndex(parent, hashtable, lvl, h_size, &flag);

			//takyto prvok v zozname nie je
			if (h != -1) {
				hashInsert(parent, hashtable, h, lvl);

				return &(*hashtable)[lvl][h];
			}

			return &(*hashtable)[lvl][flag];
		}


		//return parent;
	}
	else {
		//vytvorenie a redukovanie prvkov na poslednej urovni

		NODE* kid = malloc(sizeof(NODE));
		kid->lvl = lvl;
		kid->parent = parent;

		kid->visit = 0;
	

		//[0] je nalavo cize false
		if (bool[0] == '0') {
			kid->left = zero;
		}
		else {
			kid->left = one;
		}

		//[1] je napravo cize true
		if (bool[1] == '0') {
			kid->right = zero;
		}
		else {
			kid->right = one;
		}

		if (kid->right == kid->left) {
			
			return kid->right;
		}
		
		flag = 1;
		h = findHashIndex(kid, hashtable, lvl, h_size, &flag);
		
		//takyto prvok v zozname nie je
		if (h != -1) {
			hashInsert(kid, hashtable, h, lvl);

			return &(*hashtable)[lvl][h];
		}

		return &(*hashtable)[lvl][flag];

		
	}

}

NODE* buildBDD(NODE* parent, char* bool, int lvl, NODE*** hashtable, int* one, int* zero, char side) {
	
	
	//printf("pred\n");
	int h, h_size = pow(2, lvl), flag;
	//printf("2 %d\n", h_size);
	parent->lvl = lvl;
	NODE* buf;


	if (strlen(bool) > 2) {
		NODE* kid1 = malloc(sizeof(NODE));
		NODE* kid2 = malloc(sizeof(NODE));

		kid1->parent = parent;
		kid2->parent = parent;
		kid1->visit = 0;
		kid2->visit = 0;

		//char s1[BUFF_STRINGSIZE], s2[BUFF_STRINGSIZE];
		char *s1 = malloc(sizeof(char) *strlen(bool)), *s2 = malloc(sizeof(char) * strlen(bool));
		strncpy(s1, bool, strlen(bool) / 2);
		s1[strlen(bool) / 2] = '\0';
		strncpy(s2, bool + strlen(bool) / 2, strlen(bool) - strlen(bool) / 2);
		s2[strlen(bool) / 2] = '\0';

		parent->left = buildBDD(kid1, s1, lvl + 1, hashtable, one, zero, 'R');
		parent->right = buildBDD(kid2, s2, lvl + 1, hashtable, one, zero, 'L');

		return parent;
	}
	else {
		

		NODE* kid = malloc(sizeof(NODE));
		kid->lvl = lvl;
		kid->parent = parent;

		kid->visit = 0;


		//[0] je nalavo cize false
		if (bool[0] == '0') {
			kid->left = zero;
		}
		else {
			kid->left = one;
		}

		//[1] je napravo cize true
		if (bool[1] == '0') {
			kid->right = zero;
		}
		else {
			kid->right = one;
		}

		return kid;


	}

}

