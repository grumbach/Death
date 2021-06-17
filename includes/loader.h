/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:38:38 by agrumbac          #+#    #+#             */
/*   Updated: 2021/06/15 19:08:00 by ichkamo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOADER_H
# define LOADER_H

# include <stdbool.h>
# include <stdint.h>
# include <stddef.h>
# include <sys/types.h>

/*
** the virus header and its position in the loader's code
*/

struct			virus_header
{
	uint64_t	seed;
	size_t		virus_size;
	void		*loader_entry;
}__attribute__((packed));

void		virus_header_struct(void);

/*
** loader
*/

void		loader_entry(void);
void		call_virus(void);
void		jump_back_to_client(void);
void		loader_exit(void);

#endif
