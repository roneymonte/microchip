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

/* Palabra de configuracion para el PIC16F628A para versiones del compilador PICC desde
   la Version 9.81 en adelante */
__CONFIG(CPD_OFF & CP_OFF & LVP_OFF & BOREN_ON & MCLRE_ON & PWRTE_ON & WDTE_OFF & FOSC_INTOSCIO);

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
#define PIN_SENSOR          RB0     // Pin de entrada para sensor digital
#define TRIS_PIN_SENSOR     TRISB0  // Direccion del pin sensor
#define LED_ACTIVIDAD       RB5     // Pin de salida para LED de actividad
#define TRIS_LED_ACTIVIDAD  TRISB5  // Direccion del pin de salida para LED de actividad

/* Declaracion de funciones */
void Transmitir();    //Funcion de transmisión serial
void Parpadear( int veces, int periodo);    // Funcion para parpadear un LED
// Funcion para simular un oscilador 555 para prueba de los modulos TX y RX
// Usado solo para pruebas y depuracion:
void Oscilador_555(void);

/* Funcion principal del programa */
void main(void)
{
    /* Inicializar puertos digitales */
    TRIS_PIN_SENSOR = 1;    // Como entrada
    PIN_SENSOR = 0;         // Limpiar puerto
    TRIS_LED_ACTIVIDAD = 0; // Como salida
    LED_ACTIVIDAD = 0;      // Limpiar puerto

    /*Inicializar comunicacion serial */
    sci_Init(BAUD_RATE, SCI_EIGHT);     // Inicializar USART del PIC (BAUD_RATE,N,1)

    /* Parpadeo de inicio */
    Parpadear(2, 1000);

    while (1)       // Ciclo infinito
    {
        /* Con la ejecucion de la siguiente funcion las transmisiones se realizan
         de forma ininterrumpida a alta velocidad. Si alguna trama no es recibida
         correctamente, esta será reemplazada casi inmediatamente por otra
         siguiente */
        Transmitir();    // Funcion de transmision

        /* En caso de hacer transmisiones cortas con pausas largas se ha visto
         * que el receptor a veces no responde a la transmision de una sola
         * trama aislada debido posiblemente a las limitaciones de su diseño que
         * es bastante susceptible al ruido. En estos casos, hacer dos envios
         * consecutivos antes de un retardo largo generalmente soluciona el
         * problema. Habilitar las dos lineas siguientes adicionalmente para
         * hacer transmisiones cada 250 milisegundos: */
        Transmitir();
        __delay_ms(250);
    }
}    // Fin main()

/*******************************************************************************
*                     TRANSMITIR
*
* Descripción   :    Transmite una trama completa del protocolo
* Argumentos    :    Ninguno
* Retorna       :    Nada
* Notas         :
********************************************************************************/
void Transmitir()
{
    unsigned char dato_tx;  // Variable auxiliar para el byte a transmitirse

    /* Enviar la lectura del Puerto B (8 bits). En este ejemplo solo estamos
    usando el bit RB0 para activar remotamente un actuador pero se podrian
    usar mas bits e incluso mas puertos */
    dato_tx = PORTB;    

    /* Enviar los bytes de la trama, uno a uno, por el puerto serial */
    sci_PutByte(BYTE_SINC); // Enviar el byte de sincronia

    /* Si hay problemas en la recepcion, podria ayudar enviar algunos bytes de
     * sincronia adicionales. El codigo del receptor ignora cualquier byte
     * adicional de sincronia. Habilitar lo siguiente en caso de errores en la
     * recepcion:*/
//    sci_PutByte(BYTE_SINC);
//    sci_PutByte(BYTE_SINC);
//    sci_PutByte(BYTE_SINC);

    sci_PutByte(BYTE_INICIO);   // Enviar el byte delimitador de inicio
    sci_PutByte(BYTE_DIR_RX);   // Mandar la direccion del receptor
    sci_PutByte(dato_tx);       // Mandar el byte de dato
    sci_PutByte(BYTE_DIR_RX + dato_tx); // Calcular y mandar el checksum (direccion + datos)
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
void Parpadear( int veces, int periodo)
{
    for (int i = 0; i < veces; i++)
    {
        LED_ACTIVIDAD = 1;
        DelayBigMs(periodo >> 2);
        LED_ACTIVIDAD = 0;
        DelayBigMs(periodo >> 2);
    }
}

/*******************************************************************************
*                     OSCILADOR 555
*
* Descripción   :    Simula una oscilacion biestable con 555 para la prueba de
*                    basica del funcionamiento de los modulos ASK. Se puede
*                    alimentar la salida de LED_ACTIVIDAD a la entrada del TX
*                    para mandar una señal binaria oscilante de baja frecuencia.
*                    La salida del RX debe conectarse a un LED para visualizacion
* Argumentos    :    Ninguno
* Retorna       :    Nada
* Notas         :
********************************************************************************/
void Oscilador_555(void)
{
    #define TIME    400

    LED_ACTIVIDAD = 1;
    DelayBigMs(TIME);
    LED_ACTIVIDAD = 0;
    DelayBigMs(TIME);
}
