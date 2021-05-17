#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>
#include <stdbool.h> // para usar bool

#define LIMITE 50

//declaro 2 mutex y un semaforo global
sem_t semHorno;// el horno puede cocinar 2 panes a la vez, pueden utilizarlo 2 equipo 
pthread_mutex_t mutexSalero = PTHREAD_MUTEX_INITIALIZER;// puede utilizar un equipo a la vez el salero
pthread_mutex_t mutexSarten = PTHREAD_MUTEX_INITIALIZER;// puede utilizar un equipo a la vez la sarten


//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_agregarCarne;
    sem_t sem_empanar;
    sem_t sem_cocinarMilanesa;
    sem_t sem_panListo;
    sem_t sem_milanesaLista;
    sem_t sem_extrasListo;
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[9];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}

//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la señal a la siguiente accion (cortar me habilita mezclar)
		/*salida del txt*/
	FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - Ha cortado el ajo y las demas boludeces! \n", salida);
	fclose(salida);
    sem_post(&mydata->semaforos_param.sem_mezclar);
	
    pthread_exit(NULL);
}

void* mezclar(void *data) {

	//creo el nombre de la accion de la funcion 
        char *accion = "mezclar";
       	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_mezclar);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
		/*salida del txt*/
	FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - termino de mezclar! \n", salida);
	fclose(salida);
	//doy la señal a la siguiente accion (mezclar habilita la accion salar)
        sem_post(&mydata->semaforos_param.sem_salar);
	
    pthread_exit(NULL);
}
void* salar(void *data) {

	//creo el nombre de la accion de la funcion 
        
	char *accion = "salar";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_salar);
        
        pthread_mutex_lock(&mutexSalero);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - termino salar! \n", salida);
	fclose(salida);
	//doy la señal a la siguiente accion (salar habilita agregarCarne)
        pthread_mutex_unlock(&mutexSalero);
        sem_post(&mydata->semaforos_param.sem_agregarCarne);
	
    pthread_exit(NULL);
}
void* agregarCarne(void *data) {

	//creo el nombre de la accion de la funcion         
	char *accion = "agregarCarne";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;   
        sem_wait(&mydata->semaforos_param.sem_agregarCarne);
        
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - agrego la carne a la mezcla! \n", salida);
	fclose(salida);
	//doy la señal a la siguiente accion (agregarCarne habilita empanar)
        sem_post(&mydata->semaforos_param.sem_empanar);
	
    pthread_exit(NULL);
}
void* empanar(void *data) {

	//creo el nombre de la accion de la funcion 
        char *accion = "empanarMilanesa";
       
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_empanar);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - milanesa empanizada! \n", salida);
	fclose(salida);
	//doy la señal a la siguiente accion (empanar habilita cocinarMilanesa;)
        sem_post(&mydata->semaforos_param.sem_cocinarMilanesa);
	
    pthread_exit(NULL);
}
void* cocinarMilanesa(void *data) {

	//creo el nombre de la accion de la funcion 
        char *accion = "cocinarMilanesa";
        
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
        sem_wait(&mydata->semaforos_param.sem_cocinarMilanesa);
        pthread_mutex_lock(&mutexSarten);
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - tiene la milanesa lista! \n", salida);
	fclose(salida);
	//doy la señal a la siguiente accion (cocinarMilanesa habilita milanesaLista )
        pthread_mutex_unlock(&mutexSarten);
        sem_post(&mydata->semaforos_param.sem_milanesaLista);
	
    pthread_exit(NULL);
}
void* cortarExtras(void *data) {

	//creo el nombre de la accion de la funcion 
        char *accion = "cortarVerduras";
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - corto lechuga, cebolla,tomate y pepino! \n", salida);
	fclose(salida);
	//doy la señal a la siguiente accion (salar habilita extrasListos)
        sem_post(&mydata->semaforos_param.sem_extrasListo);
	
    pthread_exit(NULL);
}
void* hornear(void *data) {

	//creo el nombre de la accion de la funcion 
        char *accion = "cocinarPan";
        sem_wait(&semHorno);
        //creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - tiene los panes listos! \n", salida);
	fclose(salida);
        sem_post(&semHorno);
	//doy la señal a la siguiente accion (horno habilita panListo);
        sem_post(&mydata->semaforos_param.sem_panListo);
	
         pthread_exit(NULL);
}
void* terminarSanguche(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "terminarSanguche";
    	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
        //los 3 mutex habilitan terminarSanguche
	sem_wait(&mydata->semaforos_param.sem_milanesaLista);
	sem_wait(&mydata->semaforos_param.sem_panListo);
        sem_wait(&mydata->semaforos_param.sem_extrasListo);
	
        //llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
        //uso sleep para simular que que pasa tiempo
	usleep( 20000 );
      
        printf("\tEquipo %d - -----------GANO!----------- \n ",mydata->equipo_param);

	//salida del txt
	FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino existe escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - Es el ganador! ", salida);
	fclose(salida);
	
	exit(1); 
   
         pthread_exit(NULL);
}

void* ejecutarReceta(void *i) {
	
	//variables semaforos
	sem_t sem_mezclar;
        sem_t sem_salar;
        sem_t sem_agregarCarne;
        sem_t sem_empanar;
        sem_t sem_cocinarMilanesa;
        sem_t sem_milanesaLista;
        sem_t sem_extrasListo;
        sem_t sem_panListo;
        

   
		
	//variables hilos
	pthread_t p1; //cortar  ajo y perejil
        pthread_t p2; // mezclar con huevo
        pthread_t p3; // salar la preparacion
        pthread_t p4; //agregar la carne
        pthread_t p5; //empanar la milanesa
        pthread_t p6; //cocinar la milanesa en la sarten
        pthread_t p7; //hornear el pan
        pthread_t p8; //cortar extras: lechuga, tomate, cebolla, pepino
        pthread_t p9; //juntar todo y armar el sanguche :)


	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
        pthread_data->semaforos_param.sem_salar = sem_salar;
        pthread_data->semaforos_param.sem_agregarCarne = sem_agregarCarne;
        pthread_data->semaforos_param.sem_empanar = sem_empanar;
        pthread_data->semaforos_param.sem_cocinarMilanesa = sem_cocinarMilanesa;
        pthread_data->semaforos_param.sem_panListo = sem_panListo;
        pthread_data->semaforos_param.sem_milanesaLista = sem_milanesaLista;
        pthread_data->semaforos_param.sem_extrasListo = sem_extrasListo;
     
         //para leer la receta
         FILE * archivo = fopen("receta.txt", "r"); // abre la receta
         bool k=true;
         int m=0;
	     int j=0;
		
		
	 char aux[100];
		
	 while(feof(archivo) == 0){ 
	        fscanf(archivo, "%s", aux); //con fscanf agarramos una linea hasta \n y la insertamos en la variable aux
			
		for (char *split = strtok(aux, "|"); split; split = strtok(NULL, "|")){ 
				
		     if(k==true){ 
			strcpy(pthread_data->pasos_param[m].accion, split); //la agregamos
			     k=false; 
					
		     }else{
			   strcpy(pthread_data->pasos_param[m].ingredientes[j], split);
			        j++;// incrementamos j para seguir con el otro ingrediente
			  }
		}

		m++; // incremento i para para seguir a la siguiente accion
		j=0; // j vuelve a cero
		k=true; // p vuelve a True
		}
		fclose(archivo);

	

     sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
     sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
     sem_init(&(pthread_data->semaforos_param.sem_agregarCarne),0,0);
     sem_init(&(pthread_data->semaforos_param.sem_empanar),0,0);
     sem_init(&(pthread_data->semaforos_param.sem_cocinarMilanesa),0,0);
     sem_init(&(pthread_data->semaforos_param.sem_panListo),0,0);
     sem_init(&(pthread_data->semaforos_param.sem_milanesaLista),0,0);
     sem_init(&(pthread_data->semaforos_param.sem_extrasListo),0,0);
	//inicializar demas semaforos aqui


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	
     rc = pthread_create(&p2,                           //identificador unico
                           NULL,                          //atributos del thread
                                mezclar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
	
     rc = pthread_create(&p3,                           //identificador unico
                           NULL,                          //atributos del thread
                                salar,             //funcion a ejecutar
                                pthread_data);   
      rc = pthread_create(&p4,                           //identificador unico
                           NULL,                          //atributos del thread
                                agregarCarne,             //funcion a ejecutar
                                pthread_data); 

      rc = pthread_create(&p5,                           //identificador unico
                           NULL,                          //atributos del thread
                                empanar,             //funcion a ejecutar
                                pthread_data); 

       rc = pthread_create(&p6,                           //identificador unico
                           NULL,                          //atributos del thread
                                cocinarMilanesa,             //funcion a ejecutar
                                pthread_data); 
       
        rc = pthread_create(&p7,                           //identificador unico
                           NULL,                          //atributos del thread
                                cortarExtras,             //funcion a ejecutar
                                pthread_data); 
      
         rc = pthread_create(&p8,                           //identificador unico
                            NULL,                          //atributos del thread
                                hornear,            //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia

         rc = pthread_create(&p9,                           //identificador unico
                            NULL,                          //atributos del thread
                                terminarSanguche,             //funcion a ejecutar
                                pthread_data);  
         



	
	//join de todos los hilos
	pthread_join (p1,NULL);
        pthread_join (p2,NULL);
	pthread_join (p3,NULL);
        pthread_join (p4,NULL);
        pthread_join (p5,NULL);
        pthread_join (p6,NULL);
        pthread_join (p7,NULL);
        pthread_join (p8,NULL);
        pthread_join (p9,NULL);
	//crear join de demas hilos


	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	 
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
        sem_destroy(&sem_salar);
        sem_destroy(&sem_agregarCarne);
        sem_destroy(&sem_empanar);
        sem_destroy(&sem_cocinarMilanesa);
        sem_destroy(&sem_panListo);
        sem_destroy(&sem_milanesaLista);
        sem_destroy(&sem_extrasListo);
	//destruir demas semaforos 
	
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{
    
    FILE * salida = fopen("salida.txt", "w");// para borrar datos antiguos si es que existen
	fclose(salida);
	
	
	sem_init(&semHorno,0,2);
        //creo los nombres de los equipos, que son 4
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
        int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
        int *equipoNombre4 =malloc(sizeof(*equipoNombre4));


  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
        *equipoNombre3 = 3;
        *equipoNombre4 = 4;

	//creo las variables los hilos de los 4 equipos
	pthread_t equipo1; 
	pthread_t equipo2;
        pthread_t equipo3;
        pthread_t equipo4;

  
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);
   
     rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3);
   
     rc = pthread_create(&equipo4,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre4);
  //faltn inicializaciones


   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
     pthread_join (equipo1,NULL);
     pthread_join (equipo2,NULL);
     pthread_join (equipo3,NULL);
     pthread_join (equipo4,NULL);
 

    sem_destroy(&semHorno);

    pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
//Para ejecutar:   ./ejecutable
