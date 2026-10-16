/* Minimal TERM watchdog for one idma_experiment process (which does not fork).
 * This does not cancel hardware DMA or recover uninterruptible kernel waits.
 */
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    const int signals[] = { SIGALRM, SIGHUP, SIGINT, SIGTERM, SIGCHLD };
    sigset_t watched, original;
    struct sigaction action = { .sa_handler = SIG_DFL };
    unsigned long seconds;
    char *end;
    pid_t child, waited;
    int status, received, error, expired = 0, interrupted = 0;
    size_t i;

    if (argc < 3 || !argv[1][0])
        goto usage;
    for (const char *p = argv[1]; *p; ++p)
        if (*p < '0' || *p > '9')
            goto usage;
    errno = 0;
    seconds = strtoul(argv[1], &end, 10);
    if (errno || *end || !seconds || seconds > UINT_MAX)
        goto usage;

    sigemptyset(&watched);
    sigemptyset(&action.sa_mask);
    for (i = 0; i < sizeof(signals) / sizeof(signals[0]); ++i)
        sigaddset(&watched, signals[i]);
    /* Block before fork: neither termination nor SIGCHLD can race PID setup. */
    if (sigprocmask(SIG_BLOCK, &watched, &original)) {
        perror("platform-timeout: sigprocmask");
        return 125;
    }
    for (i = 0; i < sizeof(signals) / sizeof(signals[0]); ++i) {
        if (sigaction(signals[i], &action, NULL)) {
            perror("platform-timeout: sigaction");
            return 125;
        }
    }
    child = fork();
    if (child < 0) {
        perror("platform-timeout: fork");
        return 125;
    }
    if (!child) {
        sigprocmask(SIG_SETMASK, &original, NULL);
        execvp(argv[2], &argv[2]);
        error = errno;
        fprintf(stderr, "platform-timeout: %s: %s\n", argv[2], strerror(error));
        _exit(error == ENOENT ? 127 : 126);
    }

    alarm((unsigned int)seconds);
    for (;;) {
        waited = waitpid(child, &status, WNOHANG);
        if (waited == child)
            break;
        if (waited < 0) {
            if (errno == EINTR)
                continue;
            perror("platform-timeout: waitpid");
            return 125;
        }
        /* Pending signals remain blocked between waitpid and sigwait. */
        error = sigwait(&watched, &received);
        if (error) {
            errno = error;
            perror("platform-timeout: sigwait");
            kill(child, SIGTERM);
            while (waitpid(child, &status, 0) < 0 && errno == EINTR) {}
            return 125;
        }
        if (received == SIGCHLD)
            continue;
        if (received == SIGALRM)
            expired = 1;
        else
            interrupted = 128 + received;
        /* Signal only our unreaped direct child; never an unrelated group. */
        kill(child, SIGTERM);
    }
    alarm(0);
    if (expired) {
        fprintf(stderr, "platform-timeout: command exceeded %lu seconds (TERM sent)\n",
                seconds);
        return 124;
    }
    if (interrupted)
        return interrupted;
    return WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);

usage:
    fprintf(stderr, "usage: platform-timeout SECONDS COMMAND [ARG ...]\n");
    return 2;
}
