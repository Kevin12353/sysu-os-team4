#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

/*my code ---c 
 this is the .h file
 */

#include "devices/shutdown.h"
#include "threads/vaddr.h"
#include "threads/init.h"
#include "userprog/process.h"
#include <list.h>
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "devices/input.h"
#include "threads/synch.h"
/* my code ---c */

static void syscall_handler (struct intr_frame *);

/*my code ---c 
 this is system call
 */
typedef int pid_t;

struct file_list_elem{
  struct file* file;
  int fd;
  struct thread* thread;
  struct list_elem elem;
};

static struct list file_list;

static void _sys_halt (void);
static int _sys_exec (const char *cmd_line);
static int _sys_wait (pid_t pid);
static int _sys_create (const char *file, unsigned initial_size);
static int _sys_remove (const char *file);
static int _sys_open (const char *file);
static int _sys_filesize (const int fd);
static int _sys_read (int fd, void *buffer, unsigned size);
static int _sys_write (int fd, const void *buffer, unsigned length);
static void _sys_seek (int fd, unsigned pos);
static int _sys_tell (int fd);
static void _sys_close (int fd);

typedef int (*handler) (uint32_t, uint32_t, uint32_t);
static handler syscall_vec[128];
/**/
static struct file *get_file_by_fd (const int fd);
static struct file_list_elem *get_file_elem_by_fd (const int fd);
static int alloc_fid (void);

static struct lock file_lock;
static void syscall_handler (struct intr_frame *);



/*my code ---c*/

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  
  /*my code --- c*/
  syscall_vec[SYS_EXIT] = (handler)_sys_exit;
  syscall_vec[SYS_HALT] = (handler)_sys_halt;
  syscall_vec[SYS_CREATE] = (handler)_sys_create;
  syscall_vec[SYS_OPEN] = (handler)_sys_open;
  syscall_vec[SYS_CLOSE] = (handler)_sys_close;
  syscall_vec[SYS_READ] = (handler)_sys_read;
  syscall_vec[SYS_WRITE] = (handler)_sys_write;
  syscall_vec[SYS_EXEC] = (handler)_sys_exec;
  syscall_vec[SYS_WAIT] = (handler)_sys_wait;
  syscall_vec[SYS_FILESIZE] = (handler)_sys_filesize;
  syscall_vec[SYS_SEEK] = (handler)_sys_seek;
  syscall_vec[SYS_TELL] = (handler)_sys_tell;
  syscall_vec[SYS_REMOVE] = (handler)_sys_remove;

  lock_init(&file_lock);
  list_init(&file_list);
  /*my code --- c */
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
/*old code
  printf ("system call!\n");
  thread_exit ();*/
  /* my code---c */
  handler h;
  int *p;
  int ret;
  p = f->esp;
  if (!is_user_vaddr (p) || *p < SYS_HALT || *p > SYS_INUMBER )
  {
  	_sys_exit (-1);
  	return;
  }
  h = syscall_vec[*p];
  
  if (!(is_user_vaddr (p + 1) && is_user_vaddr (p + 2) && is_user_vaddr (p + 3)))
  {
  	_sys_exit (-1);
  	return;
  }
  ret = h (*(p + 1), *(p + 2), *(p + 3));
  f->eax = ret;
  return;
  /*my code---c */
}

static void
_sys_halt (void)
{
	shutdown_power_off();
}

int 
_sys_exit (int status )
{
  struct thread *t;
  t = thread_current ();
  t->ret_status = status;
  thread_exit ();
  return -1;
}

static int 
_sys_exec (const char *cmd_line)
{
  int ret;

  if (!cmd_line || !is_user_vaddr (cmd_line)) /* bad ptr */
    return -1;
    
  ret = process_execute (cmd_line);
  return ret;
}

static int 
_sys_wait (pid_t pid)
{
	int ret;
	
	ret = process_wait( pid );
	
	return ret;
}

static int 
_sys_create (const char *file, unsigned initial_size)
{
  if(file!=NULL)
    return filesys_create (file, initial_size);
	return _sys_exit(-1);
}

static int 
_sys_remove (const char *file)
{
  if(file!=NULL)
    return filesys_remove (file);
	return false;
}

static int 
_sys_open (const char *file_name)
{
  struct file* file;
  struct file_list_elem *fl_elem;
	if(file_name && (file=filesys_open(file_name))){
    fl_elem = (struct file_list_elem*)malloc(sizeof(struct file_list_elem));
    fl_elem->file = file;
    fl_elem->fd = alloc_fid();
    fl_elem->thread = thread_current();
    list_push_back(&file_list, &(fl_elem->elem));
    return fl_elem->fd;
  }
	return -1;
}

static int 
_sys_filesize (const int fd)
{
	struct file* file=get_file_by_fd(fd);
  if(file==NULL)
    return 0;
  return file_length(file);
}

static int 
_sys_read (int fd, void *buffer, unsigned size)
{
  struct file* file;
  if(!is_user_vaddr(buffer)||!is_user_vaddr(buffer+size)){
    return _sys_exit(-1);
  }
  if(fd==STDIN_FILENO){
    input_getc();
  }else if((file=get_file_by_fd(fd))!=NULL){
    return file_read(file, buffer, size);
  }
	return -1;
}

static int 
_sys_write (int fd, const void *buffer, unsigned length)
{
  struct file* file;
  if(fd==STDOUT_FILENO){
    putbuf(buffer, length);
  }else if((file=get_file_by_fd(fd))!=NULL){
    return file_write(file, buffer, length);
  }
	return -1;
}

static void 
_sys_seek (int fd, unsigned pos)
{
  struct file* file;
  if(fd>1 && (file=get_file_by_fd(fd))!=NULL){
    file_seek(file, pos);
  }
}

static int 
_sys_tell (int fd)
{
  struct file* file;
  if(fd>1 && (file=get_file_by_fd(fd))!=NULL){
    return file_tell(file);
  }
	return 0;
}

static void 
_sys_close (int fd)
{
  struct file_list_elem* fl_elem = get_file_elem_by_fd(fd);
  if(fl_elem!=NULL && fl_elem->thread->tid==thread_current()->tid)
  {
    file_close(fl_elem->file);
    list_remove(&fl_elem->elem);
    free(fl_elem);
  }
}

static struct file* 
get_file_by_fd(const int fd)
{
  struct file_list_elem* result = get_file_elem_by_fd(fd);
  return result==NULL? NULL: result->file;
}

static struct file_list_elem*
get_file_elem_by_fd (const int fd)
{
  struct file_list_elem *result;
  struct list_elem *l;
  if(list_empty(&file_list))
    return NULL;
  for (l = list_front (&file_list); l != list_end (&file_list); l = list_next (l))
  {
    result = list_entry (l, struct file_list_elem, elem);
    if (result->fd == fd){
      return result;
    }
  }
  return NULL;
}

static int 
alloc_fid (void)
{
  return list_size(&file_list)+2; 
}
