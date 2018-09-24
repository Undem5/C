#include <stdlib.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <string.h> 

#define MAXLEN 100 

void initMyAlgo();

void insertMyAlgo(unsigned int addr,unsigned int netmask,unsigned int gw);

unsigned int lookupMyAlgo(unsigned int addr);
 




struct list{
	char hex[3];
	int bits;
	struct list *next;
}list;

struct node{
	unsigned int addr;
	unsigned int gw;
	struct node *left;
	struct node *right;
};

struct node* newNode(unsigned int addr, unsigned int gw){
	struct node* node = (struct node*)malloc(sizeof(struct node));
	node->addr = addr;
	node->gw = gw;
	node->left = NULL;
	node->right = NULL;
	return node;
}



struct list* MaListe = NULL;

void push(struct list** start, char name[2], int nbBits){

//------------ push at the beginning ----------------------//
//--------------------- O(1)-------------------------------//
	
	struct list* newList = (struct list*) malloc(sizeof(struct list));
	newList->bits = nbBits;
	strcpy(newList->hex,name);
	newList->next = *start;
	*start = newList;

	printf(" Name of list: %s nbBits: %d\n",newList->hex,newList->bits);
}



// This function must not be modified !!! 
int loadFile(char *path) 
{
	FILE *fi;
	char s[MAXLEN],*p,*start; 
 	struct in_addr c; 
	unsigned int addr,netmask,gw; 

	// first sanity check 
	if (!path) return 0; 
	if ((fi=fopen(path,"r")) == NULL ) return 0; 

	// read file line by line (max = MAXLEN ) 
	while (fgets(s,MAXLEN,fi) != NULL) {
		s[MAXLEN]=0; 
		p=start=s; 
		// seek for blank char
		while (*p && (*p != ' ')) p++; 
		if (p > start) {
			*p=0; p++;  
			if (inet_aton(start,&c) == 0 ) continue; 
			addr=htonl(c.s_addr);  			
		}
		// skip remaining blank char 
		while (*p && (*p == ' ')) p++;

		// stat a new search for blank char 
		start=p; 
		while (*p && (*p != ' ')) p++; 
		if (p > start) {
			*p=0; p++;  
			if (inet_aton(start,&c) == 0 ) continue; 
			netmask=htonl(c.s_addr);
		}		
		// skip remaining blank char 
		while (*p && (*p == ' ')) p++;

		// stat a new search for blank '\n' char 		
		start=p; 
		while (*p && (*p != '\n')) p++; 
		if (p > start) { 
			*p=0; p++;  
			if (inet_aton(start,&c) == 0 ) continue; 
			gw=htonl(c.s_addr);
		}

		// call your function to insert entry to routing table 
		insertMyAlgo(addr,netmask,gw); 	
	}
	fclose(fi); 
	return 1; 
}

// only for debug purpose might be modified ! 
int main (int argc,char *argv[])
{
	char s[MAXLEN];
	int addr; 
 	struct in_addr a; 
 
//	initMyAlgo();  
	if ((argc > 1 ) && (loadFile(argv[1]))) { 
		printf("IP lookup algo\n");
		while (1) {
			fgets(s,MAXLEN,stdin);
			s[MAXLEN]=0; 
			if (inet_aton(s,&a) == 0 ) continue; 
//			addr=htonl(a.s_addr);
//			a.s_addr=htonl(lookupMyAlgo(addr)); 
//			printf("GW found = %s\n",inet_ntoa(a)); 		 
		}	
	}
	return 0; 	 
}


void initMyAlgo(){

	
}


void insertMyAlgo(unsigned int addr,unsigned int netmask,unsigned int gw){

	char* theHex = malloc(sizeof(char)*8);
	char name[2];
	unsigned int adresse = addr, nb = 0, reste;

	// fonction de hash
	while( adresse > 0){
		
		reste = adresse % 2;
		if( reste == 1){
			nb++;
		}
		adresse = adresse/2 ;
	}
	
	sprintf(theHex,"%x",addr);
	memcpy(name,theHex,sizeof(name));
	name[2] = '\0';
	
//	printf(" nbbits %d and %s\n",nb,name);	
	if(MaListe == NULL){
		// La première fois, passe par ici car insertion en tête
		push(&MaListe,name,nb);
	}	

	while(MaListe->next != NULL){
			
		if( strcmp(MaListe->hex,theHex) != 0 || nb != MaListe->bits){
			push(&MaListe,name,nb);
			break;
		}
		printf("couocu\n");
		MaListe = MaListe->next;
	}
	
	if( MaListe->next == NULL){
		//Si on a pas trouvé une liste qui possède le même nom
		printf("bite\n");
		push(&MaListe,name,nb);
	}

	//printf("hex: %s, nb de 1: %u et premier octet:%c %c \n", hex,nb, hex[0],hex[1]);
	
	free(theHex);	
	
}

unsigned int lookupMyAlgo(unsigned int addr){

	return 0;
}	
