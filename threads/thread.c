#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

const int kThreadsNum = 5;
int global_var = 1234;


void* mythread(void* arg) {
    int local_var = 10;
    static int static_local_var = 20;
    const int const_local_var = 30;
    
    /**
	 * идентификаторы процесса, родительского процесса, 
	 * потока [pthread_self() и gettid() ] 
	 */
    printf("mythread [pid: %d, ppid: %d, tid: %d, pthread_self: %lu]\n",
           getpid(), getppid(), gettid(), pthread_self());
    

	/**
	 * адреса локальной, локальной статической, локальной константной и
	 * глобальной переменных 
	 * */	   
    printf("  Local var address: %p\n", (void*)&local_var);
    printf("  Static local var address: %p\n", (void*)&static_local_var); // same
    printf("  Const local var address: %p\n", (void*)&const_local_var);
    printf("  Global var address: %p\n", (void*)&global_var); // same
    printf("  Thread function address: %p\n\n", (void*)mythread); // same
    
    return (void*)pthread_self();
}

int main() {
	pthread_t t_ids[kThreadsNum];
	int err;

	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

	for (int i = 0; i < kThreadsNum; ++i) {
		err = pthread_create(&(t_ids[i]), NULL, mythread, NULL);
		if (err) {
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}
		printf("pthread_create() returned tid: %lu \n", t_ids[i]);


	}

	void* return_value;
	for (int i = 0; i < kThreadsNum; ++i) {
		pthread_join(t_ids[i], &return_value);

		
		if (pthread_equal((pthread_t)return_value, t_ids[i])) {
			/**
			 * Сравнение результатов pthread_self() с тем что функция 
			 * pthread_create() вернула через первый аргумент.
			 */
			printf("pthread_self() and pthread_create() return values match for %d\n", i);
		}
	}	

	return 0;
}