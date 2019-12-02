#include "md5.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define READ_DATA_SIZE	1024
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)

// function declare
int Compute_string_md5(unsigned char *dest_str, unsigned int dest_len, char *md5_str);
int Compute_file_md5(const char *file_path, char *md5_str);
char* scanFiles(char* fileList);


int main() {
    

    char fileList[] = "/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1.txt "
               "/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1copy.txt "
               "/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1copy2.txt";
    char fileList2[] = "/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1.txt "
                "/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1copy.txt "
                "/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1copy2.txt";
    char fileList3[] = "/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1.txt "
                "/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1copy.txt "
                "/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1copy2.txt";


    scanFiles(fileList);

    FILE *f1;
    f1 = fopen("/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1.txt", "w");
    fputs("extra",f1);
    fclose(f1);
    scanFiles(fileList2);

    FILE *f2;
    f2 = fopen("/home/gagnonma/merkle-hash-tree(testing)/projectFiles/file1.txt", "w");
    fputs("File 1",f2);
    fclose(f2);
    scanFiles(fileList3);
}
/**
 * compute the value of a string
 * @param  dest_str
 * @param  dest_len
 * @param  md5_str
 */
int Compute_string_md5(unsigned char *dest_str, unsigned int dest_len, char *md5_str)
{
    int i;
    unsigned char md5_value[MD5_SIZE];
    MD5_CTX md5;

    // init md5
    MD5Init(&md5);

    MD5Update(&md5, dest_str, dest_len);

    MD5Final(&md5, md5_value);

    // convert md5 value to md5 string
    for(i = 0; i < MD5_SIZE; i++)
    {
        snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
    }

    return 0;
}

/**
 * compute the value of a file
 * @param  file_path
 * @param  md5_str
 * @return 0: ok, -1: fail
 */
int Compute_file_md5(const char *file_path, char *md5_str)
{
    int i;
    int fd;
    int ret;
    unsigned char data[READ_DATA_SIZE];
    unsigned char md5_value[MD5_SIZE];
    MD5_CTX md5;

    fd = open(file_path, O_RDONLY);
    if (-1 == fd)
    {
        perror("open");
        return -1;
    }

    // init md5
    MD5Init(&md5);

    while (1)
    {
        ret = read(fd, data, READ_DATA_SIZE);
        if (-1 == ret)
        {
            perror("read");
            close(fd);
            return -1;
        }

        MD5Update(&md5, data, ret);

        if (0 == ret || ret < READ_DATA_SIZE)
        {
            break;
        }
    }

    close(fd);

    MD5Final(&md5, md5_value);

    // convert md5 value to md5 string
    for(i = 0; i < MD5_SIZE; i++)
    {
        snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);
    }

    return 0;
}


char* scanFiles(char fileList[]){

    char hash[MD5_STR_LEN + 1];
    char topHash[MD5_STR_LEN + 1];
    int numOfFiles = 0;

    char* fileName = strtok(fileList, " ");

    while(fileName){
        Compute_file_md5(fileName,hash);
        if(numOfFiles == 0){
            strcpy(topHash,hash);
        }else{
            const char* mixed = strcat(topHash,hash);
            Compute_string_md5((unsigned char *)mixed,strlen(mixed),topHash);
        }
        numOfFiles++;
        fileName = strtok(NULL," ");
    }

    char answer[33];
    strcpy(answer,topHash);
    printf("Num of files is %d",numOfFiles);
    printf("\ntopHash is %s\n",topHash);
    return answer;
}
