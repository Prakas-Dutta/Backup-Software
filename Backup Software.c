#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include<conio.h>
#include<windows.h>
int Time[3];

int createFolder(const char *exFol, const char *newFol)
{
	HANDLE hExistingFolder, hNewFolder;
    FILETIME ftCreate, ftAccess, ftWrite;

    // Open the existing folder
    hExistingFolder = CreateFile(exFol, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (hExistingFolder == INVALID_HANDLE_VALUE) {
        printf("Could not open existing folder (error %d)\n", GetLastError());
        return 1;
    }

    // Get the file times of the existing folder
    if (!GetFileTime(hExistingFolder, &ftCreate, &ftAccess, &ftWrite)) {
        printf("Could not get file times (error %d)\n", GetLastError());
        CloseHandle(hExistingFolder);
        return 1;
    }
    CloseHandle(hExistingFolder);
    
    // Open the new folder
    hNewFolder = CreateFile(newFol, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (hNewFolder == INVALID_HANDLE_VALUE) {
        printf("Could not open new folder (error %d)\n", GetLastError());
        return 1;
    }

    // Set the new folder's file times to match the existing folder
    if (!SetFileTime(hNewFolder, &ftCreate, &ftAccess, &ftWrite)) {
        printf("Could not set file times (error %d)\n", GetLastError());
        CloseHandle(hNewFolder);
        return 1;
    }

    CloseHandle(hNewFolder);
    return 0;
}


void backUp(const char *sourcePath, const char *destPath) {
    struct dirent *entry;
    DIR *dp = opendir(sourcePath);

    if (dp == NULL) {
        perror("opendir");
        return;
    }
    char temp1[200], temp2[200], temp3[200], command[410];
    
    while ((entry = readdir(dp))!=NULL) {
    	if(strcmp(entry->d_name, ".")  && strcmp(entry->d_name, ".."))
    	{
    		strcpy(temp3, destPath);
    		strcpy(temp2, sourcePath);
    		strcat(temp2, "\\");
    		strcat(temp2, entry->d_name);
    		if(is_directory(temp2))
    		{
    			strcat(temp3, "\\");
    			strcat(temp3, entry->d_name);
    			
    			
    			if(!folderExists(temp3))
    			mkdir(temp3);
    			backUp(temp2, temp3);
    			createFolder(temp2, temp3);
    			if(isDirectoryEmpty(temp3))
    			rmdir(temp3);
			}
    		else
    		{
				strcat(temp3, "\\");
    			strcpy(command, "");
    			strcat(command, "copy \"");
    			strcat(command, temp2);
    			strcat(command, "\" \"");
    			strcat(command, temp3);
    			strcat(command, "\"");
    			system(command);
			}
		}
    }
    closedir(dp);
}


int isDirectoryEmpty(const char *path) {
    struct dirent *entry;
    int isEmpty = 1; // Assume the directory is empty

    // Open the directory
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("Could not open directory: %s\n", path);
        return -1; // Error opening directory
    }

    // Read entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip the special entries "." and ".."
        if (entry->d_name[0] != '.') {
            isEmpty = 0; // Directory is not empty
            break;
        }
    }

    // Close the directory
    closedir(dir);

    return isEmpty;
}


int folderExists(const char *path) {
    struct stat stats;
    if (stat(path, &stats) == 0 && S_ISDIR(stats.st_mode)) {
        return 1;
    }
    return 0;
}

int checkEdited(const char *filePath)
{
	FILE *f=fopen("info.txt", "r");
	int i;
	char buf[100], b[50];
	for(i=0; i<5; i++)
	fgets(buf, sizeof(buf), f);
	fclose(f);
	buf[strlen(buf)-1]='\0';
    strncpy(b, buf+5, strlen(buf));
    struct stat fileStat1;
    if (stat(filePath, &fileStat1) == -1) {
        perror("stat");
        return 1;
    }

    // Get the last modified time
    time_t modifiedTime1 = fileStat1.st_mtime;
    struct tm *timeInfo1 = localtime(&modifiedTime1);

    // Format the time as YYYY-MM-DD HH:MM:SS
    char timeString1[80], timeString2[80];
    strftime(timeString1, sizeof(timeString1), "%d-%m-%Y", timeInfo1);
    
//    printf("%s %s", )
    
    return !strcmp(b, timeString1);
}


int is_directory(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

void copyFolder(const char *sourcePath, const char *destPath) {
    struct dirent *entry;
    DIR *dp = opendir(sourcePath);

    if (dp == NULL) {
        perror("opendir");
        return;
    }
    char temp1[300], temp2[300], temp3[300], command[610];
    
    while ((entry = readdir(dp))!=NULL) {
    	if(strcmp(entry->d_name, ".")  && strcmp(entry->d_name, ".."))
    	{
    		strcpy(temp3, destPath);
    		strcpy(temp2, sourcePath);
    		strcat(temp2, "\\");
    		strcat(temp2, entry->d_name);

//			if(checkEdited(temp2))
//    		{
	    		if(is_directory(temp2))
	    		{
	    			strcat(temp3, "\\");
	    			strcat(temp3, entry->d_name);
	    			
	    			
	    			if(!folderExists(temp3))
	    			mkdir(temp3);
	    			copyFolder(temp2, temp3);
	    			createFolder(temp2, temp3);
	    			if(isDirectoryEmpty(temp3))
	    			rmdir(temp3);
				}
	    		else
	    		{
	    			if(checkEdited(temp2))
	    			{
						strcat(temp3, "\\");
		    			strcpy(command, "");
		    			strcat(command, "copy \"");
		    			strcat(command, temp2);
		    			strcat(command, "\" \"");
		    			strcat(command, temp3);
		    			strcat(command, "\"");
		    			system(command);
					}
				}
//			}
		}
    }
    closedir(dp);
}


int fileExists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // File exists
    }
    return 0; // File does not exist
}

void checkTime()
{
	time_t t=time(NULL);
	struct tm *tm=localtime(&t);
	printf("Sleeping...");
	sleep((((Time[0]-tm->tm_hour)*3600)+((Time[1]-tm->tm_min)*60)+(Time[2]-tm->tm_sec)));
}
void Main()
{
	time_t t=time(NULL);
	struct tm *d=localtime(&t);
	int i, j, k;
	char folderName[11], temp[20], sourcePath[100], destPath[100], prevPath[100], tempPath[100];

	strcpy(temp, "");
	strcpy(folderName, "/CFR ");
	strcpy(sourcePath, "");
	strcpy(destPath, "");
	strcpy(prevPath, "");
	strcpy(tempPath, "");
	

	strftime(temp, sizeof(temp), "%d-%m-%Y", localtime(&t));
	
	strcat(folderName, temp);


	FILE *f=fopen("info.txt", "r");
	fgets(temp, sizeof(temp), f);
	
	
	fgets(prevPath, sizeof(prevPath), f);
	fgets(sourcePath, sizeof(sourcePath), f);
	
	prevPath[strlen(prevPath)-1]='\0';
	
	if(!strcmp(prevPath, ""))
	{
		fgets(destPath, sizeof(destPath), f);
		destPath[strlen(destPath)-1]='\0';
		sourcePath[strlen(sourcePath)-1]='\0';
		strcpy(tempPath, destPath);
//		strcat(tempPath, "\\");
//		folderName[strlen(folderName)-1]='\0';
		strcat(tempPath, folderName);
	}
	else
	{
		sourcePath[strlen(sourcePath)-1]='\0';
		strcpy(destPath, sourcePath);
		strcpy(tempPath, destPath);
//		strcat(destPath, "\\..");
//		folderName[strlen(folderName)-1]='\0';
		strcat(tempPath, folderName);
	}
	fclose(f);



	if(!strcmp(prevPath, ""))
	{
		if(!(folderExists(tempPath)))
		{
			rmdir(tempPath);
			mkdir(tempPath);
		}
		else
		mkdir(tempPath);
	}
	else
	mkdir(tempPath);
	system("color 4");
	if(!strcmp(prevPath, ""))
	{
		backUp(sourcePath, tempPath);
		if(isDirectoryEmpty(tempPath))
		rmdir(destPath);
		else
		{
			f=fopen("info.txt", "w");
			fputs(temp, f);
			fputs(sourcePath, f);
			fputs("\n", f);
			fputs(destPath, f);
			fputs("\n", f);
			fputs(folderName, f);
			fputs("\n", f);
			fclose(f);
		}
	}
	else
	{
		copyFolder(prevPath, tempPath);
		if(isDirectoryEmpty(tempPath))
		rmdir(tempPath);
		else
		{
			f=fopen("info.txt", "w");
			fputs(temp, f);
			fputs(prevPath, f);
			fputs("\n", f);
			fputs(destPath, f);
			fputs("\n", f);
			fputs(folderName, f);
			fputs("\n", f);
			fclose(f);
		}
	}
}
main()
{
//	printf("%d", checkEdited("C:\\Users\\PRAKAS\\OneDrive\\Desktop\\new\\New folder2\\New folder\\dj.txt"));
	char temp[20], tmp[3];
	int i, j, k;
	while(true)
	{
		if(!fileExists("info.txt"))
		{
			printf("There is no information about me,\nI could not get the required file!!!");
			getch();
			exit(1);
		}
		FILE *f=fopen("info.txt", "r");
		fgets(temp, sizeof(temp), f);
		fclose(f);
		if(!strcmp(temp, "\n"))
		{
			printf("Parameters are not set!!!\nRun the \"set parameter.exe\" file to set all parameters");
			getch();
			exit(1);
		}

		for(i=0, k=0;temp[i]!='\0';i++)
		{
			j=0;
			strcpy(tmp, "");
			while(temp[i]!=':' && temp[i]!=10)
			tmp[j++]=temp[i++];
			tmp[j]='\0';
			Time[k++]=atoi(tmp);
		}
		system("color 2");
		checkTime();
		system("cls");
		Main();
		sleep(1);
	}
}
