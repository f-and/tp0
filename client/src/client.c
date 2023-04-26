#include "client.h"

void recibir_mensaje(int, t_log*);
void *recibir_buffer(int *, int);

int main(void) {
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char *ip, *puerto, *valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	logger = iniciar_logger();
	if (logger == NULL) {
		printf("Error al generar logger.\n ");
		return 1;
	}

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */
	config = iniciar_config();
	if (config == NULL) {
		log_info(logger, "Error al abrir config");
		log_destroy(logger);
		return 1;
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");

	// Loggeamos el valor de config
	log_info(logger, "ip: %s", ip);
	log_info(logger, "puerto: %s", puerto);
	log_info(logger, "valor: %s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */
	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	recibir_mensaje(conexion, logger);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void) {
	t_log* nuevo_logger = log_create("tp0.log", "cliente", 1, LOG_LEVEL_INFO);
	return nuevo_logger;
}

t_config* iniciar_config(void) {
	t_config* nuevo_config = config_create("cliente.config");
	return nuevo_config;
}

void leer_consola(t_log* logger) {
	char* leido;
	// La primera te la dejo de yapa
	leido = readline("> ");
	
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido, "")) {
		log_info(logger, leido);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

}

void paquete(int conexion) {
	// Ahora toca lo divertido!
	char* leido = readline("> ");
	t_paquete* paquete = crear_super_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while(strcmp(leido, "")) { 
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
		leido = readline("> ");
	}
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  log_destroy(logger);
	  config_destroy(config);
	  close(conexion);
}

void* recibir_buffer(int* size, int socket_cliente) {
	void * buffer;
	int cod_op;
	recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL);

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente, t_log* logger) {
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}
