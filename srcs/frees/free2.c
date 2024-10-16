/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebengtss <ebengtss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 18:38:10 by mtrullar          #+#    #+#             */
/*   Updated: 2024/10/15 20:34:18 by ebengtss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void	ft_close_fds(t_data *data)
{
	if (STDERR_FILENO != -1)
		close(STDERR_FILENO);
	if (STDIN_FILENO != -1)
		close(STDIN_FILENO);
	if (STDOUT_FILENO != -1)
		close(STDOUT_FILENO);
	if (data->tmpstdin != -1)
		close(data->tmpstdin);
	if (data->stdincpy != -1)
		close(data->stdincpy);
	if (data->stdoutcpy != -1)
		close(data->stdoutcpy);
}

void	ft_free_tokens(t_tokens *tok)
{
	t_tokens	*tmp;

	while (tok)
	{
		tmp = tok;
		tok = tok->next;
		free(tmp->str);
		free(tmp);
	}
}

void	ft_free_commands(t_cmds *cmds)
{
	t_cmds	*tmp;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		ft_free_tokens(tmp->tokens);
		free(tmp->cmd);
		free(tmp);
	}
}

void	ft_free_invalid_syntax(t_cmds *to_free)
{
	ft_free_tokens(to_free->tokens);
	free(to_free);
}

void	ft_free_var(t_data *data)
{
	t_var	*node;
	t_var	*temp;

	node = data->var;
	if (!node)
		return ;
	while (node)
	{
		temp = node;
		node = node->next;
		if (temp->name)
			free(temp->name);
		if (temp->content)
			free(temp->content);
		free(temp);
	}
}
