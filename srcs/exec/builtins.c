/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtrullar <mtrullar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 16:35:12 by ebengtss          #+#    #+#             */
/*   Updated: 2024/09/23 12:55:12 by mtrullar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int	is_builtin(char *cmd)
{
	size_t			i;
	const char		*builtins[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"
	};

	i = 0;
	while (i < 7)
	{
		if (!ft_ultimate_compare(cmd, (char *)builtins[i]))
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(t_data *data, char **cmdve)
{
	int	retval;

	retval = 0;
	if (!ft_ultimate_compare(cmdve[0], "echo"))
		retval = ft_echo(cmdve);
	else if (!ft_ultimate_compare(cmdve[0], "cd"))
		retval = ft_cd(data, cmdve);
	else if (!ft_ultimate_compare(cmdve[0], "pwd"))
		retval = ft_pwd(data);
	else if (!ft_ultimate_compare(cmdve[0], "export"))
		retval = ft_export(data, cmdve);
	else if (!ft_ultimate_compare(cmdve[0], "unset"))
		retval = ft_unset(data, cmdve);
	else if (!ft_ultimate_compare(cmdve[0], "env"))
		retval = ft_env(data, cmdve);
	else if (!ft_ultimate_compare(cmdve[0], "exit"))
		retval = ft_exit(data, cmdve);
	if (retval != -100)
		data->exit_status = retval;
	return (retval);
}
