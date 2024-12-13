# PEENOS
Pi Exploration & Expirements for Noobs OS

This operating system is very basic and runs predominantly on the raspberrypi3b however I added a little bit of support for the other raspberian models

So far you can read and write from the mmio buffer and when you press enter after a command the OS parses the command and arguments in a very shitty pLib.c file

I rewrote most of the basic functions that I added from opensource line esp_printf as they only handled basic cases and in some cases would break if the user tried to print an unsigned long