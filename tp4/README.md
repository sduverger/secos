# TP4 - La pagination

Le but du TP est de bien comprendre la pagination.

Vous pouvez directement modifier "tp.c".

Allez lire les fichiers "kernel/include/cr.h" et "kernel/include/pagemem.h". Vous trouverez plein d'informations, de structures et de macros utiles pour la résolution du TP.


## Rappels sur les tableaux et pointeurs en C:

Lorsque vous déclarez un tableau, le compilateur alloue la mémoire pour ce tableau automatiquement à l'endroit où vous le déclarez:

```c
int tab_a[10];

void fonction()
{
   int tab_b[10];
}
```

Dans cet exemple, `tab_a` est une variable globale non initialisée. Le compilateur et plus précisément le linker la placera dans la section ".bss". Le tableau aura une place de 10 entiers une fois le programme chargé en mémoire.

Le tableau `tab_b`, est une variable locale de `fonction()`. Le compilateur génère les instructions qui permettent d'allouer sa mémoire au moment de l'appel à `fonction()`, en déplaçant le pointeur de pile pour laisser de la place "localement", durant le temps de l'exécution de la fonction, aux 10 éléments de `tab_b`.

Dans le cas des pointeurs, nous pouvons les utiliser comme des tableaux pour adresser chaque élément, mais nous choisissons l'adresse à laquelle se trouvent les éléments. Dans vos programmes utilisateurs, vous faisiez généralement des `malloc()`. Dans notre noyau il n'y a pas de `malloc()` et nous faisons ce que nous voulons de la mémoire. Nous connaissons les zones de mémoire occupées par le code et les données du noyau, qu'il ne faut surtout pas écraser sous peine de voir le noyau crasher. Nous connaissons également la taille de la RAM et nous pouvons donc choisir des adresses disponibles en toute connaissance de cause.

```c
void fonction()
{
  int *tab_c = (int*)0x1234;
}
```

Dans cet exemple, tab_c est un pointeur sur entiers (ou une table d'entiers), pointant vers la zone mémoire à l'adresse `0x1234`. La variable `tab_c` elle-même se trouve dans la pile de `fonction()`. Mais à l'inverse d'un tableau, les éléments se trouvent à partir de `0x1234`. Peu impote que la variable tab_c soit une variable locale, ce qui compte c'est la zone mémoire adressée par le pointeur.

Le compilateur ne sait pas la taille de la zone mémoire adressée et potentiellement vous pouvez accéder à toute la mémoire à partir de cette adresse:

Dans une application classique, déclarer un tel pointeur provoquerait à coup sur un crash de l'application car l'adresse `0x1234` n'est jamais disponible pour une application (ex. sous Linux). Dans notre noyau cela ne pause pas de problème, nous utilisons la mémoire "physique" pour l'instant et nous n'avons pas de notion de tâche avec des espaces
d'adressage.


## Questions

### Question 1

**Affichez la valeur courante de `CR3`.**

---

### Question 2

**Allouez un PGD `(pde32_t*)` à l'adresse physique `0x600000` et mettez à jour `CR3` avec cette adresse.**

---

### Question 3

**Activez la pagination (`CR0`). Que se passe-t-il ? Pouquoi ?**

---

### Question 4

**Comme pour le PGD, allouez une PTB `(pte32_t*)` à l'adresse `0x601000`.**

**Initialisez la mémoire virtuelle en "identity mapping": les adresses virtuelles doivent être les mêmes que les adresses physiques. Pour cela il va falloir:**

 - **Bien étudier les plages d'adresses physiques occupées par le noyau (`readelf -e kernel.elf`, regardez les program headers).**
 - **Préparer au moins une entrée dans le PGD pour la PTB.**
 - **Préparer plusieurs entrées dans la PTB.**

---

### Question 5

**Une fois la pagination activée, essayez d'afficher le contenu d'une entrée de votre PTB. Que se passe-t-il ?**

---

### Question 6

**Trouvez la solution pour être capable de modifier les entrées de votre PTB une fois la pagination activée.**

---

### Question 7

**Faites en sorte que l'adresse virtuelle `0xc0000000` vous permette de modifier votre PGD.**

---

### Question 8

**Faites en sorte que les adresses virtuelles `0x700000` et `0x7ff000` mappent l'adresse physique `0x2000`. Affichez la chaîne de caractères à ces adresses virtuelles.**

---

### Question 9

**Effacez la première entrée du PGD. Que constatez-vous ? Expliquez pourquoi ?**
