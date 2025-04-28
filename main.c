#include <stdio.h>
#include <string.h>
#include <time.h>

#define STRING_MAX 255
#define MAX_LIVRES 255

// get current year
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

int main() {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	annee = tm.tm_year + 1900;

	ajout_livre();
	ajout_livre();
	ajout_livre();
	aff_livres();
	sup_livre();
	aff_livres();
	return 0;
}
