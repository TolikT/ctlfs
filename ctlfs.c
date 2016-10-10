/*Broken file system*/
#define FUSE_USE_VERSION 30

#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <fuse.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/sysctl.h>
#include <sys/limits.h>
#include <sys/resource.h>
#include "params.h"

int isdir(char *path) {
	FILE *readcmd;
	char cmd[2048], *line = NULL;
	ssize_t read;
	size_t len;
	unsigned int i = strlen(path);
	sprintf(cmd, "sysctl -N %s", path);
	readcmd = popen(cmd, "r");
	if (readcmd != NULL) {
		if ((read = getline(&line, &len, readcmd)) != -1) {
			line[read - 1] = 0;
			if(line[i] == '.') i++;
			if(line[i] == 0) {
				fclose(readcmd);
				return (-ENOTDIR); /* If path matches line then file is given  */
			}
			else {
				fclose(readcmd);
				return (0); /* If there is something in line after path - it is a dir */
			}
		}
		else { 
			fclose(readcmd);
			return (-ENOENT); /* If nothing matches - no such file o directory */
		}
	}
	return (-ENOENT); 
}

/* This function cuts get diff between line and path and set up entry in a proper way  */
int getentry(char *path, char *line, char *entry) {
	unsigned int i = strlen(path), j = 0;

	if(line[i] == '.') i++;

	while((line[i] != '.')  && (line[i] != 0)) {
		entry[j++] = line[i++];
	}
	entry[j] = 0;
	if(strlen(entry) > 0) return 1;
	else return 0;
}


int breaddir(const char *path, void *data, fuse_fill_dir_t filler, off_t off, struct fuse_file_info *ffi){
	FILE *readcmd;
	char *line = NULL;
	char cmd[2048], entry[2048];
	char entries[1024][2048];
	char *path2 = malloc(2048);
	int entries_count = 0, flag = 0;
	size_t len = 0;
	ssize_t read;

	strcpy(path2, path);
	if(path2[0] == '/') path2++;
	for (int i = 0; path2[i] != 0 ; i++) {
		if(path2[i] == '/') path2[i] = '.'; 
	}
	if (strcmp(path2, "") == 0){
		sprintf(cmd, "sysctl -aN");
	}
	else {
		sprintf(cmd, "sysctl -N %s", path2);
	}
	readcmd = popen(cmd, "r");
	if (readcmd != NULL) {
		while ((read = getline(&line, &len, readcmd)) != -1) {
			flag = 0;
			line[read - 1] = 0;
			getentry(path2, line, entry);
			for(int i = 0; i < entries_count; i++) {

				if(strcmp(entries[i], entry) == 0) {
					flag = 1;
					break;
				}
			}
			if(flag) {
				continue;
			}

			strcpy(entries[entries_count], entry);
			entries_count++;
			filler(data, entry, NULL, 0);
		}
		fclose(readcmd);
		free(path2);
		return(0);
	}
}

int bopen(const char *path, struct fuse_file_info *fi){
	return (0);
}

/*
 * We should guaranteed the correct path.
 * If path is invalid - undefined behaviour.
 * I don't want to check again the path, 
 * cause it will be done by caller.
 */
char *get_sys_param(const char *path){
	char buf[1000], buf2[100], *ch, *out;
	double loadavg[3]; 
	long scale;
	out = (char*)malloc(sizeof(char) * 1000);
	size_t n = 1000;
	strncpy(buf2, path + 1, strlen(path));
	ch = buf2;
	while (*ch != 0){
		if (*ch == '/') *ch = '.';
		ch++;
	}
	sysctlbyname(buf2, buf, &n, NULL, 0);
	for (int j = 0; j < STRUCT_ELEMS_COUNT; j++)
		if (strcmp(buf2, structret[j]) == 0) {
			switch(j)
			{
				case 0:
					sprintf(out, "%s: { hz = %d, tick = %d, profhz = %d, stathz = %d }\n",
							buf2, *((int*)buf), ((int*)buf)[1], ((int*)buf)[2], ((int*)buf)[3]);
					return (out);
				case 1:
					sprintf(out, "%s: { sec = %d, usec = %d }\n",
							buf2, *((int*)buf), ((int*)buf)[2]);
					return (out);
				case 2:
					scale = ((struct loadavg *)buf)->fscale;
					for (int m = 0; m < 3; m++)
						loadavg[m] = (double)(((struct loadavg *)buf)->ldavg[m]) / scale;
					sprintf(out, "%s: { %.2f %.2f %.2f }\n",
							buf2, loadavg[0], loadavg[1], loadavg[2]);
					return (out);
			}
		}
	for (int j = 0; j < STR_ELEMS_COUNT; j++)
		if (strcmp(buf2, strret[j]) == 0){
			sprintf(out, "%s: %s\n", buf2, buf);
			return (out);
		}
	sprintf(out, "%s: %d\n", buf2, *((int*)buf));
	return (out);
}

int bgetattr(const char *path, struct stat *st){
	char *path2 = malloc(2048);
	int err;
	if (strcmp(path, "/") == 0) {
		st->st_mode = S_IFDIR | 0755;
		st->st_nlink = 8;
		return (0);
	}
	strcpy(path2, path);
	if(path2[0] == '/') path2++;
	for (int i = 0; path2[i] != 0 ; i++) {
		if(path2[i] == '/') path2[i] = '.'; 
	}
	err = isdir(path2);
	if(err == 0) {
		st->st_mode = S_IFDIR | 0755;
		/*if (strcmp(path, "/security")) st->st_nlink = 2;
		  else*/ st->st_nlink = 3;
		free(path2);
		//return -10;
		return (0);
	}
	else if (err == (-ENOTDIR)) {
		st->st_mode = S_IFREG | 0444;
		st->st_nlink = 1;
		//char *outp = get_sys_param(path);
		////st->st_size = strlen(outp);
		//free(outp);
		free(path2);
		return (0);
	}
	return (-ENOENT);
}

int btruncate(const char *path, off_t offset){
	return (0);
}

int bread(const char *path, char *data, size_t size, off_t offset, struct fuse_file_info *fi){
	for (int i = 0; i < PARAM_ELEMS_COUNT; i++)
		if (strcmp(path, params[i]) == 0){
			char *out = get_sys_param(path);
			int n = strlen(out);
			if (offset >= n) return (0);
			if (offset + size > n){
				memcpy(data, out + offset, n - offset);
				free(out);
				return (n - offset);
			}
			memcpy(data, out + offset, size);
			free(out);
			return (size);
		}
	return (-ENOENT);
}

/*
 * We can't know how many "write" calls will be
 * used by one write operation. So, we expect only one "write" call
 */
int bwrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	for (int i = 0; i < RDWR_ELEMS_COUNT; i++)
		if (strcmp(path, rdwr[i]) == 0){
			char buf2[100], *ch; strncpy(buf2, path + 1, strlen(path));
			ch = buf2;
			while (*ch != 0){
				if (*ch == '/') *ch = '.';
				ch++;
			}
			for (int j = 0; j < STR_ELEMS_COUNT; j++)
				if (strcmp(buf2, strret[j]) == 0){
					if (sysctlbyname(buf2, NULL, NULL, buf, strlen(buf)) != 0)
						return (-EINVAL);
					return (strlen(buf));
				}
			long inp = strtol(buf, NULL, 10);
			if ((inp == 0 && errno == EINVAL) || 
					((inp ==  LONG_MIN || inp == LONG_MAX) && errno == ERANGE))
				return (-EINVAL);
			if (sysctlbyname(buf2, NULL, NULL, &inp, sizeof(inp)) != 0)
				return (-EINVAL);
			return (size);
		}
	for (int i = 0; i < READONLY_ELEMS_COUNT; i++)
		if (strcmp(path, readonly[i]) == 0)
			return (-EACCES);
	return (-ENOENT);
}

struct fuse_operations operations = {
	.readdir = breaddir,
	.open = bopen,
	.getattr = bgetattr,
	.read = bread,
	.write = bwrite,
	.truncate = btruncate,
};

int main (int argc, char *argv[]){
	return (fuse_main(argc, argv, &operations, NULL));
}
