/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:06:34 by hipham            #+#    #+#             */
/*   Updated: 2025/01/28 03:46:00 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

int error_sms(char *s, int ret)
{
	printf("Error: %s\n", s);
	return(ret);
}

void ft_malloc(void **data, size_t size)
{
	*data = malloc(size);
	if (*data == NULL)
	{
		printf("Malloc failed to allocate memory\n");
		exit(EXIT_FAILURE);
	}
}
