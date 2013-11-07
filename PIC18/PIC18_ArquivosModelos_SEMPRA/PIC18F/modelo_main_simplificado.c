//Biblioteca com os SFR's do PIC18F4550
#include <p18f4550.h>


//Função principal do firmware
void main (void)
{
	//Configurações do firmware
	ADCON1 = ADCON1 | 0x0f;		//Desabilita pinos analógicos
	TRISCbits.TRISC2 = 0;		//Configura pino RC2 (LED_VERDE) como saída digital	
	
	// Laço de repetição infinito que executa aplicação do firmware
	while(1)
	{
		LATCbits.LATC2 = 1;		//Liga o LED_VERDE conectado no pino RC2
	}
		
}//end main


/** V E C T O R   R E M A P P I N G ******************************************/
// ATENÇÃO: Copiar esta parte do código dentro do arquivo "main.c" dos
// projetos usados com o Bootloader USB-HID para gravação in-circuit.
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code REMAPPED_RESET_VECTOR = 0x1000
void _reset (void)
{
	_asm goto _startup _endasm
}
#pragma code
/** F I M  D A  S E Ç Ã O  D E   V E C T O R   R E M A P P I N G *************/
