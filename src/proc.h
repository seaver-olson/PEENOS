struct context{
	int eip;
	int esp;
	int ebx;
	int ecx;
	int edx;
	int esi;
	int edi;
	int ebp;
};

//going to be used for virtualization coordination
enum proc_state { UNUSED,EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct proc {
	char *mem;//start of memory address
	unsigned int sz;//size
	char *kstack;//bottom of kernel stack

	enum proc_state state;
	int pid;
	struct proc *parent;
	void *chan; //if !zero, sleeping on chan
	int killed; //if !zero, has been killed
	struct file *ofile[NOFILE]; // opens files
	struct inode *cwd; //current working directory
	struct context context; //switch here to run process
	struct trapfram *tf;//trap frame for the current interrupt
};
