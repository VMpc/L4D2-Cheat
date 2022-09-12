## L4D2-Cheat

A generic command line Left 4 Dead 2 Cheat program for Linux.

# Tested on
- [Fedora (amd64)](https://getfedora.org/)
- [Gentoo (amd64)](https://www.gentoo.org/)
- [Mint (amd64)](https://linuxmint.com/)

# How to use
- Commands: glow on/off, noboom on/off
- Up/Down arrow to toggle on/off bhop
- Bhop normally but don't press space & hold LCTRL

You'll need to be root to run L4D2-Cheat successfully.

# Features
- Bhop
- Glowhack
- Removing boomer effects

# Requirements
- GCC - see config.mk if you want to change this
- Make
- Sudo (Or anything that allows you to run programs as root)

# Building & Running it
```
git clone https://github.com/VMpc/L4D2-Cheat
cd L4D2-Cheat
make all
# The game should be launched...
sudo ./L4D2Cheat
```

# Thanks to
- https://github.com/StrafeTx (Helped with offsets)
- https://github.com/StrafeUltra (Moral support / Helped figure out the hellscape of ptrace)
- URLNOTFOUND (A federal agent / Hated my code so much there was a patch file sent to fix it)

# License
L4D2-Cheat is licensed under GPL version 3 or later.
See LICENSE for copyright information.
