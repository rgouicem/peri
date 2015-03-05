#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#define F2S "/tmp/F2S"
#define S2F "/tmp/S2F"
#define BUFSIZE 1024
#define NREQS 50

typedef struct _request {
  char msg[BUFSIZE];
  int id;
} request;

typedef struct _answer {
  int id;
  int length;
} answer;

int main(int argc, char* argv[]) {
  int fd_f2s, fd_s2f;
  int id=0, retval=-1, size;
  fd_set rset;//, wset;
  request req[NREQS];
  answer ans;

  // Initialisation
  /* if(mkfifo(F2S, 0666) == -1) { */
  /*   perror("mkfifo"); */
  /*   return -1; */
  /* } */
  
  if((fd_f2s = open(F2S, O_WRONLY)) == -1) {
    perror("open f2s");
    goto unlink;
  }

  if((fd_s2f = open(S2F, O_RDONLY)) == -1) {
    perror("open s2f");
    goto destroy;
  }

  // Traitement
  while(1) {
    FD_ZERO(&rset);
    FD_SET(STDIN_FILENO, &rset);
    FD_SET(fd_s2f, &rset);
    if(select(fd_s2f+1, &rset, NULL, NULL, NULL) == -1) {
      perror("select");
      goto destroyall;
    }
    if(FD_ISSET(fd_s2f, &rset)) {
      printf("hokuto s2f\n");
      // lecture sur s2f
      if(read(fd_s2f, &ans, sizeof(answer)) != sizeof(answer)) {
	perror("read");
	goto destroyall;
      }      
      printf("%d: %s, %d\n", ans.id, req[ans.id%NREQS].msg, ans.length);
    }
    else if(FD_ISSET(STDIN_FILENO, &rset)) {
      printf("hokuto stdin\n");
      // lecture sur stdin
      if((size = read(STDIN_FILENO, req[id%NREQS].msg, BUFSIZE-1)) < 0) {
	perror("read");
	goto destroyall;
      }
      if(size > 0) {
	req[id%NREQS].id = id;
	req[id%NREQS].msg[size-1] = '\0';
	// envoi sur pipe
	if(write(fd_f2s, req+(id%NREQS), sizeof(request)) != sizeof(request)) {
	  perror("write to f2s");
	  goto destroyall;
	}
	
	id++;
      }
    }
  }

  // Fermetures
  retval = 0;
  destroyall:
  if(close(fd_s2f) == -1) {
    perror("close");
    return -1;
  }

  destroy:
  if(close(fd_f2s) == -1) {
    perror("close");
    return -1;
  }

  unlink:
  if(unlink(F2S) == -1) {
    perror("unlink");
    return -1;
  }

  return retval;;
}
