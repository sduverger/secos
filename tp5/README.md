# TP 5 - Les appels systèmes

Le but du TP est de bien comprendre les appels systèmes. Vous pouvez directement modifier "tp.c".

## Questions

### Question 0

**Reprenez le code du TP 3 de manière à exécuter du code ring 3. Par la suite vous placerez vos expérimentations dans la fonction `userland()`. Pensez à placer une boucle infinie à la fin de cette fonction.**

---

### Question 1

**Essayez de déclencher l'interruption "48" de manière logicielle. Que se passe-t-il ?**

---

### Question 2

**Corrigez le problème de sorte qu'il soit possible d'appeler l'interruption "48" en ring 3. Inspirez-vous du TP3, attention aux piles user/kernel ! Placez un breakpoint sur l'appel à l'interruption et analysez avec Qemu l'état du processeur (info registers) avant et après l'interruption.**

---

### Question 3

**Installer la fonction `syscall_isr()` comme gestionnaire de l'interruption 48:**

```c
void syscall_isr()
{
   asm volatile (
      "leave ; pusha        \n"
      "mov %%esp, %eax      \n"
      "call syscall_handler \n"
      "popa ; iret"
      );
}

void __regparm__(1) syscall_handler(int_ctx_t *ctx)
{
   debug("SYSCALL eax = %p\n", ctx->gpr.eax);
}
```

Vous pouvez constater qu'il s'agît d'une fonction C et qu'il aurait été plus pratique d'en développer une en assembleur (pas de stack frame créée à la compilation).

**Bonus** : Implémentez la en ASM dans "idt.s". Elle doit vous permettre d'appeler la fonction `syscall_handler()` ci-dessus.

---

### Question 4

**Modifiez la fonction `syscall_handler()` pour qu'elle affiche une chaîne de caractères dont l'adresse se trouve dans le registre "ESI". Nous venons de créer un appel système permettant d'afficher un message à l'écran et prenant son argument via "ESI". Essayez cet appel système depuis votre fonction `userland()`.**

---

### Question 5

**Quel problème de sécurité voyez-vous à notre implémentation de `syscall_handler()` ? Essayez de pirater ce service, depuis `userland()`, afin de lire de la mémoire du noyau. Comment corriger ce problème ?**
