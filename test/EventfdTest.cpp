/*
 * EventfdTest.cpp
 *
 *  Created on: 2013-8-6
 *      Author: fengyu
 */



#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>

int efd = -1;

void *read_thread(void *dummy)
{
    int ret = 0;
    uint64_t count = 0;
    int ep_fd = -1;
    struct epoll_event events[10];

    if (efd < 0)
    {
        printf("efd not inited.\n");
        goto fail;
    }

    ep_fd = epoll_create(1024);
    if (ep_fd < 0)
    {
        perror("epoll_create fail: ");
        goto fail;
    }

    {
        struct epoll_event read_event;

        read_event.events = EPOLLHUP | EPOLLERR | EPOLLIN;
        read_event.data.fd = efd;

        ret = epoll_ctl(ep_fd, EPOLL_CTL_ADD, efd, &read_event);
        if (ret < 0)
        {
            perror("epoll ctl failed:");
            goto fail;
        }
    }

    while (1)
    {
        ret = epoll_wait(ep_fd, &events[0], 10, 5000);
        if (ret > 0)
        {
            int i = 0;
            for (; i < ret; i++)
            {
                if (events[i].events & EPOLLHUP)
                {
                    printf("epoll eventfd has epoll hup.\n");
                    goto fail;
                }
                else if (events[i].events & EPOLLERR)
                {
                    printf("epoll eventfd has epoll error.\n");
                    goto fail;
                }
                else if (events[i].events & EPOLLIN)
                {
                    int event_fd = events[i].data.fd;
                    ret = read(event_fd, &count, sizeof(count));
                    if (ret < 0)
                    {
                        perror("read fail:");
                        goto fail;
                    }
                    else
                    {
                        struct timeval tv;

                        gettimeofday(&tv, NULL);
                        printf("success read from efd, read %d bytes(%llu) at %lds %ldus\n",
                               ret, count, tv.tv_sec, tv.tv_usec);
                    }
                }
            }
        }
        else if (ret == 0)
        {
            /* time out */
            printf("epoll wait timed out.\n");
            break;
        }
        else
        {
            perror("epoll wait error:");
            goto fail;
        }
    }

fail:
    if (ep_fd >= 0)
    {
        close(ep_fd);
        ep_fd = -1;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t pid = 0;
    uint64_t count = 0;
    int ret = 0;
    int i = 0;

    efd = eventfd(0, 0);
    if (efd < 0)
    {
        perror("eventfd failed.");
        goto fail;
    }

    ret = pthread_create(&pid, NULL, read_thread, NULL);
    if (ret < 0)
    {
        perror("pthread create:");
        goto fail;
    }

    for (i = 0; i < 5; i++)
    {
        count = 4;
        ret = write(efd, &count, sizeof(count));
        if (ret < 0)
        {
            perror("write event fd fail:");
            goto fail;
        }
        else
        {
            struct timeval tv;

            gettimeofday(&tv, NULL);
            printf("success write to efd, write %d bytes(%llu) at %lds %ldus\n",
                   ret, count, tv.tv_sec, tv.tv_usec);
        }

        sleep(1);
    }

fail:
    if (0 != pid)
    {
        pthread_join(pid, NULL);
        pid = 0;
    }

    if (efd >= 0)
    {
        close(efd);
        efd = -1;
    }
    return ret;
}


/*
 * eventfd具体与pipe有点像，用来完成两个线程之间事件触发，但是同事说现在已经支持到进程级别，现在我还没有验证过。能够用来作为线程之间简单通讯，类似于pthread_cond_t。
epoll则是linux提供的一种多路复用技术，完成与select，poll等一样的功能，完成对多个文件描述符进行等待。
 */
