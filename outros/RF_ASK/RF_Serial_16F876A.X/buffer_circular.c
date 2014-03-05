/* *******************************************************************************
 * Curso de Capacitación "Redes Inalámbricas Zigbee de Sensores con Módulos XBee"
 * Proyecto 2. Comunicación PC/Microcontrolador con XBee Coordinador y XBee
 * Ruteador en Modo Transparente.
 *
 * Recibe comandos de texto de la PC y transmite el estado de sus puertos a la
 * misma mediante un enlace serial con módulos XBee.
 *
 * Hace uso de las librerías  UART provistas por el compilador PICC de Hi-Tech.
 * Las funciones serialRead(); serial Available(); y serialFlush(); son
 * adaptaciones por el autor, de las librerías de la plataforma Arduino.
 *
 * Hardware:        PIC16F876A, Módulo XBee Serie 2
 * Características: Clock: 4MHz (cristal externo), Frecuencia de operación:
 * 2.4GHz (ZigBee), Data rate: 9600 bps
 * Funciones:       Software para control y monitoreo mediante red ZigBee con
 *                  módulos XBee.
 * Versión:         1.1 (21 de Octubre de 2013)
 * Autor:           Raul Alvarez Torrico (www.TecBolivia.com)
 * *******************************************************************************
 * Este código se distribuye exclusivamente a los alúmnos del curso "Redes Inalám-
 * bricas ZigBee de Sensores con Módulos XBee" ofrecido en www.TecBolivia.com
 * Este código es para uso personal de cada alúmno y QUEDA TERMINANTEMENTE PROHI-
 * BIDA LA DISTRIBUCIÓN DE LOS ARCHIVOS INCLUIDOS EN ESTE PROYECTO A TERCEROS.
 * Los alúmnos inscritos en el curso tiene permiso de usar, modificar e incluir
 * el código provisto en sus proyectos personales.
 *
 *********************************************************************************/
/* Inclusión de librerías usadas en el programa */
#include <htc.h>        //Librería principal del compilador PICC
#include "buffer_circular.h"    // Libreria para buffer circular

/* Variables globales */
static unsigned char bufferRx[RX_BUFFER_SIZE];  // Buffer de recepción serial
unsigned char rx_buffer_head =0; //Indice "cabeza" para el buffer (escritura)
unsigned char rx_buffer_tail =0; //Indice "cola" para el buffer (lectura)

/*******************************************************************************
*                   FUNCION DE LECTURA DESDE EL BUFFER SERIAL
*
* Descripción   :   Permite la lectura de 1 caracter desde el buffer de
*                   recepción serial.
*
* Argumentos    :   Ninguno.
*
* Retorna       :   unsigned char c    Un caracter del buffer a la vez.
* Notas         :
*******************************************************************************/
unsigned char serialRead(void)
{
    unsigned char c;

    // si la cabeza no está delante de la cola, no tenemos ningún caracter
    if (rx_buffer_head == rx_buffer_tail) {
        return -1;
    } else {
        c = bufferRx[rx_buffer_tail];
        rx_buffer_tail = rx_buffer_tail + 1;
        rx_buffer_tail %= RX_BUFFER_SIZE;
        return c;
    }
}

/*******************************************************************************
*                   FUNCION DE VERIFICACION DE EXISTENCIA DE CARACTERES
*
* Descripción   :   Permite la verificación de la existencia de al menos 1
*                   caracter disponible en el buffer de recepción serial.
*
* Argumentos    :   Ninguno.
*
* Retorna       :   unsigned char i    El número de caracteres disponibles en el buffer.
* Notas         :
*******************************************************************************/
unsigned char serialAvailable(void)
{
    unsigned char i;

    i = RX_BUFFER_SIZE + rx_buffer_head - rx_buffer_tail;
    i %= RX_BUFFER_SIZE;

    return i;
}

/*******************************************************************************
*                   FUNCION DE VACIADO DEL BUFFER DE RECEPCION
*
* Descripción   :   Realiza el vaciado del buffer de recepción serial
*
* Argumentos    :   Ninguno.
*
* Retorna       :   Nada.
* Notas         :
*******************************************************************************/
void serialFlush(void)
{
    // no revertir el orden de esto o puede haber problemas si la interrupcion de RX
    // ocurre despues de leer el valor de rx_buffer_head pero antes de escribir
    // el valor a rx_buffer_tail; el valor previo de rx_buffer_head
    // puede ser escrito a rx_buffer_tail, haciéndolo aparecer como si el buffer
    // estuviera lleno, no vacío.
    rx_buffer_head = rx_buffer_tail;
}

/*******************************************************************************
*                   RUTINA DE INTERRUPCION
*
* Descripción   :   Rutina para la atención de las interrupciones en el PIC
*                   implementada según los requerimientos del compilador
*                   PICC de Hi-Tech
*
* Argumentos    :   Ninguno.
*
* Retorna       :   Nada
* Notas         :
*******************************************************************************/
void interrupt isr(void)    //Rutina de interrupcion de tx rx serial
{
    unsigned char i;

    if(RCIF)    //Si existe una interrupcion de rx serial,
    {
        i = rx_buffer_head + 1;
        i %= RX_BUFFER_SIZE;

        // si debieramos almacenar el caracter recibido en la localización
        // justo antes de la cola (lo cual significa que la cabeza debe avanzar
        // a la posición actual de la cola), estamos a punto de desbordar el buffer
        // por ello no escribimos el caracter ni avanzamos la cabeza.
        if (i != rx_buffer_tail) {
            bufferRx[rx_buffer_head] = RCREG;
            rx_buffer_head = i;
        }
    }
}
