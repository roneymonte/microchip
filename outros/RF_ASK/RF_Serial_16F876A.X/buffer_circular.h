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
#ifndef BUFFER_CIRCULAR_H
#define BUFFER_CIRCULAR_H

// Definir aqui el tamano para el buffer circular, mejor si son en factores
// de 2^n. Ej: 8, 16, 32, 64, etc
#define RX_BUFFER_SIZE 32    

unsigned char serialRead();         // Retorna 1 caracter del buffer de recepción
unsigned char serialAvailable();    // Retorna la cantidad de caracteres disponibles en el buffer de recepción
void serialFlush();                 // Borra el buffer de recepción

#endif
