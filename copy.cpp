#include<stdlib.h>
#include<dirent.h>
#include<string.h>
#include<stdio.h>
#include<sys/stat.h>
#include<iostream>

#define BUFFER_LENGTH 8192

int IsDir(std::string path)
{
	if(path.empty())
	{
		return 0;
	}
	struct stat st;
	if(0 != stat(path.c_str(),&st))
	{
		return 0;
	}
	if(S_ISDIR(st.st_mode))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



void CopyFile(std::string sourcePath,std::string destPath)
{
	int len = 0;
	FILE *pIn = NULL;
	FILE *pOut = NULL;
	char buff[BUFFER_LENGTH] = {0};
	
	if((pIn = fopen(sourcePath.c_str(),"r"))==NULL)
	{
		printf("Open File %s Failed...\n", sourcePath.c_str());
		exit(1);
	}
	if((pOut=fopen(destPath.c_str(),"w"))==NULL)
	{
		printf("Create Dest File Failed...\n");
		exit(1);
	}
	
	while((len = fread(buff,sizeof(char),sizeof(buff),pIn))>0)
	{
		fwrite(buff,sizeof(char),len,pOut);
	}
	fclose(pOut);
	fclose(pIn);
}
void CopyFolder(std::string sourcePath,std::string destPath)
{
	struct dirent* filename = NULL;
	if(!opendir(destPath.c_str()))
	{
		if (mkdir(destPath.c_str(),0777))
		{
		    printf("Create Dir Failed...\n");
		}
		else
		{
			printf("Creaat Dir %s Successed...\n", destPath.c_str());
		}
	}
	std::string path = sourcePath;
	if(sourcePath.back() != '/')
	{
		sourcePath += "/";
	}
	if(destPath.back() != '/')
	{
		destPath += "/";
	}
	
	DIR* dp=opendir(path.c_str());
	while(filename=readdir(dp))
	{
		std::string fileSourceFath = sourcePath;
		
		std::string fileDestPath = destPath;
		
		fileSourceFath += filename->d_name;
		fileDestPath += filename->d_name;
		if(IsDir(fileSourceFath.c_str()))
		{
			if(strncmp(filename->d_name, ".", 1) && strncmp(filename->d_name, "..", 2))
			{
				CopyFolder(fileSourceFath, fileDestPath);
			}		
		}
		else
		{
			CopyFile(fileSourceFath,fileDestPath);
			printf("Copy From %s To %s Successed\n",fileSourceFath.c_str(),fileDestPath.c_str());
		}
	}	
}

int main(int argc,char *argv[])
{
	if(argv[1]==NULL||argv[2]==NULL)
	{
		printf("Please Input Source Path and Destnation Path\n");
		exit(1);
	}
	std::string sourcePath=argv[1];//source path
	std::string destPath=argv[2];//destnation path
	DIR* source=opendir(sourcePath.c_str());
	DIR* destination=opendir(destPath.c_str());
	if(!source)
	{
		printf("Source Path Is Not Existed\n");
		return 1;
	}
	if(!destination)
	{
		printf("Destnation Path Is Not Existed\n");

	}
	CopyFolder(sourcePath,destPath);
	
	closedir(source);
	closedir(destination);
	return 0;
}
