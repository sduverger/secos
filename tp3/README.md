# TP3 - Les niveaux de privilèges

Le but du TP est de bien comprendre les niveaux de privilèges et de réussir à démarrer du code en ring 3.

Comme abordé en cours, ce chapitre de la documentation des processeurs Intel est probablement le plus complexe. Le TP ne couvre pas tous les aspects des transitions de niveaux de privilèges.

L'idée est ici de vous faire implémenter des transitions user (ring3) et kernel (ring0), comme on en trouve aujourd'hui dans les OS modernes.

Vous pouvez directement modifier "tp.c".

Je vous recommande de lire un peu de documentation sur l'assembleur en ligne GCC:

 - https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html
 - https://locklessinc.com/articles/gcc_asm/
 - https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html

:warning: Pour une aide au debugging, lisez le fichier README.DEBUG !


## Questions

### Question 1

**Modification de la GDT.**

**Reprenez vos travaux du TP1 pour la création de la GDT. Ajoutez deux nouveaux descripteurs aux index de votre choix:**
 - **Code, 32 bits RX, ring 3, flat**
 - **Data, 32 bits RW, ring 3, flat**

---

### Question 2

**Écrivez la fonction suivante:**

```c
void userland()
{
   asm volatile ("mov %eax, %cr0");
}
```

---

### Question 3

**Chargez progressivement les registres de segments avec des sélecteurs qui pointent vers les descripteurs ring 3.**

---

### Question 3.1

**Que se passe-t-il lors du chargement de DS/ES/FS/GS ?**

---

### Question 3.2

**Que se passe-t-il lors du chargement de SS ?**

---

### Question 3.3

**Essayez d'effectuer un "far jump" vers la fonction `userland()`. Pour cela il faut charger dans `CS` le sélecteur de code ring 3 et dans EIP l'adresse de la fonction `userland()`. Vous pouvez utiliser le type `fptr32_t` et la fonction `farjump()` de notre noyau.**

**Quelles conclusions en tirez-vous ? Comment un noyau fait-il pour démarrer une tâche en ring 3 ?**

---

### Question 3.4

**Essayez de faire s'exécuter la fonction `userland()` en ring3 avec une autre méthode (indice `iret`). Analysez la faute produite suite à son exécution. Est-ce normal ?**

---

### Question Bonus

**Pour les plus courageux, essayez d'écrire une API vous permettant de créer une tâche ring3, d'exécuter une fonction en ring3 depuis le ring0, avec sa propre pile, et lui permettre de revenir en ring0 ? Quels problèmes rencontrez-vous ?**
