# TP-SOR-semaforos

UNIVERSIDAD NACIONAL DE GENERAL SARMIENTO


------------

**SISTEMAS OPERATIVOS Y REDES**

****

**Alumna: Cueva Melina Alejandra**

**Primer Semestre 2021**

**Profesores: Mariano Vargas, Noelia Sosa; Ignacio Tula**
## 
Ejercicio Subway Argento

En la cocina de Subway Argento compiten cuatro equipos de cocineros para ganarse el puesto de trabajo en el local.
¿Quién será el primero en entregar 1 sándwich de milanesa terminado, de 15 cm, al dueño del local?
La dificultad de la competencia es que la cocina no cuenta con el equipamiento individual para cada equipo, sino que algunos de ellos son compartidos.
Se tienen que ordenar para cocinar las milanesas ya que solo tienen un sartén para todos los equipos y este cocina una milanesa a la vez, existe solo un horno que cocina dos panes a la vez y un solo salero.
¿Cómo se arma un sándwich de milanesa?
Para armar un sándwich necesito cortar 2 dientes de ajo y un puñado de perejil, para luego mezclarlos con 1 huevo.
Después utilizar sal a gusto, agregar la carne y luego empanar las milanesas.
Una vez que tengo todo armado se cocinan por 5’ en el sartén bien caliente.
Como corresponde una buena milanesa tiene panes recién horneados (por 10’ aproximadamente) junto a lechuga fresca, tomate, cebolla morada y pepino recién cortados.
Finalmente puedo armar el sándwich con todos los ingrediente

Realicé este trabajo comenzando por mirar el video que subió Noelia, y a partir de él, hice un pseudocódigo que me permitió tener una guía para implementar luego en el código de mi proyecto.
Entre las dificultades que se me presentaron: tuve problemas con la lectura y escritura de los archivos txt, que solucione mirando unos videos sobre programar en C.
PSEUDOCÓDIGO que plantie

//semaforo global
*sem_ semHorno // un semaforo global para el horno que puede hornear de a 2 panes
//mutex
mutex salero // el salero lo puede utilizar un equipo a la vez
mutex sarten // la sarten la puede utilizar un equipo a la vez
*

acciones:
- *cortar() // ajo y perejil
- mezclar() // con huevo
- agregarCarne() // agregar la carne a la mezcla
- empanar() // empanar la milanesa
- cocinarMilanesa()// cocinar la milanesa- usa la sarten
- hornear() // hornear el pan- usa el horno
- cortarExtras() // cortar cebolla morada, pepino, lechuga, tomate
- armarSanguche() //armar el sanguche con la milanesa cocinar, el pan horneado y las verduras cortadas*``

//semaforos
- *sem mezclar;
- sem salar;
- sem agregarCarne;
- sem empanar;
- sem cocinarMilanesa;
- sem panListo;
- sem milanesaLista;
- sem extrasListo;*

como funcionan las funciones con los semáforos
p= wait(-)
v= signal(+)

`Cortar()
V(S_mezclar)

P(S_mezclar)
Mezclar()
V(S_salar)

P(Sem_salar)
P(Mutex_salero)
Salar()
V(Mutex_salero)
V(Sem_agregarcarne)

P(Sem_agregarcarne)
Agregarcarne()
V(Sem_empanar)

P(Sem_empanar)
Empanar()
V(Sem_cocinarmilanesa)

P(Sem_cocinarmilanesa)
P(Mutex_sarten)
Cocinarmilanesa()
V(Mutex_sarten)
V(Sem_panlisto)

P(Semhorno)
Hornear()
V(Semhorno)
V(Semhorno)

Cortarextras()
V(Sem_extraslisto)

P(Sem_panlisto)
P(Sem_milanesalista)
P(Sem_extraslisto)
Armarsanguche()`

Para armar la estructura de mi programa tuve en cuenta el código base que pasaron, y a partir de los ejemplos que teníamos, cree las funciones, coloque los semáforos correspondientes para cada acción y genere un archivo .txt para la receta.

Para leer la receta, realice el siguiente código
```C

*FILE * archivo = fopen(“receta.txt”, “r”); // abre la receta
bool k=true;
int m=0;
int j=0;

 char aux[100];
 while(feof(archivo) == 0){ 
        fscanf(archivo, "%s", aux); //con fscanf agarramos una linea hasta \n y la insertamos en la variable aux
    for (char *s = strtok(aux, "|"); s; s = strtok(NULL, "|")){ 
         if(k==true){ 
        strcpy(pthread_data->pasos_param[m].accion, s); //la agregamos
             k=false; 
         }else{
           strcpy(pthread_data->pasos_param[m].ingredientes[j], s);
                j++;// incrementamos j para seguir con el otro ingrediente
          }
    }
    m++; // incremento i para para seguir a la siguiente accion
    j=0; // j vuelve a cero
    k=true; // p vuelve a True
    }
    fclose(archivo);*
    
```
La cual levanta la receta.txt y la lee linea por linea. En el archivo receta. txt podemos encontrar por línea la acción que le corresponde a la función que la llama, relacionada con los ingredientes

Para la salida por pantalla

```C

FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino existe escribe al final del archivo
fputs("Equipo ", salida); //fputs permite escribir cadena de texto
fprintf(salida, "%d", mydata->equipo_param);
fputs(" - Es el ganador! ", salida);
fclose(salida);
exit(1); 
     pthread_exit(NULL);
   
```





         
