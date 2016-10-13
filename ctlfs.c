/*Broken file system*/
#define _GNU_SOURCE
#define FUSE_USE_VERSION 30

#include <stdio.h>
#include <stdlib.h>
#include <fuse.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/sysctl.h>
#include <sys/limits.h>
#include <sys/wait.h>
#include <sys/resource.h>

/* Return 0 if entry is directory */
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

/* Return 0 if file is changeable */
int ischangeable(char* path) {
	FILE *readcmd;
	char cmd[2048], *line = NULL;
	ssize_t read;
	size_t len;
	sprintf(cmd, "sysctl -W %s", path);
	readcmd = popen(cmd, "r");
	if (readcmd != NULL) {
		if ((read = getline(&line, &len, readcmd)) > 0) {
			return (0);
		}
		else return (-EACCES);
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
	return (0);
}

int bopen(const char *path, struct fuse_file_info *fi){
	return (0);
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
		st->st_nlink = 3;
		free(path2);
		return (0);
	}
	else if (err == (-ENOTDIR)) {
		if(ischangeable(path2) == 0)
			st->st_mode = S_IFREG | 0666;
		else
			st->st_mode = S_IFREG | 0444;
		st->st_nlink = 1;
		free(path2);
		return (0);
	}
	return (-ENOENT);
}

int btruncate(const char *path, off_t offset){
	return (0);
}

int bread(const char *path, char *data, size_t size, off_t offset, struct fuse_file_info *fi){
	FILE *readcmd;
	char *line = NULL;
	char cmd[2048];
	char *path2 = malloc(2048);
	int err = 0;
	size_t len = 0;
	ssize_t read;

	strcpy(path2, path);
	if(path2[0] == '/') path2++;
	for (int i = 0; path2[i] != 0 ; i++) {
		if(path2[i] == '/') path2[i] = '.'; 
	}
	err = isdir(path2);
	if(err == (-ENOTDIR)) {
		sprintf(cmd, "sysctl %s | awk '{ print $2 }'", path2);
		readcmd = popen(cmd, "r");
		if (readcmd != NULL) {
			if ((read = getline(&line, &len, readcmd)) != -1) {
				line[--read] = 0;
				if (offset >= read) { free(path2); free(line); return (0);}
				if (offset + size > read){
					memcpy(data, line + offset, read - offset);
					free(path2);
					free(line);
					return (read - offset);
				}
				memcpy(data, line + offset, size);
				free(line);
				free(path2);
				return (size);
			}
			fclose(readcmd);
			free(line);
			free(path2);
			return(0);
		} 
		else if (err == 0)  { free(path2); return (-EISDIR); }
		else { free(path2); return (-ENOENT); }
	}
	return (-ENOENT);
}

/*
 * We can't know how many "write" calls will be
 * used by one write operation. So, we expect only one "write" call
 */
int bwrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	FILE *readcmd;
	char cmd[2048], writebuf[2048];
	char *path2 = malloc(2048);
	int err = 0;

	strcpy(path2, path);
	if(path2[0] == '/') path2++;
	for (int i = 0; path2[i] != 0 ; i++) {
		if(path2[i] == '/') path2[i] = '.'; 
	}
	err = isdir(path2);
	if(err == (-ENOTDIR)) {
		strncpy(writebuf, buf, size-1);
		writebuf[size - 1] = 0;
		sprintf(cmd, "sysctl %s=%s", path2, writebuf);
		readcmd = popen(cmd, "r");
		if (readcmd != NULL) {
			err = WEXITSTATUS(pclose(readcmd));  
			if(err == 0) { 
				free(path2);
				return size;
			}
			else {
				free(path2);
				return (-EACCES);
			}
		} 
		free(path2);
		return (-EINVAL);
	}
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
