/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deneme.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkas <bkas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 12:42:08 by bkas              #+#    #+#             */
/*   Updated: 2024/04/24 13:42:35 by bkas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int    err(char *str) {
    while(*str)
        write(1, str++, 1);
    return 1;
}

int cd(char **argv, int i) {
    if (i != 2)
        err("error: cd: bad arguments\n");
    else if (chdir(argv[i]) == -1)
        err("error: cd: cannot change directory to "), err(argv[1]), err("\n");
    return 0;
}

int exec(char **argv, char **envp, int i) {
    int fd[2];
    int status;
    int has_pipe = argv[i] && !strcmp(argv[i], "|");

    if (!has_pipe && !strcmp(argv[i], "cd"))
        return cd(argv, i);
    if (has_pipe && pipe(fd) == -1)
        return err("error: fatal\n");
    int pid = fork();
    if (!pid) {
        argv[i] = 0;
        if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1] == -1)))
            return err("error: fatal\n");
        if (!strcmp(argv[i], cd))
            return (cd(argv[i], i));
        execve(*argv, argv, envp);
        return (err("error: cannot execute "), err(argv[i]), err("\n"));
    }
    waitpid(pid, &status, 0);
    if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0] == -1), close(fd[1] == -1)))
        return err("error: fatal\n");
    return WIFEXITED(status) && WEXITSTATUS(status);
}

int main(int argc, char **argv, char **envp) {
    int i = 0;
    int status = 0;
    
    if (argc > 1) {
        while (argv[i] && argv[++i]) {
            argv = argv + i;
            i = 0;
            while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
                i++;
            if (i)
                status = exec(argv, envp, i);
        }
    }
    return status;
}