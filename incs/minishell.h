/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebengtss <ebengtss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:28:45 by mtrullar          #+#    #+#             */
/*   Updated: 2024/10/09 15:18:20 by ebengtss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE

# include "../libft/incs/ft_printf.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <errno.h>
# include <fcntl.h>
# include <termcap.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum s_type
{
	WAIT,
	NO_TYPE,
	CMD,
	ARGS,
	PIPE,
	INFILE,
	LIMITER,
	OUTFILE,
	APPEND,
	REDIR,
	ERROR,
	ASK,
	ENV
}	t_type;

typedef struct s_hd
{
	int			fd;
	struct s_hd	*next;
	struct s_hd	*prev;
}				t_hd;

typedef struct s_tokens
{
	char			*str;
	t_type			type;
	struct s_tokens	*next;
	struct s_tokens	*prev;
}					t_tokens;

typedef struct s_cmds
{
	char			*cmd;
	t_tokens		*tokens;
	struct s_cmds	*next;
	struct s_cmds	*prev;
}					t_cmds;

typedef struct s_var
{
	char			*name;
	char			*content;
	struct s_var	*next;
	struct s_var	*prev;
}					t_var;

typedef struct s_env
{
	char			*key;
	char			*val;
	int				exp_noval;
	struct s_env	*prev;
	struct s_env	*next;
}					t_env;

typedef struct s_envs
{
	t_env	*env;
	t_env	*exp;
	t_env	*tmpenv;
	t_env	*l_env;
	char	**tmpenvve;
	char	**envve;
}			t_envs;

typedef struct s_data
{
	t_cmds	*cmds;
	t_envs	*envs;
	t_var	*var;
	t_hd	*heredoc;
	t_hd	*hd_filler;
	char	***cmdve;
	char	*read;
	char	*tmpexitstatus;
	int		exit_status;
	int		stdincpy;
	int		stdoutcpy;
	int		isoutred;
	int		cmdvesize;
	int		iheredoc;
	int		isrunned;
}			t_data;

//	PARSING
//		parsing.c
int			ft_parser(char *line, t_cmds **commands, t_data *data);
int			print_commands(t_cmds *com);

//		check_error.c
int			ft_check_quote_syntax(char *str);

//		commands_struct.c
t_cmds		*ft_get_last_commands(t_cmds *tmp);
int			add_commands(t_cmds *new, t_cmds **head);

//		handle_variable.c
int			ft_add_variable(char *str, t_data *data);
int			ft_check_variable(char *str, t_data *data);
char		*ft_get_variable_value(char *key, t_data *data);
int			ft_update_variable(char *key, char *val, t_data *data);

//		here_docs.c
int			ft_heredoc_handler(t_tokens *head, t_data *data);
int			ft_ask_handler(t_tokens *head, t_data *data);
int			add_heredoc_list(int fd, t_data *data);
t_hd		*ft_last_hd(t_hd *head);
void		add_back_heredoc_list(t_hd *new_node, t_hd **head);
void		del_top_hd(t_data *data);

//		tokenization.c
int			ft_is_pipe(t_tokens *current);
int			ft_is_redirect_sign(t_tokens *current);
int			ft_is_args(t_tokens *node);
int			ft_is_commands(t_tokens *node);
int			add_tokens_between(char *str, t_tokens *current, t_type type);

//		tokens_struct.c
t_tokens	*ft_get_last_token(t_tokens *head);
t_tokens	*create_token_list(char *line, t_data *data);
int			add_new_token(char *str, t_tokens **head, t_type type);
int			get_type(t_tokens *head_node);

//		var_list_func.c
t_var		*ft_last_var(t_var *head);
t_var		*ft_is_var_exist(char *str, t_var *head, int i);
void		ft_var_add_back(t_var *new_node, t_var **head);

//		parsing_utils.c
char		*ft_flat_string(char *str, t_data *data);
int			count_cmd(t_tokens *actu);

//		utils.c
void		ft_fix(t_tokens *head, char **splitted);

//	HISTORY
int			ft_get_history(void);
int			ft_write_history(t_cmds *cmds);

//	FREE
//		conditionnal_free.c
void		ft_free_invalid_syntax(t_cmds *to_free);
void		ft_free_var(t_data *data);

//		free.c
void		ft_free_heredoc(t_data *data);
void		free_main(t_data *data, int i);
void		ft_free_cmdve(t_data *data);
void		ft_free_tokens(t_tokens *tok);
void		ft_free_commands(t_cmds *cmds);
void		ft_free_env(t_env **lst);

/* EXEC */
int			exec(t_data *data, t_cmds *cmd);
int			run_cmd(t_data *data, int i, int islast);
int			is_redirs(t_data *data, t_cmds *cmd, int i);
t_tokens	*skip_tokens(t_cmds *cmd, int i);
int			is_builtin(char *cmd);
int			exec_builtin(t_data *data, char **cmdve);
int			run_gtw(t_data *data, t_cmds *cmd, int *i, int islast);
char		***ft_make_cmdve(t_data *data, t_cmds *cmd);
int			fill_cmdve(char ***cmdve, t_cmds *cmd);
int			cmds_path(char ***cmdve, t_data *data);
int			reset_fds(t_data *data, int std);

/* BUILTINS */
int			ft_echo(char **cmdve);
int			echo_option_check(char *option);
int			ft_cd(t_data *data, char **cmdve);
int			ft_pwd(t_data *data);
int			ft_export(t_data *data, char **cmdve);
int			ft_unset(t_data *data, char **cmdve);
int			ft_env(t_data *data, char **cmdve);
int			ft_exit(t_data *data, char **cmdve);

/* ENV */
t_env		*envnew_gtw(char *str, int is_exp_no_val);
size_t		ft_envsize(t_env *lst);
int			env_init(t_data	*data, char **env);
t_env		*ft_envnew(char *key, char *val, int is_exp_no_val);
void		ft_envadd_front(t_env **lst, t_env *new);
void		ft_envadd_back(t_env **lst, t_env *new);
t_env		*ft_envlast(t_env *lst);
int			ft_envdup(t_data *data);
int			incr_shlvl(t_data *data);
int			envtab_update(t_data *data);
int			is_inenv_key(t_env *env, char *keycheck);
int			is_inenv_str(t_env *env, char *strcheck);
void		ft_envdelone(t_data *data, t_env *node);
char		**env_to_tab(t_env *env);
void		print_env(t_env *env, int env_or_exp);
int			tmp_env_add(t_data *data, char *cmdve);
int			tmp_env_setup(t_data *data, t_cmds *cmd, int i);
int			env_update(t_env *lst, char *str);
int			check_env(t_data *data);
int			isvalid_env(char *str, int only_key);

#endif