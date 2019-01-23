# TP1 - La segmentation

Le but du TP est de bien comprendre la segmentation.

Vous pouvez directement modifier "tp.c".

Allez lire le fichier "kernel/include/segmem.h". Vous y trouverez plein d'informations, de structures et de macros utiles pour la résolution du TP.

:warning: **QEMU ne supporte pas "complètement" la segmentation.**

Il est nécessaire d'utiliser KVM à la place. Vous devez donc modifier au préalable le fichier utils/config.mk à la ligne:

```bash
 QEMU := $(shell which qemu-system-x86_64)
```

par

```bash
 QEMU := $(shell which kvm)
```


## Questions

### Question 1

**Grub a démarré notre noyau en mode protégé. Il a donc configuré une GDT avant d'exécuter notre point d'entrée. Affichez le contenu de cette GDT. Que constatez-vous ?**

**Servez-vous des outils présents dans notre OS (`get_gdtr(), seg_desc_t et gdt_reg_t`)**

---

### Question 2

**Configurez votre propre GDT contenant des descripteurs ring 0:**
 - **Code, 32 bits RX, flat, indice 1**
 - **Données, 32 bits RW, flat, indice 2**

**Vous pouvez placer ces descripteurs où vous le souhaitez dans la GDT. Attention de bien respecter les restrictions matérielles :**
 - **La GDT doit avoir une adresse de base alignée sur 8 octets**
 - **Le premier descripteur (indice 0) doit être NULL**

**Chargez cette GDT, puis initialisez les registres de segments (cs/ss/ds/...) avec les bons sélecteurs afin qu'ils pointent vers vos nouveaux descripteurs.**

---

### Question 3

**Essayez d'exécuter le code suivant :**

```c
  #include <string.h>

  char  src[64];
  char *dst = 0;

  memset(src, 0xff, 64);
```

**Configurez un nouveau descripteur de données à l'index de votre choix :**
 - **data, ring 0**
 - **32 bits RW**
 - **base 0x600000**
 - **limite 32 octets**

**Chargez le registre de segment "es" de manière à adresser votre nouveau descripteur de données. Puis exécutez le code suivant :**

```c
  _memcpy8(dst, src, 32);
```

**Que se passe-t-il ? Pourquoi n'y a-t-il pas de faute mémoire alors que le pointeur `dst` est NULL ?**

**Effectuez à présent une copie de 64 octets. Que se passe-t-il ?**
