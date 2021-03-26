#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

#include <texted/permissions.h>
#include <texted/texted.h>

mode_t get_file_permissions(const char* Filename)
{
	struct stat st;

	if(!~stat(Filename, &st)) {
		perror(RED "Failed to read file infos" RESET);
		return -1;
	}

	return st.st_mode;
}

char* get_file_user(const char* Filename)
{
	struct stat st;
	struct passwd* pw;

	if(!~stat(Filename, &st)) {
		perror(RED "Failed to read file infos" RESET);
		return NULL;
	}

	// Don't free
	pw = getpwuid(st.st_uid);
	if(!pw)
		return NULL;
	
	return pw->pw_name;
}

char* get_file_group(const char* Filename)
{
	struct stat st;
	struct group* gr;

	if(!~stat(Filename, &st)) {
		perror(RED "Failed to read file infos" RESET);
		return NULL;
	}

	// Don't free
	gr = getgrgid(st.st_gid);
	if(!gr)
		return NULL;
	
	return gr->gr_name;
}

char get_file_type(const mode_t mode)
{
	char* mask = "bcdp-ls";
	char type = '\0';
	uint8_t bit_mask = 0;

	bit_mask |= S_ISBLK(mode)  << 6
			 |  S_ISCHR(mode)  << 5
			 |  S_ISDIR(mode)  << 4
			 |  S_ISFIFO(mode) << 3
			 |  S_ISREG(mode)  << 2
			 |  S_ISLNK(mode)  << 1
			 |  S_ISSOCK(mode) << 0;
	
	for(uint8_t b = 1, i = 0; i < 7; b <<= 1, i++) {
		if(b & bit_mask) {
			type = mask[i];
			break;
		}
	}
	
	return type;
}

char* get_string_permissions(const mode_t mode)
{
	const size_t size = 8;
	char* permissions = strdup("----------");
	char* mask = "rwxrwxrwx";

	permissions[0] = get_file_type(mode);
	for(size_t i = 0; i < 9; i++)
		if(mode & (1 << i))
			permissions[size + 1 - i] = mask[size - i];
	
	return permissions;
}

finfo_s* finfo(const char* Filename)
{
	char* tmp;
	finfo_s* fi = malloc(sizeof(finfo_s));
	char* fname = strdup(Filename);

	fi->fi_name = strdup(Filename);
	fi->fi_permissions = get_string_permissions(get_file_permissions(Filename));
	
	if(strchr(Filename, '.')) {
		tmp = strtok(fname, ".");
		tmp = strtok(NULL, ".");
		fi->fi_extension = strdup(tmp);
	} else {
		fi->fi_extension = NULL;
	}

	//! Check if free can be done
	fi->fi_user = get_file_user(Filename);
	fi->fi_group = get_file_group(Filename);

	return fi;
}

void finfo_free(finfo_s* fi)
{
	free(fi->fi_name);
	free(fi->fi_permissions);
	//free(fi->fi_user);
	//free(fi->fi_group);
	if(fi->fi_extension)
		free(fi->fi_extension);

	free(fi);
}

void ed_print_permissions(const char* Filename)
{
	finfo_s* fi = finfo(Filename);
	printf(BOLD MAGENTA "%s\t%s %s\t%s\n" RESET,
		   fi->fi_permissions,
		   fi->fi_user,
		   fi->fi_group,
		   fi->fi_name);
	
	finfo_free(fi);
}