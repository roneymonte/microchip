/* *******************************************************************************
 * Comunicacion Serial Mediante Frecuencia con Modulos RF ASK
 *
 * Este es un ejemplo de una comunicacion inalambrica con modulos ASK 315/433MHZ
 * de bajo costo. Se muestra un ejemplo sencillo de protocolo de comunicacion
 * que incluye direccionamiento, sincronizacion y comprobacion de errores para
 * una comunicacion mas confiable con estos modulos. Este ejemplo es meramente
 * didactico y contiene solamente la funcionalidad basica para demostracion.
 *
 * Hardware TX:    PIC16F628A, Transmisor RF 315/433MHz ASK generico
 * Hardware RX:    PIC16F876A, Receptor RF 315/433MHz ASK generico
 * Funciones:   Software ejemplo para la implementacion de un protocolo (TX/RX)
 * Compilador:  Microchip Hi-Tech PICC v9.83
 * Version:     0.2 (14 de Noviembre de 2013)
 * Autor:       Raul Alvarez Torrico (www.TecBolivia.com)
 *
 * Descargado de www.TecBolivia.com - Ingresa al sitio para más ejemplos similares.
**********************************************************************************/

#include <htc.h>    // Inclusion obligatoria para el compilador PICC
#include "sci.h"    // Libreria serial
#include "delay.h"  // Libreria de retardos
#include "buffer_circular.h"    // Libreria para el buffer circular de recepcion

/* Palabra de configuracion para el PIC16F876A Para versiones desde
   la Version 9.81 en adelante */
__CONFIG(LVP_OFF & BOREN_OFF & PWRTE_ON & WDTE_OFF & FOSC_XT);

/* Definicion de constantes */
#define _XTAL_FREQ      4000000    // Necesario para las funciones __delay_xx

// La tasa maxima de transmision de estos modulos es de 2400 y hasta 4800 en
// algunos casos, sin embargo, a mayor tasa de transmision el rango para una
// comunicacion confiable sin errores disminuye. Para lograr mayores distancias
// usar tasas menores o iguales a 1200 baudios */
#define BAUD_RATE       1200        // Tasa de transmision

// Definicion de bytes para el protocolo
#define BYTE_SINC       0xAA    // Byte de sincronia
#define BYTE_INICIO     0x7E    // Byte delimitador de inicio
#define BYTE_DIR_RX     0x11    // Dirección asignada al nodo receptor
#define TAM_TRAMA       4       // Cantidad de bytes en una trama

// Definicion de entradas/salidas
#define LED_ACTIVIDAD       RB5     // LED indicador de recepcion
#define TRIS_LED_ACTIVIDAD  TRISB5  // Direccion del LED indicador
#define LED_ACTUADOR        RB0     // LED que indica la activacion de un actuador
#define TRIS_LED_ACTUADOR   TRISB0  // Direccion del LED indicador del actuador

/* Declaracion de funciones */
void Recibir();    //Funcion de recepcion serial
void Parpadear( int veces, int periodo);    // Funcion para parpadear un LED

/* Funcion principal del programa */
void main(void)
{
    /* Inicializar puertos digitales */
    TRIS_LED_ACTIVIDAD = 0; // Como salida
    LED_ACTIVIDAD = 0;      // Limpiar puerto
    TRIS_LED_ACTUADOR = 0;  // Como salida
    LED_ACTUADOR = 0;       // Limpiar puerto

    /*Inicializar comunicacion serial */
    sci_Init(BAUD_RATE,SCI_EIGHT);     // Inicializar UART del Pic (BAUD_RATE,N,1)

    /* Inicializar interrupciones para recepcion serial. La libreria del buffer
       serial para recepcion las utiliza */
    PIR1 = 0x00;    // Limpiar interrupciones pendientes
    PEIE = 1;       // Habilitar interrupciones de perifericos
    GIE  = 1;       // Habilitar interrupcion global
    RCIE = 1;       // Habilitar interrupcion de recepcion serial
    
    /* Parpadeo de inicio */
    Parpadear(2, 1000);
    
    while (1)   // Ciclo infinito
    {     

        Recibir();       // Funcion de recepcion
    }
}    // Fin main()

/*******************************************************************************
*                     RECIBIR
*
* Descripción   :    Recibe una trama completa del protocolo. Esta funcion ira
*                    leyendo uno a uno los bytes del buffer circular hasta
*                    encontrar un delimitador de inicio para luego procesar toda
*                    la trama que sigue. Los bytes de sincronia son ignorados.
* Argumentos    :    Ninguno
* Retorna       :    Nada
* Notas         :
********************************************************************************/
void Recibir()
{
    // Variables para almacenamiento de bytes de la trama
    static unsigned char dir, dato;

    // Ejecutar solo si se recibieron la cantidad minima de caracteres por trama
    if (serialAvailable() >= TAM_TRAMA)    
    {
        // Buscar un byte de inicio, si se lo encuentra procesar los bytes
        // siguientes (son parte de la trama)
        if (serialRead() == BYTE_INICIO)
        {
            dir = serialRead(); // Leer el byte de direccion
            // Verificar que la trama este dirigida a nosotros
            if (dir == BYTE_DIR_RX)  
            {
                dato = serialRead();    // Leer el byte de datos
                if (serialRead() == (dir + dato))  // Verificar el checksum
                {
                    /* En este punto podemos estar seguros que la informacion no
                       contiene errores */
                    // Cambiar estado LED de actividad para visualizacion externa
                    LED_ACTIVIDAD = ~LED_ACTIVIDAD; 
                    
                    /* Aplicar el bit correspondiente recibido en los datos
                    para activar/desactivar el actuador.
                    En este ejemplo solo estamos usando el bit RB0 pero se
                    podrian usar mas bits e incluso mas puertos */
                    LED_ACTUADOR = dato & 0b00000001;   // Mascarar el bit 0
                }
            }
        }
    }
}

/*******************************************************************************
*                     PARPADEAR
*
* Descripción   :    Parpadea un LED
* Argumentos    :    int veces: cantidad de parpadeos
*                    int periodo: periodo del parpadeo
* Retorna       :    Nada
* Notas         :
********************************************************************************/
void Parpadear( int veces, int periodo) {
    for (int i = 0; i < veces; i++) {
        LED_ACTIVIDAD = 1;
        DelayBigMs(periodo >> 2);
        LED_ACTIVIDAD = 0;
        DelayBigMs(periodo >> 2);
    }
}
