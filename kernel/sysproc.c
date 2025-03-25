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
   if (*pte & PTE_A){
    return 1;
   }
   return 0;
}


int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 addr;
  argaddr(0, &addr);

  int npgs;
  argint(1, &npgs);

  uint64 abitsaddr; 
  argaddr(2, &abitsaddr);

  struct proc *p = myproc();

  unsigned int mask = 0; // 最大检测页的范围为32位，受限于unsigned int 
  // mask是int类型，最多存储32位的数据
  if (npgs > 32){
    printf("The process size out of the max checking size");
    return -1;
  }
  
  for (int i = 0; i < npgs; ++i){
    // 获取对应的用户空间地址对应的物理地址（以页面为单位）
      pte_t * pte = walk (p->pagetable, addr + i *PGSIZE, 0);
      int isset = is_accessed(pte);
      if(isset < 0) {printf("Error in access the pte\n");}
      if (isset) {
        *pte  ^= PTE_A;
        mask |= (1 << i);
      }
     
  }
  copyout(p->pagetable, abitsaddr, (char *) &mask, sizeof(mask));
  return 0;

}

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
