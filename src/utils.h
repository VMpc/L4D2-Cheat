#ifndef _UTILS_H
#define _UTILS_H

#define NOP (char)0x90

/* Structs */
struct user_regs_struct
{
  long int ebx;
  long int ecx;
  long int edx;
  long int esi;
  long int edi;
  long int ebp;
  long int eax;
  long int xds;
  long int xes;
  long int xfs;
  long int xgs;
  long int orig_eax;
  long int eip;
  long int xcs;
  long int eflags;
  long int esp;
  long int xss;
};

typedef union PokeUnion
{
  long val;
  char chars[sizeof(long)];
} pokeData;

/* Funcs */
void checkAllowed(void);

void doSleep(int);

void exitProgram(const char *, const int);

pid_t findPid(char *);

char *getLine(char *);

int32_t moduleAddr(pid_t, char *);

char pokeAddr(pid_t, long, char *, int);

char readAddr(pid_t, int32_t, void *, ssize_t);

char writeAddr(pid_t, int32_t, void *, ssize_t);

#endif
