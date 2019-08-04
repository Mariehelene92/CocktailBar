#include "Arduino.h"

/*************************************************************************************************/
/********************** Instructions pour ajouter des cocktails à la liste  **********************/
/****************** Adapter rows avec le nombre de recettes prises en comptes  *******************/
/******* Ajouter une ligne dans cocktails avec les numéros de bouteilles et les quantités  *******/
/************* Ajouter le nom du cocktail dans cocktailName, au même numéro de ligne *************/
/*************************************************************************************************/

const int rows = 3; // Nombre de recettes de cocktails + 1 (la boisson aléatoire.
const int columns = 3; // Ne pas modifier

/*************************************************************************************************/
/****************** Attention! Toujours laisser le cocktail surprise en dernier ******************/
/*************************************************************************************************/
int cocktails[rows][2 * columns] = 
{
  { 1, 2, 0,    40,      40,      0  },//Mojito : Rhum = 5 cl - Limonade = 6 cl
  { 1, 3, 0,    40,      40,      0  },//Gin Tonic : Gin 4 cl - Tonic = 8 cl
  { 2, 3, 0,    40,      40,      0  },//Sex on the beach : Vodka = 4 cl - Ananas = 4 cl - Cranberry = 4 cl
};
/*int cocktails[rows][2 * columns] = 
{
  { 1, 0, 0,    40,      0,      0  },//Pump 1
  { 2, 0, 0,    40,      0,      0  },//Pump 2
  { 3, 0, 0,    40,      0,      0  },//Pump 3
};*/

/* cocktails décrit les recettes des cocktails : chaque ligne décrit un cocktail différent.
Pour une ligne donnée, les trois premières colonnes correspondent aux boissons à ajouter dans le cocktail suivant la liste ci-dessous: 
    1 Gin
    2 Tonic
    3 Cranberry
    4 Cointreau
    5 Rhum
    6 Limonade
    7 Jus ananas
    8 Vodka


Les trois dernières colonnes correspondent au poids en g de chaque boisson : la quatrième colonne correspond à la quantité de la boisson de la première colonne, la cinquième correspond à la deuxième, et la sixième correspond à la troisième.

La dernière ligne correspond à la boisson aléatoire. Les numéros des boissons sont égaux à 0 car ils sont calculés aléatoirement plus tard dans le processus.
*/

const String cocktailName[rows] = {
  "Pump 1&2",
  "Pump 1&3",
  "Pump 2&3"
};
/*cocktailName contient les noms des cocktails à afficher sur l'écran. Les lignes correspondant à chaque cocktail doivent correspondre entre cocktailName et le tableau cocktails ci-dessus.*/

