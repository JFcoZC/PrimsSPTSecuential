//Librerias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

//Constante que representa infnito
const int INF = 9999;
int VERTICES;
int MAXCHAR = 300;		//Hasta 300 chars por linea EN ARCHIVO
int MAXSTR = 100;		//Hasta 100 lineas por archivo

//Funciones
//--------------------------------------------------------------
char **readFile(char* filename)
{
	int stringActual;
	FILE *fp;
	char str[MAXCHAR];		//Linea actual
	char* strsInFile[MAXSTR];		
	
	stringActual = 0;
	VERTICES = 0;
	
	//Abrir archivo para lectura
	fp = fopen(filename, "r");
	
	//No se encontro archivo
	if(fp == NULL)
	{
		printf("No se pudo abrir el archivo %s \n\n", filename);
	}//Fin if 1
	else
	{
		while(fgets(str, MAXCHAR, fp) != NULL)
		{
			//----------------------
			//Linea por linea que se va leyendo del archivo
			//printf("%s\n", str);
			//----------------------
			
			//***strsInFile[stringActual] = str;
			//1)Asignar la memoria suficiente para cada puntero (Cada linea)
			strsInFile[stringActual] = (char *) malloc(MAXCHAR);
			//2)Copiar el numero de bytes indicado por MAXCHAR de str a strsInFile[stringActual]
			//recordar que en c cada caracter ocupa un byte
			memcpy(strsInFile[stringActual],str, MAXCHAR);
		
			//--------------------------
			//printf("a) %s %x \n", str, &str);
			//printf("b) %s %x \n", strsInFile[stringActual], &strsInFile[stringActual]);
			//--------------------------
			
			stringActual++;
			VERTICES++;
		}//FIN WHILE
		
	}//Fin else 1
	
	fclose(fp);
	
	//Regresar arreglo de arreglos/punteros que apuntatan a otros punteros
	return strsInFile;
	
	//Deallocate all memory previously used by malloc
	free(strsInFile);
	for(int j = 0; j < MAXSTR; j++)
	{
		free(strsInFile[j]);
		strsInFile[j] = NULL;
		//printf("%s\n", strsInFile[j]);
	}//Fin for deallocate
	
	
}//FIn funcion readFile
//--------------------------------------------------------------
void writeFile(char* filename, char* content)
{
	FILE *fp;
	int error;
	
	//Open a text file for append stream to the end
	//If there is no file, it is created
	fp = fopen(filename,"a+");
	

	//Write content
	error = fputs(content,fp);
	if( error  == EOF )
	{
		printf("Error al escribir en archivo!\n");
		printf("ERROR: %i = %s\n", error ,strerror(errno));
		//clearerr(fp);
		//fflush(stdout);
		//Llamada recursiva para ver si ya se hace la escritura sin error
		//writeFile(filename,content);
		
	}//Fin if 1
	
	//Cerrar archivo
	fclose(fp);
	
}//Fin writeFile
//----------------------------------------------------
void printMatrix( int **matrix )
{
	for(int i = 0; i < VERTICES; i++)
	{
		
		printf("(%2i) ", i);
		
		for(int j = 0; j < VERTICES; j++)
		{
			printf("%4i ", matrix[i][j]);
			
		}//Fin for 2
		
		printf("\n");
		
	}//Fin for 1
	
	
}//Fin funcion printMatrix
//--------------------------------------------------------------
int** getGraph()
{
	char **filasArchivo;
	int filaActual;
	int colActual;
	char *filaActualFile;
	char* tokenizedstr;
	int actualNum;
	int** graph;

	//Guardar filas leidas del archivo en un arreglo de strings, donde cada fila es una string
	//100 lineas se guardan en 800 bits .:. 1 linea/fila ocupa 8 bits
	filasArchivo = (char**) malloc(800);
	memcpy(filasArchivo, readFile("datosMatriz.txt"), 800);
	
	printf("Numero de vertices en matriz de archivo: %i \n", VERTICES);
	//printf("Size of int: %i \n", sizeof(int));
	//int graph[VERTICES][VERTICES];	//Declarar despues de que se manda a llamara readFile()
	filaActual = 0;
	colActual = 0;
	
	//Asignar espacio de memoria para el numero de filas; 1 FILA OCUPA (FILAS+1)*(4=SizeOfInt)
	graph = (int **)malloc( (VERTICES+1)*sizeof(int));
	
	//Asignar el espacio para el numero de columnas que hay en cada fila	
	for(int k = 0; k  < VERTICES; k++)
	{
		graph[k] = (int*)malloc(VERTICES*sizeof(int));
		
	}//Fin for 0
	
	//Llenar la matriz con los numeros leidos del archivo
	for(int i = 0; i < VERTICES; i++)
	{
		
		//Fila de achivo actual
		filaActualFile = filasArchivo[i];
		//-------
		//printf("Fila actual: %s \n", filaActualFile);
		//-------
		
		//Tokenizacion de cada linea del archivo
		tokenizedstr = strtok(filaActualFile, " ");	//Primera llamada strok	
		actualNum = atoi(tokenizedstr);				//Convert str to int
		//-----
		//printf("Actual line number first %i \n", actualNum);
		//-----
		graph[i][0] = actualNum;
		
		for(int j = 1; j < VERTICES; j++)
		{
			//Tokenizacion de cada linea del archivo
			tokenizedstr = strtok(NULL, " ");	//Pasar NULL para continuar tokenizando a partir de la string pasada en la primera llamada
			actualNum = atoi(tokenizedstr);		//Convert str to int
			graph[i][j] = actualNum;
			
		}//Fin for 2
		
		
	}//Fin for 1
	
	//lIBERAR ESPACIO
	free(filasArchivo);
	filasArchivo = NULL;

	//---------------------
	//printMatrix(graph);
	//---------------------
	
	return graph;
	
	free(graph);
	graph = NULL;
	printf("despues\n");
	
}//Fin funcion get graph
//-------------------------------------------------------------
void primsMST()
{
	int isVinSetVt;						//1 = si /0 = no
	int elementsInSetVt;				//Num de Vertices en setVt
	int random;							//r = random root of tree
	int nextShortestVertex;	
	int setVt[VERTICES];				//Vt
	int weigthsEdgesActualV[VERTICES+20]; 	//d[1..VERTICES]
	int **Graph;
	char resultado[10];
	char strtottime[15];
	double starttime;
	double endtime;
	double totaltime;
	
	//-------------------------------------------------------
	for(int z = 0; z < 20; z++)
	{
		printf("%i ", weigthsEdgesActualV[z]);
		
	}//Fin for
	printf("\n");
	/*for(int y = 0; y < VERTICES; y++)
	{
		printf("%i ", setVt[y]);
		
	}//Fin for
	printf("\n");*/
	printf("-------------------sas----\n");
	//--------------------------------------------------------
	
	//Apuntar hacia direccion en memoria en HEAP donde se encuentra guardada la matriz de enteros
	Graph = getGraph();
	
	//------------------------------------------
	for(int z = 0; z < 20; z++)
	{
		printf("%i ", weigthsEdgesActualV[z]);
		
	}//Fin for
	printf("\n");
	/*for(int y = 0; y < VERTICES; y++)
	{
		printf("%i ", setVt[y]);
		
	}//Fin for
	printf("\n");*/
	printf("-------------------sas----\n");
	//------------------------------------------
	
	//Generar vertice de inicio de forma aleatoria [0 - VERTICES-1]
	srand(time(NULL));	//Seed to generate random numbers
	random = rand() % VERTICES;
	printf("Root vertex: %i \n\n", random);
	
	elementsInSetVt = 0;
	setVt[elementsInSetVt] = random; 
	elementsInSetVt++;
	
	for(int i = 0; i < VERTICES; i++)
	{
		weigthsEdgesActualV[i] = Graph[random][i];
		
	}//Fin for 1
	
	//+++++++++
	weigthsEdgesActualV[6] = 17;
	weigthsEdgesActualV[7] = 18;
	weigthsEdgesActualV[8] = 19;
	weigthsEdgesActualV[9] = 20;
	weigthsEdgesActualV[10] = 21;
	weigthsEdgesActualV[11]= 22;
	//+++++++++
	
	//d[r] = 0
	weigthsEdgesActualV[random] = 0;
	
	printMatrix(Graph);
	
	//----------------------------------------
	//printf("\n");
	//printf("vertices %i\n",VERTICES);
	//printf("random: %i\n",random);
	//printf("elementsInSetVt: %i\n",elementsInSetVt);
	for(int z = 0; z < 20; z++)
	{
		printf("%i ", weigthsEdgesActualV[z]);
		
	}//Fin for
	printf("\n");
	for(int y = 0; y < VERTICES; y++)
	{
		printf("%i ", setVt[y]);
		
	}//Fin for
	printf("\n");
	printf("-------------------sas2----\n");
	//--------------------------------------------
	
	starttime = omp_get_wtime();
	
	while(elementsInSetVt < VERTICES)
	{
		nextShortestVertex = -1;
		
		//---------------------------------------------------
		//Imrimir Pesos actuales de acuerdo al vertice actual
		printf("--++--\n");
		printf("Vertice actual #%i: %i \n", (elementsInSetVt-1) ,setVt[ (elementsInSetVt-1)]);
		//printf("Vertice REAL #%i: %i \n", 0 ,setVt[0]);
		printf("d[v]: ");
		for(int indx = 0; indx < VERTICES; indx++)
		{
			printf("%4i ", weigthsEdgesActualV[indx]);
			
		}//Fin for 0.0
		
		printf("\nVertices recorridos: ");
		for(int in = 0; in < VERTICES; in++) //in < (elementsInSetVt-1)
		{
			printf("%i ", setVt[in]);
			
		}//Fin for 0.1
		printf("\n");
		//---------------------------------------------------
		
		
		//Find next shortest EDGE to a vertex; the vertex can not be itself or a vertex that is already
		//in setVt;
		for(int j = 0; j < VERTICES; j++)
		{
			//Verificar que el vertice todavia no este en setVt
			for(int k = 0; k < elementsInSetVt; k++)
			{
				
				//-----------
				//printf("%i == %i ? OR %i == 9999 ?\n",setVt[k],j,weigthsEdgesActualV[j]);
				//----------
				if(  (weigthsEdgesActualV[j] == INF) )
				{
					//El vertice actual no tiene camino al vertice j
					//NO HACER NADA
					//-------------
					//printf("Vertice %i no tiene camino valido desde el vertice actual %i \n", j, setVt[ (elementsInSetVt-1)]);
					//--------------
					
				}//Fin fi 1
				else
				{
					//Verificar que el vertice con camino todavio no este en setVt, con cada uno de los
					//Posibles elmenetos que existen
					for(int index = 0; index < elementsInSetVt ; index++)
					{
						if(j == setVt[index] )
						{
							isVinSetVt = 1;
							index = elementsInSetVt; //Forzar salida del loop
						}//Fin if 3.1
						
					}//Fin for 3
					
					if(isVinSetVt == 0)
					{
						//ELEMENTO TODAVIA NO ESTA EN EL SETVT
						
						//Si todavia no se ha guardado ningun posible
						// next shortest vertex; guardar el primero que se encuentre
						if(nextShortestVertex == -1)
						{
							nextShortestVertex = j;
							//---------
							//printf("Posible siguiente vertice: %i \n",j);
							//----------
						}//Fin if 3
						else
						{
							//Si ya hay un next shortest vertex; comparar el actual con el 
							//que esta guardado y quedarse al mas grande
							if(weigthsEdgesActualV[j] < weigthsEdgesActualV[nextShortestVertex])
							{
								nextShortestVertex = j;
								//--------------
								//printf("Nuevo siguiente vertice: %i \n",j);
								//--------------
							}//Fin if 4
							
						}//Fin else 3
						
					}//Fin if 2
					else
					{
							//--------------
							//printf("Vertice %i ya esta en el setVt \n", j);
							//---------------
					}//Fin else 2
					
				} //Fin else 1
				
				isVinSetVt = 0;
				
			}//Fin for 2
			
		}//Fin for 1
		
		printf("Mejor siguiente vertice: %i \n", nextShortestVertex);
		//Guardar el nextShortesVertex en set de vertexes
		setVt[elementsInSetVt] = nextShortestVertex;
		//Incrementar numero de Vertex en el set setVt
		elementsInSetVt++;
				
		//Actualizar los valores de los pesos de acuerdo al nuevo vertice encontrado
		for(int l = 0; l < VERTICES; l++)
		{
			weigthsEdgesActualV[l] = Graph[nextShortestVertex][l]; 
					
		}//Fin for 3
		
	}//Fin while
	
	endtime = omp_get_wtime();
	totaltime = endtime - starttime;
	
	//RESUTADO
	printf("\nRESULTADO DEL MST: \n");
	for(int in = 0; in < VERTICES; in++)
	{
		printf("%i ", setVt[in]);
			
	}//Fin for resultado final
	printf("\n");
	
	//Convertir entero de numero de vertices a cadena
	//Convertir int to char itoa(value,string,base)
	itoa(VERTICES,resultado,10); //+1 para no usar 0 en calendario y llegar a ultimo dia
	
	//Convert double to string
	sprintf(strtottime,"%f",totaltime);
	
	//Append en archivo de resultados
	writeFile("resultados.txt",resultado);
	writeFile("resultados.txt"," ");
	writeFile("resultados.txt",strtottime);
	writeFile("resultados.txt","\n");
	
}//Fin funcion primsMST
//-------------------------------------------------------------

//Inicio del programa
int main()
{
	//Declaracion de varirables
	
	//Datos de entrada
	
	//Proceso
	primsMST();
	
	//Datos de salida
	
	//Fin del programa
	puts("Fin del programa v1");
	
	//Detener pantalla
	scanf("%");
	
	//Indicar al SO que todo termino OK
	return 0;
	
}//Fin del main