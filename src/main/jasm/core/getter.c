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
#include <string.h>

#ifdef __unix__
#include <sys/utsname.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#endif

#ifdef __gnu_linux__
#include <sys/sysinfo.h>
#endif

#include "jasmbuild_info.h"
#include "getter.h"
#include "logger.h"
#include "ipc.h"

char error[BUFSIZ];

char getterName[NGETTER][BUFSIZ] = {"Version", "Copyright", "Hostname", "KernelName",
                                    "KernelRelease", "KernelVersion","PosixVersion", "Machine",
                                    "CpuProcessor","CpuNumber","UpTime","TotalRAM","FreeRAM","Processes",
                                    "SchedulerVersion" ,"SchedulerInfo", "CpuName", "CacheSize", "CoreNum", "CoreSpeeds",
                                    "AddressSizes", "CreatedProcNum", "IfSwap", "FileHandlesNum"
                                   };

void (*getterFunction[NGETTER]) (int) = {getVersion, getCopyright, getHostname,
                                         getKernelName, getKernelRelease, getKernelVersion,getPosixVersion, getMachine,
                                         getCpuProcessor, getCpuNumber, getUpTime, getTotalRAM, getFreeRAM,
                                         getProcesses, getSchedulerVersion, getSchedulerInfo, getCpuName, getCacheSize, getCoreNum,
                                         getCoreSpeeds, getAddressSizes,getCreatedProcNum, getIfSwap, getFileHandlesNum
                                        };

/*
 *  scrive su fd int numero getter e N stringhe nomiGetter
 */
void getGetter (int fd)
{
        int i;
        int n_getter = NGETTER;
        size_t count = 0;
        ssize_t ret_val;

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

                sendMsg(fd,getterName[i]);
        }
}

void getVersion (int fd)
{
        sendMsg(fd,VERSION);
}

void getCopyright (int fd)
{
        sendMsg(fd,COPYRIGHT);
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
                sendMsg(fd,buf);
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
                sendMsg(fd,buf);
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
                sendMsg(fd,buf);
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
                sendMsg(fd,buf);
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
                sendMsg(fd,buf);
        }
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getUpTime (int fd)
{
#ifdef __gnu_linux__
      char buf[BUFSIZ];
      struct sysinfo sys_info;
      long days,hours,min,sec;

      if( sysinfo (&sys_info) != 0)
      {
            log_error ("getUpTime() Failed");
            return;
      }
      else{
            days = sys_info.uptime / 86400;
            hours = (sys_info.uptime / 3600) - (days * 24);
            min = (sys_info.uptime / 60) - (hours * 60) - (days * 1440);
            sec = (sys_info.uptime) - (min * 60) - (hours * 3600) - (days * 86400);
            sprintf(buf,"%ld:%ld:%ld:%ld",days,hours,min,sec);
      }
      sendMsg(fd,buf);
#else
        log_string("[INFO]Support for getUpTime() not availible! ");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getTotalRAM (int fd)
{
#ifdef __gnu_linux__
      struct sysinfo sys_info;
      char buf[BUFSIZ];
      const unsigned long megabyte = 1024*1024;

      if( sysinfo (&sys_info) != 0)
      {
            log_error ("getTotalRAM() Failed");
            return;
      }
      else
      {
            sprintf (buf,"%lu MB",sys_info.totalram/megabyte);
            sendMsg(fd,buf);
      }
#else
    log_string("[INFO] Support for getTotalRAM() not availible! ");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getFreeRAM (int fd)
{
#ifdef __gnu_linux__
      struct sysinfo sys_info;
      char buf[BUFSIZ];
      const unsigned long MEGABYTE = 1024*1024;

      if( sysinfo (&sys_info) != 0)
      {
            log_error ("getFreeRAM() Failed");
            return;
      }
      else
      {
            sprintf (buf,"%lu MB",sys_info.freeram/MEGABYTE);
            sendMsg(fd,buf);
      }
#else
    log_string("[INFO]Support for getFreeRAM() not availible");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getProcesses (int fd)
{
#ifdef __gnu_linux__
      char buf[BUFSIZ];
      struct sysinfo sys_info;
      if( sysinfo(&sys_info) != 0)
      {
            log_error("getProcesses() Failed");
            return;
      }
      else
      {
            sprintf(buf,"%u",sys_info.procs);
            sendMsg(fd,buf);
      }
#else
    log_string("[INFO]Support for getProcesses not availible!");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getCpuProcessor (int fd)
{
      long numCPU,onCPU;
      char buf[BUFSIZ];
      //TODO error checking
      if( (numCPU=sysconf(_SC_NPROCESSORS_CONF))==-1 || (onCPU=sysconf(_SC_NPROCESSORS_ONLN))==-1)
      {
            log_error("getCpuProcessor() Failed");
            return;
      }
      sprintf(buf,"Number of CPU configured: %ld Online: %ld",numCPU,onCPU);
      sendMsg(fd,buf);
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getPosixVersion (int fd)
{
      long version;
      char buf[BUFSIZ];
      if( (version=sysconf(_SC_2_VERSION))==-1)
      {
            log_error("getPosixVersion() Failed");
            return;
      }
      else
      {
            sprintf(buf,"Version: %ld (YYYYMML)",version);
            sendMsg(fd,buf);
      }
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getCpuNumber (int fd)
{
#ifdef __gnu_linux__
      int cpu_num;
      char buf[BUFSIZ];
      if( (cpu_num = sched_getcpu()) == -1 )
      {
            log_error("getCpuNumber() Failed");
            return;
      }
      else
      {
            sprintf(buf,"The process is running on cpu: %d ",cpu_num);
            sendMsg(fd,buf);
      }
#else
    log_string("[INFO] Support for getCpuNumber() not availible!");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getSchedulerVersion (int fd)
{
      int file;
      int i=0;
      char temp[BUFSIZ];
      char buf[BUFSIZ];
      if ( (file = open("/proc/sched_debug",O_RDONLY))==-1)
      {
            log_error("getSchedulerVersion() Unable to open /proc/sched_debug");
            return;
      }
      if( (read(file, temp, BUFSIZ))<=0 )
      {
            log_error("getSchedulerVersion() Failed to read");
            close(file);
            return;
      }
      close(file);
      while(temp[i]!='\n') // copy the first line
      {
            buf[i]=temp[i];
            i++;
      }
      buf[i]='\0';
      sendMsg(fd,buf);
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getSchedulerInfo (int fd)
{
      int file,lat,gran,slice;
      char buf[BUFSIZ];
      char temp[BUFSIZ];
      if ( (file = open("/proc/sys/kernel/sched_latency_ns",O_RDONLY))==-1)
      {
            log_error("getSchedulerInfo() Unable to open /proc/sys/kernel/sched_latency_ns");
            return;
      }
      if( (read(file, temp, BUFSIZ))<=0 )
      {
            log_error("getSchedulerInfo() Failed to read");
            close(file);
            return;
      }
      close(file);
      lat = atoi(temp);
      if ( (file = open("/proc/sys/kernel/sched_min_granularity_ns",O_RDONLY))==-1)
      {
            log_error("getSchedulerInfo() Unable to open /proc/sys/kernel/sched_min_granularity_ns");
            return;
      }
      if( (read(file, temp, BUFSIZ))<=0 )
      {
            log_error("getSchedulerInfo() Failed to read");
            close(file);
            return;
      }
      close(file);
      gran = atoi(temp);
      if ( (file = open("/proc/sys/kernel/sched_rr_timeslice_ms",O_RDONLY))==-1)
      {
            log_error("getSchedulerInfo() Unable to open /proc/sys/kernel/sched_rr_timeslice_ms");
            return;
      }
      if( (read(file, temp, BUFSIZ))<=0 )
      {
            log_error("getSchedulerInfo() Failed to read");
            close(file);
            return;
      }
      close(file);
      slice = atoi(temp);
      sprintf(buf,"Latency:%d Granularity:%d Timeslice:%d ",lat,gran,slice);
      sendMsg(fd,buf);
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
void getCpuName (int fd) //Returns the name of the Cpu
{
		int cpu_fd;
		char buf[BUFSIZ];
		char *string;
		int i = 0;
		
		cpu_fd = open("/proc/cpuinfo", O_RDONLY);
		if(read(cpu_fd, buf, BUFSIZ) <= 0){
                sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                log_error ("[JASM-DAEMON][getCpuName][read()] Error!");
                log_error (error);
		}
		string = &strstr(buf, "model name	: ")[strlen("model name	: ")];
		while(string[i] != '\n') ++i; //conta i caratteri della descrizione della cpu
		sprintf(buf, "%.*s", i, string); //ora buf contiene il modello del processore
        sendMsg(fd,buf);
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
void getCacheSize (int fd) //Returns the cache size of the cpu
{
    int cpu_fd;
    char buf[BUFSIZ];
    char *string;
    int i = 0;

    cpu_fd = open("/proc/cpuinfo", O_RDONLY);
    if (read(cpu_fd, buf, BUFSIZ) <= 0) {
        sprintf(error, "[JASM-DAEMON][errno] %s", strerror(errno));
        log_error("[JASM-DAEMON][getCacheSize][read()] Error!");
        log_error(error);
    }
    string = &strstr(buf, "cache size	: ")[strlen("cache size	: ")];
    while (string[i] != '\n') ++i; //conta i caratteri della descrizione della cpu
    sprintf(buf, "%.*s", i, string); //ora buf contiene il modello del processore
    sendMsg(fd, buf);
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
void getCoreNum (int fd) //Returns the number of physical cores
{
		int cpu_fd;
		char info[20] = "cpu cores	: ";
		char buf[BUFSIZ];
		char *string;
		int i = 0;
	
		cpu_fd = open("/proc/cpuinfo", O_RDONLY);
		if(read(cpu_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCoreNum][read()] Error!");
        log_error (error);
		}
		string = &strstr(buf, info)[strlen(info)];
		while(string[i] != '\n') ++i; //conta i caratteri
		sprintf(buf, "%.*s", i, string); //ora buf contiene il numero di core
		close(cpu_fd);
        sendMsg(fd,buf);
}


/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
void getCoreSpeeds (int fd) //Returns the core speeds
{
	int cpu_fd;
	char info[20] = "cpu MHz		: "; //use strncpy!!
	char buf[CPU_FILE_SIZE];
	char final_string[BUFSIZ];
	char speeds[MAX_CORE][CPU_FILE_SIZE];
	char *tmp;
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
    sendMsg(fd,buf);
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
void getAddressSizes (int fd) //Returns cpu address size
{
		int cpu_fd;
		char info[20] = "address sizes	: ";
		char buf[BUFSIZ];
		char *string;
		int i = 0;
	
		cpu_fd = open("/proc/cpuinfo", O_RDONLY);
		if(read(cpu_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getAddressSizes][read()] Error!");
        log_error (error);
		}
		string = &strstr(buf, info)[strlen(info)];
		while(string[i] != '\n') ++i;
		sprintf(buf, "%.*s", i, string);
        sendMsg(fd,buf);
		close(cpu_fd);
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
void getCreatedProcNum (int fd) //Returns the number of created processes since the start
{
		int proc_fd;
		char info[20] = "processes ";
		char buf[BUFSIZ];
		char *string;
		int i = 0;
	
		proc_fd = open("/proc/stat", O_RDONLY);
		if(read(proc_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getCreatedProcNum][read()] Error!");
        log_error (error);
		}
		string = &strstr(buf, info)[strlen(info)];
		while(string[i] != '\n') ++i;
		sprintf(buf, "%.*s", i, string);
        sendMsg(fd,buf);
		close(proc_fd);

}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
void getIfSwap (int fd) //Check for the existence of swap partitions
{
		int swaps_fd;
		char buf[BUFSIZ];
		char result[4];
		int i = 0;
	
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

        sendMsg(fd,buf);
		close(swaps_fd);

}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
void getFileHandlesNum (int fd) //Returns opened file handles (and file descriptors) number
{
		int file_fd;
		char buf[BUFSIZ];
		char result[10];
		int i = 0;
	
		file_fd = open("/proc/sys/fs/file-nr", O_RDONLY);
		if(read(file_fd, buf, BUFSIZ) <= 0){
				sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        log_error ("[JASM-DAEMON][getFileHandlesNum][read()] Error!");
        log_error (error);
		}
		while(buf[i] != '\t') ++i;
		sprintf(result, "%.*s", i, buf);

        sendMsg(fd,buf);
		close(file_fd);

}


