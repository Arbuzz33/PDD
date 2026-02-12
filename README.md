# PDD
Disk diagnostic utility for Linux only
Distros: Ubuntu, Gentoo, Debian, Arch and etc.

TODO:
1) CLI keywords parsing +-
2) ~~Add themes~~
3) ~~Directory LookUpper~~
4) ~~ASCII Logo~~
5) ~~Installer (in makefile)~~
6) Ivan has to write docs and presentation
7) ~~Add secondary disk properties~~
8) ~~Add formating with fdisk~~


USING:
  **help** - prints explanation of all commands
  **quit/exit** - quits the program
  **textinvert** - inverts text
  **textdefault** - text format and colors become default
  **diskinfo** - prints info about all disks with their partitions (also lets you mount theme to get more info)
  **unmountsdisks** - use it after diskinfo if disks were mounted
  **printfiles** - prints files in a given directory. Ex: "printfiles -sgh5 /bin" - prints first 5 files (h5) with sizes (s) from bigger to lesser (g)
  **printdirs** - prints directories in a given directory. Ex: "printdirs -slt5 /home/user/Documents" - prints last 5 files (t5) with sizes (s) from lesser to bigger (l)
  **speedtest** - does speed test of a given partition (need to be mounted). Ex: "speedtest sda1" - makes speedtest of a first partition of disk sda
  **temperature** - prints temperature of a given disk. Ex: "temperature nvme0n1" - prints the temperature of disk nvme0n1
  **badblks** - checks bad blocks on a disk. Ex: "badblks sdb" - makes check of badblocks on disk sdb
  **checkio** - checks input/output errors on a disk. Ex: "checkio nvme1n1" - prints errors and recomendations on disk nvme1n1
  **format** - formats the disks (at this moment only partition table). Ex: "format /dev/sdc" - lets you create new partition table on sdc (uses fdisk)
