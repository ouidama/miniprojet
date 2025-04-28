#include <stdio.h>

#define STRING_MAX 255
#define MAX_LIVRES 255

struct livre {
	int id;
	char titre[STRING_MAX];
	char auteur[STRING_MAX];
	int date;
};

struct livre livres_t[MAX_LIVRES];

int main() {
	return 0;
}
