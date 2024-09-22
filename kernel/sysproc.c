#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// #define LAB_PGTBL

#ifdef LAB_PGTBL
int is_accessed(pte_t *pte){
   if ((*pte & PTE_V) == 0){
      return -1;
   }
   if ((*pte & PTE_A) > 0){
    return 1;
   }

   return 0;
}


int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  // printf("hello\n");
  // int npgs;
  // argint(1, &npgs);
  // // printf("The number of pages is %d", npgs);
  // int bitmask;
  // argint(2, &bitmask);
  // 获取地址空间
  uint64 buf_addr;
  argaddr(0, &buf_addr);

  int npgs;
  argint(1, &npgs);

  uint64 bitmask_addr;
  argaddr(2, &bitmask_addr);

  struct proc *p = myproc();

  int mask = 0;
  // mask是int类型，最多存储32位的数据
  if (npgs > 32){
    printf("The process size out of the max checking size");
    return -1;
  }
  
  for (int i = 0; i < npgs; ++i){
    // 获取对应的用户空间地址对应的物理地址（以页面为单位）
      pte_t * pte = walk (p->pagetable, buf_addr + i *PGSIZE, 0);
      int abit = is_accessed(pte);
      if (abit < 0) {printf("Error in access the leaf pte\n"); return -1;}
      if (abit ==1) {*pte  &= ~PTE_A;}
      mask |= (abit << i);
  }
  printf("print: the mask is %p\n", mask);
  copyout(p->pagetable, bitmask_addr, (char *) &mask, sizeof(mask));
  return 0;

  // struct proc *p = myproc();
  // int mask=0;
  // for (int i = 0; i < npgs; ++i){
  //   // check the access bit
  //   // from virual address to pysical address
  //   pte_t* pte = walk(p->pagetable, addr + i * PGSIZE, 0);
  //   if(pte == 0)
  //     panic("walk pte fail");
    
  //   // uint64 pa = PTE2PA(*pte);
  //   // int access = (pa & PTE_A) >> 6;
  //   // printf("The access bit is: %d | The pa is: %p  | The PTE_A is: %p \n", access, pa, PTE_A);
  //   // printf("The equal access bit is %d \n", *pte&PTE_A);
  //   int access = (*pte&PTE_A) >> 6;
  //   if (access!=0){*pte = *pte & ~PTE_A;}
  //   printf("The access bit is: %d\n", access);
  //   // mask = (mask & ~ PTE_A) | (access <<6);
  //   mask = mask | access << i;
  //   // clear the access bit in pysical address
  //   // todo:
  // }
  // printf("| The mask is %p\n", mask);
  // copyout(p->pagetable, bitmask, (char *)&mask, sizeof(mask));
  // // printf("| The address is %s\n", buf);
  // return 0;
}

// int vm_pgacess(pagetable_t pagetable, uint64 va)
// {
//   pte_t *pte;

//   if (va >= MAXVA)
//     return 0;
//   pte = walk(pagetable, va, 0);
//   if (pte == 0)
//     return 0;
//   if ((*pte & PTE_V) == 0)
//     return 0;
//   if ((*pte & PTE_A) != 0)
//   {
//     *pte = *pte & (~PTE_A);//置零
//     return 1;
//   }
//   return 0;
// }

// int sys_pgaccess(void)
// {
//   // lab pgtbl: your code here.
//   uint64 addr;
//   int len;
//   int bitmask;
//   //获取当前进程
//   struct proc *p = myproc();
//   if (argint(1, &len) < 0)
//   {
//     return -1;
//   }
//   if (argaddr(0, &addr) < 0)
//   {
//     return -1;
//   }
//   if (argint(2, &bitmask) < 0)
//   {
//     return -1;
//   }
//   int res = 0;
//   for (int i = 0; i < len; i ++)
//   {
//     int va=addr+i*PGSIZE;
//     int abit = vm_pgacess(p->pagetable, va);
//     res = res | abit << i; //置在那一位上
//   }
//   if (copyout(p->pagetable, bitmask, (char *)&res, sizeof(res)) < 0)
//     return -1;
//   return 0;
// }

#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
