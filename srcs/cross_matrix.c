/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cross_matrix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcarrot <kcarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/31 20:49:40 by kcarrot           #+#    #+#             */
/*   Updated: 2019/02/07 18:24:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

 static void				print_bits(unsigned char octet)
{
	int		x;
	char	res[8];

	x = 8;
	while (x--)
	{
		res[x] = (octet % 2) ? '1' : '0';
		octet = octet / 2;
	}
	write(1, res, 8);
}

static void				print_matrix(unsigned char **cross_m, int l)
{
	int i;
	int j;
	int len;

	len = l / 8;
	len += (l % 8) ? 1 : 0;
	i = 0;
	while (i < l)
	{
		j = 0;
		while (j < len)
		{
			print_bits(cross_m[i][j]);
			ft_putchar(' ');
			j++;
		}
		i++;
		ft_putstr("\n\n");
	}
}

int						no_crosses(unsigned char **cross_m,
							int *res, int j, int mv)
{
	unsigned char	check;
	int i;

	i = 0;
	//	C(1)
	while (res[i] != -1)
	{
	  //	C(2)
	  printf("----------%i-------\n", res[i]);
	  printf("----------%i-------\n", mv);
	  printf("%p\n",cross_m[res[i]]);
	  
	check = cross_m[res[i]][j] >> mv;
		//	C(3)
		if (check & (unsigned char)1)
			return (0);
		i++;
	}
	return (1);
}

static unsigned char	check_cross(t_path **paths, unsigned char **matrix,
									int i, int j)
{
	t_path			*r1;
	t_path			*r2;
	unsigned char	c;

	if (i == j)
		return (1);
	if (i > j)
	{
		c = matrix[j][i / 8];
		c >>= 7 - (i % 8);
		return (c & (unsigned char)1);
	}
	r2 = paths[j];
	while (r2)
	{
		r1 = paths[i];
		while (r1)
		{
			if (r1->room == r2->room && r1->room->state >= 0)
				return (1);
			r1 = r1->next;
		}
		r2 = r2->next;
	}
	return (0);
}

static void				set_bits(t_path **paths, unsigned char **matrix,
								int *rows, int l)
{
	unsigned char	check_c;
	int				chars;
	int				j;

	j = 0;
	chars = 0;
	matrix[*rows] = (unsigned char*)malloc(sizeof(unsigned char) *
	((l / 8) + 2));
	matrix[*rows][chars] = 0;
	while (j < l)
	{
		if (j && !(j % 8))
			chars++;
		check_c = check_cross(paths, matrix, *rows, j);
		check_c <<= (7 - j % 8);
		matrix[*rows][chars] |= check_c;
		j++;
	}
	
	j = j % 8;
	while (j <= 7)
		matrix[*rows][chars] |= (unsigned char)255 >> j++;
	(*rows)++;
}

unsigned char			**make_cross_matrix(t_path **paths, int l)
{
	int				rows;
	unsigned char	**matrix;

	rows = 0;
	matrix = (unsigned char**)malloc(sizeof(unsigned char*) * (l + 1));
	while (rows < l)
		set_bits(paths, matrix, &rows, l);
	matrix[rows] = NULL;
	print_matrix(matrix, l);
	return (matrix);
}
