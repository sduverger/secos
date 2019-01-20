# Debugging:

Vous aurez surement besoin de debugger votre code dans ce TP. Pour cela, vous pouvez vous servir du serveur GDB de Qemu (appelé gdb stub).

Au lieu de faire "make qemu" pour lancer votre VM, utilisez "make debug"

Ceci ajoute les options "-s -S" dans la variable "QOPT" du fichier "utils/config.mk" et ordonne à Qemu de bloquer le CPU tant qu'un client GDB n'est pas connecté.

Depuis un second terminal, connectez-vous à Qemu à l'aide du client GDB sur "localhost:1234".

## Exemple de session GDB:

```bash
$ gdb kernel.elf
GNU gdb (Ubuntu 8.0.1-0ubuntu1) 8.0.1
[...]
Reading symbols from kernel.elf...done.
```
(gdb) target remote :1234
Remote debugging using :1234
0x0000fff0 in ?? ()

(gdb) b tp
Breakpoint 1 at 0x304481: file tp.c, line 98.
(gdb) c
Continuing.

Breakpoint 1, tp () at tp.c:98
98	{
(gdb) x/i $pc
=> 0x304481 <tp>:	push   %ebp
```

Le noyau est toujours compilé avec les symboles de debug, mais je vous recommande de mettre des breakpoints à des adresses d'instructions précises surtout lorsque vous utilisez de l'assembleur "in-line".

Désassemblez le binaire "kernel.elf" pour retrouver les adresses des instructions sur lequelles installer des breakpoints, soit depuis GDB ("disassemble") soit à l'aide votre désassembleur préféré.

```bash
(gdb) disassemble tp
Dump of assembler code for function tp:
=> 0x00304482 <+0>:	push   %ebp
   0x00304483 <+1>:	mov    %esp,%ebp
   0x00304485 <+3>:	call   0x3049cd <__x86.get_pc_thunk.ax>
   0x0030448a <+8>:	add    $0x892,%eax
   0x0030448f <+13>:	call   0x30414d <init_gdt>
   0x00304494 <+18>:	call   0x304447 <test_user>
   0x00304499 <+23>:	nop
   0x0030449a <+24>:	pop    %ebp
   0x0030449b <+25>:	ret
End of assembler dump.

(gdb) b *tp+25
```

Pour quitter gdb, évitez de taper "q", car cela terminerait la session GDB et tuerait Qemu. Il est préférable de se détacher à l'aide de la commande "detach" et ensuite de quitter gdb avec <ctrl-d>, afin de permettre à Qemu de continuer son exécution et de décider ou non de l'arrêter par la suite: <ctrl-a> <x>.

Vous pouvez également créer des petits scripts GDB lancés automatiquement grâce à l'option '-x' sur la ligne de commande de GDB.

```bash
$ cat gdbrc
target remote :1234
display /i $pc
b tp
c

$ gdb -x gdbrc kernel.elf
[...]
0x0000fff0 in ?? ()
Breakpoint 1 at 0x304482: file tp.c, line 98.

Breakpoint 1, tp () at tp.c:98
98	{
1: x/i $pc
=> 0x304482 <tp>:	push   %ebp
(gdb)
```

GBB se connecte tout seul à Qemu, automatise l'affichage de l'instruction courante à chaque évènement, installe un breakpoint sur la fonction "tp" et dit à Qemu de continuer son exécution.

Dès qu'on atteint "tp", GDB nous affiche l'instruction courante et nous donne la main.

N'oubliez pas que vous avez tout de même le contrôle du Monitor de Qemu même si vous êtes connecté avec GDB.

Dans le terminal de Qemu, vous pouvez toujours taper <ctrl-a> <c> <enter>:

```bash
QEMU 2.10.1 monitor - type 'help' for more information
(qemu) info registers
EAX=00000023 EBX=00304dcc ECX=00304e80 EDX=00000022
ESI=0002bfc2 EDI=0002bfc3 EBP=00302008 ESP=00301fec
EIP=0030452c EFL=00000006 [-----P-] CPL=0 II=0 A20=1 SMM=0 HLT=0
ES =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
CS =0008 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]
SS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
DS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
FS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
GS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
LDT=0000 00000000 0000ffff 00008200 DPL=0 LDT
TR =0000 00000000 0000ffff 00008b00 DPL=0 TSS32-busy
GDT=     00008f8c 00000027
IDT=     00305280 000007ff
CR0=00000011 CR2=00000000 CR3=00000000 CR4=00000000
DR0=00000000 DR1=00000000 DR2=00000000 DR3=00000000
DR6=ffff0ff0 DR7=00000400
EFER=0000000000000000
FCW=037f FSW=0000 [ST=0] FTW=00 MXCSR=00001f80
FPR0=0000000000000000 0000 FPR1=0000000000000000 0000
FPR2=0000000000000000 0000 FPR3=0000000000000000 0000
FPR4=0000000000000000 0000 FPR5=0000000000000000 0000
FPR6=0000000000000000 0000 FPR7=0000000000000000 0000
XMM00=00000000000000000000000000000000 XMM01=00000000000000000000000000000000
XMM02=00000000000000000000000000000000 XMM03=00000000000000000000000000000000
XMM04=00000000000000000000000000000000 XMM05=00000000000000000000000000000000
XMM06=00000000000000000000000000000000 XMM07=00000000000000000000000000000000
```

Vous pouvez constater que la définition des registres systèmes est beaucoup plus détaillée avec le Monitor.
