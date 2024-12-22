# **PEENOS**  
**Pi Exploration & Experimentation for Noobs Operating System**  

Welcome to PEENOS, a lightweight, baremetal operating system built for the Raspberry Pi 3B! PEENOS is designed to empower hobbyists, students, and developers with a simple yet functional environment for exploring the fundamentals of operating system development.  

Whether you're experimenting with hardware, diving into low-level programming, or simply curious about how operating systems work, PEENOS is your gateway to hands-on learning.  

---

## **Features**  
- **Baremetal Architecture**: PEENOS runs directly on the hardware, without any underlying OS.  
- **ARMv8 Support**: Optimized for the ARM Cortex-A53 architecture of the Raspberry Pi 3B.  
- **Custom Memory Management**: Includes a basic memory management unit (MMU) and paging system.  
- **Peripheral Support**: Built-in drivers for SD cards, timers, and FAT file systems.  
- **Command Shell**: Execute basic commands through an intuitive custom shell.  
- **Interrupt Handling**: Robust exception and interrupt handling mechanisms.  
- **Minimalist Design**: Focused on clarity and simplicity for educational purposes.  

---

## **Directory Structure**
```plaintext
├── src
│   ├── arch
│   │   └── arm
│   │       ├── boot.s
│   │       └── vector_table.s
│   ├── commands
│   │   ├── cmd
│   │   └── cmds.c
│   ├── drivers
│   │   ├── fat.c
│   │   ├── sd.c
│   │   └── systimer.c
│   ├── kernel
│   │   ├── exceptions
│   │   │   ├── exc.c
│   │   │   └── interrupt.c
│   │   ├── kernel_main.c
│   │   ├── mmu.c
│   │   └── page.c
│   └── libc
│       ├── delays.c
│       ├── pLib.c
│       └── rprintf.c
├── obj
│   └── (compiled object files)
├── include
│   └── (header files)
├── kernel.ld
├── Makefile
└── README.md
```

## **Getting Started**
### **Requirements**
- Raspberry Pi 3B  
- Cross-compiler for ARM (e.g., `aarch64-linux-gnu-gcc`)  
- QEMU (for emulation and debugging)  
- GNU Make  


