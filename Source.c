#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define BUFF_STRINGSIZE 100000

typedef struct node{
	void* left;
	void* right;
	
	int lvl;

	//na testy
	int visit;

}NODE;

typedef struct bdd {
	NODE* head;
	int size;
	int vars;
}BDD;

NODE* buildBDD(NODE** parent, char* bool, int lvl, int* one, int* zero, int* size);
NODE* buildROBDD(NODE** parent, char* bool, int lvl, NODE*** hashtable, int* one, int* zero, int* size);
int getResult(NODE** parent, char* BF);
int getResult_(NODE** parent, char* bool);//tmp
int getHash(void* ptr);
NODE** init(char* bool);
void hashInsert(NODE* node, NODE*** hashtable, int h, int lvl);
int findHashIndex(NODE* node, NODE*** hashtable, int lvl, int h_size, int *flag);
int getNodesCount(NODE* start);
void testResults(BDD* reduced, BDD* full, char* BF);
char* incBinaryString(char* string);
int testDecidingTime(BDD* bdd, char* s);

char* generateRandVector(int size, int a);
char* generateVector(int size, int a);
void overalTesting(BDD* robdd, BDD* bdd, char* s);

BDD* ROBDD_create(char* BF, int* one, int* zero);
BDD* BDD_create(char* BF, int* one, int* zero);
char BDD_use(BDD* bdd, char* vstupy);


int main() {
	srand(time(NULL));
	int i, one = 1, zero = 0, size = 0, size2 = 2;
	NODE* a;
	time_t dt;


	char* s = generateRandVector(pow(2, 16), 1000);
	//char* s = generateVector(pow(2, 16), 50);
	//printf("%s\n", s);
	printf("========%f=====\n", log2(strlen(s)) );

	dt = clock();
	BDD * robdd = ROBDD_create(s, &one, &zero);
	printf("Vytvorenie redukovaneho stromu trval: %dms\n", (clock() - dt)*1000 / CLOCKS_PER_SEC);
	dt = clock();
	BDD* bdd = BDD_create(s, &one, &zero);
	printf("Vytvorenie neredukovaneho stromu trval: %dms\n\n", (clock() - dt)*1000 / CLOCKS_PER_SEC);
	//printf("..Building finished\n\nTesting..\n");

	overalTesting(robdd, bdd, s);


	
	printf("End");
	return 0;
}


BDD* ROBDD_create(char *BF, int* one, int* zero) {
	//alokovanie potrebnej pamate
	int i, size = 2;
	NODE** hashtable = init(BF);
	NODE* start = malloc(sizeof(NODE));;
	BDD* bdd = malloc(sizeof(BDD));

	//rekurzivna funkcia na vytvorenie diagramu
	buildROBDD(start, BF, 0, &hashtable, one, zero, &size);
	bdd->head = hashtable[0];
	bdd->size = size;
	bdd->vars = log2(strlen(BF));
	
	return bdd;
}

BDD* BDD_create(char* BF, int* one, int* zero) {
	//alokovanie pamate
	int i, size = 0;
	NODE* start = malloc(sizeof(NODE));;
	BDD* bdd = malloc(sizeof(BDD));

	//rekurzivna funkcia na vytvorenie diagramu
	start = buildBDD(start, BF, 0, one, zero, &size);
	bdd->head = start;
	bdd->size = size;
	bdd->vars = log2(strlen(BF));

	return bdd;
}

char BDD_use(BDD* bdd, char* vstupy) {
	return getResult(bdd->head, vstupy);
}

//hlavná testovacia funkcia
void overalTesting(BDD * robdd, BDD* bdd, char *s) {
	int i, msR = 0, msF = 0;
	int tests = 10;
	NODE *reducedTree = robdd->head;
	NODE *fullTree = bdd->head;

	//vypis poctu buniek a vypocitanie miery redukovania
	printf("Redukovany | pocet buniek: %d\n", robdd->size);
	printf("BDD | pocet buniek: %d\n", bdd->size);
	printf("Percentualna miera zredukovania: %.2f%%\n\n", 100 - (float)robdd->size / bdd->size * 100);
	
	//otestuje ci redukovany aj neredukovany tvar diagramu maju rovnake vysledky
	testResults(robdd, bdd, s);

	//otestuje casy, za ktore prejde vsetky kluce vo vektore
	for (i = 0; i < tests; i++) {
		msR += testDecidingTime(robdd, s);
		msF += testDecidingTime(bdd, s);
	}
	printf("Redukovany | Priemerny cas rozhodovania: %dms\n", msR/tests);
	printf("BDD | Priemerny cas rozhodovania: %dms\n", msF/tests);
}

//generovanie pravidelneho vektora
char* generateVector(int size, int a) {
	char* s = malloc(sizeof(char) * size);
	int r = 1, i;

	for (i = 0; i < size; i++) {
		
		//kontrola kedy ma zmenit ci zapisuje 0 alebo 1
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

//generovanie nahodneho vektora
char* generateRandVector(int size, int a) {
	char* s = malloc(sizeof(char) * size);
	int r,i;
	//naplnenie vektora nahodne 0 alebo 1 podla zadaneho pomeru (a)
	for (i = 0; i < size; i++) {
		
		r = rand() % (2+a);
		switch(r) {
			case 0: s[i] = '0';
				break;
			default: s[i] = '1';
				break;
		}
		
	}
	s[i] = '\0';
	return s;
}

//testovanie ci redukovany aj neredukovany tvar maju rovnaky vysledky
void testResults(BDD * reduced, BDD * full, char* BF) {
	int i,log = log2(strlen(BF));
	char* key = malloc(sizeof(char) * log);
	
	//prvy kluc
	for (i = 0; i < log; i++)
		key[i] = '0';
	key[i] = '\0';

	for (i = 0; i < pow(2, log); i++) {
		//zvacsovanie kluca 0000..-1111..
		strcpy(key, incBinaryString(key));
		//printf("%s\n", key );
		//kontrola
		if (BDD_use(reduced, key) != BDD_use(full, key)) {
			printf("ERROR %s reduced: %d - full: %d\n", key, BDD_use(reduced, key), BDD_use(full, key));

		}

	}
	
}

//meranie casu, za ktory skontroluje diagram pre vsetky mozne kombinacie klucov
int testDecidingTime(BDD* bdd, char*s) {
	int i, log = log2(strlen(s));
	char* key = malloc(sizeof(char) * log);

	for (i = 0; i < log; i++)
		key[i] = '0';
	key[i] = '\0';

	clock_t dt = 0, past = clock();
	for (i = 0; i < pow(2, log); i++) {
		//zmena kluca
		strcpy(key, incBinaryString(key));
		
		//stopky
		past = clock();
		BDD_use(bdd, s);
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

//vrati pocet prvkov (okrem 0 a 1).... 
//vo finale nepotrebna
//int getNodesCount(NODE* start) {
//
//	if (start->visit == 1)
//		return 0;
//	
//	int* p = start;
//	if (*p == 1 || *p == 0)
//		return 0;
//
//	start->visit = 1;
//	return getNodesCount(start->left) + getNodesCount(start->right) + 1;
//}

//inicializacia hashtable
NODE** init(char* bool) {
	int i, o, p, log = log2(strlen(bool));
	
	NODE** hashtable = malloc(sizeof(NODE*) * log);

	//alokuje pamat po vzoru trojuholnika, tym padom sa alokuje len tolko kolko naozaj potrebuje
	for (i = 0; i < log; i++) {
		p = pow(2, i);
		hashtable[i] = malloc(sizeof(NODE) * p);

		for (o = 0; o < p; o++) {
			hashtable[i][o].lvl = -1;
		}
	}


	return hashtable;
}

//vrati vyslednu hodnotu rovnice podla kluca (BF)
int getResult(NODE* parent, char* BF) {
	int* x = parent, sub;
	//ak je nakonci (v 0 alebo v 1), konci
	if (*x == 1 || *x == 0)
		return *x;

	//inak sa posuva do podkorenov
	if (BF[parent->lvl] == '1') {
		x = parent->right;
	}
	else {
		x = parent->left;
	}

	return getResult(x, BF);
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
	
	//ziska hash index podla pointra z jeho praveho a laveho korena
	int h = (getHash(node->left)>>1 + getHash(node->right)) % h_size;
	if (h == -1) {
		printf("NULL pointer in hash\n");
		return -2;
	}
	NODE a = (*hashtable)[lvl][h];

	//zistuje ci je nastala zhoda a ak ano, ci je to rovnaky uzol
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
	

	(*hashtable)[lvl][h].right = node->right;
	(*hashtable)[lvl][h].left = node->left;
	(*hashtable)[lvl][h].lvl = node->lvl;

}

//vytvor redukovany diagram
NODE* buildROBDD(NODE* parent, char* bool, int lvl, NODE*** hashtable, int* one, int* zero, int * size) {
	int h, h_size = pow(2, lvl), flag ;
	parent->lvl = lvl;
	NODE* buf;


	if (strlen(bool) > 2) {
		NODE* kid1 = malloc(sizeof(NODE));
		NODE* kid2 = malloc(sizeof(NODE));

		kid1->visit = 0;
		kid2->visit = 0;

		//rozdelenie vektora na 2 casti
		char* s1 = malloc(sizeof(char) * strlen(bool)), * s2 = malloc(sizeof(char) * strlen(bool));
		strncpy(s1, bool, strlen(bool) / 2);
		s1[strlen(bool) / 2] = '\0';
		strncpy(s2, bool + strlen(bool) / 2, strlen(bool) - strlen(bool) / 2);
		s2[strlen(bool) / 2] = '\0';

		//vytvorenie a priradenie korenov, pricom do kazdeho posle jednu polovicu z vektora
		parent->left = buildROBDD(kid1, s1, lvl + 1, hashtable, one, zero,size);
		parent->right = buildROBDD(kid2, s2, lvl + 1, hashtable, one, zero,size);

		//redukcia typu S - korene su ten isty uzol
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
			//redukcia typu I - ci uz rovnaky uzol existuje
			flag = 1;
			h = findHashIndex(parent, hashtable, lvl, h_size, &flag);

			//takyto prvok v zozname nie je
			if (h != -1) {
				hashInsert(parent, hashtable, h, lvl);
				(*size) += 1;
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
			(*size) += 1;
			return &(*hashtable)[lvl][h];
		}

		
		return &(*hashtable)[lvl][flag];

		
	}

}

//vytvori neredukovany rozhodovaci diagram
NODE* buildBDD(NODE* parent, char* bool, int lvl, int* one, int* zero, int* size) {
	
	
	int h, h_size = pow(2, lvl), flag;
	parent->lvl = lvl;
	NODE* buf;


	if (strlen(bool) > 2) {
		NODE* kid1 = malloc(sizeof(NODE));
		NODE* kid2 = malloc(sizeof(NODE));

		kid1->visit = 0;
		kid2->visit = 0;
		//rodelenie vektora na 2 casti
		char *s1 = malloc(sizeof(char) *strlen(bool)), *s2 = malloc(sizeof(char) * strlen(bool));
		strncpy(s1, bool, strlen(bool) / 2);
		s1[strlen(bool) / 2] = '\0';
		strncpy(s2, bool + strlen(bool) / 2, strlen(bool) - strlen(bool) / 2);
		s2[strlen(bool) / 2] = '\0';

		//rekurzivne vytvorenie a priradenie korenov uzla
		parent->left = buildBDD(kid1, s1, lvl + 1, one, zero, size);
		parent->right = buildBDD(kid2, s2, lvl + 1, one, zero, size);

		(*size) += 1;
		return parent;
	}
	else {
		//vytvarania poslednej urovne uzlov (1 a 0)

		NODE* kid = malloc(sizeof(NODE));
		kid->lvl = lvl;


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

		(*size) += 3;

		return kid;


	}

}

