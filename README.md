# KFS-1 - Kernel From Scratch

Projet de création d'un kernel bootable avec GRUB pour l'école 42.

## Compilation

by default in docker container

```bash
make
```

## Commandes disponibles

```bash
make           # Compile kernel.bin + kfs.iso (dans Docker)
make clean     # Nettoie les fichiers objets
make fclean    # Nettoie tout (kernel.bin, kfs.iso, etc.)
make re        # Recompile tout (fclean + all)

# Test
make run       # Lance le kernel directement avec QEMU
make run-iso   # Lance l'ISO bootable avec QEMU
make run-kvm   # Lance avec accélération KVM
make debug     # Lance en mode debug (GDB sur port 1234)
```

## Prérequis

### Docker
- Docker installé et accessible

### Sans Docker (mode natif)
- `gcc` avec support 32-bit
- `as`, `ld`, `grub-mkrescue`, `xorriso`, `mtools`
- `qemu-system-i386` (pour tester)

Utilisez `make manualy-iso` pour compiler sans Docker.

## Fonctionnalités

### Partie obligatoire
- Kernel bootable avec GRUB (multiboot)
- Code ASM + C
- Affiche "42" à l'écran
- Bibliothèque kernel de base

### Bonus
- Support couleurs (16 couleurs VGA)
- Scroll et curseur hardware
- printk() style printf
- Support clavier (scancodes)
- Multi-écrans (F1-F5 pour switcher)

## Structure du projet

```
kfs-1/
├── boot/boot.S         # Boot ASM (multiboot header)
├── kernel/             # Code kernel (kernel.c, vga.c, etc.)
├── lib/                # Bibliothèque (string.c, printk.c)
├── include/            # Headers (types.h, multiboot.h)
├── linker.ld           # Script du linker
├── grub.cfg            # Configuration GRUB
├── Dockerfile          # Image Alpine (244 MB)
└── Makefile            # Build avec Docker intégré
```

## Fichiers générés

- `kernel.bin` : 16 KB (kernel compilé)
- `kfs.iso` : 4.9 MB (image ISO bootable)

## Auteur

Projet réalisé dans le cadre du cursus 42.