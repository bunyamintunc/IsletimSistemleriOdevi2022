#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <string.h> // correct header
#include <strings.h> // incorrect header - change this in your code to string.h
#include <sys/mman.h>
#include <sys/mman.h>

void* create_shared_memory(size_t size) {
  int protection = PROT_READ | PROT_WRITE;

  int visibility = MAP_SHARED | MAP_ANONYMOUS;

  
  return mmap(NULL, size, protection, visibility, -1, 0);
}
 
 
pthread_t t_id[4];

#define BUFFER_SIZE 152
#define READ_END 0
#define WRITE_END 1
char sireliMetin[152]="";
void *runner(char str[],int threadId) {
    int i = 0;
    int shift = 1;
    while (str[i] != '\0') {
        str[i]= toupper(str[i]);
        if (str[i] >= 'A' && str[i]<= 'Z') {
            char c = str[i] - 'A';
            c += shift;
            c = c % 26;
            str[i] = c + 'A';
        }
        i++;
    }
    printf(" sifreli metin --> %s\n", str);
    strcat(sireliMetin,str);
    return (void*) str;
}




void main()
{


    char write_msg[BUFFER_SIZE] = "Lorem ipsum dolora sit amet consectetur adipisicing elita Possimus laboriosam beatae explicaboa ameta vero saepe illo nihil dolor numquam aliquam rem eveniet harum enim voluptatibusa Blanditiis vero sequi dicta porroa";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    void* shmem = create_shared_memory(152);
    pid_t pid;

    if(pipe(fd) == -1){
        fprintf(stderr,"pipe acilamadi");
    }

    pid = fork();

    if (pid == -1)
    {
        printf("Fork gerçekleþtirilemedi...\n");
        exit(1);
    }
    if(pid == 0)
    {
        printf("I am child...\n");
        close(fd[WRITE_END]);
        read(fd[READ_END],read_msg,BUFFER_SIZE);
        printf("child proses pipten okudu: %s \n",read_msg);

        close(fd[READ_END]);
        int i,j,a=0,uzunluk;
        char bolum1[38];
        char bolum2[38];
        char bolum3[38];
        char bolum4[38];

        uzunluk = strlen(read_msg)/4;



        for(j=0;j<4;j++){
            for(i=0;i<38;i++){
                if(j==0){
                    bolum1[i]= read_msg[i];
                }
                if(j==1){
                    bolum2[i]=read_msg[i+a];
                }
                if(j==2){
                    bolum3[i]=read_msg[i+a];
                }else{
                    bolum4[i] = read_msg[i+a];
                }

            }
            a+=uzunluk;
        }
        char *dene;
        pthread_create(&t_id[0],NULL,runner,(void*)bolum1);
        //pthread_join(t_id[0],NULL);
       
        pthread_create(&t_id[1],NULL,runner,(void*)bolum2);
        //pthread_join(t_id[1],NULL);
        pthread_create(&t_id[2],NULL,runner,(void*)bolum3);
        //pthread_join(t_id[2],NULL);
        pthread_create(&t_id[3],NULL,runner,(void*)bolum4);
        //pthread_join(t_id[3],NULL);
        

        for(i=0;i<4;i++){
        pthread_join(t_id[i],NULL);
        }

        memcpy(shmem, sireliMetin, sizeof(sireliMetin));
        printf("child proses sonlandý.. \n");

    }
    else
    {
        //parent proses
        printf("I am parent...\n");
        close(fd[READ_END]);
        write(fd[WRITE_END],write_msg, strlen(write_msg)+1);
        printf("parent pipe yazdý \n");

        close(fd[WRITE_END]);
        wait(NULL);
        
        printf("Parent read: %s\n", shmem);
        printf("parent proses sonlandi \n");
    }
    

}
