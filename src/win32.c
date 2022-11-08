#include <common.h>

#define SHMEM_NAME "data"

HANDLE SHMEM = INVALID_HANDLE_VALUE;

void create_shared_memory()
{
	 SHMEM = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		256,
		(LPCSTR)SHMEM_NAME
	);
	if (SHMEM == NULL)
		fprintf(stderr, "create_shared_memory: CreateFileMapping failed!\n");
}

int32_t init_shared_memory()
{
	create_shared_memory();
	if (SHMEM == INVALID_HANDLE_VALUE)
		return 1;

	LPVOID memory_view = MapViewOfFile(
		SHMEM,
		FILE_MAP_WRITE,
		0,
		0,
		sizeof(int32_t)
	);
	if (memory_view == NULL)
	{
		fprintf(stderr, "init_shared_memory: MapViewOfFile failed!\n");
		return 1;
	}

	int32_t data = 42;
	CopyMemory(memory_view, &data, sizeof(int32_t));

	UnmapViewOfFile(memory_view);

	return 0;
}

void remove_shared_memory()
{
	CloseHandle(SHMEM);
}

int32_t modify_shared_memory(int32_t value)
{
	HANDLE file_mapping = OpenFileMapping(
		FILE_MAP_WRITE,
		FALSE,
		(LPCSTR)SHMEM_NAME
	);
	if (file_mapping == NULL)
	{
		fprintf(stderr, "write_shared_memory: OpenFileMapping failed!\n");
		return 1;
	}

	LPVOID memory_view = MapViewOfFile(
		SHMEM,
		FILE_MAP_WRITE,
		0,
		0,
		sizeof(int32_t)
	);
	if (memory_view == NULL)
	{
		fprintf(stderr, "write_shared_memory: MapViewOfFile failed!\n");
		CloseHandle(file_mapping);
		return 1;
	}

	CopyMemory(memory_view, &value, sizeof(int32_t));

	UnmapViewOfFile(memory_view);

	return 0;
}

DWORD WINAPI read_shared_memory(HANDLE memory)
{
	HANDLE file_mapping = OpenFileMapping(
		FILE_MAP_READ,
		FALSE,
		(LPCSTR)SHMEM_NAME
	);
	if (file_mapping == NULL)
	{
		fprintf(stderr, "read_shared_memory: OpenFileMapping failed!\n");
		return 1;
	}

	LPVOID memory_view = MapViewOfFile(
		memory,
		FILE_MAP_READ,
		0,
		0,
		sizeof(int32_t)
	);
	if (memory_view == NULL)
	{
		fprintf(stderr, "read_shared_memory: MapViewOfFile failed!\n");
		CloseHandle(file_mapping);
		return 1;
	}

	int32_t data;
	CopyMemory(&data, memory_view, sizeof(int32_t));
	printf("shared memory: %d\n", data);

	UnmapViewOfFile(memory_view);

	return 0;
}

void print_shared_memory_value()
{
	DWORD thread_id;
	HANDLE thread = CreateThread(
		NULL,
		0,
		read_shared_memory,
		SHMEM,
		0,
		&thread_id
	);
	if (thread == NULL)
	{
		fprintf(stderr, "print_shared_memory: CreateThread failed!\n");
		return;
	}


	WaitForMultipleObjects(1, &thread, TRUE, INFINITE);

	CloseHandle(thread);
}
