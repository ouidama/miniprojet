#include <stdio.h>
#include <string.h>

#define STRING_MAX 255
#define MAX_LIVRES 255

struct livre {
	int id;
	char titre[STRING_MAX];
	char auteur[STRING_MAX];
	int date;
};

struct livre livres_t[MAX_LIVRES];
int nb_livres=0;

void ajout_livre(){
  struct livre *l=&livres_t[nb_livres];
  do{
  printf("Saisir l identifiant : ");
  scanf("%d",&l->id);
  }while(l->id<0);
  nb_livres++;
}

void aff_livres(){
 for(int i=0;i<nb_livres;i++)
 {
     struct livre *l=&livres_t[i];
     printf("%d\t%s\t%s\t%d\n",l->id,l->titre,l->auteur,l->date);
 }
}

int main() {
    ajout_livre();
    strcpy(livres_t[0].titre,"test");
   aff_livres();
	return 0;
}
