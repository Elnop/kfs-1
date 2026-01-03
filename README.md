# KFS-1 - Kernel From Scratch

Premier projet de la série "Kernel From Scratch" : un kernel x86 bare-metal bootable via GRUB.

## Description

KFS-1 est un kernel minimaliste qui implémente :
- **Partie obligatoire** : Affichage de "42" à l'écran
- **Bonus** : Couleurs, clavier et multi-écrans

## Fonctionnalités

### 📋 Partie Obligatoire ✅

- ✅ **Boot via GRUB** - Kernel multiboot compliant
- ✅ **Code ASM de boot** - Bootstrap avec setup stack (boot.S)
- ✅ **Driver VGA text mode** - Affichage 80x25 caractères
- ✅ **Affichage "42"** - Exigence principale du sujet
- ✅ **Bibliothèque de base** - strlen, strcmp, strcpy, memset, memcpy
- ✅ **Makefile complet** - Targets: all, clean, fclean, re, run
- ✅ **Linker script custom** - Pas d'utilisation du linker du host
- ✅ **Flags freestanding** - Compilation sans dépendances systèmes

---

### 🌟 Partie Bonus ✅

#### Interface VGA Améliorée
- ✅ **Couleurs** - Support de 16 couleurs (foreground + background)
- ✅ **Scroll automatique** - Défilement quand l'écran est plein
- ✅ **Hardware cursor** - Gestion du curseur VGA via ports I/O

#### Sortie Formatée
- ✅ **printk** - printf-style pour le kernel
  - Format `%s` (string)
  - Format `%c` (char)
  - Format `%d` / `%i` (signed int)
  - Format `%u` (unsigned int)
  - Format `%x` / `%X` (hexadecimal)
  - Format `%%` (literal %)

#### Entrée Clavier
- ✅ **Driver PS/2** - Lecture scancodes depuis port 0x60
- ✅ **Mapping complet** - Scancodes → ASCII (QWERTY US)
- ✅ **Modificateurs** - Support shift/ctrl/alt
- ✅ **Echo clavier** - Affichage des touches saisies

#### Multi-écrans Virtuels
- ✅ **5 écrans** - Accessibles via F1-F5
- ✅ **Persistance** - Sauvegarde du contenu par écran
- ✅ **Switch instantané** - Changement d'écran fluide
- ✅ **État indépendant** - Cursor et couleur par écran

## Architecture

- **Langage** : C + Assembly (boot)
- **Architecture cible** : i386 (x86 32-bit)
- **Bootloader** : GRUB
- **Compilation** : GCC + GNU AS

## Structure du projet

```
kfs-1/
├── boot/
│   └── boot.S              # Bootstrap ASM (multiboot)
├── kernel/
│   ├── kernel.c            # Point d'entrée
│   ├── vga.c/h             # Driver VGA
│   ├── keyboard.c/h        # Driver clavier
│   └── screen.c/h          # Gestionnaire multi-écrans
├── lib/
│   ├── string.c/h          # Fonctions utilitaires
│   └── printk.c/h          # printf pour kernel
├── include/
│   ├── types.h             # Types de base
│   └── multiboot.h         # Spécification multiboot
├── linker.ld               # Script de liaison
├── grub.cfg                # Configuration GRUB
└── Makefile                # Build system
```

## Compilation

```bash
# Compiler le kernel
make

# Nettoyer les fichiers objets
make clean

# Nettoyer complètement
make fclean

# Rebuild complet
make re
```

## Exécution

### Avec QEMU (direct kernel boot)
```bash
make run
```

### Avec KVM (accélération)
```bash
make run-kvm
```

### Avec ISO (nécessite mtools)
```bash
make run-iso
```

### Debug avec GDB
```bash
# Terminal 1
make debug

# Terminal 2
gdb kernel.bin
(gdb) target remote :1234
(gdb) continue
```

## Utilisation

Au boot, le kernel affiche :
1. "42" (exigence obligatoire)
2. Message de bienvenue avec liste des fonctionnalités
3. Instructions pour l'utilisateur

### Commandes
- **F1-F5** : Changer d'écran virtuel
- **Touches classiques** : Saisie de texte (avec shift)
- **Entrée** : Nouvelle ligne
- **Tab** : Tabulation
- **Backspace** : Effacer

## Détails techniques

### Mémoire
- Kernel chargé à 0x00100000 (1 MB)
- VGA text buffer à 0xB8000
- Stack de 16 KB

### Ports I/O
- 0x60 : Keyboard data
- 0x64 : Keyboard status
- 0x3D4 : VGA cursor command
- 0x3D5 : VGA cursor data

### Flags de compilation
```
-m32                    # Architecture i386
-ffreestanding          # Environnement freestanding
-fno-builtin            # Pas de built-ins
-fno-exceptions         # Pas d'exceptions
-fno-stack-protector    # Pas de stack protector
-nostdlib               # Pas de standard library
-nodefaultlibs          # Pas de default libs
```

## Limitations

- Pas de système de fichiers
- Pas de gestion mémoire dynamique
- Pas d'interruptions (polling pour clavier)
- Pas de multitâche

## Ressources

- [OSDev Wiki](https://wiki.osdev.org/)
- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/)
- [Intel x86 Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
