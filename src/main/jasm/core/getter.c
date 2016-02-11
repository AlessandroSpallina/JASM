/****************************************************************************
* Copyright © 2015 Alessandro Spallina
* email: alessandrospallina1@gmail.com
* github: https://github.com/AlessandroSpallina
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/utsname.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

#include "getter.h"
#include "miscellaneous.h"
#include "logger.h"

char error[BUFSIZ];

//TODO: checks + errno

void getVersion (int fd);
void getCopyright (int fd);
void getHostname (int fd);
void getKernelName (int fd);
void getKernelRelease (int fd);
void getKernelVersion (int fd);
void getMachine (int fd);
void getGetter (int fd);
void getCpuName (int fd);
void getCacheSize (int fd);
void getCoreNum (int fd);
void getCoreSpeeds (int fd);
void getAddressSizes (int fd);
void getCreatedProcNum (int fd);
void getIfSwap (int fd);
void getFileHandlesNum (int fd);

char getterName[NGETTER][BUFSIZ] = {"Version", "Copyright", "Hostname", "KernelName",
                                    "KernelRelease", "KernelVersion", "Machine",
                                    "CpuName", "CacheSize", "CoreNum", "CoreSpeeds",
                                    "AddressSizes", "CreatedProcNum", "IfSwap", "FileHandlesNum"
                                   };

void (*getterFunction[NGETTER]) (int) = {getVersion, getCopyright, getHostname,
                                         getKernelName, getKernelRelease, getKernelVersion, getMachine,
                                         getCpuName, getCacheSize, getCoreNum, getCoreSpeeds, getAddressSizes,
                                         getCreatedProcNum, getIfSwap, getFileHandlesNum
                                        };

/*
 *  scrive su fd int numero getter e N stringhe nomiGetter
 */
void getGetter (int fd)
{
    int i;
    int n_getter = NGETTER;
    int count = 0;
    //start from an error condition, to make sure we do not miss anything
    ssize_t ret_val = -1;

    ret_val = write (fd, &n_getter, sizeof (n_getter) );

    if (ret_val == 0 || ret_val == -1)
    {
        log_error ("[JASM-DAEMON][getGetter][write()] Error! ret_val is 0 or -1");
        log_error (error);
    }

    for (i = 0; i < NGETTER; i++)
    {
        count = strlen (getterName[i]);
        ret_val = write (fd, &count, sizeof (count) );

        if (ret_val == 0 || ret_val == -1)
        {
            log_error ("[JASM-DAEMON][getGetter][write()] Error! ret_val is 0 or -1");
            log_error (error);
        }
        ret_val = write (fd, getterName[i], count);

        if (ret_val == 0 || ret_val == -1)
        {
            log_error ("[JASM-DAEMON][getGetter][write()] Error! ret_val is 0 or -1");
            log_error (error);
        }
    }
}

void getVersion (int fd)
{
    int n;

    n = write (fd, VERSION, strlen (VERSION) );
    if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getVersion][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (VERSION) )
        {
            sprintf (error, "[JASM-DAEMON][getVersion][write()] sent %d byte, correct num byte is %zu", n, strlen (VERSION) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getVersion][write()] sent %d byte", n);
            log_string (error);
        }
    }
}

void getCopyright (int fd)
{
    int n;

    n = write (fd, COPYRIGHT, strlen (COPYRIGHT) );
    if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCopyright][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (COPYRIGHT) )
        {
            sprintf (error, "[JASM-DAEMON][getCopyright][write()] sent %d byte, correct num byte is %zu", n, strlen (COPYRIGHT) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getCopyright][write()] sent %d byte", n);
            log_string (error);
        }
    }
}

void getHostname (int fd)
{
    struct utsname info;
    char buf[BUFSIZ];

    if (uname (&info) == -1)
    {
        log_error ("getHostname() [uname] failed");
        return;
    }
    else
    {
        strcpy (buf, info.nodename);
        int n = write (fd, buf, strlen (buf) );
        if (n < 0)
        {
            sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
            log_error ("[JASM-DAEMON][getHostname][write()] Error!");
            log_error (error);
        }
        else
        {
            if (n < strlen (buf) )
            {
                sprintf (error, "[JASM-DAEMON][getHostname][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                log_error (error);
            }
            else
            {
                sprintf (error, "[JASM-DAEMON][getHostname][write()] sent %d byte", n);
                log_string (error);
            }
        }
    }
}

void getKernelName (int fd)
{
    struct utsname info;
    char buf[BUFSIZ];

    if (uname (&info) == -1)
    {
        log_error ("getKernelName() [uname] failed");
        return;
    }
    else
    {
        strcpy (buf, info.sysname);
        int n = write (fd, buf, strlen (buf) );
        if (n < 0)
        {
            sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
            log_error ("[JASM-DAEMON][getKernelName][write()] Error!");
            log_error (error);
        }
        else
        {
            if (n < strlen (buf) )
            {
                sprintf (error, "[JASM-DAEMON][getKernelName][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                log_error (error);
            }
            else
            {
                sprintf (error, "[JASM-DAEMON][getKernelName][write()] sent %d byte", n);
                log_string (error);
            }
        }
    }
}

void getKernelRelease (int fd)
{
    struct utsname info;
    char buf[BUFSIZ];

    if (uname (&info) == -1)
    {
        log_error ("getKernelRelease() [uname] failed");
        return;
    }
    else
    {
        strcpy (buf, info.release);
        int n = write (fd, buf, strlen (buf) );
        if (n < 0)
        {
            sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
            log_error ("[JASM-DAEMON][getKernelRelease][write()] Error!");
            log_error (error);
        }
        else
        {
            if (n < strlen (buf) )
            {
                sprintf (error, "[JASM-DAEMON][getKernelRelease][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                log_error (error);
            }
            else
            {
                sprintf (error, "[JASM-DAEMON][getKernelRelease][write()] sent %d byte", n);
                log_string (error);
            }
        }
    }
}

void getKernelVersion (int fd)
{
    struct utsname info;
    char buf[BUFSIZ];

    if (uname (&info) == -1)
    {
        log_error ("getKernelVersion() [uname] failed");
        return;
    }
    else
    {
        strcpy (buf, info.version);
        int n = write (fd, buf, strlen (buf) );
        if (n < 0)
        {
            sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
            log_error ("[JASM-DAEMON][getKernelVersion][write()] Error!");
            log_error (error);
        }
        else
        {
            if (n < strlen (buf) )
            {
                sprintf (error, "[JASM-DAEMON][getKernelVersion][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                log_error (error);
            }
            else
            {
                sprintf (error, "[JASM-DAEMON][getKernelVersion][write()] sent %d byte", n);
                log_string (error);
            }
        }
    }
}

void getMachine (int fd)
{
    struct utsname info;
    char buf[BUFSIZ];

    if (uname (&info) == -1)
    {
        log_error ("getMachine() [uname]  failed");
        return;
    }
    else
    {
        strcpy (buf, info.machine);
        int n = write (fd, buf, strlen (buf) );
        if (n < 0)
        {
            sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
            log_error ("[JASM-DAEMON][getMachine][write()] Error!");
            log_error (error);
        }
        else
        {
            if (n < strlen (buf) )
            {
                sprintf (error, "[JASM-DAEMON][getMachine][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                log_error (error);
            }
            else
            {
                sprintf (error, "[JASM-DAEMON][getMachine][write()] sent %d byte", n);
                log_string (error);
            }
        }
    }
}

void getCpuName (int fd) //Returns the name of the Cpu
{
		int cpu_fd;
		char buf[BUFSIZ];
		char *string;
		int i = 0;
		int n;
		
		cpu_fd = open("/proc/cpuinfo", O_RDONLY);
		if(read(cpu_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCpuName][read()] Error!");
        log_error (error);
		}
		string = &strstr(buf, "model name	: ")[strlen("model name	: ")];
		while(string[i] != '\n') ++i; //conta i caratteri della descrizione della cpu
		sprintf(buf, "%.*s", i, string); //ora buf contiene il modello del processore
		n = write (fd, buf, strlen(buf));
		close(cpu_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCpuName][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getCpuName][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getCpuName][write()] sent %d byte", n);
            log_string (error);
        }
    }
}

void getCacheSize (int fd) //Returns the cache size of the cpu
{
		int cpu_fd;
		char buf[BUFSIZ];
		char *string;
		int i = 0;
		int n;
		
		cpu_fd = open("/proc/cpuinfo", O_RDONLY);
		if(read(cpu_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCacheSize][read()] Error!");
        log_error (error);
		}
		string = &strstr(buf, "cache size	: ")[strlen("cache size	: ")];
		while(string[i] != '\n') ++i; //conta i caratteri della descrizione della cpu
		sprintf(buf, "%.*s", i, string); //ora buf contiene il modello del processore
		n = write (fd, buf, strlen(buf));
		close(cpu_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCacheSize][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getCacheSize][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getCacheSize][write()] sent %d byte", n);
            log_string (error);
        }
    }
}

void getCoreNum (int fd) //Returns the number of physical cores
{
		int cpu_fd;
		char info[20] = "cpu cores	: ";
		char buf[BUFSIZ];
		char *string;
		int i = 0;
		int n;
	
		cpu_fd = open("/proc/cpuinfo", O_RDONLY);
		if(read(cpu_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCoreNum][read()] Error!");
        log_error (error);
		}
		string = &strstr(buf, info)[strlen(info)];
		while(string[i] != '\n') ++i; //conta i caratteri
		sprintf(buf, "%.*s", i, string); //ora buf contiene il numero di core
		n = write (fd, buf, strlen(buf));
		close(cpu_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCoreNum][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getCoreNum][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getCoreNum][write()] sent %d byte", n);
            log_string (error);
        }
    }
}



void getCoreSpeeds (int fd) //Returns the core speeds
{
	int cpu_fd;
	char info[20] = "cpu MHz		: ";
	char buf[CPU_FILE_SIZE];
	char final_string[BUFSIZ];
	char speeds[MAX_CORE][CPU_FILE_SIZE];
	char *tmp;
	int n;
	int i;
	int j = 0;

	cpu_fd = open("/proc/cpuinfo", O_RDONLY);
	read(cpu_fd, buf, CPU_FILE_SIZE); //read the entire file
	tmp = &strstr(buf, info)[strlen(info)]; //tmp holds the buffer starting from the first speed's point
	while(1){
		i = 0;
		while(tmp[i] != '\n') ++i;
		sprintf(speeds[j], "%.*s", i, tmp); 
		if(strstr(tmp, info) == NULL) break; //if it's NULL, it was the last core, and it can exit from loop
		else{
			tmp = &strstr(tmp, info)[strlen(info)];
		}
		++j;
	}
	
	/*This code is just for formatting*/
	for (i = 0; i < j+1; i += 1)
	{
			sprintf(final_string + strlen(final_string), "%d: %s Mhz", i, speeds[i]); 
			if(i != j){
				sprintf(final_string + strlen(final_string), " | ");
			}
	}
	
	close(cpu_fd);
	n = write (fd, final_string, strlen(final_string));
	
	if (n < 0)
  	{
  	     sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
  	     log_error ("[JASM-DAEMON][getCoreSpeeds][write()] Error!");
  	     log_error (error);
    }
    else
    {
       if (n < strlen (buf) )
       {
          sprintf (error, "[JASM-DAEMON][getCoreSpeeds][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
          log_error (error);
       }
       else
       {
          sprintf (error, "[JASM-DAEMON][getCoreSpeeds][write()] sent %d byte", n);
          log_string (error);
       }
   }
}

void getAddressSizes (int fd) //Returns cpu address size
{
		int cpu_fd;
		char info[20] = "address sizes	: ";
		char buf[BUFSIZ];
		char *string;
		int i = 0;
		int n;
	
		cpu_fd = open("/proc/cpuinfo", O_RDONLY);
		if(read(cpu_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getAddressSizes][read()] Error!");
        log_error (error);
		}
		string = &strstr(buf, info)[strlen(info)];
		while(string[i] != '\n') ++i;
		sprintf(buf, "%.*s", i, string);
		n = write (fd, buf, strlen(buf));
		close(cpu_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getAddressSizes][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getAddressSizes][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getAddressSizes][write()] sent %d byte", n);
            log_string (error);
        }
    }
}

void getCreatedProcNum (int fd) //Returns the number of created processes since the start
{
		int proc_fd;
		char info[20] = "processes ";
		char buf[BUFSIZ];
		char *string;
		int i = 0;
		int n;
	
		proc_fd = open("/proc/stat", O_RDONLY);
		if(read(proc_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCreatedProcNum][read()] Error!");
        log_error (error);
		}
		string = &strstr(buf, info)[strlen(info)];
		while(string[i] != '\n') ++i;
		sprintf(buf, "%.*s", i, string);
		n = write (fd, buf, strlen(buf));
		close(proc_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCreatedProcNum][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getCreatedProcNum][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getCreatedProcNum][write()] sent %d byte", n);
            log_string (error);
        }
    }
}

void getIfSwap (int fd) //Check for the existence of swap partitions
{
		int swaps_fd;
		char buf[BUFSIZ];
		char result[4];
		int i = 0;
		int n;
	
		swaps_fd = open("/proc/swaps", O_RDONLY);
		if(read(swaps_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getIfSwap][read()] Error!");
        log_error (error);
		}
		while(buf[i] != '\n') ++i;
		if(buf[i+1] == EOF){
			strcpy(result, "no"); //if the first line is the only line of the file
		}
		else{
			strcpy(result, "yes");
		}
		
		n = write (fd, result, strlen(result));
		close(swaps_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getIfSwap][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getIfSwap][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getIfSwap][write()] sent %d byte", n);
            log_string (error);
        }
    }
}

void getFileHandlesNum (int fd) //Returns opened file handles (and file descriptors) number
{
		int file_fd;
		char buf[BUFSIZ];
		char result[10];
		int i = 0;
		int n;
	
		file_fd = open("/proc/sys/fs/file-nr", O_RDONLY);
		if(read(file_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getFileHandlesNum][read()] Error!");
        log_error (error);
		}
		while(buf[i] != '\t') ++i;
		sprintf(result, "%.*s", i, buf);
		
		n = write (fd, result, strlen(result));
		close(file_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getFileHandlesNum][write()] Error!");
        log_error (error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getFileHandlesNum][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            log_error (error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getFileHandlesNum][write()] sent %d byte", n);
            log_string (error);
        }
    }
}


