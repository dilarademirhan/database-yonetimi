#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

//veri dosyalarindaki satirlar
typedef struct{
    char* number;
    char* ad;
} Satir;

int main()
{
    //program.c'deki verileri almak icin namedpipe'lar
    int npipefd;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    
    while (1)
    {
        const char *is[1000];
        npipefd = open(myfifo,O_RDONLY);
        read(npipefd, is, 1000);
        close(npipefd);
        const char *query[1000];
        query[0] = strtok(is, " ");
        for(int i = 1; i < 1000; i++){
          query[i] = strtok (NULL, " ");
        }
       
        int isInputFalse = 0; // kullanıcının yazdığı sorgu doğru mu
        if((!strcmp(query[0], "select")) && (!strcmp(query[1], "*") || !strcmp(query[1], "number") || !strcmp(query[1], "ad")) && (!strcmp(query[3], "veri1.txt") || !strcmp(query[3], "veri2.txt")) 
        && (!strcmp(query[4], "where"))) {
        	char *sonkisim[1000]; 
		sonkisim[0] = strtok(query[5], "=");
		for(int i = 1; i < 1000; i++){
		  sonkisim[i] = strtok (NULL, "=");
		}
		if(!strcmp(sonkisim[0], "ad") || !strcmp(sonkisim[0], "number")){
			FILE *dosya = fopen(query[3], "r");
			char satir[255];
			char *k;
			
			
			char gonderilecek[100];
			gonderilecek[0]='\0';
			Satir s;
			
			while(fgets(satir, 255, dosya)) {  // her satırı tek tek structa al
			    k = strtok (satir," ");
			    for (int j = 0; k != NULL; j++) {
				if(j == 0){
				    s.ad = k;
				}else if(j == 1){
				    s.number = k;
				}
				k = strtok(NULL, " ");
			    }
			    
			    
			    if(!strcmp(query[1], "*")){
				    if(!strcmp(sonkisim[0], "ad")){
				    	if(!strcmp(sonkisim[1], s.ad)){
	     					strcat(gonderilecek, s.ad);
                    				strcat(gonderilecek, " ");
                    				strcat(gonderilecek, s.number);
				    	}
				   } else if(!strcmp(sonkisim[0], "number")){
				   	if(atoi(sonkisim[1]) == atoi(s.number)){
	     					strcat(gonderilecek, s.ad);
                    				strcat(gonderilecek, " ");
                    				strcat(gonderilecek, s.number);
				    	}
				   }
		      	    } else if(!strcmp(query[1], "ad")){
				    if(!strcmp(sonkisim[0], "ad")){
				    	if(!strcmp(sonkisim[1], s.ad)){
				    		strcat(gonderilecek, s.ad);
				    		strcat(gonderilecek, " ");
				        }
				   } else if(!strcmp(sonkisim[0], "number")){
				   	if(atoi(sonkisim[1]) == atoi(s.number)){
					    	strcat(gonderilecek, s.ad);
					    	strcat(gonderilecek, " ");
				    	}
				   }
		      	    } else if(!strcmp(query[1], "number")){
				    if(!strcmp(sonkisim[0], "ad")){
				    	if(!strcmp(sonkisim[1], s.ad)){
				    		strcat(gonderilecek, s.number);
				    		strcat(gonderilecek, " ");
				    	}
				   } else if(!strcmp(sonkisim[0],"number")){
				   	if(atoi(sonkisim[1]) == atoi(s.number)){
				    		strcat(gonderilecek, s.number);
				    		strcat(gonderilecek, " ");
				    	}
				   }
		      	    }
		      }
		      if(!strcmp(gonderilecek, "\0")){
		      	strcat(gonderilecek, "null");
		      }
		      npipefd = open(myfifo, O_WRONLY);
	     	      write(npipefd, gonderilecek, 100);
	     	      close(npipefd);
		      
	 }else{
	 	isInputFalse = 1;
	 }
	
	} else {
        	isInputFalse = 1;
        }
        	
        if(isInputFalse){
        	npipefd = open(myfifo, O_WRONLY);
	     	write(npipefd, "null", strlen("null")+1);
	     	close(npipefd);
        }
        	
        
        printf("\n");
        close(npipefd);
    }

	return 0;
}
