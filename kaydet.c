#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // programa gelen sonucu pipe'a oku.
    char s[1000];
    read(3, s, 100);
    
    // okunan sonucu dosyaya yaz.
    FILE *dosya = fopen("sonuc.txt","a");
    fprintf(dosya, "%s\n", s);
    fclose(dosya);
    close(3);
    return 0;
}
