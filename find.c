#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int contains(char *options,char op)
{
    int i=0;
    while(options[i]!='\0')
    {
        if(options[i]==op)
            return 1;
        i++;
    }
  	return 0;
}


int checkDirectory(char *fileName){
	printf("%s\n", fileName);
	struct stat info;
	int rv = lstat(fileName,&info);
	if(rv==-1){
		perror("lstat function failed");
		exit(1);
	}
	int mode=info.st_mode;
	if ((mode &  0170000) == 0040000) 
	 	return 1;
    return 0;
}


void findByType(char * path, char *fileType){
	struct dirent *entry;
	DIR *dp = opendir(path);
	int count=0;

	if(dp==NULL){
		fprintf(stderr, "Cannot open directory:%s\n",path);
		return;
	} 
	printf("%s\n",path );
	while((entry = readdir(dp)) != NULL){
	   	if(entry == NULL && errno != 0){
	        perror("readdir failed");
	        exit(1);
	    } 
	   			if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)
	   				continue;
	   			
	   			int i=0,j=0;
	   			char temp[100]={'\0'};

	   			if(path[0]=='/') { 
	    			
	    			while(path[i]!='\0'){
	    				temp[i]=path[i];
	    				i++;
	    			}
	    			if(temp[i-1]!='/'){
	    				temp[i]='/';	    			
	    				i++;
					}	    				
	    		}
	    		while(entry->d_name[j]!='\0'){
	    			temp[i]=entry->d_name[j];
	    			i++;
	    			j++;
	    		}

	   			struct stat info;
				int rv = lstat(temp,&info);
				if(rv==-1){
					continue;
				}
				int mode=info.st_mode;

				char getType;
				if ((mode &  0170000) == 0010000) 
	 				getType='p';
    			else if ((mode &  0170000) == 0020000) 
	 				getType='c';
    			else if ((mode &  0170000) == 0040000) 
	 				getType='d';
    			else if ((mode &  0170000) == 0060000) 
					getType='b';
    			else if ((mode &  0170000) == 0100000) 
					getType='f';
    			else if ((mode &  0170000) == 0120000) 
					getType='l';
    			else if ((mode &  0170000) == 0140000) 
					getType='s';
    			else 
					getType='0';


				if(fileType[0]==getType){
					printf("%s\n", temp);
				}


				if ((mode &  0170000) == 0040000){
					int k=0,l=0;
		   			char check[10000]="\0";
		   			while(path[k]!='\0'){
		   				check[k]=path[k];
		   				k++;
		   			}
		   			check[k]='/';
		   			k++;
		   			while(entry->d_name[l]!='\0'){
	    				check[k]=entry->d_name[l];
	    				k++;l++;
	    			}
		   			findByType(check,fileType);
	   			}
	}
}




void findByName(char * path, char * fileName){
	struct dirent *entry;
	DIR *dp = opendir(path);
	int count=0;

	if(dp==NULL){
		fprintf(stderr, "Cannot open directory:%s\n",path);
		return;
	} 
	while((entry = readdir(dp)) != NULL){
	   	if(entry == NULL && errno != 0){
	        perror("readdir failed");
	        exit(1);
	    } else {
	       	
	       	if(strcmp(entry->d_name,fileName)==0){
	       		printf("%s/%s\n", path,fileName);

	   		}
	   			if(entry->d_name[0]=='.')
	   				continue;
	   			
	   			int i=0,j=0;
	   			char temp[100]="\0";
	   			if(path[0]=='/') { 
	    			
	    			while(path[i]!='\0'){
	    				temp[i]=path[i];
	    				i++;
	    			}
	    			if(temp[i-1]!='/'){
	    				temp[i]='/';	    			
	    				i++;
					}	    				
	    		}
	    		while(entry->d_name[j]!='\0'){
	    			temp[i]=entry->d_name[j];
	    			i++;
	    			j++;
	    		}

	   			struct stat info;
				int rv = lstat(temp,&info);
				if(rv==-1){
					continue;
				}
				int result=0;
				int mode=info.st_mode;

				if ((mode &  0170000) == 0040000){
					int k=0,l=0;
		   			char check[10000]="\0";
		   			while(path[k]!='\0'){
		   				check[k]=path[k];
		   				k++;
		   			}
		   			check[k]='/';
		   			k++;
		   			while(entry->d_name[l]!='\0'){
	    				check[k]=entry->d_name[l];
	    				k++;l++;
	    			}
		   			findByName(check,fileName);
	   			}  		
	    }
	}
}



int main(int argc, char *argv[]){
	
	if(argc<4){
		printf("Invalid number of arguments. Please Enter a correct number of arguments\n");
	} 
	else {
	
		if(contains(argv[2],'-') && contains(argv[2],'n') && contains(argv[2],'a') && contains(argv[2],'m') && contains(argv[2],'e') )
		{
			findByName(argv[1],argv[3]);
		}
		if(contains(argv[2],'-') && contains(argv[2],'t') && contains(argv[2],'y') && contains(argv[2],'p') && contains(argv[2],'e') )
		{
			findByType(argv[1],argv[3]);
		}
		//else 
			//printf("please check your command again\n");
		//printf("else\n");
	}



	return 0;
}