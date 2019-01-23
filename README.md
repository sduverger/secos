# SECOS


## Pré-requis:

```bash
bash$ sudo apt-get install qemu-system-x86 qemu-kvm gcc-multilib make git
bash$ git clone https://github.com/sduverger/secos
```

## Organisation du cours:

```bash
$ cd secos ; ls
kernel  README  tp0  tp1  tp2  tp3  tp4  tp5  tp_exam  utils
```

Le cours vous donne l'occasion d'appliquer vos connaissances à l'aide de petits TPs. Chaque TP dispose de son répertoire et permet la compilation d'un noyau spécifique à votre TP.

Chaque TP embarque ainsi un fichier "tp.c" dans lequel vous pouvez faire vos développements sans avoir à modifier l'environnement de compilation, ni les autres fichiers du noyau.

Le code des composants essentiels du noyau se trouve dans "kernel/".

Le noyau est simpliste. Il boot en mode protégé en 32 bits grâce à Grub. A ce titre, il accède à la mémoire physique directement sans pagination. Le mode protégé a été partiellement initialisé par Grub, il faudra au fil des TPs, le configurer petit à petit: segmentation, interruptions, pagination, ...

Il propose des services de base:
 - `printf` / `debug` sur le port série
 - `memcpy`, `memset`, ...
 - L'accès aux structures multiboot de Grub
 - Les structures matérielles du mode protégé pour x86:
   + GDT/IDT/LDT, descripteurs, sélecteurs
   + Tables de pages, PTE/PDE
   + TSS
   + Registres de contrôle (CR)
   + Registres de flags

Allez fouiller dans "kernel/include" pour y trouver les définitions. Les structures utilisent des bitfields permettant d'accéder facilement aux champs des structures matérielles sans avoir à faire de masques ou décallages. Si vous n'aimez pas les bitfields, vous avez toujours le loisir d'utiliser des masques en utilisant le champ ".raw" de ces structures qui vous donne accès à l'intégralité des données.


## Démarrer un TP:

```bash
$ cd tp0
$ make clean all
    AS    ../kernel/core/entry.s
    CC    ../kernel/core/start.c
    CC    ../kernel/core/print.c
    CC    ../kernel/core/uart.c
    CC    ../kernel/core/pic.c
    CC    tp.c
    LD    kernel.elf

$ make qemu
hit <ctl+a> <h> <enter>, for Qemu help
vvfat . chs 1024,16,63

secos-a241db6-59e4545 (c) Airbus
kernel mem [0x302010 - 0x303820]
```


## Comprendre le démarrage:


L'environnement de développement s'appuye sur Qemu. Le makefile lance qemu avec une suite d'options que vous pouvez retrouver dans "utils/config.mk".

Qemu boot une VM équipée d'un processeur x86 32 bits et de 128MB de RAM. La VM démarre sur une disquette "grub.floppy" qui contient le bootloader "grub". Celui-ci est configuré pour charger le fichier "kernel.elf" depuis le disque dur "HDD0". Le fichier "kernel.elf" est notre noyau d'OS.

Qemu nous permet d'émuler un disque dur directement depuis un répertoire, sans avoir à créer une image virtuelle de disque. Ceci est très pratique, car à chaque modification de notre noyau "kernel.elf", nous n'aurons pas besoin de recréer cette image de disque. Qemu prendra automatiquement la dernière version du fichier à chaque lancement de VM.

L'image ELF de votre noyau se trouve dans le répertoire de TP dans lequel vous travaillez.

Par défaut, le port série de la VM est redirigé dans votre terminal (stdio). La VM ne dispose pas d'affichage graphique.

Vous pouvez accéder au "monitor" de Qemu, grâce à la séquence **\<ctrl-a\>** puis **\<c\>** puis **\<enter\>**.

Il vous affiche:

```bash
QEMU 2.xx.yy monitor - type 'help' for more information
(qemu)
```

Vous aurez accès à de nombreuses commandes internes de Qemu qui permettent d'inspecter la machine virtuelle.

Pour rebasculer dans l'affichage des logs envoyées sur le port série, tapez de nouveau la séquence **\<ctrl-a\> \<c\> \<enter\>**. Le monitor est multiplexé avec le port série sur stdio.

Si vous voulez quitter Qemu:
 - Soit directement **\<ctrl-a\> \<x\>**
 - Soit depuis le mode monitor, tapez **q**.
