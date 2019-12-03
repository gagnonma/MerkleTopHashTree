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
int power(int x, int y);


int main() {
    

    char fileList[] = "/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1.txt "
               "/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1copy.txt "
               "/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1copy2.txt";
    char fileList2[] = "/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1.txt "
                       "/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1copy.txt "
                       "/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1copy2.txt";;
    char fileList3[] = "/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1.txt "
                       "/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1copy.txt "
                       "/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1copy2.txt";;


    scanFiles(fileList);

    FILE *f1;
    f1 = fopen("/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1.txt", "w");
    fputs("extra",f1);
    fclose(f1);
    scanFiles(fileList2);

    FILE *f2;
    f2 = fopen("/home/gagnonma/CIS241projectfinalsolution/merkle-hash-tree/projectFiles/file1.txt", "w");
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

int power(int x, int y){
    int ans = 1;
    for (int i = 0; i < y; ++i) {
        ans*=x;
    }
    return ans;
}

char* scanFiles(char fileList[]){
    int bufferSize = strlen(fileList);
    char buffer[bufferSize];
    char* listCopy;
    listCopy = buffer;
    strcpy(listCopy,fileList);
    int n = 0;
    char* token = strtok(listCopy, " ");
    while(token){
        n++;
        token = strtok(NULL, " ");
    }
    //printf("num of files: %d",n);

    int h  = 1;
    int height = 1;
    while(n > h){
        h*=2;
        height++;
    }
    //printf("\n%d\n",height);
    int maxAmountNodes = power(2,height)-1;
    //printf("\n%d\n",maxAmountNodes);
    char tree[maxAmountNodes][33];
    char hash[MD5_STR_LEN + 1];
    char topHash[MD5_STR_LEN + 1];
    int numOfFiles = 0;

    for(int j =0; j < maxAmountNodes; j++){
        strcpy(tree[j],"");
    }
    /*for(int j =0; j < maxAmountNodes; j++){
        printf("tree[%d] = %s\n",j,tree[j]);
    }*/
    //populate tree
    char* fileName = strtok(fileList, " ");
    int i = 0;
    while(fileName){
        Compute_file_md5(fileName,hash);
        strcpy(tree[i],hash);
        //printf("%d : %s\n",i,tree[i]);
        fileName = strtok(NULL," ");
        i++;
    }
    /*for(int j =0; j < maxAmountNodes; j++){
        printf("tree[%d] = %s\n",j,tree[j]);
    }*/
    //combine hashes
    int index = 0;
    int levelStart = 0;
    int currHeight = height;
    int dest = power(2,height-1);
    while(index < maxAmountNodes){
        //combine index and index+1
        //at the last file but it doesnt fill the tree
        if(strcmp(tree[index+1],"") == 0){
            strcpy(tree[dest],tree[index]);
            int diff = levelStart+power(2,currHeight-1) - n;
            dest += diff;
            index = levelStart+power(2,currHeight-1);
            levelStart = index;
            currHeight--;
        }else {
            const char *mixed = strcat(tree[index], tree[index + 1]);
            Compute_string_md5((unsigned char *) mixed, strlen(mixed), tree[dest]);
            dest++;
            index += 2;
            if(index == levelStart+power(2,currHeight-1)){
                levelStart = index;
                currHeight--;
            }
        }
    }

    /*for(int j =0; j < maxAmountNodes; j++){
        printf("tree[%d] = %s\n",j,tree[j]);
    }*/
    printf("Num of files is %d",n);
    printf("\ntopHash is %s\n",tree[maxAmountNodes-1]);
    return tree[maxAmountNodes-1];
}
