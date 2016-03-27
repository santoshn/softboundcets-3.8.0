//=== softboundcetsmpx-wrappers.c- SoftBoundCETS + MPX Wrappers for libraries --*- C -*===// 
// Copyright (c) 2015 Santosh Nagarakatte. All rights reserved.

// Developed by: Santosh Nagarakatte, Rutgers University
//               http://www.cs.rutgers.edu/~santosh.nagarakatte/softbound/

// The  SoftBoundCETS project had contributions from:
// Santosh Nagarakatte, Rutgers University,
// Milo M K Martin, University of Pennsylvania,
// Steve Zdancewic, University of Pennsylvania,
// Jianzhou Zhao, University of Pennsylvania


// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal with the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

//   1. Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimers.

//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimers in the
//      documentation and/or other materials provided with the distribution.

//   3. Neither the names of its developers nor the names of its
//      contributors may be used to endorse or promote products
//      derived from this software without specific prior written
//      permission.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// WITH THE SOFTWARE.
//===---------------------------------------------------------------------===//

#include <arpa/inet.h>

#if defined(__linux__)
#include<bits/errno.h>
#include<sys/wait.h>
#include <wait.h>
#include <obstack.h>
#include <libintl.h>
#endif

#include<sys/mman.h>
#include<sys/times.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<sys/socket.h>
#include<fnmatch.h>
#include <wchar.h>


#include<netinet/in.h>

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <getopt.h>
#include <glob.h>
#include <limits.h>
#include <math.h>
#include <netdb.h>
#include <pwd.h>
#include <syslog.h>
#include <setjmp.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include<stdio.h>
#include<stdlib.h>

#include <ttyent.h>
#include <time.h>
#include <unistd.h>
#include <langinfo.h>
#include <regex.h>

#ifdef HAVE_ICONV_H
#include <iconv.h>
#endif

#include <utime.h>
#include <math.h>
#include <locale.h>

#include <fcntl.h>
#include <wctype.h>


typedef size_t key_type;
typedef void* lock_type;

#include "softboundcetsmpx.h"

typedef void(*sighandler_t)(int);
typedef void(*void_func_ptr)(void);

extern size_t* __softboundcetsmpx_global_lock;

extern void __softboundcetsmpx_process_memory_total();



__WEAK_INLINE void 
__softboundcetsmpx_read_shadow_stack_metadata_store(char** endptr, int arg_num){
  

    void* nptr_base = __softboundcetsmpx_load_base_shadow_stack(arg_num);
    void* nptr_bound = __softboundcetsmpx_load_bound_shadow_stack(arg_num);
    size_t nptr_key = __softboundcetsmpx_load_key_shadow_stack(arg_num);
    void* nptr_lock = __softboundcetsmpx_load_lock_shadow_stack(arg_num);
    __softboundcetsmpx_metadata_store(endptr, nptr_base, nptr_bound, nptr_key, 
				      nptr_lock, *((void**)endptr));

}

__WEAK_INLINE void 
__softboundcetsmpx_propagate_metadata_shadow_stack_from(int from_argnum, 
                                                     int to_argnum){
  
#ifdef __SOFTBOUNDCETSMPX_SPATIAL

  void* base = __softboundcetsmpx_load_base_shadow_stack(from_argnum);
  void* bound = __softboundcetsmpx_load_bound_shadow_stack(from_argnum);
  __softboundcetsmpx_store_base_shadow_stack(base, to_argnum);
  __softboundcetsmpx_store_bound_shadow_stack(bound, to_argnum);

 
#elif __SOFTBOUNDCETSMPX_TEMPORAL

 size_t key = __softboundcetsmpx_load_key_shadow_stack(from_argnum);
 void* lock = __softboundcetsmpx_load_lock_shadow_stack(from_argnum);
 __softboundcetsmpx_store_key_shadow_stack(key, to_argnum);
 __softboundcetsmpx_store_lock_shadow_stack(lock, to_argnum);
 
#elif __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL

  void* base = __softboundcetsmpx_load_base_shadow_stack(from_argnum);
  void* bound = __softboundcetsmpx_load_bound_shadow_stack(from_argnum);
  size_t key = __softboundcetsmpx_load_key_shadow_stack(from_argnum);
  void* lock = __softboundcetsmpx_load_lock_shadow_stack(from_argnum);
  
  __softboundcetsmpx_store_base_shadow_stack(base, to_argnum);
  __softboundcetsmpx_store_bound_shadow_stack(bound, to_argnum);
  __softboundcetsmpx_store_key_shadow_stack(key, to_argnum);
  __softboundcetsmpx_store_lock_shadow_stack(lock, to_argnum);

#else

  void* base = __softboundcetsmpx_load_base_shadow_stack(from_argnum);
  void* bound = __softboundcetsmpx_load_bound_shadow_stack(from_argnum);
  size_t key = __softboundcetsmpx_load_key_shadow_stack(from_argnum);
  void* lock = __softboundcetsmpx_load_lock_shadow_stack(from_argnum);
  
  __softboundcetsmpx_store_base_shadow_stack(base, to_argnum);
  __softboundcetsmpx_store_bound_shadow_stack(bound, to_argnum);
  __softboundcetsmpx_store_key_shadow_stack(key, to_argnum);
  __softboundcetsmpx_store_lock_shadow_stack(lock, to_argnum);

#endif


}

__WEAK_INLINE void __softboundcetsmpx_store_null_return_metadata(){

#ifdef __SOFTBOUNDCETSMPX_SPATIAL

  __softboundcetsmpx_store_base_shadow_stack(NULL, 0);
  __softboundcetsmpx_store_bound_shadow_stack(NULL, 0);

#elif __SOFTBOUNDCETSMPX_TEMPORAL

  __softboundcetsmpx_store_key_shadow_stack(0, 0);
  __softboundcetsmpx_store_lock_shadow_stack(NULL, 0);

#elif __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL

  __softboundcetsmpx_store_base_shadow_stack(NULL, 0);
  __softboundcetsmpx_store_bound_shadow_stack(NULL, 0);
  __softboundcetsmpx_store_key_shadow_stack(0, 0);
  __softboundcetsmpx_store_lock_shadow_stack(NULL, 0);

#else 

  __softboundcetsmpx_store_base_shadow_stack(NULL, 0);
  __softboundcetsmpx_store_bound_shadow_stack(NULL, 0);
  __softboundcetsmpx_store_key_shadow_stack(0, 0);
  __softboundcetsmpx_store_lock_shadow_stack(NULL, 0);

#endif

}

__WEAK_INLINE void 
__softboundcetsmpx_store_return_metadata(void* base, void* bound, size_t key, 
                                      void* lock){

#ifdef __SOFTBOUNDCETSMPX_SPATIAL

  __softboundcetsmpx_store_base_shadow_stack(base, 0);
  __softboundcetsmpx_store_bound_shadow_stack(bound, 0);


#elif __SOFTBOUNDCETSMPX_TEMPORAL

  __softboundcetsmpx_store_key_shadow_stack(key, 0);
  __softboundcetsmpx_store_lock_shadow_stack(lock, 0);


#elif __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL

  __softboundcetsmpx_store_base_shadow_stack(base, 0);
  __softboundcetsmpx_store_bound_shadow_stack(bound, 0);
  __softboundcetsmpx_store_key_shadow_stack(key, 0);
  __softboundcetsmpx_store_lock_shadow_stack(lock, 0);

#else

  __softboundcetsmpx_store_base_shadow_stack(base, 0);
  __softboundcetsmpx_store_bound_shadow_stack(bound, 0);
  __softboundcetsmpx_store_key_shadow_stack(key, 0);
  __softboundcetsmpx_store_lock_shadow_stack(lock, 0);

#endif
}



/* wrappers for library calls (incomplete) */
//////////////////////system wrappers //////////////////////

__WEAK_INLINE int 
softboundcetsmpx_setenv(const char *name, const char *value, int overwrite){

  return setenv(name, value, overwrite);
}

__WEAK_INLINE 
int softboundcetsmpx_unsetenv(const char *name){
  
  return unsetenv(name);
}


__WEAK_INLINE int softboundcetsmpx_system(char* ptr){

  return system(ptr);
}

__WEAK_INLINE int softboundcetsmpx_setreuid(uid_t ruid, uid_t euid) {

  /* tested */
  return setreuid(ruid, euid);
}

__WEAK_INLINE int softboundcetsmpx_mkstemp(char* template) {
  
  /* tested */
  return mkstemp(template);
}

__WEAK_INLINE uid_t softboundcetsmpx_geteuid(){
  return geteuid();
}

__WEAK_INLINE uid_t softboundcetsmpx_getuid(void) {

  /* tested */
  return getuid();
}

__WEAK_INLINE int 
softboundcetsmpx_getrlimit(int resource, struct rlimit* rlim){

  /* tested */
  return getrlimit(resource, rlim);
}

__WEAK_INLINE int 
softboundcetsmpx_setrlimit(int resource, const struct rlimit* rlim){

  /* tested */
  return setrlimit(resource, rlim);
}

__WEAK_INLINE size_t
softboundcetsmpx_fread_unlocked(void *ptr, size_t size, 
                             size_t n, FILE *stream){
  
  return fread_unlocked(ptr, size, n, stream);
}

#if 0
__WEAK_INLINE int 
softboundcetsmpx_fputs_unlocked(const char *s, FILE *stream){
  return fputs_unlocked(s, stream);
}
#endif

__WEAK_INLINE size_t 
softboundcetsmpx_fread(void *ptr, size_t size, size_t nmemb, FILE *stream){
  /* tested */
  return fread(ptr, size, nmemb, stream);
}

__WEAK_INLINE mode_t softboundcetsmpx_umask(mode_t mask) {

  /* tested */
  return umask(mask);
}

__WEAK_INLINE int softboundcetsmpx_mkdir(const char* pathname, mode_t mode){
  
  /* tested */
  return mkdir(pathname, mode);
}

__WEAK_INLINE int softboundcetsmpx_chroot(const char* path){
  /* tested */
  return chroot(path);
}

__WEAK_INLINE int softboundcetsmpx_rmdir(const char* pathname){

  /* tested */
  return rmdir(pathname);
}

__WEAK_INLINE int softboundcetsmpx_stat(const char* path, struct stat* buf){
  /* tested */
  return stat(path, buf);
}

__WEAK_INLINE int softboundcetsmpx_fputc(int c, FILE* stream){

  /* tested */
  return fputc(c, stream);
}

__WEAK_INLINE int softboundcetsmpx_fileno(FILE* stream){

  return fileno(stream);
}

__WEAK_INLINE int softboundcetsmpx_fgetc(FILE* stream){

  return fgetc(stream);
}

__WEAK_INLINE int softboundcetsmpx_ungetc(int c,  FILE* stream){
  
  return ungetc(c, stream);
}

__WEAK_INLINE int 
softboundcetsmpx_strncmp(const char* s1, const char* s2, size_t n){
  return strncmp(s1, s2, n);
}

__WEAK_INLINE double softboundcetsmpx_log(double x) {

  return log(x);
}


__WEAK_INLINE  long long 
softboundcetsmpx_fwrite(char* ptr, size_t size, size_t nmemb, FILE* stream){
  return fwrite(ptr, size, nmemb, stream);
}

__WEAK_INLINE double softboundcetsmpx_atof(char* ptr){
  return atof(ptr);
}

__WEAK_INLINE int softboundcetsmpx_feof(FILE* stream){
  return feof(stream);
}

__WEAK_INLINE int softboundcetsmpx_remove(const char* pathname){

  return remove(pathname);
}

///////////////////// Math library functions here //////////////////

__WEAK_INLINE double softboundcetsmpx_acos(double x) {

  return acos(x);
}

__WEAK_INLINE double softboundcetsmpx_atan2(double y, double x) {

  return atan2(y, x);
}
__WEAK_INLINE float softboundcetsmpx_sqrtf(float x) {

  return sqrtf(x);
}

__WEAK_INLINE float softboundcetsmpx_expf(float x) {

  return expf(x);
}

double exp2(double);

__WEAK_INLINE double softboundcetsmpx_exp2(double x) {

  return exp2(x);
}


__WEAK_INLINE float softboundcetsmpx_floorf(float x) {

  return floorf(x);
}

__WEAK_INLINE double softboundcetsmpx_ceil(double x){

  return ceil(x);
}

__WEAK_INLINE float softboundcetsmpx_ceilf(float x) {

  return ceilf(x);
}
__WEAK_INLINE double softboundcetsmpx_floor(double x) {

  return floor(x);
}

__WEAK_INLINE double softboundcetsmpx_sqrt(double x) {

  return sqrt(x);
}

__WEAK_INLINE double softboundcetsmpx_fabs(double x) {

  return fabs(x);
}

__WEAK_INLINE int softboundcetsmpx_abs(int j){
  return abs(j);
}

__WEAK_INLINE void softboundcetsmpx_srand(unsigned int seed){
  srand(seed);
}

__WEAK_INLINE void softboundcetsmpx_srand48(long int seed){
  srand48(seed);
}


__WEAK_INLINE double softboundcetsmpx_pow(double x, double y) {
  
  return pow(x,y);

}

__WEAK_INLINE float softboundcetsmpx_fabsf(float x) {

  return fabsf(x);
}

__WEAK_INLINE double softboundcetsmpx_tan(double x ) {

  return tan(x);
}

__WEAK_INLINE float softboundcetsmpx_tanf(float x) {

  return tanf(x);
}

__WEAK_INLINE long double softboundcetsmpx_tanl(long double x) {


  return tanl(x);
}

__WEAK_INLINE double softboundcetsmpx_log10(double x) {

  return log10(x);
}
__WEAK_INLINE double softboundcetsmpx_sin(double x) {

  return sin(x);
}

__WEAK_INLINE float softboundcetsmpx_sinf(float x) {

  return sinf(x);
}

__WEAK_INLINE long double softboundcetsmpx_sinl(long double x) {

  return sinl(x);
}

__WEAK_INLINE double softboundcetsmpx_cos(double x) {

  return cos(x);
}

__WEAK_INLINE float softboundcetsmpx_cosf(float x) {

  return cosf(x);
}

__WEAK_INLINE long double softboundcetsmpx_cosl(long double x) {

  return cosl(x);
}

__WEAK_INLINE double softboundcetsmpx_exp(double x) {

  return exp(x);
}

__WEAK_INLINE double softboundcetsmpx_ldexp(double x, int exp) {
  
  return ldexp(x, exp);
}


////////////////File Library Wrappers here //////////////////////

__WEAK_INLINE FILE* softboundcetsmpx_tmpfile(void) {

  void* ret_ptr = tmpfile();
  void* ret_ptr_bound = (char*) ret_ptr + sizeof(FILE);
  __softboundcetsmpx_store_return_metadata(ret_ptr, ret_ptr_bound, 
                                        1, __softboundcetsmpx_global_lock);
  return ret_ptr;
}

__WEAK_INLINE int softboundcetsmpx_ferror(FILE* stream){

  return ferror(stream);
}

__WEAK_INLINE long softboundcetsmpx_ftell(FILE* stream){ 

  return ftell(stream);
}

__WEAK_INLINE int softboundcetsmpx_fstat(int filedes, struct stat* buff){

  return fstat(filedes, buff);
}

__WEAK_INLINE int softboundcetsmpx___lxstat (int __ver, const char *__filename,     
                                          struct stat *__stat_buf) {

  return __lxstat(__ver, __filename, __stat_buf);
}

__WEAK_INLINE size_t softboundcetsmpx_mbrtowc(wchar_t *pwc, const char *s, 
                                           size_t n, mbstate_t *ps){
  return mbrtowc(pwc, s, n, ps);
}


__WEAK_INLINE int softboundcetsmpx_mbsinit(const mbstate_t *ps){
  return mbsinit(ps);
}



__WEAK_INLINE int softboundcetsmpx___fxstat(int ver, int file_des, struct stat * stat_struct){
  return __fxstat(ver, file_des, stat_struct);
}

__WEAK_INLINE int softboundcetsmpx___fxstatat(int ver, int file_des, const char* filename, struct stat * stat_struct, int flag){
  return __fxstatat(ver, file_des, filename, stat_struct, flag);
}


__WEAK_INLINE int softboundcetsmpx_fflush(FILE* stream){

  return fflush(stream);
}

__WEAK_INLINE int softboundcetsmpx_fputs(const char* s, FILE* stream){
  
  return fputs(s, stream);
}

__WEAK_INLINE int softboundcetsmpx_fsync(int fd){
  return fsync(fd);
}

__WEAK_INLINE DIR* softboundcetsmpx_fdopendir(int fd){
  
  void* ret_ptr = (void*) fdopendir(fd);
  void* ret_ptr_bound = (char *) ret_ptr + 1024 * 1024;
    __softboundcetsmpx_store_return_metadata(ret_ptr, ret_ptr_bound, 
                                        1, (void*) __softboundcetsmpx_global_lock);
  return (DIR*)ret_ptr;
  
}

__WEAK_INLINE int softboundcetsmpx_fseeko(FILE *stream, off_t offset, int whence){
  
  return fseeko(stream, offset, whence);
}

__WEAK_INLINE  char * softboundcetsmpx_mkdtemp(char *template){
  
  char* ret_ptr = mkdtemp(template);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
}

__WEAK_INLINE int softboundcetsmpx_raise(int sig){
  return raise(sig);
}

__WEAK_INLINE int softboundcetsmpx_linkat(int olddirfd, const char *oldpath,
                                       int newdirfd, const char *newpath, 
                                       int flags){

  return  linkat(olddirfd, oldpath, newdirfd, newpath, flags);
}

__WEAK_INLINE int softboundcetsmpx_utimes(const char *filename, 
                                       const struct timeval times[2]){
  
  return utimes(filename, times);
}

#if 0
__WEAK_INLINE int softboundcetsmpx_futimesat(int dirfd, const char *pathname,
                                          const struct timeval times[2]){
  
  return futimesat(dirfd, pathname, times);
}
#endif

__WEAK_INLINE int softboundcetsmpx_futimens(int fd, const struct timespec times[2]){

  return futimens(fd, times);
}

__WEAK_INLINE int 
softboundcetsmpx_utimensat(int dirfd, const char *pathname,
                        const struct timespec times[2], int flags){

  return utimensat(dirfd, pathname, times, flags);
}

__WEAK_INLINE size_t 
softboundcetsmpx___ctype_get_mb_cur_max(void){
  return __ctype_get_mb_cur_max();  
}

__WEAK_INLINE int 
softboundcetsmpx_iswprint(wint_t wc){
  return iswprint(wc);
}

__WEAK_INLINE int 
softboundcetsmpx_getpagesize(void){
  return getpagesize();
}

__WEAK_INLINE int softboundcetsmpx_dirfd(DIR *dirp){
  return dirfd(dirp);
}

__WEAK_INLINE struct lconv *softboundcetsmpx_localeconv(void){
  struct lconv* temp = localeconv();

  __softboundcetsmpx_store_return_metadata(temp, temp + 1024, 
                                        1, (void*) __softboundcetsmpx_global_lock);
  
  return temp;
}

__WEAK_INLINE struct tm *softboundcetsmpx_gmtime(const time_t *timep){

  struct tm * temp = gmtime(timep);

  __softboundcetsmpx_store_return_metadata(temp, temp + 1024, 
                                        1, (void*) __softboundcetsmpx_global_lock);

  return temp;
}

__WEAK_INLINE void *
softboundcetsmpx_bsearch(const void *key, const void *base,
                      size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *)){
  
  void* ret_ptr = bsearch(key, base, nmemb, size, compar);

  __softboundcetsmpx_propagate_metadata_shadow_stack_from(2, 0);
    return ret_ptr;

}

__WEAK_INLINE 
struct group *softboundcetsmpx_getgrnam(const char *name){
  void* ret_ptr = getgrnam(name);
  __softboundcetsmpx_store_return_metadata(ret_ptr, (char*) ret_ptr + 1024 * 1024, 
                                        1, (void*) __softboundcetsmpx_global_lock);

  return ret_ptr;  
}

__WEAK_INLINE
int softboundcetsmpx_rpmatch(const char *response){
  return rpmatch(response);
}

__WEAK_INLINE
int softboundcetsmpx_regcomp(regex_t *preg, const char *regex, int cflags){
  return regcomp(preg, regex, cflags);
}

__WEAK_INLINE
size_t softboundcetsmpx_regerror(int errcode, const regex_t *preg, char *errbuf,
                              size_t errbuf_size){

  return regerror(errcode, preg, errbuf, errbuf_size);
}

__WEAK_INLINE
int softboundcetsmpx_regexec(const regex_t *preg, const char *string, 
                          size_t nmatch,
                          regmatch_t pmatch[], int eflags){
  return regexec(preg, string, nmatch, pmatch, eflags);
}


#ifdef HAVE_ICONV_H
__WEAK_INLINE
size_t softboundcetsmpx_iconv(iconv_t cd,
                           char **inbuf, size_t *inbytesleft,
                           char **outbuf, size_t *outbytesleft){

  return iconv(cd, inbuf, inbytesleft, outbuf, outbytesleft);
}

__WEAK_INLINE
iconv_t softboundcetsmpx_iconv_open(const char *tocode, const char *fromcode){

  return iconv_open(tocode, fromcode);
}
#endif




__WEAK_INLINE 
struct passwd * softboundcetsmpx_getpwnam(const char *name){
  void* ret_ptr = getpwnam(name);
  __softboundcetsmpx_store_return_metadata(ret_ptr, (char*) ret_ptr + 1024 * 1024, 
                                        1, (void*) __softboundcetsmpx_global_lock);

  return ret_ptr;  

  
}


__WEAK_INLINE struct passwd *softboundcetsmpx_getpwuid(uid_t uid){
  void* ret_ptr= getpwuid(uid);

  __softboundcetsmpx_store_return_metadata(ret_ptr, (char*) ret_ptr + 1024 * 1024, 
                                        1, (void*) __softboundcetsmpx_global_lock);

  return ret_ptr;  
}

__WEAK_INLINE struct group *softboundcetsmpx_getgrgid(gid_t gid){
  
  void* ret_ptr = getgrgid(gid);
  __softboundcetsmpx_store_return_metadata(ret_ptr, (char*) ret_ptr + 1024 * 1024, 
                                        1, (void*) __softboundcetsmpx_global_lock);

  return ret_ptr;  

}




__WEAK_INLINE FILE* softboundcetsmpx_fopen(const char* path, const char* mode){                                  

  void* ret_ptr = (void*) fopen(path, mode);
  void* ret_ptr_bound = (char*) ret_ptr + sizeof(FILE);

  __softboundcetsmpx_store_return_metadata(ret_ptr, ret_ptr_bound, 
                                        1, (void*) __softboundcetsmpx_global_lock);
  return (FILE*)ret_ptr;
}

__WEAK_INLINE FILE* softboundcetsmpx_fdopen(int fildes, const char* mode){

  void* ret_ptr = (void*) fdopen(fildes, mode);
  void* ret_ptr_bound = (char*) ret_ptr + sizeof(FILE);

  __softboundcetsmpx_store_return_metadata(ret_ptr, ret_ptr_bound, 
                                        1, (void*)__softboundcetsmpx_global_lock);
  return (FILE*)ret_ptr;
}


__WEAK_INLINE int softboundcetsmpx_fseek(FILE* stream, long offset, int whence){

  return fseek(stream, offset, whence);
}

__WEAK_INLINE int softboundcetsmpx_ftruncate(int fd, off_t length) {
  return ftruncate(fd, length);
}



__WEAK_INLINE FILE* softboundcetsmpx_popen(const char* command, const char* type){

  void* ret_ptr = (void*) popen(command, type);
  void* ret_ptr_bound = (char*)ret_ptr + sizeof(FILE);

  __softboundcetsmpx_store_return_metadata(ret_ptr, ret_ptr_bound, 
                                        1, (void*) __softboundcetsmpx_global_lock);  
  return (FILE*)ret_ptr;

}

__WEAK_INLINE int softboundcetsmpx_fclose(FILE* fp){
  return fclose(fp);
}


__WEAK_INLINE int softboundcetsmpx_pclose(FILE* stream){

  return pclose(stream);
}

__WEAK_INLINE void softboundcetsmpx_rewind(FILE* stream){ 
  
  rewind(stream);
}

__WEAK_INLINE struct dirent*  softboundcetsmpx_readdir(DIR* dir){

  void* ret_ptr = (void*) readdir(dir);
  void* ret_ptr_bound = (char*)ret_ptr + sizeof(struct dirent);

  __softboundcetsmpx_store_return_metadata(ret_ptr, ret_ptr_bound, 
                                        1, (void*) __softboundcetsmpx_global_lock);

  return (struct dirent*)ret_ptr;

}

__WEAK_INLINE  int
softboundcetsmpx_creat(const char *pathname, mode_t mode){
  
  return creat(pathname, mode);
}

__WEAK_INLINE int 
softboundcetsmpx_fnmatch(const char *pattern, const char *string, int flags){

  return fnmatch(pattern, string, flags);
}




__WEAK_INLINE DIR* softboundcetsmpx_opendir(const char* name){

  void* ret_ptr = opendir(name);

  /* FIX Required, don't know the sizeof(DIR) */
  void* ret_ptr_bound = (char*) ret_ptr + 1024* 1024;

  __softboundcetsmpx_store_return_metadata(ret_ptr, ret_ptr_bound, 
                                        1,  (void*)__softboundcetsmpx_global_lock);

  return (DIR*)ret_ptr;
}

__WEAK_INLINE int softboundcetsmpx_closedir(DIR* dir){

  return closedir(dir);
}

__WEAK_INLINE int 
softboundcetsmpx_rename(const char* old_path, const char* new_path){

  return rename(old_path, new_path);
}


////////////////////unistd.h wrappers ////////////////////////////////

__WEAK_INLINE unsigned int softboundcetsmpx_sleep(unsigned int seconds) {

  return sleep(seconds);
}

__WEAK_INLINE char* softboundcetsmpx_getcwd(char* buf, size_t size){ 

  if(buf == NULL) {
    printf("This case not handled, requesting memory from system\n");
    __softboundcetsmpx_abort();
  }

#ifdef __SOFTBOUNDCETSMPX_SPATIAL
  
  char* base = (char*)__softboundcetsmpx_load_base_shadow_stack(1);
  char* bound = (char*)__softboundcetsmpx_load_bound_shadow_stack(1);
  
  if (buf < base || buf + size > bound){
    __softboundcetsmpx_printf("[getcwd], overflow in buf in getcwd\n");
    __softboundcetsmpx_abort();
  }
  
#endif

#ifdef __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL
  char* base = (char *)__softboundcetsmpx_load_base_shadow_stack(1);
  char* bound = (char *) __softboundcetsmpx_load_bound_shadow_stack(1);
  
  if (buf < base || buf + size > bound){
    __softboundcetsmpx_printf("[getcwd], overflow in buf in getcwd\n");
    __softboundcetsmpx_abort();
  }


#endif

  char* ret_ptr = getcwd(buf, size);

  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);

  return ret_ptr;
}

__WEAK_INLINE int softboundcetsmpx_setgid(gid_t gid){
  return setgid(gid);
}



__WEAK_INLINE gid_t softboundcetsmpx_getgid(void){
  return getgid();
}

__WEAK_INLINE gid_t softboundcetsmpx_getegid(void){
  return getegid();
}
__WEAK_INLINE
int softboundcetsmpx_readlinkat(int dirfd, const char *pathname,
                             char *buf, size_t bufsiz){
  return readlinkat(dirfd, pathname, buf, bufsiz);
}

__WEAK_INLINE
int softboundcetsmpx_renameat(int olddirfd, const char *oldpath,
                           int newdirfd, const char *newpath){
  return renameat(olddirfd, oldpath, newdirfd, newpath);
}

__WEAK_INLINE 
int softboundcetsmpx_unlinkat(int dirfd, const char *pathname, int flags){
  return unlinkat(dirfd, pathname, flags);
}

__WEAK_INLINE
int softboundcetsmpx_symlinkat(const char *oldpath, int newdirfd, 
                            const char *newpath){

  return symlinkat(oldpath, newdirfd, newpath);
}

__WEAK_INLINE
int softboundcetsmpx_mkdirat(int dirfd, const char *pathname, mode_t mode){

  return mkdirat(dirfd, pathname, mode);
}

__WEAK_INLINE
int softboundcetsmpx_fchown(int fd, uid_t owner, gid_t group){
  return fchown(fd, owner, group);
}

__WEAK_INLINE
int softboundcetsmpx_fchownat(int dirfd, const char *pathname,
                           uid_t owner, gid_t group, int flags){
  
  return fchownat(dirfd, pathname, owner, group, flags);
}

__WEAK_INLINE
int softboundcetsmpx_fchmod(int fd, mode_t mode){
  return fchmod(fd, mode);
}

__WEAK_INLINE
int softboundcetsmpx_chmod(const char *path, mode_t mode){
  return chmod(path, mode);
}

__WEAK_INLINE 
int softboundcetsmpx_openat(int dirfd, const char *pathname, int flags){
  return openat(dirfd, pathname, flags);
}

__WEAK_INLINE
int softboundcetsmpx_fchmodat(int dirfd, const char *pathname, 
                           mode_t mode, int flags){
  return fchmodat(dirfd, pathname, mode, flags);
}

#if defined (__linux__)

__WEAK_INLINE 
int softboundcetsmpx___xmknodat (int __ver, int __fd, const char *__path,
                              __mode_t __mode, __dev_t *__dev){
 return  __xmknodat(__ver, __fd, __path, __mode, __dev);
}

__WEAK_INLINE
int softboundcetsmpx_mkfifoat(int dirfd, const char *pathname, mode_t mode){
  return mkfifoat(dirfd, pathname, mode);
}

#endif

__WEAK_INLINE 
pid_t softboundcetsmpx_getpid(void){
  return getpid();
}

__WEAK_INLINE pid_t softboundcetsmpx_getppid(void){
  return getppid();
}


#if 0
__WEAK_INLINE 
int softboundcetsmpx_openat(int dirfd, const char *pathname, int flags, mode_t mode){
  return opennat(dirfd, pathname, flags, mode);
}
#endif



__WEAK_INLINE int 
softboundcetsmpx_chown(const char* path, uid_t owner, gid_t group){  
  return chown(path, owner, group);
  
}

__WEAK_INLINE
wint_t softboundcetsmpx_towlower(wint_t wc){
  return towlower(wc);
}



__WEAK_INLINE int softboundcetsmpx_isatty(int desc) {

  return isatty(desc);
}

__WEAK_INLINE int softboundcetsmpx_chdir(const char* path){
  return chdir(path);
}

__WEAK_INLINE
int softboundcetsmpx_fchdir(int fd){
  return fchdir(fd);
}


///////////////////String related wrappers ////////////////////////////


__WEAK_INLINE int softboundcetsmpx_strcmp(const char* s1, const char* s2){
  
  return strcmp(s1, s2);
}


__WEAK_INLINE int softboundcetsmpx_strcasecmp(const char* s1, const char* s2){

  return strcasecmp(s1,s2);
}

__WEAK_INLINE int 
softboundcetsmpx_strncasecmp(const char* s1, const char* s2, size_t n){
  return strncasecmp(s1, s2, n);
}

__WEAK_INLINE size_t softboundcetsmpx_strlen(const char* s){

  return strlen(s);
}

__WEAK_INLINE char* softboundcetsmpx_strpbrk(const char* s, const char* accept){ 

  char* ret_ptr = strpbrk(s, accept);
  if(ret_ptr != NULL) {

    __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  }
  else {

    __softboundcetsmpx_store_null_return_metadata();
  }

  return ret_ptr;
}



__WEAK_INLINE char* softboundcetsmpx_gets(char* s){ 

  printf("[SBCETS] gets used and should not be used\n");
  __softboundcetsmpx_abort();
#if 0
  printf("[Softboundcetsmpx][Warning] Should not use gets\n");
  char* ret_ptr = gets(s);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
#endif
  return NULL;

}

__WEAK_INLINE char* softboundcetsmpx_fgets(char* s, int size, FILE* stream){

  char* ret_ptr = fgets(s, size, stream);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1,0);

  return ret_ptr;
}


__WEAK_INLINE void softboundcetsmpx_perror(const char* s){
  perror(s);
}

__WEAK_INLINE size_t softboundcetsmpx_strspn(const char* s, const char* accept){
  
  return strspn(s, accept);
}

__WEAK_INLINE size_t softboundcetsmpx_strcspn(const char* s, const char* reject){
  
  return strcspn(s, reject);
}

#ifdef _GNU_SOURCE

__WEAK_INLINE void* softboundcetsmpx_mempcpy(void * dest, const void * src, size_t n){

  // IMP: need to copy the metadata 
  void* ret_ptr = mempcpy(dest, src, n);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1,0);
  return ret_ptr;
}

#endif

__WEAK_INLINE int 
softboundcetsmpx_memcmp(const void* s1, const void* s2, size_t n){
  return memcmp(s1, s2, n);
}

#ifdef _GNU_SOURCE

__WEAK_INLINE void* softboundcetsmpx_memrchr(const void * s, int c, size_t n){  
  void* ret_ptr = memrchr(s, c, n);
  if(ret_ptr != NULL) {
    __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  }
  else{
    __softboundcetsmpx_store_null_return_metadata();
  }
  return ret_ptr;
}
#endif

__WEAK_INLINE void softboundcetsmpx_rewinddir(DIR *dirp){
  rewinddir(dirp);  
}



__WEAK_INLINE void* softboundcetsmpx_memchr(const void * s, int c, size_t n){  
  void* ret_ptr = memchr(s, c, n);
  if(ret_ptr != NULL) {
    __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  }
  else{
    __softboundcetsmpx_store_null_return_metadata();
  }
  return ret_ptr;
}

__WEAK_INLINE char* softboundcetsmpx_rindex(char* s, int c){

  char* ret_ptr = rindex(s,c);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1,0);
  return ret_ptr;
}

__WEAK_INLINE ssize_t 
softboundcetsmpx___getdelim(char **lineptr, size_t *n, int delim, FILE *stream){
  
  int metadata_prop = 1;
  if(*lineptr == NULL){
    metadata_prop = 0;
  }

  ssize_t ret_val = getdelim(lineptr, n, delim, stream);
  
  if(metadata_prop){
    __softboundcetsmpx_read_shadow_stack_metadata_store(lineptr, 1);
  }
  else{
    __softboundcetsmpx_store_return_metadata(*lineptr, 
                                          (*lineptr) + strlen(*lineptr),
                                          1, __softboundcetsmpx_global_lock);
  }
  
  return ret_val;
  

}

__WEAK_INLINE unsigned long int 
softboundcetsmpx_strtoul(const char* nptr, char ** endptr, int base){

  unsigned long temp = strtoul(nptr, endptr, base);
  if(endptr != NULL){
    __softboundcetsmpx_read_shadow_stack_metadata_store(endptr, 1);
    
  }

  return temp;
}

__WEAK_INLINE double softboundcetsmpx_strtod(const char* nptr, char** endptr){

  double temp = strtod(nptr, endptr);
  
  if(endptr != NULL) {
    __softboundcetsmpx_read_shadow_stack_metadata_store(endptr, 1);
  }
  return temp;
 }
 
__WEAK_INLINE long 
softboundcetsmpx_strtol(const char* nptr, char **endptr, int base){
 
   long temp = strtol(nptr, endptr, base);
   if(endptr != NULL) {
     //    __softboundcetsmpx_printf("*endptr=%p\n", *endptr);
     __softboundcetsmpx_read_shadow_stack_metadata_store(endptr, 1);
  }
  return temp;
}

#ifdef _GNU_SOURCE

__WEAK_INLINE char* softboundcetsmpx_strchrnul(const char* s, int c){

  char* ret_ptr = strchrnul(s, c);
   __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
   return ret_ptr;
}
#endif

__WEAK_INLINE char* softboundcetsmpx_strchr(const char* s, int c){

  char* ret_ptr = strchr(s, c);
   __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
   return ret_ptr;
}

__WEAK_INLINE char* softboundcetsmpx_strrchr(const char* s, int c){

  char* ret_ptr = strrchr(s, c);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
}

__WEAK_INLINE char* softboundcetsmpx_stpcpy(char* dest, char* src){

  void* ret_ptr = stpcpy(dest, src);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
}

__WEAK_INLINE char* softboundcetsmpx_strcpy(char* dest, char* src){
  
#ifdef __SOFTBOUNDCETSMPX_SPATIAL  
  char* dest_base = __softboundcetsmpx_load_base_shadow_stack(1);
  char* dest_bound = __softboundcetsmpx_load_bound_shadow_stack(1);

  char* src_base = __softboundcetsmpx_load_base_shadow_stack(2);
  char* src_bound = __softboundcetsmpx_load_bound_shadow_stack(2);

  /* There will be an out-of-bound read before we trigger an error as
     we currently use strlen. Can either (dest + size) or (src + size)
     overflow?
  */
  size_t size = strlen(src);
  if(dest < dest_base || (dest > dest_bound - size -1) || (size > (size_t) dest_bound)){
    printf("[strcpy] overflow in strcpy with dest\n");
    __softboundcetsmpx_abort();
  }  
  if(src < src_base || (src > src_bound -size -1) || (size > (size_t) src_bound)){
    printf("[strcpy] overflow in strcpy with src\n");
    __softboundcetsmpx_abort();
  }
#endif

#ifdef __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL
  
  char* dest_base = __softboundcetsmpx_load_base_shadow_stack(1);
  char* dest_bound = __softboundcetsmpx_load_bound_shadow_stack(1);

  char* src_base = __softboundcetsmpx_load_base_shadow_stack(2);
  char* src_bound = __softboundcetsmpx_load_bound_shadow_stack(2);

  /* There will be an out-of-bound read before we trigger an error as
     we currently use strlen. Can either (dest + size) or (src + size)
     overflow?
  */
#ifndef __NOSIM_CHECKS
  size_t size = strlen(src);
  if(dest < dest_base || (dest > dest_bound - size -1) || (size > (size_t) dest_bound)){
    printf("[strcpy] overflow in strcpy with dest\n");
    __softboundcetsmpx_abort();
  }  
  if(src < src_base || (src > src_bound -size -1 ) || (size > (size_t) src_bound)){
    printf("[strcpy] overflow in strcpy with src\n");
    __softboundcetsmpx_abort();
  }
#endif
#endif
        
  void * ret_ptr = strcpy(dest, src);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
}


__WEAK_INLINE void softboundcetsmpx_abort() {
  abort();
}

 __WEAK_INLINE int softboundcetsmpx_rand() {
   return rand();
 }
 
 /////////////////* TODO *///////////////////////////

 __WEAK_INLINE int softboundcetsmpx_atoi(const char* ptr){

  if(ptr == NULL) {
    __softboundcetsmpx_abort();
  }
  return atoi(ptr);    
 }
 
 __WEAK_INLINE void softboundcetsmpx_puts(char* ptr){
   puts(ptr);
 }


__WEAK_INLINE void softboundcetsmpx_exit(int status) {
  
  exit(status);
}

__WEAK_INLINE char*  softboundcetsmpx_strtok(char* str, const char* delim){
  
  char* ret_ptr = strtok(str, delim);   
  __softboundcetsmpx_store_return_metadata((void*)0, (void*)(281474976710656), 
                                        1, __softboundcetsmpx_global_lock);
  return ret_ptr;
}

__WEAK_INLINE void __softboundcetsmpx_strdup_handler(void* ret_ptr){
  key_type ptr_key;
  lock_type ptr_lock;
  
  if(ret_ptr == NULL) {
    __softboundcetsmpx_store_null_return_metadata();
  }
  else {
    //    printf("strndup malloced pointer %p\n", ret_ptr);    
    __softboundcetsmpx_memory_allocation(ret_ptr, &ptr_lock, &ptr_key);
    __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                          (void*)
                                          ((char*)ret_ptr + strlen(ret_ptr) + 1), 
                                          ptr_key, ptr_lock); 
  } 
}

//strdup, allocates memory from the system using malloc, thus can be freed
__WEAK_INLINE char* softboundcetsmpx_strndup(const char* s, size_t n){
  
  /* IMP: strndup just copies the string s */  
  char* ret_ptr = strndup(s, n);
  __softboundcetsmpx_strdup_handler(ret_ptr);  
  return ret_ptr;
 }


//strdup, allocates memory from the system using malloc, thus can be freed
__WEAK_INLINE char* softboundcetsmpx_strdup(const char* s){
  
  /* IMP: strdup just copies the string s */  
  void* ret_ptr = strdup(s);
  
  __softboundcetsmpx_strdup_handler(ret_ptr);
  return ret_ptr;
 }

__WEAK_INLINE char* softboundcetsmpx___strdup(const char* s){

  void* ret_ptr = strdup(s);
  __softboundcetsmpx_strdup_handler(ret_ptr);
  return ret_ptr;
}


 __WEAK_INLINE char* softboundcetsmpx_strcat (char* dest, const char* src){

#if 0
  if(dest + strlen(dest) + strlen(src) > dest_bound){
    printf("overflow with strcat, dest = %p, strlen(dest)=%d, 
            strlen(src)=%d, dest_bound=%p \n", 
           dest, strlen(dest), strlen(src), dest_bound);
    __softboundcetsmpx_abort();
  } 
#endif
  
  char* ret_ptr = strcat(dest, src);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
}

__WEAK_INLINE char* 
softboundcetsmpx_strncat (char* dest,const char* src, size_t n){

  char* ret_ptr = strncat(dest, src, n);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
}

__WEAK_INLINE char* 
softboundcetsmpx_strncpy(char* dest, char* src, size_t n){
 
#ifdef __SOFTBOUNDCETSMPX_SPATIAL  
  char* dest_base = __softboundcetsmpx_load_base_shadow_stack(1);
  char* dest_bound = __softboundcetsmpx_load_bound_shadow_stack(1);

  char* src_base = __softboundcetsmpx_load_base_shadow_stack(2);
  char* src_bound = __softboundcetsmpx_load_bound_shadow_stack(2);

  /* Can either (dest + n) or (src + n) overflow? */
  if(dest < dest_base || (dest > dest_bound - n) || (n > (size_t) dest_bound)){
    printf("[strncpy] overflow in strncpy with dest\n");
    __softboundcetsmpx_abort();
  }  
  if(src < src_base || (src > src_bound -n) || (n > (size_t) src_bound)){
    __softboundcetsmpx_abort();
  }
#endif

#ifdef __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL

#ifdef __SOFTBOUNDCETSMPX_WRAPPER_CHECKS

  char* dest_base = __softboundcetsmpx_load_base_shadow_stack(1);
  char* dest_bound = __softboundcetsmpx_load_bound_shadow_stack(1);

  char* src_base = __softboundcetsmpx_load_base_shadow_stack(2);
  char* src_bound = __softboundcetsmpx_load_bound_shadow_stack(2);

  /* Can either (dest + n) or (src + n) overflow? */
  if(dest < dest_base || dest + n > dest_bound){
    printf("[strncpy] overflow in strncpy with dest\n");
    __softboundcetsmpx_abort();
  }  
  if(src < src_base || src + n > src_bound){
    //    printf("[strncpy] overflow in strncpy with src, src=%zx, src_base=%zx, src_bound=%zx\n", src, src_base, src_bound);
    __softboundcetsmpx_abort();
  }
#endif
#endif
  
  char* ret_ptr = strncpy(dest, src, n);
  __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  return ret_ptr;
}

__WEAK_INLINE char* 
softboundcetsmpx_strstr(const char* haystack, const char* needle){
  
  char* ret_ptr = strstr(haystack, needle);
  if(ret_ptr != NULL) {    
    __softboundcetsmpx_propagate_metadata_shadow_stack_from(1, 0);
  }
  else {
    __softboundcetsmpx_store_null_return_metadata();
  }
  return ret_ptr;
}

__WEAK_INLINE sighandler_t 
softboundcetsmpx_signal(int signum, sighandler_t handler){

  sighandler_t ptr = signal(signum, handler);
  __softboundcetsmpx_store_return_metadata((void*)ptr, (void*) ptr, 
                                        1, __softboundcetsmpx_global_lock);
  return ptr;
}

__WEAK_INLINE clock_t softboundcetsmpx_clock(void){
  return clock();
}


__WEAK_INLINE long softboundcetsmpx_atol(const char* nptr){ 
  return atol(nptr);
}

__WEAK_INLINE void* softboundcetsmpx_realloc(void* ptr, size_t size){
  
#if 0
  /* TODO: may be necessary to copy metadata */
   printf("performing relloc, which can cause ptr=%p\n", ptr);
#endif
   void* ret_ptr = realloc(ptr, size);
   size_t ptr_key = 1;
   void* ptr_lock = __softboundcetsmpx_global_lock;

#ifdef __SOFTBOUNDCETSMPX_TEMPORAL
   ptr_key = __softboundcetsmpx_load_key_shadow_stack(1);
   ptr_lock = __softboundcetsmpx_load_lock_shadow_stack(1);
#elif __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL
   ptr_key = __softboundcetsmpx_load_key_shadow_stack(1);
   ptr_lock = __softboundcetsmpx_load_lock_shadow_stack(1);
#else
   ptr_key = __softboundcetsmpx_load_key_shadow_stack(1);
   ptr_lock = __softboundcetsmpx_load_lock_shadow_stack(1);
#endif

   __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                         (char*)(ret_ptr) + size, 
                                         ptr_key, ptr_lock);
   if(ret_ptr != ptr){
     __softboundcetsmpx_check_remove_from_free_map(ptr_key, ptr);
     __softboundcetsmpx_add_to_free_map(ptr_key, ret_ptr);
     __softboundcetsmpx_copy_metadata(ret_ptr, ptr, size);
   }
   
   return ret_ptr;
 }

 __WEAK_INLINE void* softboundcetsmpx_calloc(size_t nmemb, size_t size) {
   
   key_type ptr_key = 1 ;
   lock_type  ptr_lock = NULL;
  
   void* ret_ptr = calloc(nmemb, size);   
   if(ret_ptr != NULL) {    

#ifdef __SOFTBOUNDCETSMPX_TEMPORAL     
     __softboundcetsmpx_memory_allocation(ret_ptr, &ptr_lock, &ptr_key);     
#elif __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL
     __softboundcetsmpx_memory_allocation(ret_ptr, &ptr_lock, &ptr_key);     
#endif

     __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                           ((char*)(ret_ptr) + (nmemb * size)), 
                                           ptr_key, ptr_lock); 

     if(__SOFTBOUNDCETSMPX_FREE_MAP) {
#if 0
       __softboundcetsmpx_printf("calloc ptr=%p, ptr_key=%zx\n", 
                              ret_ptr, ptr_key);
#endif
       //       __softboundcetsmpx_add_to_free_map(ptr_key, ret_ptr);
     }
   }
   else{
     __softboundcetsmpx_store_null_return_metadata();
   } 
   return ret_ptr;
 }

__WEAK_INLINE void* softboundcetsmpx_mmap(void* addr, size_t length, 
                                       int prot, int flags, int fd, 
                                       off_t offset){

  key_type ptr_key=1;
  lock_type ptr_lock=__softboundcetsmpx_global_lock;
  char* ret_ptr = mmap(addr, length, prot, flags, fd, offset);
  if(ret_ptr == (void*) -1){
    __softboundcetsmpx_store_null_return_metadata();
  }
  else{

    char* ret_bound = ret_ptr + length;
    __softboundcetsmpx_store_return_metadata(ret_ptr, ret_bound, 
                                          ptr_key, ptr_lock);
    
  }
  return ret_ptr;
}
 
__WEAK_INLINE void* softboundcetsmpx_malloc(size_t size) {
  
  key_type ptr_key=1;
  lock_type ptr_lock=NULL;

  char* ret_ptr = (char*)malloc(size);
  if(ret_ptr == NULL){
    __softboundcetsmpx_store_null_return_metadata();
  }
  else{

#ifdef __SOFTBOUNDCETSMPX_TEMPORAL 
    __softboundcetsmpx_memory_allocation(ret_ptr, &ptr_lock, &ptr_key);
#elif __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL
    __softboundcetsmpx_memory_allocation(ret_ptr, &ptr_lock, &ptr_key);
#endif

    char* ret_bound = ret_ptr + size;
    __softboundcetsmpx_store_return_metadata(ret_ptr, ret_bound, 
                                          ptr_key, ptr_lock);

    if(__SOFTBOUNDCETSMPX_FREE_MAP) {
#if 0
       __softboundcetsmpx_printf("malloc ptr=%p, ptr_key=%zx\n", 
                              ret_ptr, ptr_key);
#endif
       //      __softboundcetsmpx_add_to_free_map(ptr_key, ret_ptr);
    }
  }
  return ret_ptr;
}


__WEAK_INLINE int softboundcetsmpx_putchar(int c) {

  return putchar(c);
}


 __WEAK_INLINE clock_t softboundcetsmpx_times(struct tms* buf){
  return times(buf);
}

__WEAK_INLINE size_t 
softboundcetsmpx_strftime(char* s, size_t max, 
                       const char* format, const struct tm *tm){
  
  return strftime(s, max, format, tm);
}


__WEAK_INLINE time_t softboundcetsmpx_mktime(struct tm *tm){
  return mktime(tm);
}

__WEAK_INLINE long softboundcetsmpx_pathconf(char *path, int name){
  return pathconf(path, name);
}


__WEAK_INLINE struct tm* softboundcetsmpx_localtime(const time_t* timep){

  struct tm * ret_ptr = localtime(timep);
  __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                        (char*)ret_ptr + sizeof(struct tm), 
                                        1, __softboundcetsmpx_global_lock); 
  return ret_ptr;
}

 __WEAK_INLINE time_t softboundcetsmpx_time(time_t* t){

   return time(t);
 }

__WEAK_INLINE double softboundcetsmpx_drand48(){

  return drand48();
}

__WEAK_INLINE void softboundcetsmpx_free(void* ptr){
  /* more checks required to check if it is a malloced address */
#ifdef __SOFTBOUNDCETSMPX_TEMPORAL 
  if(ptr != NULL){
    void* ptr_lock = __softboundcetsmpx_load_lock_shadow_stack(1);
    size_t ptr_key = __softboundcetsmpx_load_key_shadow_stack(1);
    
    __softboundcetsmpx_memory_deallocation(ptr_lock, ptr_key);
    
    if(__SOFTBOUNDCETSMPX_FREE_MAP){
#if 0
      __softboundcetsmpx_printf("free =%p, ptr_key=%zx\n", ptr, ptr_key);
#endif
      __softboundcetsmpx_check_remove_from_free_map(ptr_key, ptr);
    }
  }  
#endif  

#ifdef __SOFTBOUNDCETSMPX_SPATIAL_TEMPORAL
  if(ptr != NULL){
    void* ptr_lock = __softboundcetsmpx_load_lock_shadow_stack(1);
    size_t ptr_key = __softboundcetsmpx_load_key_shadow_stack(1);
    __softboundcetsmpx_memory_deallocation(ptr_lock, ptr_key);
     
    if(__SOFTBOUNDCETSMPX_FREE_MAP){
#if 0
      __softboundcetsmpx_printf("free =%p, ptr_key=%zx\n", ptr, ptr_key);
#endif
      __softboundcetsmpx_check_remove_from_free_map(ptr_key, ptr);
    }
  }
#endif
   free(ptr);
}


__WEAK_INLINE long int softboundcetsmpx_lrand48(){
  return lrand48();
}


/* ////////////////////Time Related Library Wrappers///////////////////////// */


__WEAK_INLINE char* softboundcetsmpx_ctime( const time_t* timep){
  
  char* ret_ptr = ctime(timep);

  if(ret_ptr == NULL){
    __softboundcetsmpx_store_null_return_metadata();
  }
  else {
    __softboundcetsmpx_store_return_metadata(ret_ptr, ret_ptr + strlen(ret_ptr) + 1, 
                                          1, __softboundcetsmpx_global_lock);
  }
  return ret_ptr;

}



__WEAK_INLINE double softboundcetsmpx_difftime(time_t time1, time_t time0) {  
  return difftime(time1, time0);
}

__WEAK_INLINE int softboundcetsmpx_toupper(int c) {

  return toupper(c);
}

__WEAK_INLINE int softboundcetsmpx_tolower(int c){

  return tolower(c);
}

__WEAK_INLINE void softboundcetsmpx_setbuf(FILE* stream, char* buf){  
  setbuf(stream, buf);
}

__WEAK_INLINE char* softboundcetsmpx_getenv(const char* name){
   
  char* ret_ptr = getenv(name);
   
  if(ret_ptr != NULL){
    __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                          ret_ptr + strlen(ret_ptr) + 1, 
                                          1, __softboundcetsmpx_global_lock);
  }
  else {
    __softboundcetsmpx_store_null_return_metadata();
  }

  return ret_ptr;
}

__WEAK_INLINE int softboundcetsmpx_atexit(void_func_ptr function){
  return atexit(function);

}

#ifdef _GNU_SOURCE
__WEAK_INLINE char* softboundcetsmpx_strerror_r(int errnum, char* buf, 
                                             size_t buf_len) {

  void* ret_ptr = strerror_r(errnum, buf, buf_len);
  __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                        (void*)
                                        ((char*)ret_ptr + strlen(ret_ptr) +1),
                                        1, __softboundcetsmpx_global_lock);
  return ret_ptr;
}
#endif

__WEAK_INLINE char* softboundcetsmpx_strerror(int errnum) {

  void* ret_ptr = strerror(errnum);
  __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                        (void*)
                                        ((char*)ret_ptr + strlen(ret_ptr) +1),
                                        1, __softboundcetsmpx_global_lock);
  return ret_ptr;
}


__WEAK_INLINE int softboundcetsmpx_unlink(const char* pathname){
  return unlink(pathname);
}


__WEAK_INLINE int softboundcetsmpx_close(int fd) {

  return close(fd);
}


__WEAK_INLINE int softboundcetsmpx_open(const char *pathname, int flags){  
  return open(pathname, flags);

}

__WEAK_INLINE ssize_t softboundcetsmpx_read(int fd, void* buf, size_t count){
  
  return read(fd, buf, count);
}

__WEAK_INLINE ssize_t softboundcetsmpx_write(int fd, void* buf, size_t count){
  return write(fd, buf, count);
}


__WEAK_INLINE off_t softboundcetsmpx_lseek(int fildes, off_t offset, int whence) {

  return lseek(fildes, offset, whence);
}


__WEAK_INLINE int 
softboundcetsmpx_gettimeofday(struct timeval* tv, struct timezone* tz){
  return gettimeofday(tv, tz);
}


__WEAK_INLINE int 
softboundcetsmpx_select(int nfds, fd_set* readfds, fd_set* writefds,
                     fd_set* exceptfds, struct timeval* timeout){
  return select(nfds, readfds, writefds, exceptfds, timeout);
}

#if defined (__linux__)

__WEAK_INLINE char* 
softboundcetsmpx_setlocale(int category, const char* locale){
  
  void* ret_ptr = setlocale(category, locale);
  __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                        (void*) 
                                        ((char*) ret_ptr+ strlen(ret_ptr)),
                                        1, __softboundcetsmpx_global_lock);
  return ret_ptr;  

}

__WEAK_INLINE char*
softboundcetsmpx_textdomain(const char* domainname){
  
  void* ret_ptr = textdomain(domainname);
  __softboundcetsmpx_store_return_metadata(ret_ptr,
                                        (void *)
                                        ((char*) ret_ptr + strlen(ret_ptr)),
                                        1, __softboundcetsmpx_global_lock);
  
  return ret_ptr;
  
}


__WEAK_INLINE char*
softboundcetsmpx_bindtextdomain(const char* domainname, const char* dirname){
  
  void* ret_ptr = bindtextdomain(domainname, dirname);
  __softboundcetsmpx_store_return_metadata(ret_ptr,
                                        (void *)
                                        ((char*) ret_ptr + strlen(ret_ptr)),
                                        1, __softboundcetsmpx_global_lock);

  return ret_ptr;

}

__WEAK_INLINE char * 
softboundcetsmpx_gettext(const char * msgid){
  
  void* ret_ptr = gettext(msgid);
  __softboundcetsmpx_store_return_metadata(ret_ptr,
                                        (void*)
                                        ((char*) ret_ptr + strlen(ret_ptr)),
                                        1, __softboundcetsmpx_global_lock);

  return ret_ptr;  

}


__WEAK_INLINE char * 
softboundcetsmpx_dcngettext (const char * domainname,
                          const char * msgid, const char * msgid_plural,
                          unsigned long int n, int category){
  
  void* ret_ptr = dcngettext(domainname, msgid, msgid_plural, n, category);
  __softboundcetsmpx_store_return_metadata(ret_ptr,
                                        (void*)
                                        ((char*) ret_ptr + strlen(ret_ptr)),
                                        1, __softboundcetsmpx_global_lock);

  return ret_ptr;  
    
}


/* IMP: struct hostent may have pointers in the structure being returned,
   we need to store the metadata for all those pointers */
__WEAK_INLINE 
struct hostent * softboundcetsmpx_gethostbyname(const char *name){
  
  struct hostent * ret_ptr = gethostbyname(name);

  void* ret_bound = ret_ptr + sizeof(struct hostent);
  __softboundcetsmpx_store_return_metadata(ret_ptr,
                                        ret_bound,
                                        1, __softboundcetsmpx_global_lock);
  
  return ret_ptr;  
}



__WEAK_INLINE char*
softboundcetsmpx_dcgettext (const char * domainname, 
                         const char * msgid,
                         int category) {

  void* ret_ptr = dcgettext(domainname, msgid, category);
  __softboundcetsmpx_store_return_metadata(ret_ptr,
                                        (void*)
                                        ((char*) ret_ptr + strlen(ret_ptr)),
                                        1, __softboundcetsmpx_global_lock);

  return ret_ptr;  
  
}

#endif

#if defined(__linux__)
__WEAK_INLINE int* softboundcetsmpx___errno_location() {
  void* ret_ptr = (int *)__errno_location();
  //  printf("ERRNO: ptr is %lx", ptrs->ptr);
  __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                        (void*)((char*)ret_ptr + sizeof(int*)),
                                        1, __softboundcetsmpx_global_lock);
  
  return ret_ptr;
}

__WEAK_INLINE unsigned short const** 
softboundcetsmpx___ctype_b_loc(void) {

  unsigned short const** ret_ptr =__ctype_b_loc();
  __softboundcetsmpx_store_return_metadata((void*) ret_ptr, 
                                        (void*)
                                        ((char*) ret_ptr + 1024*1024), 
                                        1, __softboundcetsmpx_global_lock);
  return ret_ptr;
}

__WEAK_INLINE int const**  softboundcetsmpx___ctype_toupper_loc(void) {
  
  int const ** ret_ptr  =  __ctype_toupper_loc();  
  __softboundcetsmpx_store_return_metadata((void*) ret_ptr, 
                                        (void*)
                                        ((char*)ret_ptr + 1024*1024), 
                                        1, __softboundcetsmpx_global_lock);
  return ret_ptr;

}


__WEAK_INLINE int const**  softboundcetsmpx___ctype_tolower_loc(void) {
  
  int const ** ret_ptr  =  __ctype_tolower_loc();  
  __softboundcetsmpx_store_return_metadata((void*) ret_ptr, 
                                        (void*) ((char*)ret_ptr + 1024*1024),
                                        1, __softboundcetsmpx_global_lock);
  return ret_ptr;

}
#endif

#if 0
/* This is a custom implementation of qsort */

static int 
compare_elements_helper(void* base, size_t element_size, 
                        int idx1, int idx2, 
                        int (*comparer)(const void*, const void*)){
  
  char* base_bytes = base;
  return comparer(&base_bytes[idx1 * element_size], 
                  &base_bytes[idx2*element_size]);
}

#define element_less_than(i,j) (compare_elements_helper(base, element_size, (i), (j), comparer) < 0)

static void 
exchange_elements_helper(void* base, size_t element_size, 
                         int idx1, int idx2){

  char* base_bytes = base;
  size_t i;

  for (i=0; i < element_size; i++){
    char temp = base_bytes[idx1* element_size + i];
    base_bytes[idx1 * element_size + i] = base_bytes[idx2 * element_size + i];
    base_bytes[idx2 * element_size + i] = temp;
  }  

  for(i=0; i < element_size; i+= 8){
    void* base_idx1;
    void* bound_idx1;

    void* base_idx2;
    void* bound_idx2;

    size_t key_idx1=1;
    size_t key_idx2=1;

    void* lock_idx1=NULL;
    void* lock_idx2=NULL;

    char* addr_idx1 = &base_bytes[idx1 * element_size + i];
    char* addr_idx2 = &base_bytes[idx2 * element_size + i];
    
    __softboundcetsmpx_metadata_load(addr_idx1, &base_idx1, &bound_idx1, 
				     &key_idx1, &lock_idx1);
    __softboundcetsmpx_metadata_load(addr_idx2, &base_idx2, &bound_idx2, 
                                  &key_idx2, &lock_idx2);

    __softboundcetsmpx_metadata_store(addr_idx1, base_idx2, bound_idx2, 
                                   key_idx2, lock_idx2);
    __softboundcetsmpx_metadata_store(addr_idx2, base_idx1, bound_idx1, 
                                   key_idx1, lock_idx1);
        
  }

}


#define exchange_elements(i,j) (exchange_elements_helper(base, element_size, (i), (j)))

#define MIN_QSORT_LIST_SIZE 32

__WEAK__ 
void my_qsort(void* base, size_t num_elements, 
              size_t element_size, 
              int (*comparer)(const void*, const void*)){

  size_t i;

  for(i = 0; i < num_elements; i++){
    int j;
    for (j = i - 1; j >= 0; j--){      
      if(element_less_than(j, j + 1)) break;
      exchange_elements(j, j + 1);
    }
  }
  /* may be implement qsort here */

}

#endif

__WEAK_INLINE void 
softboundcetsmpx_qsort(void* base, size_t nmemb, size_t size, 
                    int (*compar)(const void*, const void*)){

  qsort(base, nmemb, size, compar);
  //  my_qsort(base, nmemb, size, compar);
}

#if defined(__linux__)

__WEAK_INLINE 
void softboundcetsmpx__obstack_newchunk(struct obstack *obj, int b){
  
  _obstack_newchunk(obj, b);
}

__WEAK_INLINE
int softboundcetsmpx__obstack_begin(struct obstack * obj, int a, int b, 
                                 void *(foo) (long), void (bar) (void *)){
  return _obstack_begin(obj, a, b, foo, bar);
}

__WEAK_INLINE
void softboundcetsmpx_obstack_free(struct obstack *obj, void *object){
  obstack_free(obj, object);
}


__WEAK_INLINE 
char * softboundcetsmpx_nl_langinfo(nl_item item){
  
  char* ret_ptr = nl_langinfo(item);

  __softboundcetsmpx_store_return_metadata(ret_ptr, 
                                        ret_ptr + 1024 * 1024,
                                        1, __softboundcetsmpx_global_lock);
  
  return ret_ptr;
}

__WEAK_INLINE
int softboundcetsmpx_clock_gettime(clockid_t clk_id, struct timespec *tp){
  return clock_gettime(clk_id, tp);
}

#endif

#if 0

int softboundcetsmpx__obstack_memory_used(struct obstack *h){
  return _obstack_memory_used(h);
}

#endif
