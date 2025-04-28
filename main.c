#include <stdio.h>
#include <string.h>
#include <time.h>

#define STRING_MAX 255
#define MAX_LIVRES 255

// for getting current year
int annee;

struct livre {
	int id;
	char titre[STRING_MAX];
	char auteur[STRING_MAX];
	int date;
};

struct livre livres_t[MAX_LIVRES];
int nb_livres = 0;

void ajout_livre() {
	if (nb_livres >= MAX_LIVRES) {
		printf("Vous avez atteint le nombre maximum de livres.\n");
		return;
	}

	struct livre *l = &livres_t[nb_livres];

	// TODO: check if id is already used, can't have repeat ids
	do {
		printf("Saisir l'identifiant : ");
		scanf("%d",&l->id);
	} while(l->id < 0);

	printf("Saisir le titre : ");
	scanf("%s", &l->titre);

	printf("Saisir le auteur : ");
	scanf("%s", &l->auteur);

	do {
		printf("Saisir la date : ");
		scanf("%d", &l->date);
	} while(l->date <= 0 || l->date > annee);

	nb_livres++;
}

void aff_livres() {
	if (nb_livres == 0) {
		printf("Il y'a aucun livre.\n");
		return;
	}

	printf("ID\tTitre\tAuteur\tdate\n");
	for (int i = 0; i < nb_livres; i++) {
		struct livre *l=&livres_t[i];
		printf("%d\t%s\t%s\t%d\n",l->id,l->titre,l->auteur,l->date);
	}
}

void sup_livre() {
	if (nb_livres == 0) {
		printf("Il y'a aucun livre.\n");
		return;
	}

	int id, trouve;
	trouve = 0;
	do {
		printf("Saisir l'identifiant du livre a supprimer : ");
		scanf("%d", &id);
	} while(id < 0);

	for (int i = 0; i < nb_livres; i++) {
		if (id == livres_t[i].id) {
			trouve = 1;
			for (int j = i; j < nb_livres - 1; j++) {
				livres_t[j] = livres_t[j+1];
			}
			nb_livres--;
		}
	}

	if (trouve) {
		printf("Le livre avec cet identifiant a ete suprimmer.\n");
	} else {
		printf("Aucun livre avec cet identifiant existe.\n");
	}
}

void ch_livre_id(){
	int id;
	int trouve=0;
	do{
		printf("Saisir l'identifiant du livre que vous voulez\n");
		scanf("%d",&id);
	} while(id<0);

	for(int i=0 ; i<nb_livres ; i++)
	{
		if(id==livres_t[i].id)
		{
			trouve=1;
			struct livre *l=&livres_t[i];
			printf("%d\t%s\t%s\t%d\n",l->id,l->titre,l->auteur,l->date);
		}
	}
	if(!trouve)
	{
		printf("Il n'y a aucun livre avec cet identifiant\n");
	}
}

void ch_livre_titre() {
	char titre[STRING_MAX];
	int trouvee = 0;

	printf("Saisir le titre du livre que vous voulez\n");
	scanf("%s", &titre);

	for(int i = 0; i < nb_livres; i++)
	{
		if(!strcmp(titre, livres_t[i].titre))
		{
			trouvee = 1;
			struct livre *l=&livres_t[i];
			printf("%d\t%s\t%s\t%d\n",l->id,l->titre,l->auteur,l->date);
		}
	}

	if (!trouvee) {
		printf("Il n'y a aucun livre avec ce titre.\n");
	}
}

void ch_livre(){
	int choix;
	printf("Voulez-vous chercher le livre par son titre ou son identifiant?\n");
	do{
		printf("Taper 0 pour le titre ou 1 pour l'identifiant : ");
		scanf("%d",&choix);
	}while(choix!=0 && choix!=1);

	if (choix) {
		ch_livre_id();
	} else {
		ch_livre_titre();
	}
}

int main() {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	annee = tm.tm_year + 1900;

	int choix;
	while(1) {
		printf("1: Ajouter un livre\n");
		printf("2: Supprimer un livre\n");
		printf("3: Chercher un livre\n");
		printf("4: Afficher tous les livres\n");

		scanf("%d", &choix);

		switch(choix) {
			case 1: ajout_livre(); break;
			case 2: sup_livre(); break;
			case 3: ch_livre(); break;
			case 4: aff_livres(); break;
		}
	}

	return 0;
}
