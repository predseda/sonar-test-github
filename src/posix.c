#include <common.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHMEM_NAME "data"

int32_t *open_shared_memory()
{
  int memory_fd = shm_open(SHMEM_NAME, O_CREAT|O_RDWR, 0644);
  if(memory_fd == -1)
  {
    fprintf(stderr, "open_shared_memory: shm_open failed!\n");
    return NULL;
  }

  int *memory = (int *)mmap(NULL, sizeof(int32_t), PROT_READ|PROT_WRITE, MAP_SHARED, memory_fd, 0);
  if(memory == MAP_FAILED)
  {
    fprintf(stderr, "open_shared_memory: mmap failed!\n");
    return NULL;
  }

  close(memory_fd);

  return memory;
}

void close_shared_memory(int *memory)
{
  if(memory != NULL)
    munmap(memory, sizeof(int32_t));
}

int32_t init_shared_memory()
{
  
  int memory_fd = shm_open(SHMEM_NAME, O_CREAT|O_RDWR, 0644);
  if(memory_fd == -1)
  {
    fprintf(stderr, "init_shared_memory: shm_open failed!\n");
    return 1;
    exit(1);
  }

  if(ftruncate(memory_fd, sizeof(int32_t)) == -1)
  {
    fprintf(stderr, "init_shared_memory: ftruncate failed!\n");
    return 1;
  }

  int *memory = (int *)mmap(NULL, sizeof(int32_t), PROT_READ|PROT_WRITE, MAP_SHARED, memory_fd, 0);
  if(memory == MAP_FAILED)
  {
    fprintf(stderr, "init_shared_memory: mmap failed!\n");
    return 1;
  }

  *memory = 42;

  close_shared_memory(memory);

  return 0;
}

void clean_shared_memory()
{
  shm_unlink(SHMEM_NAME);
}

int32_t modify_shared_memory(int32_t value)
{
  pid_t writer = fork();
  if(writer == -1)
  {
    fprintf(stderr, "modify_shared_memory: fork failed!\n");
    return -1;
  }
  else if(writer == 0)
  {
    int *memory = open_shared_memory();
    if(memory == NULL)
    {
      fprintf(stderr, "modify_shared_memory: failed to open shared memory\n");
      exit(-1);
    }

    *memory = value;

    close_shared_memory(memory);

    exit(0);
  }
  else
  {
    waitpid(writer, NULL, 0);
  }

  return 0;
}

void print_shared_memory_value()
{
  pid_t reader = fork();
  if(reader == -1)
  {
    fprintf(stderr, "print_shared_memory_value: fork failed!\n");
    return;
  }
  else if(reader == 0)
  {
    int32_t *memory = open_shared_memory();
    if(memory == NULL)
    {
      fprintf(stderr, "print_shared_memory_value: failed to open shared memory\n");
      exit(1);
    }

    printf("Shared memory value: %d\n", *memory);

    close_shared_memory(memory);

    exit(0);
  }
  else
  {
    waitpid(reader, NULL, 0);
  }
}
