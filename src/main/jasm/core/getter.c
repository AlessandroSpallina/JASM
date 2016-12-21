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
#include <arpa/inet.h>
#include <sys/utsname.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#endif

#ifdef __gnu_linux__
#include <sys/sysinfo.h>
#endif

#include "ipc.h"
#include "jasmbuild_info.h"
#include "getter.h"
#include "miscellaneous.h"
#include "logger.h"

#define MAX_CORE 16
#define CPU_FILE_SIZE 65536

//static char errlog[MAX_LOG_CHARS];
static char error[MAX_LOG_CHARS];

char getterName[NGETTER][BUFSIZ] = {"Version", "Copyright", "Hostname", "KernelName",
                                    "KernelRelease", "KernelVersion","PosixVersion", "Machine",
                                    "CpuProcessor","CpuNumber","UpTime","TotalRAM","FreeRAM","Processes",
                                    "SchedulerVersion" ,"SchedulerInfo", "CpuName", "CacheSize", "CoreNum", "CoreSpeeds",
                                    "AddressSizes", "CreatedProcNum", "IfSwap", "FileHandlesNum","CHRDevices","BLKDevices",
                                    "EmulatedFSDevices", "RealFSDevices","Meminfo","Modules","Buddyinfo","Dma", "IOmem","KeyUsr", "Mtrr","Misc","Interrupts"
                                   };

void (*getterFunction[NGETTER]) (int) = {getVersion, getCopyright, getHostname,
                                         getKernelName, getKernelRelease, getKernelVersion,getPosixVersion, getMachine,
                                         getCpuProcessor, getCpuNumber, getUpTime, getTotalRAM, getFreeRAM,
                                         getProcesses, getSchedulerVersion, getSchedulerInfo, getCpuName, getCacheSize, getCoreNum,
                                         getCoreSpeeds, getAddressSizes,getCreatedProcNum, getIfSwap, getFileHandlesNum, getCHRDevices, getBLKDevices,
                                         getEmulatedFSDevices, getRealFSDevices, getMeminfo, getModules, getBuddyinfo, getDma, getIOmem,
                                         getKeyUsr,getMtrr,getMisc,getInterrupts
                                        };

/*
 *  scrive su fd int numero getter e N stringhe nomiGetter
 */
void getGetter (int fd)
{
        int i;
        int n_getter = NGETTER;
        int count = 0;
        char conv_buffer[BUFSIZ];
        //start from an error condition, to make sure we do not miss anything
        ssize_t ret_val = -1;

        sprintf(conv_buffer, "%d", n_getter);
        ret_val = sendMsg (fd, conv_buffer);

        if (ret_val == 0 || ret_val == -1)
        {
                wlogev(EV_ERROR, "[JASM-DAEMON][getGetter][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
        }

        for (i = 0; i < NGETTER; i++)
        {
                count = strlen (getterName[i]);
                sprintf(conv_buffer, "%d", count);
                ret_val = sendMsg (fd, conv_buffer);

                if (ret_val == 0 || ret_val == -1)
                {
                        wlogev(EV_ERROR, "[JASM-DAEMON][getGetter][write()] Error! ret_val is 0 or -1");
                        wlogev(EV_ERROR, error);
                }
                ret_val = sendMsg (fd, getterName[i]);

                if (ret_val == 0 || ret_val == -1)
                {
                        wlogev(EV_ERROR, "[JASM-DAEMON][getGetter][write()] Error! ret_val is 0 or -1");
                        wlogev(EV_ERROR, error);
                }
        }
}

void getVersion (int fd)
{
        int n;

        n = sendMsg (fd, VERSION);
        if (n < 0)
        {
                sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                wlogev(EV_ERROR, "[JASM-DAEMON][getVersion][write()] Error!");
                wlogev(EV_ERROR, error);
        }

        else
        {

                if ( n < strlen (VERSION) )
                {
                        sprintf (error, "[JASM-DAEMON][getVersion][write()] sent %d byte, correct num byte is %zu", n, strlen (VERSION) );
                        wlogev(EV_WARN, error);

                }
                else
                {
                        sprintf (error, "[JASM-DAEMON][getVersion][write()] sent %d byte", n);
                        wlogev(EV_INFO, error);
                }
        }
}

void getCopyright (int fd)
{
        int n;

        n = sendMsg (fd, COPYRIGHT);
        if (n < 0)
        {
                sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                wlogev(EV_ERROR, "[JASM-DAEMON][getCopyright][write()] Error!");
                wlogev(EV_ERROR, error);
        }

        else
        {
                if ( n < strlen (COPYRIGHT) )
                {
                        sprintf (error, "[JASM-DAEMON][getCopyright][write()] sent %d byte, correct num byte is %zu", n, strlen (COPYRIGHT) );
                        wlogev(EV_WARN, error);
                }
                else
                {
                        sprintf (error, "[JASM-DAEMON][getCopyright][write()] sent %d byte", n);
                        wlogev(EV_INFO, error);
                }
        }
}

void getHostname (int fd)
{
        struct utsname info;
        char buf[BUFSIZ];

        if (uname (&info) == -1)
        {
                wlogev(EV_ERROR, "getHostname() [uname] failed");
                return;
        }
        else
        {
                strcpy (buf, info.nodename);
                int n = sendMsg (fd, buf);
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                        wlogev(EV_ERROR, "[JASM-DAEMON][getHostname][write()] Error!");
                        wlogev(EV_ERROR, error);
                }

                else
                {
                        if ( n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getHostname][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                wlogev(EV_WARN, error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getHostname][write()] sent %d byte", n);
                                wlogev(EV_INFO, error);
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

                wlogev(EV_ERROR, "getKernelName() [uname] failed");
                return;
        }
        else
        {
                strcpy (buf, info.sysname);
                int n = sendMsg (fd, buf);
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );

                        wlogev(EV_ERROR, "[JASM-DAEMON][getKernelName][write()] Error!");
                        wlogev(EV_ERROR, error);
                }

                else
                {
                        if (n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelName][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                wlogev(EV_WARN, error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelName][write()] sent %d byte", n);
                                wlogev(EV_INFO, error);
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
                wlogev(EV_ERROR, "getKernelRelease() [uname] failed");

                return;
        }
        else
        {
                strcpy (buf, info.release);
                int n = sendMsg (fd, buf);
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );

                        wlogev(EV_ERROR, "[JASM-DAEMON][getKernelRelease][write()] Error!");
                        wlogev(EV_ERROR, error);
                }
                else
                {
                        if (n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelRelease][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                wlogev(EV_WARN, error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelRelease][write()] sent %d byte", n);
                                wlogev(EV_INFO, error);
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
                wlogev(EV_ERROR, "getKernelVersion() [uname] failed");

                return;
        }
        else
        {
                strcpy (buf, info.version);
                int n = sendMsg (fd, buf);
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                        wlogev(EV_ERROR, "[JASM-DAEMON][getKernelVersion][write()] Error!");
                        wlogev(EV_ERROR, error);

                }
                else
                {
                        if (n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelVersion][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                wlogev(EV_WARN, error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getKernelVersion][write()] sent %d byte", n);

                                wlogev(EV_INFO, error);
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
                wlogev(EV_ERROR, "getMachine() [uname]  failed");

                return;
        }
        else
        {
                strcpy (buf, info.machine);
                int n = sendMsg (fd, buf);
                if (n < 0)
                {
                        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
                        wlogev(EV_ERROR, "[JASM-DAEMON][getMachine][write()] Error!");
                        wlogev(EV_ERROR, error);
                }
                else
                {
                        if (n < strlen (buf) )
                        {
                                sprintf (error, "[JASM-DAEMON][getMachine][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                                wlogev(EV_WARN, error);
                        }
                        else
                        {
                                sprintf (error, "[JASM-DAEMON][getMachine][write()] sent %d byte", n);

                                wlogev(EV_INFO, error);
                        }
                }
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
      int days,hours,min,sec;
      int n;

      if( sysinfo (&sys_info) != 0)
      {
            wlogev(EV_ERROR, "getUpTime() Failed");

            return;
      }
      else{
            days = sys_info.uptime / 86400;
            hours = (sys_info.uptime / 3600) - (days * 24);
            min = (sys_info.uptime / 60) - (hours * 60) - (days * 1440);
            sec = (sys_info.uptime) - (min * 60) - (hours * 3600) - (days * 86400);
            sprintf(buf,"%02d:%02d:%02d:%02d",days,hours,min,sec);
      }
      n = sendMsg (fd, buf);
        if (n < 0)
        {
            sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
            wlogev(EV_ERROR, "[JASM-DAEMON][getUptime][write()] Error!");
            wlogev(EV_ERROR, error);
        }
        else
        {
            if (n < strlen (buf) )
            {
                sprintf (error, "[JASM-DAEMON][getUptime][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                wlogev(EV_WARN, error);
            }
            else
            {
                sprintf (error, "[JASM-DAEMON][getUptime][write()] sent %d byte", n);
                wlogev(EV_INFO, error);
            }
        }
#else
        wlogev(EV_WARN, "[INFO]Support for getUpTime() not availible! ");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getTotalRAM (int fd)
{
#ifdef __gnu_linux__
      int n;
      struct sysinfo sys_info;
      char buf[BUFSIZ];
      const unsigned long megabyte = 1024*1024;

      if( sysinfo (&sys_info) != 0)
      {
            wlogev(EV_ERROR, "getTotalRAM() Failed");

            return;
      }
      else
      {
            sprintf (buf,"%lu MB",sys_info.totalram/megabyte);
            n = sendMsg (fd, buf);
            if (n < 0)
            {
                  sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
                  wlogev(EV_ERROR, "[JASM-DEAMON][getTotalRAM][write()] Error!");
                  wlogev(EV_ERROR, error);

            }
            if (n <strlen(buf))
            {
                  sprintf (error, "[JASM-DAEMON][getTotalRAM][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                  wlogev(EV_WARN, error);

            }
            else
            {
                  sprintf (error, "[JASM-DAEMON][getTotalRAM][write()] sent %d byte", n);
                  wlogev(EV_INFO, error);
            }
      }
#else
    wlogev(EV_WARN, "[INFO] Support for getTotalRAM() not availible! ");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getFreeRAM (int fd)
{
#ifdef __gnu_linux__
      int n;
      struct sysinfo sys_info;
      char buf[BUFSIZ];
      const unsigned long megabyte = 1024*1024;

      if( sysinfo (&sys_info) != 0)
      {
            wlogev(EV_ERROR, "getFreeRAM() Failed");
            return;
      }
      else
      {
            sprintf (buf,"%lu MB",sys_info.freeram/megabyte);
            n = sendMsg (fd, buf);
            if (n < 0)
            {
                  sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
                  wlogev(EV_ERROR, "[JASM-DEAMON][getFreeRAM][write()] Error!");
                  wlogev(EV_ERROR, error);

            }
            if (n < strlen(buf))
            {
                  sprintf (error, "[JASM-DAEMON][getFreeRAM][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                  wlogev(EV_WARN, error);
            }
            else
            {
                  sprintf (error, "[JASM-DAEMON][getFreeRAM][write()] sent %d byte", n);
                  wlogev(EV_INFO, error);
            }
      }
#else
    wlogev(EV_WARN, "[INFO]Support for getFreeRAM() not availible");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getProcesses (int fd)
{
#ifdef __gnu_linux__
      int n;
      char buf[BUFSIZ];
      struct sysinfo sys_info;
      if( sysinfo(&sys_info) != 0)
      {
            wlogev(EV_ERROR, "getProcesses() Failed");
            return;
      }
      else
      {
            sprintf(buf,"%u",sys_info.procs);
            n = sendMsg(fd,buf);
            if (n < 0)
            {
                  sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
                  wlogev(EV_ERROR, "[JASM-DEAMON][getProceses][write()] Error!");
                  wlogev(EV_ERROR, error);
            }
            if (n < strlen(buf) )
            {
                  sprintf (error, "[JASM-DAEMON][getProcesses][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                  wlogev(EV_WARN, error);
            }
            else
            {
                  sprintf (error, "[JASM-DAEMON][getProcesses][write()] sent %d byte", n);
                  wlogev(EV_INFO, error);
            }
      }
#else
    wlogev(EV_WARN, "[INFO]Support for getProcesses not availible!");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getCpuProcessor (int fd)
{
      int n;
      int numCPU,onCPU;
      char buf[BUFSIZ];
      //TODO error checking
      if( (numCPU=sysconf(_SC_NPROCESSORS_CONF))==-1 || (onCPU=sysconf(_SC_NPROCESSORS_ONLN))==-1)
      {
            wlogev(EV_ERROR, "getCpuProcessor() Failed");
            return;
      }
      sprintf(buf,"Number of CPU configured: %d Online: %d",numCPU,onCPU);
      n = sendMsg(fd,buf);
      if (n < 0)
      {
            sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
            wlogev(EV_ERROR, "[JASM-DEAMON][getCpuProcessor][write()] Error!");
            wlogev(EV_ERROR, error);
      }
      if (n < strlen(buf) )
      {
            sprintf (error, "[JASM-DAEMON][getCpuProcessor][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            wlogev(EV_WARN, error);
      }
      else
      {
            sprintf (error, "[JASM-DAEMON][getCpuProcessor][write()] sent %d byte", n);
            wlogev(EV_INFO, error);
      }
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getPosixVersion (int fd)
{
      int n;
      long version;
      char buf[BUFSIZ];
      if( (version=sysconf(_SC_2_VERSION))==-1)
      {
            wlogev(EV_ERROR, "getPosixVersion() Failed");

            return;
      }
      else
      {
            sprintf(buf,"Version: %ld (YYYYMML)",version);
            n = sendMsg(fd, buf);
            if(n < 0)
            {
                sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
                  wlogev(EV_ERROR, "[JASM-DEAMON][getPosixVersion][write()] Error!");
                  wlogev(EV_ERROR, error);
            }
            if (n < strlen(buf) )
            {
                  sprintf (error, "[JASM-DAEMON][getPosixVersion][write()] sent %d byte, correct num byte is %zu",n,strlen(buf));
                  wlogev(EV_WARN, error);
            }
            else
            {
                  sprintf (error, "[JASM-DAEMON][getPosixVersion][write()] sent %d byte", n);
                  wlogev(EV_INFO, error);
            }
      }
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getCpuNumber (int fd)
{
#ifdef __gnu_linux__
      int n,cpu_num;
      char buf[BUFSIZ];
      if( (cpu_num = sched_getcpu()) == -1 )
      {
            wlogev(EV_ERROR, "getCpuNumber() Failed");
            return;
      }
      else
      {
            sprintf(buf,"The process is running on cpu: %d ",cpu_num);
            n = sendMsg(fd, buf);
            if(n < 0)
            {
                sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
                wlogev(EV_ERROR, "[JASM-DEAMON][getCpuNumber][write()] Error!");
                wlogev(EV_ERROR, error);
            }
            if (n < strlen(buf) )
            {
                  sprintf (error, "[JASM-DAEMON][getCpuNumber][write()] sent %d byte, correct num byte is %zu",n,strlen(buf));
                  wlogev(EV_WARN, error);
            }
            else
            {
                  sprintf (error, "[JASM-DAEMON][getCpuNumber][write()] sent %d byte", n);
                  wlogev(EV_INFO, error);
            }
      }
#else
    wlogev(EV_WARN, "[INFO] Support for getCpuNumber() not availible!");
#endif
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getSchedulerVersion (int fd)
{
      int file;
      int n,i=0;
      char temp[BUFSIZ];
      char buf[BUFSIZ];
      if ( (file = open("/proc/sched_debug",O_RDONLY))==-1)
      {
            wlogev(EV_ERROR, "getSchedulerVersion() Unable to open /proc/sched_debug");
            return;
      }
      if( (read(file, temp, BUFSIZ))<=0 )
      {
            wlogev(EV_ERROR, "getSchedulerVersion() Failed to read");
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
      n = sendMsg(fd, buf);
      if(n < 0)
      {
          sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
          wlogev(EV_ERROR, "[JASM-DEAMON][getSchedulerVersion][write()] Error!");
          wlogev(EV_ERROR, error);
      }
      if (n < strlen(buf) )
      {
            sprintf (error, "[JASM-DAEMON][getSchedulerVersion][write()] sent %d byte, correct num byte is %zu",n,strlen(buf));
            wlogev(EV_WARN, error);
      }
      else
      {
            sprintf (error, "[JASM-DAEMON][getSchedulerVersion][write()] sent %d byte", n);
            wlogev(EV_INFO, error);
      }
}

/*
 *  Getter coded by Ennio Nasca
 *  https://github.com/ennnas
 */
void getSchedulerInfo (int fd)
{
      int n;
      int file;
      int lat,gran,slice;
      char buf[BUFSIZ];
      char temp[BUFSIZ];
      if ( (file = open("/proc/sys/kernel/sched_latency_ns",O_RDONLY))==-1)
      {
            wlogev(EV_ERROR, "getSchedulerInfo() Unable to open /proc/sys/kernel/sched_latency_ns");
            return;
      }
      if( (read(file, temp, BUFSIZ))<=0 )
      {
            wlogev(EV_ERROR, "getSchedulerInfo() Failed to read");
            close(file);
            return;
      }
      close(file);
      lat = atoi(temp);
      if ((file = open("/proc/sys/kernel/sched_min_granularity_ns",O_RDONLY))==-1)
      {
            wlogev(EV_ERROR, "getSchedulerInfo() Unable to open /proc/sys/kernel/sched_min_granularity_ns");
            return;
      }
      if( (read(file, temp, BUFSIZ))<=0 )
      {
            wlogev(EV_ERROR, "getSchedulerInfo() Failed to read");
            close(file);
            return;
      }
      close(file);
      gran = atoi(temp);
      if ((file = open("/proc/sys/kernel/sched_rr_timeslice_ms",O_RDONLY))==-1)
      {
            wlogev(EV_ERROR, "getSchedulerInfo() Unable to open /proc/sys/kernel/sched_rr_timeslice_ms");
            return;
      }
      if( (read(file, temp, BUFSIZ))<=0 )
      {
            wlogev(EV_ERROR, "getSchedulerInfo() Failed to read");
            close(file);
            return;
      }
      close(file);
      slice = atoi(temp);
      sprintf(buf,"Latency:%d Granularity:%d Timeslice:%d ",lat,gran,slice);
      n = sendMsg(fd, buf);
      if(n < 0)
      {
          sprintf (error, "[JASM-DEAMON][errno] %s", strerror (errno) );
          wlogev(EV_ERROR, "[JASM-DEAMON][getSchedulerInfo][write()] Error!");
          wlogev(EV_ERROR, error);
      }
      if (n < strlen(buf) )
      {
            sprintf (error, "[JASM-DAEMON][getSchedulerInfo][write()] sent %d byte, correct num byte is %zu",n,strlen(buf));
            wlogev(EV_WARN, error);
      }
      else
      {
            sprintf (error, "[JASM-DAEMON][getSchedulerInfo][write()] sent %d byte", n);
            wlogev(EV_INFO, error);
      }
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
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
                wlogev(EV_ERROR, "[JASM-DAEMON][getCpuName][read()] Error!");
                wlogev(EV_ERROR, error);
		}
		string = &strstr(buf, "model name	: ")[strlen("model name	: ")];
		while(string[i] != '\n') ++i; //conta i caratteri della descrizione della cpu
		sprintf(buf, "%.*s", i, string); //ora buf contiene il modello del processore
		n = sendMsg (fd, buf);
		close(cpu_fd);
        if (n < 0) {
                sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno));
                wlogev(EV_ERROR, "[JASM-DAEMON][getCpuName][write()] Error!");
                wlogev(EV_ERROR, error);
        } else {
            if (n < strlen (buf) ) {
                sprintf (error, "[JASM-DAEMON][getCpuName][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
                wlogev(EV_WARN, error);
            } else {
                sprintf (error, "[JASM-DAEMON][getCpuName][write()] sent %d byte", n);
                wlogev(EV_INFO, error);
            }
        }
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
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
        wlogev(EV_ERROR, "[JASM-DAEMON][getCacheSize][read()] Error!");
        wlogev(EV_ERROR, error);
		}
		string = &strstr(buf, "cache size	: ")[strlen("cache size	: ")];
		while(string[i] != '\n') ++i; //conta i caratteri della descrizione della cpu
		sprintf(buf, "%.*s", i, string); //ora buf contiene il modello del processore
		n = sendMsg (fd, buf);
		close(cpu_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno));
        wlogev(EV_ERROR, "[JASM-DAEMON][getCacheSize][write()] Error!");
        wlogev(EV_ERROR, error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getCacheSize][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            wlogev(EV_WARN, error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getCacheSize][write()] sent %d byte", n);
            wlogev(EV_INFO, error);
        }
    }
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
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
        wlogev(EV_ERROR, "[JASM-DAEMON][getCoreNum][read()] Error!");
        wlogev(EV_ERROR, error);
		}
		string = &strstr(buf, info)[strlen(info)];
		while(string[i] != '\n') ++i; //conta i caratteri
		sprintf(buf, "%.*s", i, string); //ora buf contiene il numero di core
		n = sendMsg (fd, buf);
		close(cpu_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        wlogev(EV_ERROR, "[JASM-DAEMON][getCoreNum][write()] Error!");
        wlogev(EV_ERROR, error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getCoreNum][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            wlogev(EV_WARN, error);

        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getCoreNum][write()] sent %d byte", n);
            wlogev(EV_INFO, error);
        }
    }
}


/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
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
	n = sendMsg (fd, final_string);

	if (n < 0)
  	{
  	     sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
         wlogev(EV_ERROR, "[JASM-DAEMON][getCoreSpeeds][write()] Error!");
         wlogev(EV_ERROR, error);
    }
    else
    {
       if (n < strlen (final_string))
       {
          sprintf (error, "[JASM-DAEMON][getCoreSpeeds][write()] sent %d byte, correct num byte is %zu", n, strlen (final_string));
          wlogev(EV_WARN, error);
       }
       else
       {
          sprintf (error, "[JASM-DAEMON][getCoreSpeeds][write()] sent %d byte", n);
          wlogev(EV_INFO, error);
       }
   }
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
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
        wlogev(EV_ERROR, "[JASM-DAEMON][getAddressSizes][read()] Error!");
        wlogev(EV_ERROR, error);
		}
		string = &strstr(buf, info)[strlen(info)];
		while(string[i] != '\n') ++i;
		sprintf(buf, "%.*s", i, string);
		n = sendMsg (fd, buf);
		close(cpu_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        wlogev(EV_ERROR, "[JASM-DAEMON][getAddressSizes][write()] Error!");
        wlogev(EV_ERROR, error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getAddressSizes][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            wlogev(EV_WARN, error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getAddressSizes][write()] sent %d byte", n);
            wlogev(EV_INFO, error);
        }
    }
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
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
        wlogev(EV_ERROR, "[JASM-DAEMON][getCreatedProcNum][read()] Error!");
        wlogev(EV_ERROR, error);
		}
		string = &strstr(buf, info)[strlen(info)];
		while(string[i] != '\n') ++i;
		sprintf(buf, "%.*s", i, string);
		n = sendMsg (fd, buf);
		close(proc_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        wlogev(EV_ERROR, "[JASM-DAEMON][getCreatedProcNum][write()] Error!");
        wlogev(EV_ERROR, error);
    }
    else
    {
        if (n < strlen (buf) )
        {
            sprintf (error, "[JASM-DAEMON][getCreatedProcNum][write()] sent %d byte, correct num byte is %zu", n, strlen (buf) );
            wlogev(EV_WARN, error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getCreatedProcNum][write()] sent %d byte", n);
            wlogev(EV_INFO, error);
        }
    }
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
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
        wlogev(EV_ERROR, "[JASM-DAEMON][getIfSwap][read()] Error!");
        wlogev(EV_ERROR, error);
		}
		while(buf[i] != '\n') ++i;
		if(buf[i+1] == EOF){
			strcpy(result, "no"); //if the first line is the only line of the file
		}
		else{
			strcpy(result, "yes");
		}

		n = sendMsg (fd, result);
		close(swaps_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        wlogev(EV_ERROR, "[JASM-DAEMON][getIfSwap][write()] Error!");
        wlogev(EV_ERROR, error);
    }
    else
    {
        if (n < strlen (result) )
        {
            sprintf (error, "[JASM-DAEMON][getIfSwap][write()] sent %d byte, correct num byte is %zu", n, strlen (result) );
            wlogev(EV_WARN, error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getIfSwap][write()] sent %d byte", n);
            wlogev(EV_INFO, error);
        }
    }
}

/*Getter coded by Pierluca D'Oro https://github.com/Bellamy442/*/
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
        wlogev(EV_ERROR, "[JASM-DAEMON][getFileHandlesNum][read()] Error!");
        wlogev(EV_ERROR, error);
		}
		while(buf[i] != '\t') ++i;
		sprintf(result, "%.*s", i, buf);

		n = sendMsg (fd, result);
		close(file_fd);
		if (n < 0)
    {
        sprintf (error, "[JASM-DAEMON][errno] %s", strerror (errno) );
        wlogev(EV_ERROR, "[JASM-DAEMON][getFileHandlesNum][write()] Error!");
        wlogev(EV_ERROR, error);
    }
    else
    {
        if (n < strlen (result) )
        {
            sprintf (error, "[JASM-DAEMON][getFileHandlesNum][write()] sent %d byte, correct num byte is %zu", n, strlen (result) );
            wlogev(EV_WARN, error);
        }
        else
        {
            sprintf (error, "[JASM-DAEMON][getFileHandlesNum][write()] sent %d byte", n);
            wlogev(EV_INFO, error);
        }
    }
}

/*
 *  Getter coded by Emanuele Virgillito
 *  https://github.com/EmaVirg
 */
void getCHRDevices (int fd)
{
        int lenght=50;
        int file,res;
        int lines=0,i=0;
        char *path="/proc/devices";
        char devices[lenght];
        ssize_t ret_val = -1;


        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getCHRDevices][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }


        while((res=read_line(file,devices,lenght))!=0)
        {
            if((strncmp(devices,"Block",5))==0) break;
            lines++;
        }

        lines=lines-2;
        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getCHRDevices][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);

        if (ret_val == 0 || ret_val == -1)
        {
                wlogev(EV_ERROR, "[JASM-DAEMON][getCHRDevices][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
                return;
        }

        lines=lines+1;
        lseek(file,0,SEEK_SET);
        for (i = 0; i < lines; i++)
        {
          if (i==0)
          {
            res=read_line(file,devices,lenght);
          }
          else
          {
                res=read_line(file,devices,lenght);

                ret_val = sendMsg (fd, devices);

                if (ret_val == 0 || ret_val == -1)
                {
                  wlogev(EV_ERROR, "[JASM-DAEMON][getCHRDevices][write()] Error! ret_val is 0 or -1");
                  wlogev(EV_ERROR, error);
                        return;
                }
        }
      }
      close(file);
}

/*
 *  Getter coded by Emanuele Virgillito
 *  https://github.com/EmaVirg
 */
void getBLKDevices (int fd)
{
        int lenght=50;
        int file,res;
        int lines=0,i=0,j=0,k=0;
        char *path="/proc/devices";
        char devices[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getBLKDevices][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,devices,lenght))!=0)
        {

            if((strncmp(devices,"Block",5))==0)
            {
              j=1;
            }
            if(j==1)
            {

               lines++;
            }
            else
            {

               k++;
            }

        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getBLKDevices][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);
        if (ret_val == 0 || ret_val == -1)
        {
              wlogev(EV_ERROR, "[JASM-DAEMON][getBLKDevices][write()] Error! ret_val is 0 or -1");
              wlogev(EV_ERROR, error);
              return;
        }

        lseek(file,0,SEEK_SET);
        for(i=0;i<k;i++)
        {
          res=read_line(file,devices,lenght);
        }
        for (i = 0; i < lines; i++)
        {
          if (i==0)
          {
            res=read_line(file,devices,lenght);
          }
                    res=read_line(file,devices,lenght);
                    ret_val = sendMsg (fd, devices);

                    if (ret_val == 0 || ret_val == -1)
                    {
                      wlogev(EV_ERROR, "[JASM-DAEMON][getBLKDevices][write()] Error! ret_val is 0 or -1");
                      wlogev(EV_ERROR, error);
                      return;
                    }
        }
        close(file);
        return;
}

/*
 *  Getter coded by Emanuele Virgillito
 *  https://github.com/EmaVirg
 */
void getEmulatedFSDevices (int fd)
{
        int lenght=50;
        int file,res;
        int lines=0;
        char *path="/proc/filesystems";
        char devices[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getEmulatedFSDevices][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,devices,lenght))!=0)
        {

            if((strncmp(devices,"nodev",5))==0)
            {
              lines++;
            }
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getEmulatedFSDevices][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);
        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getEmulatedFSDevices][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
          return;
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,devices,lenght))!=0)
        {
            if((strncmp(devices,"nodev",5))==0)
            {
                    ret_val = sendMsg (fd, devices);

                    if (ret_val == 0 || ret_val == -1)
                    {
                      wlogev(EV_ERROR, "[JASM-DAEMON][getEmulatedFSDevices][write()] Error! ret_val is 0 or -1");
                      wlogev(EV_ERROR, error);
                      return;
                    }
            }
        }
        close(file);

}

/*
 *  Getter coded by Emanuele Virgillito
 *  https://github.com/EmaVirg
 */
void getRealFSDevices (int fd)
{
        int lenght=50;
        int file,res;
        int lines=0;
        char *path="/proc/filesystems";
        char devices[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getRealFSDevices][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,devices,lenght))!=0)
        {

            if((strncmp(devices,"nodev",5))==0);
            else lines++;
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getRealFSDevices][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);
        if (ret_val == 0 || ret_val == -1)
        {
                wlogev(EV_ERROR, "[JASM-DAEMON][getRealFSDevices][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
                return;
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,devices,lenght))!=0)
        {
            if((strncmp(devices,"nodev",5))==0);
            else
            {
              ret_val = sendMsg (fd, devices);

              if (ret_val == 0 || ret_val == -1)
              {
                      wlogev(EV_ERROR, "[JASM-DAEMON][getRealFSDevices][write()] Error! ret_val is 0 or -1");
                      wlogev(EV_ERROR, error);
                      return;
              }

            }
        }
        close(file);

}

/*
 *  Getter coded by Emanuele Virgillito
 *  https://github.com/EmaVirg
 */
void getMeminfo (int fd)
{
        int lenght=50;
        int file,res;
        int lines=0;
        char *path="/proc/meminfo";
        char devices[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getMeminfo][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,devices,lenght))!=0)
        {
            lines++;
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getMeminfo][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);
        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getMeminfo][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
          return;
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,devices,lenght))!=0)
        {

              ret_val = sendMsg (fd, devices);

              if (ret_val == 0 || ret_val == -1)
              {
                wlogev(EV_ERROR, "[JASM-DAEMON][getMeminfo][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
                return;
              }


        }
        close(file);

}

/*
 *  Getter coded by Emanuele Virgillito
 *  https://github.com/EmaVirg
 */
void getModules (int fd)
{
        int lenght=100;
        int file,res;
        int lines=0;
        char *path="/proc/modules";
        char devices[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getModules][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,devices,lenght))!=0)
        {
            lines++;
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getModules][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);
        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getModules][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
          return;
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,devices,lenght))!=0)
        {

              ret_val = sendMsg (fd, devices);

              if (ret_val == 0 || ret_val == -1)
              {
                wlogev(EV_ERROR, "[JASM-DAEMON][getModules][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
                return;
              }
        }
        close(file);
        return;
}
/*
 *  Getter coded by Orazio Scavo
 *  https://github.com/orazioscavo13
 */
void getBuddyinfo (int fd)
{
        int lenght=120;
        int file,res;
        int lines=0;
        char *path="/proc/buddyinfo";
        char riga[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getBuddyinfo][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,riga,lenght))!=0)
        {
            lines++;
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getBuddyinfo][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);
        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getBuddyinfo][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,riga,lenght))!=0)
        {

              ret_val = sendMsg (fd, riga);

              if (ret_val == 0 || ret_val == -1)
              {
                wlogev(EV_ERROR, "[JASM-DAEMON][getBuddyinfo][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
              }
        }
        close(file);
        return;
}

/*
 *  Getter coded by Orazio Scavo
 *  https://github.com/orazioscavo13
 */
void getDma (int fd)
{
        int lenght=120;
        int file,res;
        int lines=0;
        char *path="/proc/dma";
        char riga[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getDma][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,riga,lenght))!=0)
        {
            lines++;
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getDma][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);

        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getDma][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,riga,lenght))!=0)
        {
              ret_val = sendMsg (fd, riga);

              if (ret_val == 0 || ret_val == -1)
              {
                wlogev(EV_ERROR, "[JASM-DAEMON][getDma][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
              }
        }
        close(file);
        return;
}

/*
 *  Getter coded by Orazio Scavo
 *  https://github.com/orazioscavo13
 */
void getIOmem (int fd)
{
        int lenght=120;
        int file,res;
        int lines=0;
        char *path="/proc/iomem";
        char riga[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getIOmem][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,riga,lenght))!=0)
        {
            lines++;
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getIOmem][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);
        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getIOmem][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,riga,lenght))!=0)
        {

              ret_val = sendMsg (fd, riga);

              if (ret_val == 0 || ret_val == -1)
              {
                wlogev(EV_ERROR, "[JASM-DAEMON][getIOmem][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
              }
        }
        close(file);
        return;
}

/*
 *  Getter coded by Orazio Scavo
 *  https://github.com/orazioscavo13
 */
void getKeyUsr (int fd)
{
        int lenght=120;
        int file,res;
        int lines=0;
        char *path="/proc/key-users";
        char riga[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getKeyUsr][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,riga,lenght))!=0)
        {
            lines++;
        }

        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getKeyUsr][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }
        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);

        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getKeyUsr][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,riga,lenght))!=0)
        {

              ret_val = sendMsg (fd, riga);

              if (ret_val == 0 || ret_val == -1)
              {
                wlogev(EV_ERROR, "[JASM-DAEMON][getKeyUsr][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
              }
        }
        close(file);
        return;
}

/*
 *  Getter coded by Orazio Scavo
 *  https://github.com/orazioscavo13
 */
void getMtrr (int fd)
{
        int lenght=120;
        int file,res;
        int lines=0;
        char *path="/proc/mtrr";
        char riga[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getMtrr][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,riga,lenght))!=0)
        {
            lines++;
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getMtrr][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }
        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);

        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getMtrr][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,riga,lenght))!=0)
        {
              ret_val = sendMsg (fd, riga);

              if (ret_val == 0 || ret_val == -1)
              {
                wlogev(EV_ERROR, "[JASM-DAEMON][getMtrr][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
              }
        }
        close(file);
        return;
}

/*
 *  Getter coded by Orazio Scavo
 *  https://github.com/orazioscavo13
 */
void getMisc (int fd)
{
        int lenght=120;
        int file,res;
        int lines=1;
        char *path="/proc/misc";
        char riga[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getMisc][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,riga,lenght))!=0)
        {
            lines++;
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getMisc][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }

        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);
        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getMisc][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,riga,lenght))!=0)
        {

              ret_val = sendMsg (fd, riga);

              if (ret_val == 0 || ret_val == -1)
              {
                wlogev(EV_ERROR, "[JASM-DAEMON][getMisc][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
              }
        }
        close(file);
        return;
}

/*
 *  Getter coded by Orazio Scavo
 *  https://github.com/orazioscavo13
 */
void getInterrupts (int fd)
{
        int lenght=120;
        int file,res;
        int lines=0;
        char *path="/proc/interrupts";
        char riga[lenght];
        int ret_val = -1;

        file=open(path,O_RDONLY);
        if(file==-1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getInterrupts][open()] Error! file is -1");
          wlogev(EV_ERROR, error);
          return;
        }

        while((res=read_line(file,riga,lenght))!=0)
        {
            lines++;
        }


        if (lines == 0)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getInterrupts][write()] Error! lines is 0");
          wlogev(EV_ERROR, error);
          return;
        }
        char conv_buffer[BUFSIZ];
        sprintf(conv_buffer, "%d", lines);
        ret_val = sendMsg (fd, conv_buffer);

        if (ret_val == 0 || ret_val == -1)
        {
          wlogev(EV_ERROR, "[JASM-DAEMON][getInterrupts][write()] Error! ret_val is 0 or -1");
          wlogev(EV_ERROR, error);
        }

        lseek(file,0,SEEK_SET);

        while((res=read_line(file,riga,lenght))!=0)
        {
              ret_val = sendMsg (fd, riga);

              if (ret_val == 0 || ret_val == -1)
              {
                wlogev(EV_ERROR, "[JASM-DAEMON][getInterrupts][write()] Error! ret_val is 0 or -1");
                wlogev(EV_ERROR, error);
              }
        }
        close(file);
        return;
}
