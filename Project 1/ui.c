// Project done by Anurag And Naveen Team 29


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UART.h"
#include "stm32l476xx.h"

#include "SysClock.h"
#include "Tim.h"            // The header file for initialising timer and gpio

uint8_t buffer[100];

//display strings

char s1[] = "enter the lower limit value within range of 50 to 9950\r\n";
char s2[] = "The upper limit value will be";
char instr[] = "Do you want to play with the default lower and upper values of 950 and 1050. Press Y if yes and N if no. \r\n";
char endinstr[] = "\r\nThe output is given above. Do you want to play again? Press Y if you want to play\r\n";

char st[25],st2[25];





int main(){
	uint8_t temp;                                   //This reads the usart input of limit value
	int k=0;
	char rxByte;                                    //To take simple input from usart
    int a[101]=0;                                   //The bucket array
	int n,l=0,u=0,j=0;                              //l,u are the respective lower and upper limit variable values
	int i=0,x=0,y=0,x1=0,temp1=0;                   // The temporary variables used in the program
	int count=0;                                    // Initialising the counter used in the program
	
    
    // Initialising the system clock, UART, GPIO and Timer for input capture 
    System_Clock_Init();                            
	UART2_Init();                                   
	gpio_initialise();                                  
	timer_initialize();
	
	
	
	// The main app functioning
	if(posttester()){                                               //check the POST Test on the program
		do{
			USART_Write(USART2,(uint8_t *)instr,strlen(instr));     //Dsipalying first line of the program asking user to set or use upper and lower values 
			rxByte = USART_Read(USART2);
			if(rxByte == 'N' || rxByte == 'n'){                     //To alter the lower limit value
				do{
				USART_Write(USART2,(uint8_t *)s1, strlen(s1));      //prompt to enter lower limit value
				while(1){
						
						temp= USART_Read(USART2);                    // byte-wise read the lower limit value (one digit at a time)
						
						if(temp==0xD){
							st[k]=0;
							break;
						}
					
						st[k]=(char) temp;
						rxByte=st[k];
						USART_Write(USART2,&st[k],1);                  // Display the digits being written
						k++;
						
						
					}
					
					l=atoi(st);                                         // coonverting the character array to integer
					k=0;
					
				}while((l<50 || l>9950));                                // checking if the value is in the range
				
				n=sprintf((char *)buffer, "The lower limit value is : %d\r\n", l);
				USART_Write(USART2,buffer,n);
				
				k=0;
				
				n=sprintf((char *)buffer, "The upper limit value is : %d\r\n", l+100);
				USART_Write(USART2,buffer,n);
					
			
				u=l+100;
				 
				
					
			}
			else{                                           // default values for lower and upper limit
				l = 950;
				u = 1050;
			}
			count=0;
			while(count<1000){                          // Running the timer to get 1000 values
				
                // Taking the difference between timer counter values at each rising edge such that we get the time period
				while(!(TIM2->SR&2));
					x=TIM2->CCR1;
				while(!(TIM2->SR&2));
					x1=TIM2->CCR1;
				temp1=x1-x;
				
				
				if(temp1<u && temp1>l){                     //checking if the difference is within the limits provided and incrementing the count accordingly
					a[temp1-l]+=1;
					
				}
				count+=1;
				
				
				
			
		}
				count=0;
				n = sprintf((char *)buffer, "\r\n Histogram \r\n Bucket(Time Period in uS)   -     Count", i);
				USART_Write(USART2, buffer,n);
                j=0;
        
				while(j<100)                                 // checking for number of counts in the bucket array
		{
					if((a[j])){
						
						count++;
						n = sprintf((char *)buffer, "\r\n      %d                   -       %d ", l+j,a[j]);
                        a[j]=0;
						USART_Write(USART2,buffer,n);
					}
					j++;
				}                                              // if no value exists in the range, then get to the ending
				if(count==0){
					n=sprintf((char *)buffer, "\r\nNo values lie within the given range of yours. Do you want ot play again? Press Y if you want to play again.\r\n");
					USART_Write(USART2,buffer,n);
					rxByte=USART_Read(USART2);
				}
				
				else{                                           // prompt to ask if you want to play again
					USART_Write(USART2,(uint8_t *)endinstr,strlen(endinstr));
					rxByte = USART_Read(USART2);
				}
			}while(rxByte=='y' || rxByte == 'Y');
            //// End the program 
            n=sprintf((char *)buffer, "Terminating the program\r\n");
			USART_Write(USART2,buffer,n);
		}
	}

	


int posttester(){                                       

	int x=0,i=0,n,z=0;                          //temp. variables
	char firstByte;
	z:                                          // goto instance
	while(i<100000){                             // run loop for 100 ms
		i++;
	}
	
	x=TIM2->CNT;                                 // check the timke counter of TIMER 2
	
	z=TIM2->SR;                                           
	
	while((TIM2->CNT-x)<100000);                  // wait for 100 ms 
	
	if((TIM2->SR&2)){                             // check for rising edge. If we get that then go inside and print the below command
		n=sprintf((char *)buffer, "POST test is successful. Now we can go to play mode\r\n");
		USART_Write(USART2,buffer,n);
		return 1;
	}
	else{                                           // if no edge detected then failed and will ask the user to conduct the test again or else end the program        
		n=sprintf((char *)buffer, "POST test has failed. do you want to try again or go, Press Y to try again or N to end. \r\n");
		USART_Write(USART2,buffer,n);
		firstByte= USART_Read(USART2);
		if(firstByte=='y' || firstByte == 'Y'){
			goto z;
		}
		else if(firstByte == 'n' || firstByte == 'N'){              // End the program on no result
			n=sprintf((char *)buffer, "Terminating the program\r\n");
			USART_Write(USART2,buffer,n);
			return 0;
		}
		
	}
	return 1;
}


	