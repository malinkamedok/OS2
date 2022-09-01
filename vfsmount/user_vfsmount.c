#include <stdio.h>
 
int main() {
   FILE *fvfs;
   FILE *fargs;
   int input = 0;
   char *line = NULL;
   size_t len = 0;

   fargs = fopen("/sys/kernel/debug/lab2os/args", "w");

   printf("Enter FD: \n");
   scanf("%d", &input);
   char inbuf[256];
      sprintf(inbuf, "%d", input);
      fwrite(&inbuf, 1, sizeof(inbuf), fargs);
   fclose(fargs);

   printf("VFSMOUNT structure:\n");

   fvfs = fopen("/sys/kernel/debug/lab2os/vfs", "r");
      
   while (getline(&line, &len, fvfs) != -1) {
      printf("%s", line);
   }  

   fclose(fvfs);
   
       return 0;
}