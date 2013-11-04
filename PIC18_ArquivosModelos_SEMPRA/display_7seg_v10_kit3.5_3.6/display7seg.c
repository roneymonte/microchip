/******************************************************************************
 *
 *            Curso SEMPRA de Microcontroladores (PIC18F4550)
 *
 ******************************************************************************
 * Nome do Arquivo:     display7seg.c
 * Dependencias:    	Veja seção INCLUDES
 * Processador:       	PIC18F4550
 * Opção de Clock:		HS 20MHz (externo) - CPU: 48MHz	
 * Compilador:        	C18 v3.20 ou superior
 * Empresa:         	SEMPRA - Axoon - UFMG
 *
 * Plataforma:			E-P18 v3.5, v3.6
 *
 * Autor:				M.Sc. Henrique Resende Martins
 * 						Mauricio Ferrari Santos Corrêa
 *						Bruno Silveira Avelar
 *
 * Revisão:				v1 - 16/Fev/2009 - M.Sc. Henrique Resende Martins
 *						v2 - 27/Abr/2009 - Mauricio Ferrari Santos Corrêa
 *						v3 - 29/Out/2009 - Mauricio Ferrari Santos Corrêa
 *						v4 - 21/jan/2010 - Mauricio Ferrari Santos Corrêa
 *						v5 - 28/fev/2010 - Bruno Silveira Avelar
 *						v6 - 30/abr/2010 - Mauricio Ferrari Santos Corrêa
 *						v7 - 24/set/2010 - Mauricio Ferrari Santos Corrêa
 *						v8 - 16/nov/2010 - Bruno Silveira Avelar, Mauricio Ferrari Santos Corrêa
 *						v9 - 19/jan/2011 - Mauricio Ferrari Santos Corrêa
 *						v10- 04/Out/2011 - Mauricio Ferrari Santos Corrêa
 *
 * Descrição:        	Biblioteca de funções para escrever no display duplo
 *						de 7 segmentos da placa de desenvolvimento E-P18.
 *						O display duplo é do tipo ANODO COMUM e seus segmentos 
 *						são LIGADOS com nivel lógico ZERO.
 *						O display apresenta 2 dois digitos e seus respectivos
 *						pontos decimais.
 *						Nesta biblioteca usa-se os rótulos:
 *						D1 - digito mais significativo
 *						D2 - digito menos significativo
 *						p1 - ponto decimal do digito mais significativo
 *						p2 - ponto decimal do digito mais significativo
 *
 *						Nomes usados para os segmentos do display duplo presente
 *						na placa E-P18:

 * 
 *						D1: Digito mais			D2: Digito menos
 *					    significativo		    significativo
 *						  ---a1---  			 ---a2---  
 *						 |        |				|        |	
 *						 f1       b1			f2       b2
 *						 |        |				|        |
 *						  ---g1--- 				 ---g2--- 
 *						 |        |				|        |
 *						 e1       c1			e2       c2
 *						 |        |				|        |
 *						  ---d1---  .p1			 ---d2---  .p2
 *
 * 	 LEMBRANDO QUE O DISPLAY É DO TIPO ANODO COMUM e para cada segmento:
 *			0: LIGADO
 *			1: desligado
 *
 *****************************************************************************/


/** I N C L U D E S **********************************************************/
#include	"display7seg.h"


/** F U N C O E S ************************************************************/

/******************************************************************************
 * Funcao:        	void ConfiguraDisplay7seg (void)
 * Entrada:         Nenhuma (void)
 * Saída:          	Nenhuma (void)
 * Descrição:       Configura portas do PIC 18F4550 que estão conectadas ao 
 *					display duplo de 7 segmentos como saida. 
 *					Desliga todos os segmentos do display apos a configuração
 *
 *****************************************************************************/
void ConfiguraDisplay7seg (void)
{
	// Desabilita todas as 13 portas analógicas ANx (portas do conversor A/D),
	// assim as mesmas fucionam como pinos digitais
	ADCON1 |= 0x0F;

	// Configura os pinos conectadas no Dígito 1 (Mais Significativo) como pinos de Saída Digital
	TRIS_a1=0;
	TRIS_b1=0;
	TRIS_c1=0;
	TRIS_d1=0;
	TRIS_e1=0;
	TRIS_f1=0;
	TRIS_g1=0;
	TRIS_p1=0;

	// Configura os pinos conectadas do Dígito 2 (Menos Significativo) como pinos de Saída Digital
	TRIS_a2=0;
	TRIS_b2=0;
	TRIS_c2=0;
	TRIS_d2=0;
	TRIS_e2=0;
	TRIS_f2=0;
	TRIS_g2=0;
	TRIS_p2=0;

	// desliga o digito 1 (display é do tipo ANODO COMUM, logo "1" desliga o segmento)
	Display_a1=1;
    Display_b1=1;
    Display_c1=1;
    Display_d1=1;
    Display_e1=1;
    Display_f1=1;
    Display_g1=1;
   	Display_p1=1;
    
	// desliga o digito 2 (display é do tipo ANODO COMUM, logo "1" desliga o segmento)
    Display_a2=1;
    Display_b2=1;
    Display_c2=1;
    Display_d2=1;
    Display_e2=1;
    Display_f2=1;
    Display_g2=1;
    Display_p2=1;
}


/******************************************************************************
 * Funcao:		void EscreveDisplay7seg(unsigned char valor, unsigned char ponto)
 * 
 * Entrada:		unsigned char valor: valor decimal de 0 a 99 a ser apresentado
 *					no display de 7 segmentos. Se esse valor estiver acima
 *					de 99 será mostrado o simbolo de erro --.
 *
 *				unsigned char ponto: valor correspondente a configuração dos 
 *					pontos decimais. Use os #define para facilitar a configuração
 *						PONTO_NENHUM:	Desliga ambos pontos decimais.
 *						PONTO_ESQUERDA:	Liga ponto decimal da esquerda.
 *						PONTO_AMBOS:	Liga ambos pontos decimais.
 *					    PONTO_DIREITA:	Liga o ponto decimal da direita.
 * 
 * Saída:       Nenhuma (void)
 * Descrição:   Escreve valor decimal de 00 a 99 nos display's de 7 segmentos.
 *				Liga/desliga os pontos decimais.
 *
 *****************************************************************************/
void EscreveDisplay7seg(unsigned char valor, unsigned char ponto)
{
	// calcula o digito menos significativo através da função que retorna
	// o resto da divisão inteira por 10.
	unsigned char digito_unidade = valor%10;	
	
	// calcula o digito mais significativo através da função que retorna 
	// o quociente da divisão inteira por 10.											 
	unsigned char digito_dezena = valor/10;		
	
	// configura os pontos decimais
	switch (ponto)
	{
		case PONTO_NENHUM:  //Desliga ambos pontos decimais
			Display_p1=1;	//desliga ponto decimal de D1
			Display_p2=1;	//desliga ponto decimal de D2
		break;
		case PONTO_DIREITA: // Liga ponto decimal da direita
			Display_p1=1;	//desliga ponto decimal de D1
			Display_p2=0;	//liga ponto decimal de D2
		break;
		case PONTO_ESQUERDA://Liga ponto decimal da esquerda
			Display_p1=0;	//liga ponto decimal de D1
			Display_p2=1;	//desliga ponto decimal de D2
		break;
		case PONTO_AMBOS:	//Liga ponto decimal da esquerda
			Display_p1=0;	//liga ponto decimal de D1
			Display_p2=0;	//desliga ponto decimal de D2
		break;
		default: 			//Liga ambos pontos decimais 
			Display_p1=1;	//liga ponto decimal de D1
			Display_p2=1;	//liga ponto decimal de D2
		break;
	}
	
	// testa se valor está acima da faixa de 0 a 99, ou seja, utiliza mais
	// dois digitos. Se acima da faixa carrega valores inválidos para mostrar
	// simbolo de erro -- no display.
	if ( valor > (unsigned char)99)
	{
		// valores inválidos para mostrar -- no display
		digito_unidade = 20;
		digito_dezena  = 20;
	}
	
	// liga segmentos correspondentes a D1(mais significativo)
	// display é do tipo ANODO COMUM, logo "1" desliga o segmento
	switch(digito_dezena) 
	{ 
		case 0: 
			Display_a1=0;
		    Display_b1=0;
		    Display_c1=0;
		    Display_d1=0;
		    Display_e1=0;
		    Display_f1=0;
		    Display_g1=1;
		break; 
		
		case 1: 
			Display_a1=1;
		    Display_b1=0;
		    Display_c1=0;
		    Display_d1=1;
		    Display_e1=1;
		    Display_f1=1;
		    Display_g1=1;
		break; 

		case 2: 
			Display_a1=0;
		    Display_b1=0;
		    Display_c1=1;
		    Display_d1=0;
		    Display_e1=0;
		    Display_f1=1;
		    Display_g1=0;
		break; 

		case 3: 
			Display_a1=0;
		    Display_b1=0;
		    Display_c1=0;
		    Display_d1=0;
		    Display_e1=1;
		    Display_f1=1;
		    Display_g1=0;
		break; 
		
		case 4: 
			Display_a1=1;
		    Display_b1=0;
		    Display_c1=0;
		    Display_d1=1;
		    Display_e1=1;
		    Display_f1=0;
		    Display_g1=0;
		break; 

		case 5: 
			Display_a1=0;
		    Display_b1=1;
		    Display_c1=0;
		    Display_d1=0;
		    Display_e1=1;
		    Display_f1=0;
		    Display_g1=0;
		break; 

		case 6: 
			Display_a1=0;
		    Display_b1=1;
		    Display_c1=0;
		    Display_d1=0;
		    Display_e1=0;
		    Display_f1=0;
		    Display_g1=0;
		break; 

		case 7: 
			Display_a1=0;
		    Display_b1=0;
		    Display_c1=0;
		    Display_d1=1;
		    Display_e1=1;
		    Display_f1=1;
		    Display_g1=1;
		break; 
		
		case 8: 
			Display_a1=0;
		    Display_b1=0;
		    Display_c1=0;
		    Display_d1=0;
		    Display_e1=0;
		    Display_f1=0;
		    Display_g1=0;
		break; 

		case 9: 
			Display_a1=0;
		    Display_b1=0;
		    Display_c1=0;
		    Display_d1=1;
		    Display_e1=1;
		    Display_f1=0;
		    Display_g1=0;
		break;
		
		default:
			//simbolo -  
			Display_a1=1;
		    Display_b1=1;
		    Display_c1=1;
		    Display_d1=1;
		    Display_e1=1;
		    Display_f1=1;
		    Display_g1=0;
		break; 
	} 
	// liga segmentos correspondentes a D2(menos significativo)
	// display é do tipo ANODO COMUM, logo "1" desliga o segmento
	switch(digito_unidade) 
	{ 
		case 0: 
			Display_a2=0;
		    Display_b2=0;
		    Display_c2=0;
		    Display_d2=0;
		    Display_e2=0;
		    Display_f2=0;
		    Display_g2=1;
		break; 
		
		case 1: 
			Display_a2=1;
		    Display_b2=0;
		    Display_c2=0;
		    Display_d2=1;
		    Display_e2=1;
		    Display_f2=1;
		    Display_g2=1;
		break; 

		case 2: 
			Display_a2=0;
		    Display_b2=0;
		    Display_c2=1;
		    Display_d2=0;
		    Display_e2=0;
		    Display_f2=1;
		    Display_g2=0;
		break; 

		case 3: 
			Display_a2=0;
		    Display_b2=0;
		    Display_c2=0;
		    Display_d2=0;
		    Display_e2=1;
		    Display_f2=1;
		    Display_g2=0;
		break; 
		
		case 4: 
			Display_a2=1;
		    Display_b2=0;
		    Display_c2=0;
		    Display_d2=1;
		    Display_e2=1;
		    Display_f2=0;
		    Display_g2=0;
		break; 

		case 5: 
			Display_a2=0;
		    Display_b2=1;
		    Display_c2=0;
		    Display_d2=0;
		    Display_e2=1;
		    Display_f2=0;
		    Display_g2=0;
		break; 

		case 6: 
			Display_a2=0;
		    Display_b2=1;
		    Display_c2=0;
		    Display_d2=0;
		    Display_e2=0;
		    Display_f2=0;
		    Display_g2=0;
		break; 

		case 7: 
			Display_a2=0;
		    Display_b2=0;
		    Display_c2=0;
		    Display_d2=1;
		    Display_e2=1;
		    Display_f2=1;
		    Display_g2=1;
		break; 
		
		case 8: 
			Display_a2=0;
		    Display_b2=0;
		    Display_c2=0;
		    Display_d2=0;
		    Display_e2=0;
		    Display_f2=0;
		    Display_g2=0;
		break; 

		case 9: 
			Display_a2=0;
		    Display_b2=0;
		    Display_c2=0;
		    Display_d2=1;
		    Display_e2=1;
		    Display_f2=0;
		    Display_g2=0;
		break;
		
		default:
			//simbolo - 
			Display_a2=1;
		    Display_b2=1;
		    Display_c2=1;
		    Display_d2=1;
		    Display_e2=1;
		    Display_f2=1;
		    Display_g2=0;
		break; 
	} 


}

