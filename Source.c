#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
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
int testDecidingTime(NODE* tree, char* s);

char* generateRandVector(int size, int a);
char* generateVector(int size, int a);
void overalTesting(NODE* reducedTree, NODE* fullTree, char* s);

int main() {
	srand(time(NULL));
	int i, one = 1, zero = 0;
	NODE* a;

	//char* s = generateRandVector(pow(2, 14), 0);
	char* s = generateVector(pow(2, 18), 4);

	NODE** hashtable= init(s);
	
	printf("********%f\n", log2(strlen(s)) );
	NODE* start = malloc(sizeof(NODE));
	NODE* BDDstart = malloc(sizeof(NODE));
	
	BDDstart = buildBDD(BDDstart, s, 0, &hashtable, &one, &zero, NULL);
	buildROBDD(start, s, 0, &hashtable, &one, &zero, NULL);
	

	overalTesting(hashtable[0], BDDstart, s);


	
	printf("end");
	return 0;
}

void overalTesting(NODE* reducedTree, NODE* fullTree, char *s) {
	int i, msR = 0, msF = 0;
	int tests = 20;

	printf("Redukovany | pocet buniek: %d\n", getNodesCount(reducedTree) + 2);
	printf("BDD | pocet buniek: %d\n", getNodesCount(fullTree)*2 + 1 );

	testResults(reducedTree, fullTree, s);

	for (i = 0; i < tests; i++) {
		msR += testDecidingTime(reducedTree, s);
		msF += testDecidingTime(fullTree, s);
	}
	printf("Redukovany | Priemerny cas rozhodovania: %dms\n", msR/tests);
	printf("BDD | Priemerny cas rozhodovania: %dms\n", msF/tests);
}


char* generateVector(int size, int a) {
	char* s = malloc(sizeof(char) * size);
	int r = 1, i;

	for (i = 0; i < size; i++) {
		//srand(time(NULL));
		if (i % (size / a) == 0)
			r *= -1;

		switch (r) {
		case 1: s[i] = '0';
			break;
		default: s[i] = '1';
			break;
		}
		
	}
	s[i] = '\0';
	return s;
}

char* generateRandVector(int size, int a) {
	char* s = malloc(sizeof(char) * size);
	int r,i;

	for (i = 0; i < size; i++) {
		
		r = rand() % (2+a);
		switch(r) {
			case 0: s[i] = '0';
				break;
			default: s[i] = '1';
				break;
		}
		//s[i] = r+'0';
	}
	s[i] = '\0';
	return s;
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
		if (getResult_(reduced, key) != getResult_(full, key)) {
			printf("ERROR %s reduced: %d - full: %d\n",key, getResult_(reduced, key), getResult_(full, key));

		}
	}

	
}

int testDecidingTime(NODE* tree, char*s) {
	int i, log = log2(strlen(s));
	char* key = malloc(sizeof(char) * log);

	for (i = 0; i < log; i++)
		key[i] = '0';
	key[i] = '\0';

	clock_t dt = 0, past = clock();
	for (i = 0; i < pow(2, log); i++) {
		strcpy(key, incBinaryString(key));
		past = clock();
		getResult(tree, key);
		dt += clock() - past;
	}
	

	//printf("Vsetky moznosti overene za: %dms\n", dt*1000/CLOCKS_PER_SEC);
	return dt * 1000 / CLOCKS_PER_SEC;
}

//zvysi vektor binarne o 1
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

//vrati pocet prvkov (okrem 0 a 1)
int getNodesCount(NODE* start) {

	if (start->visit == 1)
		return 0;
	
	int* p = start;
	if (*p == 1 || *p == 0)
		return 0;

	start->visit = 1;
	return getNodesCount(start->left) + getNodesCount(start->right) + 1;
}

//inicializacia hashtable
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

//vrati vyslednu hodnotu rovnice podla kluca (bool)
int getResult(NODE* parent, char* bool) {
	int* x = parent, sub;
	NODE* a;
	if (*x == 1 || *x == 0)
		return *x;

	if (bool[parent->lvl] == '1') {
		a = parent->right;
		x = parent->right;
	
	}
	else {
		a = parent->left;
		x = parent->left;

	}

	return getResult(x, bool);
}

//kontrolna verzia, tmp
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

//vrati hash z pointra
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

//najde index v hashtable
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

//vlozi do hashtable
void hashInsert(NODE* node, NODE*** hashtable, int h, int lvl) {
	
	(*hashtable)[lvl][h].parent = node->parent;
	(*hashtable)[lvl][h].right = node->right;
	(*hashtable)[lvl][h].left = node->left;
	(*hashtable)[lvl][h].lvl = node->lvl;

}

//vytvor redukovany diagram
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
			
			
			//ak je to uplne prvy pointer, tak tam musi nieco vlozit, aby to nebolo prazde aj ked je to redukovane na 0
			if (lvl == 0) {
				hashInsert(parent, hashtable, 0, lvl);
			}

			//return parent->right;
			return parent->right;
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

//vytvori neredukovany rozhodovaci diagram
NODE* buildBDD(NODE* parent, char* bool, int lvl, NODE*** hashtable, int* one, int* zero, char side) {
	
	
	int h, h_size = pow(2, lvl), flag;
	parent->lvl = lvl;
	NODE* buf;


	if (strlen(bool) > 2) {
		NODE* kid1 = malloc(sizeof(NODE));
		NODE* kid2 = malloc(sizeof(NODE));

		kid1->parent = parent;
		kid2->parent = parent;
		kid1->visit = 0;
		kid2->visit = 0;

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

