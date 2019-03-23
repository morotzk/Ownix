#ifndef FILE_READ_H
#define FILE_READ_H
#include <sys/types.h>
#include <fs/fs_const.h>
#include <fs/file_descriptor.h>

#define EBADF			-1
#define EISDIR			-2
/* Valid only until indirect block handling is not 
 * implemented! It means to access file's
 * data blocks after an offset of 7K.
 */
#define ENOIMP			-4
#define EIO			-8
/* Unknown inode.i_mode value	*/
#define EBADINO			-16

ssize_t read(int fd, void* buf, size_t count);

#endif
