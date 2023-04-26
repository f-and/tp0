#include "server.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>


typedef struct {
	int socket;
	int num;
} hilos_t;

void handler(int);
int servidor(hilos_t *);
int cliente_fd;
int terminar;
sem_t socket_is_free;

int main(void) {
	terminar = 0;
	int i = 0;
	signal(SIGINT, handler);
	//socket_is_free = sem_open("Socket is free", O_CREAT);
	sem_init(&socket_is_free, 0, 1);
	logger = log_create("tp0.log", "Servidor", 1, LOG_LEVEL_DEBUG);
	int server_fd = iniciar_servidor();
	while(!terminar) {
		sem_wait(&socket_is_free);
		if(!terminar) {
			hilos_t cpy;
			cpy.socket = server_fd;
			cpy.num = i;
			i++;
			pthread_t hilo;
			log_info(logger, "%d | Servidor listo para recibir al cliente", cpy.num);
			if(!pthread_create(&hilo, NULL, (void *) servidor, (void *) &cpy))
				pthread_detach(hilo);
		} else sem_post(&socket_is_free);
	}
	sem_wait(&socket_is_free);
	sem_destroy(&socket_is_free);
	log_destroy(logger);

//	t_list* lista;

}

int servidor(hilos_t *cpy) {	
	sleep(1);
	int number = (*cpy).num;
	int socket = (*cpy).socket;
	cliente_fd = esperar_cliente(socket);
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			t_list* lista = recibir_paquete(cliente_fd);
			log_info(logger, "%d | Me llegaron los siguientes valores:", number);
			list_iterate(lista, (void*) iterator);
//			list_destroy_and_destroy_elements(lista, (void*)iterator);
			break;
		case -1:
			log_error(logger, "%d | el cliente se desconecto. Terminando servidor", number);
			sem_post(&socket_is_free);
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	sem_post(&socket_is_free);
	return EXIT_SUCCESS;

}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void handler(int sig) {
	terminar = 1;
}