#include <common.h>

int main()
{
  if(init_shared_memory() != 0)
    return 1;

  print_shared_memory_value();  

  modify_shared_memory(-6);
  print_shared_memory_value();

  modify_shared_memory(256);
  print_shared_memory_value();

  #ifndef _WIN32
  clean_shared_memory();
  #else
  remove_shared_memory();
  #endif

  return 0;
}
