/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebengtss <ebengtss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 16:45:11 by ebengtss          #+#    #+#             */
/*   Updated: 2024/10/11 13:57:45 by ebengtss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int	echo_option_check(char *arg)
{
	size_t	i;

	i = 1;
	if (!arg || (arg[0] && arg[0] != '-'))
		return (0);
	if (ft_strlen(arg) < 2)
		return (0);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_echo_bis(char *buffer)
{
	if (write(STDOUT_FILENO, buffer, ft_strlen(buffer)) < 0)
	{
		ft_puterror("minishell: 'echo': write error: ");
		perror(NULL);
		return (1);
	}
	return (0);
}

int	ft_echo(char **cmdve)
{
	t_nk	nk;
	char	*buffer;

	nk.i = 1;
	nk.j = 0;
	buffer = NULL;
	if (!cmdve[1])
		return (write(STDOUT_FILENO, "\n", 1), 0);
	if (cmdve[nk.i] && cmdve[nk.i] && echo_option_check(cmdve[nk.i]))
	{
		nk.j = 1;
		while (cmdve[nk.i] && echo_option_check(cmdve[nk.i]))
			nk.i++;
	}
	if (!cmdve[nk.i])
		return (0);
	while (cmdve[nk.i])
		buffer = ft_strjoin_c(buffer, cmdve[nk.i++], ' ', 1);
	if (!nk.j)
		buffer = ft_strjoin_s1(buffer, "\n");
	if (!buffer)
		return (-100);
	if (ft_echo_bis(buffer))
		return (free(buffer), 1);
	return (free(buffer), 0);
}
