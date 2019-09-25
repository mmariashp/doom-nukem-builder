
#include "builder.h"

unsigned 				open_for_write(const char *path, int *fd)
{
	if (!path)
		return (FAIL);
	*fd = open(path,O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR |\
	S_IRGRP | S_IROTH);
	if (*fd == -1)
		return (FAIL);
	return (SUCCESS);
}

unsigned 				close_file(int fd)
{
	if (close(fd) == -1)
		return (FAIL);
	return (SUCCESS);
}

unsigned 				open_for_read(const char *path, int *fd)
{
	if (!path)
		return (FAIL);
	*fd = open(path, O_RDONLY);
	if (*fd == -1)
		return (FAIL);
	return (SUCCESS);
}
