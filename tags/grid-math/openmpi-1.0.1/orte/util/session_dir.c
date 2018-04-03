/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 *
 */

#include "orte_config.h"

#include <stdio.h>
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_LIBGEN_H 
#include <libgen.h>
#endif
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <errno.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#endif

#include "include/constants.h"

#include "util/univ_info.h"
#include "util/sys_info.h"
#include "util/proc_info.h"
#include "opal/util/output.h"
#include "opal/util/os_path.h"
#include "opal/util/os_create_dirpath.h"

#include "mca/errmgr/errmgr.h"
#include "runtime/runtime.h"

#include "util/session_dir.h"

static int orte_check_dir(bool create, char *directory);

static void orte_dir_empty(char *pathname);

static bool orte_is_empty(char *pathname);

#ifdef WIN32
#define OMPI_DEFAULT_TMPDIR "C:\\TEMP"
#else
#define OMPI_DEFAULT_TMPDIR "/tmp"
#endif

#define OMPI_PRINTF_FIX_STRING(a) ((NULL == a) ? "(null)" : a)

static int orte_check_dir(bool create, char *directory)
{
#ifndef WIN32
    struct stat buf;
    mode_t my_mode = S_IRWXU;  /* at the least, I need to be able to do anything */
#else
    struct __stat64 buf;
    mode_t my_mode = _S_IREAD | _S_IWRITE | _S_IEXEC;
#endif

#ifndef WIN32
    if (0 == stat(directory, &buf)) { /* exists - check access */
#else
    if (0 == _stat64(directory, &buf)) { /* exist -- check */
#endif
        if ((buf.st_mode & my_mode) == my_mode) { /* okay, I can work here */
            return(ORTE_SUCCESS);
        }
    }
    if (create) {
	return(opal_os_create_dirpath(directory, my_mode)); /* try to create it with proper mode */
    }
    return(ORTE_ERROR);  /* couldn't find it, or don't have access rights, and not asked to create it */
}

int orte_session_dir(bool create, char *prfx, char *usr, char *hostid,
		     char *batchid, char *univ, char *job, char *proc)
{
    char *fulldirpath=NULL, *tmp=NULL, *hostname=NULL, *batchname=NULL;
    char *sessions=NULL, *frontend=NULL, *user=NULL, *universe=NULL;
    char *prefix=NULL, *sav=NULL;
    int return_code;

    /* ensure that system info is set */
    orte_sys_info();

    if (NULL == usr) {  /* check if user set elsewhere */
    	if (NULL == orte_system_info.user) { /* error condition */
    	    return ORTE_ERROR;
    	} else {
    	    user = strdup(orte_system_info.user);
    	}
    } else {
    	user = strdup(usr);
    }

    if (NULL == univ) { /* see if universe set elsewhere */
    	if (NULL == orte_universe_info.name) {  /* error condition */
    	    return ORTE_ERROR;
    	} else {
    	    universe = strdup(orte_universe_info.name);
    	}
    } else {
    	universe = strdup(univ);
    }

    if (NULL == job && NULL != proc) { /* can't give a proc without a job */
    	return ORTE_ERROR;
    }

    if (NULL == hostid) {  /* check if hostname set elsewhere */
    	if (NULL == orte_system_info.nodename) { /* don't have a hostname anywhere  - error */
    	    return_code = ORTE_ERROR;
    	    goto CLEANUP;
    	} else {
    	    hostname = strdup(orte_system_info.nodename);
    	}
    } else {
    	hostname = strdup(hostid);
    }

    if (NULL == batchid) {
    	batchname = strdup("0");
    } else {
    	batchname = batchid;
    }

    if (NULL == orte_process_info.top_session_dir) {
    	if (0 > asprintf(&frontend, "openmpi-sessions-%s@%s_%s", user, hostname, batchname)) {
    	    return_code = ORTE_ERROR;
    	    goto CLEANUP;
    	}
    } else {
    	frontend = strdup(orte_process_info.top_session_dir);
    }


    if (NULL != proc) {
    	if (0 > asprintf(&sessions, "%s%s%s%s%s%s%s", frontend,
    			 orte_system_info.path_sep, universe,
    		         orte_system_info.path_sep, job,
    			 orte_system_info.path_sep, proc)) {
    	    return_code = ORTE_ERROR;
    	    goto CLEANUP;
    	}
    } else if (NULL != job) {
    	if (0 > asprintf(&sessions, "%s%s%s%s%s", frontend,
    			 orte_system_info.path_sep, universe,
    			 orte_system_info.path_sep, job)) {
    	    return_code = ORTE_ERROR;
    	    goto CLEANUP;
    	}
    } else {
    	if (0 > asprintf(&sessions, "%s%s%s", frontend, orte_system_info.path_sep, universe)) {
    	    return_code = ORTE_ERROR;
    	    goto CLEANUP;
    	}
    }


    if (NULL != prefix) {  /* if a prefix is specified, start looking here */
    	tmp = strdup(prefix);
    	fulldirpath = strdup(opal_os_path(false, tmp, sessions, NULL)); /* make sure it's an absolute pathname */
    	if (ORTE_SUCCESS == orte_check_dir(create, fulldirpath)) { /* check for existence and access, or create it */
    	    return_code = ORTE_SUCCESS;
    	    goto COMPLETE;
    	}
   }

    /* didn't find it, so first clear fulldirpath and tmp */
    if (NULL != fulldirpath) {
        free(fulldirpath); fulldirpath = NULL;
    }
    if (NULL != tmp) {
        free(tmp); tmp = NULL;
    }
    
    /* no prefix was specified, so check other options in order */
    if (NULL != orte_process_info.tmpdir_base) {  /* stored value previously */
    	tmp = strdup(orte_process_info.tmpdir_base);
    	fulldirpath = opal_os_path(false, tmp, sessions, NULL);
    	if (ORTE_SUCCESS == orte_check_dir(create, fulldirpath)) { /* check for existence and access, or create it */
    	    return_code = ORTE_SUCCESS;
    	    goto COMPLETE;
    	}
        free(tmp); tmp = NULL;
        free(fulldirpath); fulldirpath = NULL;
   } else if (NULL != getenv("OMPI_PREFIX_ENV")) {  /* we have prefix enviro var - try that next */
    	tmp = strdup(getenv("OMPI_PREFIX_ENV"));
    	fulldirpath = strdup(opal_os_path(false, tmp, sessions, NULL));
    	if (ORTE_SUCCESS == orte_check_dir(create, fulldirpath)) { /* check for existence and access, or create it */
    	    return_code = ORTE_SUCCESS;
    	    goto COMPLETE;
    	}
        free(tmp); tmp = NULL;
        free(fulldirpath); fulldirpath = NULL;
    } else if (NULL != getenv("TMPDIR")) {
    	tmp = strdup(getenv("TMPDIR"));
    	fulldirpath = strdup(opal_os_path(false, tmp, sessions, NULL));
    	if (ORTE_SUCCESS == orte_check_dir(create, fulldirpath)) { /* check for existence and access, or create it */
    	    return_code = ORTE_SUCCESS;
    	    goto COMPLETE;
    	}
        free(tmp); tmp = NULL;
        free(fulldirpath); fulldirpath = NULL;
    } else if (NULL != getenv("TMP")) {
    	tmp = strdup(getenv("TMP"));
    	fulldirpath = strdup(opal_os_path(false, tmp, sessions, NULL));
    	if (ORTE_SUCCESS == orte_check_dir(create, fulldirpath)) { /* check for existence and access, or create it */
    	    return_code = ORTE_SUCCESS;
    	    goto COMPLETE;
    	}
        free(tmp); tmp = NULL;
        free(fulldirpath); fulldirpath = NULL;
    } else {
    	tmp = strdup(OMPI_DEFAULT_TMPDIR);
    	fulldirpath = opal_os_path(false, tmp, sessions, NULL);
    	if (ORTE_SUCCESS == orte_check_dir(create, fulldirpath)) { /* check for existence and access, or create it */
    	    return_code = ORTE_SUCCESS;
    	    goto COMPLETE;
    	}
        free(tmp); tmp = NULL;
        free(fulldirpath); fulldirpath = NULL;
   }

    /* couldn't find anything - return error */
	return_code = ORTE_ERROR;
	goto CLEANUP;


 COMPLETE:
    if (create) {  /* if creating the dir tree, overwrite the fields */
    	if (NULL != orte_process_info.tmpdir_base) {
    	    free(orte_process_info.tmpdir_base);
    	    orte_process_info.tmpdir_base = NULL;
    	}
    
    	if (NULL != orte_process_info.top_session_dir) {
    	    free(orte_process_info.top_session_dir);
    	    orte_process_info.top_session_dir = NULL;
    	}
    }

    if (NULL == orte_process_info.tmpdir_base) {
    	orte_process_info.tmpdir_base = strdup(tmp); /* fill in if empty */
    }

    if (NULL == orte_process_info.top_session_dir) {
    	orte_process_info.top_session_dir = strdup(frontend);
    }

    if (NULL != proc) {
    	if (create) { /* overwrite if creating */
    	    if (NULL != orte_process_info.proc_session_dir) {
    		free(orte_process_info.proc_session_dir);
    		orte_process_info.proc_session_dir = NULL;
    	    }
    	}
    	if (NULL == orte_process_info.proc_session_dir) {
    	    orte_process_info.proc_session_dir = strdup(fulldirpath);
    	}
    	sav = strdup(fulldirpath);
    	free(fulldirpath);
    	fulldirpath = strdup(dirname(sav));
    	free(sav);
    }

    if (NULL != job) {
    	if (create) { /* overwrite if creating */
    	    if (NULL != orte_process_info.job_session_dir) {
    		free(orte_process_info.job_session_dir);
    		orte_process_info.job_session_dir = NULL;
    	    }
    	}
    	if (NULL == orte_process_info.job_session_dir) {
    	    orte_process_info.job_session_dir = strdup(fulldirpath);
    	}
    	sav = strdup(fulldirpath);
    	free(fulldirpath);
    	fulldirpath = strdup(dirname(sav));
    	free(sav);
    }

    if (create) { /* overwrite if creating */
    	if (NULL != orte_process_info.universe_session_dir) {
    	    free(orte_process_info.universe_session_dir);
    	    orte_process_info.universe_session_dir = NULL;
    	}
    }
    if (NULL == orte_process_info.universe_session_dir) {
        orte_process_info.universe_session_dir = strdup(fulldirpath);
    }

    if (orte_debug_flag) {
    	opal_output(0, "procdir: %s", 
                    OMPI_PRINTF_FIX_STRING(orte_process_info.proc_session_dir));
    	opal_output(0, "jobdir: %s", 
                    OMPI_PRINTF_FIX_STRING(orte_process_info.job_session_dir));
    	opal_output(0, "unidir: %s", 
                    OMPI_PRINTF_FIX_STRING(orte_process_info.universe_session_dir));
    	opal_output(0, "top: %s", 
                    OMPI_PRINTF_FIX_STRING(orte_process_info.top_session_dir));
    	opal_output(0, "tmp: %s", 
                    OMPI_PRINTF_FIX_STRING(orte_process_info.tmpdir_base));
    }

 CLEANUP:
    if (tmp) {
        free(tmp);
    }
    if (fulldirpath) {
        free(fulldirpath);
    }
    if (frontend) {
	   free(frontend);
    }
    if (batchname) {
	   free(batchname);
    }
    if (hostname) {
	   free(hostname);
    }
    if (universe) {
       free(universe);
    }
    if (sessions) {
	   free(sessions);
    }
    if (user) {
        free(user);
    }
    
    return return_code;
}


int
orte_session_dir_finalize(orte_process_name_t *proc)
{
    int rc;
    char *tmp;
    char *job, *job_session_dir, *vpid, *proc_session_dir;

    /* need to setup the top_session_dir with the prefix */
    tmp = opal_os_path(false,
                       orte_process_info.tmpdir_base,
                       orte_process_info.top_session_dir, NULL);
    
    /* define the proc and job session directories for this process */
    if (ORTE_SUCCESS != (rc = orte_ns.get_jobid_string(&job, proc))) {
        ORTE_ERROR_LOG(rc);
        free(tmp);
        return rc;
    }
    if (ORTE_SUCCESS != (rc = orte_ns.get_vpid_string(&vpid, proc))) {
        ORTE_ERROR_LOG(rc);
        free(tmp);
        free(job);
        return rc;
    }
    
    if (0 > asprintf(&job_session_dir, "%s%s%s",
                        orte_process_info.universe_session_dir,
                        orte_system_info.path_sep, job)) {
        ORTE_ERROR_LOG(ORTE_ERR_OUT_OF_RESOURCE);
        free(tmp);
        free(job);
        free(vpid);
        return ORTE_ERR_OUT_OF_RESOURCE;
    }
    if (0 > asprintf(&proc_session_dir, "%s%s%s",
                        job_session_dir,
                        orte_system_info.path_sep, vpid)) {
        ORTE_ERROR_LOG(ORTE_ERR_OUT_OF_RESOURCE);
        free(tmp);
        free(job);
        free(vpid);
        free(job_session_dir);
        return ORTE_ERR_OUT_OF_RESOURCE;
    }
    
    orte_dir_empty(proc_session_dir);
    orte_dir_empty(job_session_dir);
    orte_dir_empty(orte_process_info.universe_session_dir);
    orte_dir_empty(tmp);

    if (orte_is_empty(proc_session_dir)) {
    	if (orte_debug_flag) {
    	    opal_output(0, "sess_dir_finalize: found proc session dir empty - deleting");
    	}
    	rmdir(proc_session_dir);
    } else {
    	if (orte_debug_flag) {
    	    opal_output(0, "sess_dir_finalize: proc session dir not empty - leaving");
    	}
        goto CLEANUP;
    }

    if (orte_is_empty(job_session_dir)) {
    	if (orte_debug_flag) {
    	    opal_output(0, "sess_dir_finalize: found job session dir empty - deleting");
    	}
    	rmdir(job_session_dir);
    } else {
    	if (orte_debug_flag) {
    	    opal_output(0, "sess_dir_finalize: job session dir not empty - leaving");
    	}
        goto CLEANUP;
    }

    if (orte_is_empty(orte_process_info.universe_session_dir)) {
    	if (orte_debug_flag) {
    	    opal_output(0, "sess_dir_finalize: found univ session dir empty - deleting");
    	}
    	rmdir(orte_process_info.universe_session_dir);
    } else {
    	if (orte_debug_flag) {
    	    opal_output(0, "sess_dir_finalize: univ session dir not empty - leaving");
    	}
    	goto CLEANUP;
    }

    if (orte_is_empty(tmp)) {
    	if (orte_debug_flag) {
    	    opal_output(0, "sess_dir_finalize: found top session dir empty - deleting");
    	}
    	rmdir(tmp);
    } else {
    	if (orte_debug_flag) {
    	    opal_output(0, "sess_dir_finalize: top session dir not empty - leaving");
    	}
    }

CLEANUP:
    free(tmp);
    free(job);
    free(vpid);
    free(job_session_dir);
    free(proc_session_dir);
    return ORTE_SUCCESS;
}

static void
orte_dir_empty(char *pathname)
{
#ifndef WIN32
    DIR *dp;
    struct dirent *ep;
    char *filenm;
#ifndef HAVE_STRUCT_DIRENT_D_TYPE 
    int ret;
    struct stat buf;
#endif

    if (NULL == pathname) {  /* protect against error */
        return;
    }

    dp = opendir(pathname);
    if (NULL == dp) {
        return;
    }

    while (NULL != (ep = readdir(dp)) ) {
        /* skip:
         *  - . and ..
         *  - directories
         *  - files starting with "output-"
         *  - universe contact (universe-setup.txt)
         */
        if ((0 != strcmp(ep->d_name, ".")) &&
            (0 != strcmp(ep->d_name, "..")) &&
            (0 != strncmp(ep->d_name, "output-", strlen("output-"))) &&
            (0 != strcmp(ep->d_name, "universe-setup.txt"))) {

            filenm = opal_os_path(false, pathname, ep->d_name, NULL);

            /* make sure it's not a directory */
#ifdef HAVE_STRUCT_DIRENT_D_TYPE
            if (DT_DIR == ep->d_type) {
                free(filenm);
                continue;
            }
#else /* have dirent.d_type */
            ret = stat(filenm, &buf);
            if (ret < 0 || S_ISDIR(buf.st_mode)) {
                free(filenm);
                continue;
            }
#endif /* have dirent.d_type */
            unlink(filenm);
            free(filenm);
        }
    }
    closedir(dp);
#else
    bool empty = false;
    char search_path[MAX_PATH];
    HANDLE file;
    WIN32_FIND_DATA file_data;
    TCHAR *file_name;
                        
    if (NULL != pathname) {
        strncpy(search_path, pathname, strlen(pathname)+1);
        strncat (search_path, "\\*", 3);
        file = FindFirstFile(search_path, &file_data);

        if (INVALID_HANDLE_VALUE == file) {
            FindClose(&file_data);
            return;
        } 
        
        do {
            if ((0 != strcmp(file_data.cFileName, ".")) &&
                (0 != strcmp(file_data.cFileName, "..")) &&
                (!(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) &&
                (0 != strncmp(file_data.cFileName,"output-", strlen("output-"))) &&
                (0 != strcmp(file_data.cFileName,"universe-setup.txt-"))) {
                
		            file_name = opal_os_path(false, pathname, file_data.cFileName, NULL);
                    DeleteFile(file_name);

            }
            if (0 == FindNextFile(file, &file_data)) {
                    empty = true;
            }
        } while(!empty);
        FindClose(&file_data);
    }
#endif
}

/* tests if the directory is empty */
static bool orte_is_empty(char *pathname)
{
#ifndef WIN32
    DIR *dp;
    struct dirent *ep;
    if (NULL != pathname) {  /* protect against error */
    	dp = opendir(pathname);
    	if (NULL != dp) {
    	    while ((ep = readdir(dp))) {
        		if ((0 != strcmp(ep->d_name, ".")) &&
        		    (0 != strcmp(ep->d_name, ".."))) {
                            closedir(dp);
        		    return false;
        		}
    	    }
    	    closedir(dp);
    	    return true;
    	}
    	return false;
    }
    return true;
#else 
    char search_path[MAX_PATH];
    HANDLE file;
    WIN32_FIND_DATA file_data;

    if (NULL != pathname) {
        strncpy(search_path, pathname, strlen(pathname)+1);
        strncat (search_path, "\\*", 3);

        file = FindFirstFile(search_path, &file_data);
        if (INVALID_HANDLE_VALUE == file) {
            FindClose(&file_data);
            return true;
        }

        if (0 != strcmp(file_data.cFileName, ".") || 0 != strcmp(file_data.cFileName, "..")) {
            FindClose(&file_data);
            return false;
        }

        while (0 != FindNextFile(file, &file_data)) {
            if (0 != strcmp(file_data.cFileName, ".") || 0 != strcmp(file_data.cFileName, "..")) {
                FindClose(&file_data);
                return false;
            }
        }
    }

    FindClose(&file_data);
    return true;
#endif /* ifndef WIN32 */
}