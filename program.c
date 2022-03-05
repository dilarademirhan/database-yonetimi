#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>


int main()
{
    int npipefd;
    char * myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);
    char cevap;
    char *is[1000];
    char *sonuc[1000]; 
    //char *sonuc = (char*)calloc(1000, sizeof(char));
    
     while(1) {
             // sorgu al    	
 	     printf("\nsorgu: ");
	     gets(is);
	     npipefd = open(myfifo, O_WRONLY);
	     write(npipefd, is, strlen(is)+1);
	     close(npipefd);
	     
	     // girilen sorgunun sonucunu database programıyla 
	     // al ve sonuc stringine oku
	         
	     npipefd = open(myfifo, O_RDONLY);
             read(npipefd, sonuc, 1000);
             printf("%s \n", sonuc);
             close(npipefd);
             
             // sonuc null degilse kaydet programi ile kaydet
            if(strcmp(sonuc, "null")){
             	printf("sorgu sonucu kaydedilsin mi? e/h  ");
             	scanf("%c",&cevap);
            }
            if(cevap == 'e') {
             	int pipefd[2]; 
             	
				if (pipe(pipefd) < 0){
					perror("pipe");
					exit(1);
				}
				
				int pid = fork();
				int c;
					
				if (pid == 0) { 
					write(pipefd[1], sonuc, strlen(sonuc)+1);
					c = execv("kaydet", NULL);
					perror("");
					close(pipefd[1]);
				} else {
					wait(&c);
				} 
            	
            }
      } 
    
    return 0;
}
