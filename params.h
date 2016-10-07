#ifndef _PARAMS_H
#define _PARAMS_H

#define ROOT_ELEMS_COUNT 8
#define KERN_ELEMS_COUNT 29
#define SECURITY_ELEMS_COUNT 3
#define SECURBSD_ELEMS_COUNT 6
#define VM_ELEMS_COUNT 3
#define HW_ELEMS_COUNT 12
#define MACHDEP_ELEMS_COUNT 5
#define USER_ELEMS_COUNT 22
#define FOLDER_ELEMS_COUNT 7
#define READONLY_ELEMS_COUNT 45
#define RDWR_ELEMS_COUNT 20
#define STR_ELEMS_COUNT /*13*/ 12
#define STRUCT_ELEMS_COUNT 3
#define PARAM_ELEMS_COUNT 65

char *root[ROOT_ELEMS_COUNT] = {".", "..", "kern", "security", "vm", "hw", "machdep", "user"};

char *kern[KERN_ELEMS_COUNT] = {".", "..", "ostype", "osrelease", "osrevision", "version", "maxvnodes", "maxproc", "maxprocperuid", "maxfiles", "maxfilesperproc", "argmax", "securelevel", "hostname", "hostid", "clockrate", "posix1version", "ngroups", "job_control", "saved_ids", "boottime", "domainname", "filedelay", "dirdelay", "metadelay", "osreldate", "bootfile", "corefile", "logsigexit"};

char *secur[SECURITY_ELEMS_COUNT] = {".", "..", "bsd"};

char *securbsd[SECURBSD_ELEMS_COUNT] = {".", "..", "suser_enabled", "see_other_uids", "unprivileged_proc_debug", "unprivileged_read_msgbuf"};

char *vm[VM_ELEMS_COUNT] = {".", "..", "loadavg"};

char *hw[HW_ELEMS_COUNT] = {".", "..", "machine", "model", "ncpu", "byteorder", "physmem", "usermem", "pagesize", "floatingpoint", "machine_arch", "realmem"};

char* machdep[MACHDEP_ELEMS_COUNT] = {".", "..", "adjkerntz", "disable_rtc_set", "guessed_bootdev"};

char *user[USER_ELEMS_COUNT] = {".", "..", "cs_path", "bc_base_max", "bc_dim_max", "bc_scale_max", "bc_string_max", "coll_weights_max", "expr_nest_max", "line_max", "re_dup_max", "posix2_version", "posix2_c_bind", "posix2_c_dev", "posix2_char_term", "posix2_fort_dev", "posix2_fort_run", "posix2_localedef", "posix2_sw_dev", "posix2_upe", "stream_max", "tzname_max"};

char *folders[FOLDER_ELEMS_COUNT] = {"/kern", "/security", "/vm", "/hw", "/machdep", "/user","/security/bsd"};

char *readonly[READONLY_ELEMS_COUNT] = {"/kern/ostype", "/kern/osrelease", "/kern/osrevision","/kern/version", "/kern/maxproc", "/kern/argmax", "/kern/clockrate", "/kern/posix1version", "/kern/ngroups", "/kern/job_control", "/kern/saved_ids", "/kern/boottime", "/kern/osreldate", "/vm/loadavg", "/hw/machine", "/hw/model", "/hw/ncpu", "/hw/byteorder", "/hw/physmem", "/hw/usermem", "/hw/pagesize", "/hw/floatingpoint", "/hw/machine_arch", "/hw/realmem", "/machdep/guessed_bootdev", "/user/cs_path", "/user/bc_base_max", "/user/bc_dim_max", "/user/bc_scale_max", "/user/bc_string_max", "/user/coll_weights_max", "/user/expr_nest_max", "/user/line_max", "/user/re_dup_max", "/user/posix2_version", "/user/posix2_c_bind", "/user/posix2_c_dev", "/user/posix2_char_term", "/user/posix2_fort_dev", "/user/posix2_fort_run", "/user/posix2_localedef", "/user/posix2_sw_dev", "/user/posix2_upe", "/user/stream_max", "/user/tzname_max"};

char *rdwr[RDWR_ELEMS_COUNT] = {"/kern/maxvnodes", "/kern/maxprocperuid", "/kern/maxfiles", "/kern/maxfilesperproc", "/kern/securelevel", "/kern/hostname", "/kern/hostid", "/kern/domainname", "/kern/filedelay", "/kern/dirdelay", "/kern/metadelay", "/kern/bootfile", "/kern/corefile", "/kern/logsigexit", "/security/bsd/suser_enabled", "/security/bsd/see_other_uids", "/security/bsd/unprivileged_proc_debug", "/security/bsd/unprivileged_read_msgbuf", "/machdep/adjkerntz", "/machdep/disable_rtc_set"};

char *strret[STR_ELEMS_COUNT] = {"kern.ostype", "kern.osrelease", "kern.version", "kern.hostname", "kern.domainname", /*"kern.osreldate",*/ "kern.bootfile", "kern.corefile", "hw.machine","hw.model", "hw.machine_arch", "machdep.guessed_bootdev", "user.cs_path"};

char *structret[STRUCT_ELEMS_COUNT] = {"kern.clockrate", "kern.boottime", "vm.loadavg"};

char *params[PARAM_ELEMS_COUNT] = {"/kern/ostype", "/kern/osrelease", "/kern/osrevision", "/kern/version", "/kern/maxproc", "/kern/argmax", "/kern/clockrate", "/kern/posix1version", "/kern/ngroups", "/kern/job_control", "/kern/saved_ids", "/kern/boottime", "/kern/osreldate", "/vm/loadavg", "/hw/machine", "/hw/model", "/hw/ncpu", "/hw/byteorder", "/hw/physmem", "/hw/usermem", "/hw/pagesize", "/hw/floatingpoint", "/hw/machine_arch", "/hw/realmem", "/machdep/guessed_bootdev", "/user/cs_path", "/user/bc_base_max", "/user/bc_dim_max", "/user/bc_scale_max", "/user/bc_string_max", "/user/coll_weights_max", "/user/expr_nest_max", "/user/line_max", "/user/re_dup_max", "/user/posix2_version", "/user/posix2_c_bind", "/user/posix2_c_dev", "/user/posix2_char_term", "/user/posix2_fort_dev", "/user/posix2_fort_run", "/user/posix2_localedef", "/user/posix2_sw_dev", "/user/posix2_upe", "/user/stream_max", "/user/tzname_max", "/kern/maxvnodes","/kern/maxprocperuid", "/kern/maxfiles", "/kern/maxfilesperproc", "/kern/securelevel", "/kern/hostname", "/kern/hostid", "/kern/domainname", "/kern/filedelay", "/kern/dirdelay", "/kern/metadelay", "/kern/bootfile", "/kern/corefile", "/kern/logsigexit", "/security/bsd/suser_enabled", "/security/bsd/see_other_uids", "/security/bsd/unprivileged_proc_debug", "/security/bsd/unprivileged_read_msgbuf", "/machdep/adjkerntz", "/machdep/disable_rtc_set"};

#endif
